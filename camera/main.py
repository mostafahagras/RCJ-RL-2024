import sensor, image, os, tf, math, uos, gc

# Constants
WIDTH = 240
HEIGHT = 240

# Init Camera
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.set_windowing((WIDTH, HEIGHT))
sensor.skip_frames(time=2000)

# Init UART
from machine import UART

uart = UART(1, 9600)
uart.init(9600)

# Init ML Model
net = None
labels = None
try:
    labels, net = tf.load_builtin_model("trained")
except Exception as e:
    raise Exception(e)

# ML Model settings
MIN_CONFIDENCE = 0.8

# Messages
TOGGLE_EVACUATION = "t"
PICK = "P"
FAILED_PICK = "p"
DROP = "D"
LEFT = "l"
RIGHT = "r"
FORWARD = "F"
SEARCH = "S"
SEARCH_TRIANGLE = "s"
START_WALL_FOLLOWING = "w"
TOGGLE_CHECK_ENTRANCE = "a"

# States
FIRST_SILVER_VICTIM = 0
FIRST_GREEN_TRIANGLE = 1
SECOND_SILVER_VICTIM = 2
SECOND_GREEN_TRIANGLE = 3
BLACK_VICTIM = 4
RED_TRIANGLE = 5

# Variables
img = None
inEvacuation = False
isWallFollowing = False
state = 0
sentMessage = False
checkForEntrance = False
recievedMessage = ""

# Thresholds
black_threshold = [(0, 11, -11, 1, -4, 7)]
green_threshold = [(48, 71, -53, -24, 29, 57), (32, 52, -51, -26, 24, 54)]#, (42, 55, -51, -24, 18, 49), (16, 50, -48, -20, 22, 49)]
red_threshold = [(4, 14, -5, 15, -9, 11)]#(10, 29, 21, 43, -6, 29)]#(9, 22, -1, 17, -9, 13)]
red_line_threshold = [(16, 39, 3, 32, -8, 22)]#[(25, 54, 15, 61, -11, 35)]#[(22, 100, 14, 127, 6, 127)]

# Functions
def sendMessage(message):
    global sentMessage
    uart.write(message)
    print("Sent: ", message)
    sentMessage = True

def readMessage():
    global recievedMessage
    if(not uart.any()):
        return ""
    try:
        message = uart.readline()
        recievedMessage = message.decode("utf-8").strip()
        if message:
            print("Recieved: ", message.decode("utf-8").strip())
            return message.decode("utf-8").strip()
        else:
            return ""
    except Exception as e:
        print("Error: ", e)
        return ""


def waitForMessage(message):
    while readMessage() != message:
        print("Waiting for: ", message)

def verifyPick():
    global state
    m = readMessage()
    while m != PICK and m != FAILED_PICK:
        m = readMessage()
    if(m == PICK):
        state += 1

def toggleInEvacuation():
    global inEvacuation
    inEvacuation = not inEvacuation
    state = 0
    uart.write(START_WALL_FOLLOWING)
    isWallFollowing = True

def getRegion(cx, cy):
    if cx < WIDTH / 3:
        return LEFT
    elif cx > 2 / 3 * WIDTH:
        return RIGHT
    elif cy < 2 / 3 * HEIGHT + 10:
        return FORWARD
    else:
        return PICK


def goToBall(cx, cy):
    global state
    region = getRegion(cx, cy)
    if region == PICK:
        sendMessage(PICK)
#        verifyPick()
        if(state == 4):
            waitForMessage(PICK)
            state += 1
        else:
            verifyPick()
    else:
        sendMessage(region)


def searchForSilverVictim():
    for i, detection_list in enumerate(
        net.detect(img, thresholds=[(math.ceil(MIN_CONFIDENCE * 255), 255)])
    ):
        if i != 2:
            continue  # Silver victims are at index 2
        if len(detection_list) == 0:
            continue  # no detections
        filtered = list(filter(lambda b: b.rect()[1] > 100, detection_list))
        if len(filtered) == 0:
            continue
        closest = max(filtered, key=lambda b: b.rect()[1] + b.rect()[3] / 2)
        [x, y, w, h] = closest.rect()
        center_x = x + w // 2
        center_y = y + h // 2
        img.draw_circle((center_x, center_y, (w + h) // 4))
        goToBall(center_x, center_y)

def searchForBlackVictim():
    blobs = img.find_blobs(black_threshold, roi=(0, 110, 240, 130))
    if len(blobs) != 0:
        filtered = list(
            filter(
                lambda b: b.rect()[2] / b.rect()[3] < 1.2
                and b.rect()[2] / b.rect()[3] > 0.8,
                blobs,
            )
        )
        if len(filtered) != 0:
            largest = max(filtered, key=lambda b: b.rect()[2] * b.rect()[3])
            img.draw_rectangle(largest.rect(), color=(255, 255, 255))
            x = largest.cx()
            y = largest.cy()
            goToBall(x, y)


def goToTriangle(cx, cy, w, h):
    global state, inEvacuation
    region = getRegion(cx, cy)
    if region == PICK:
        if w * h > 7000:
            sendMessage(DROP)
            waitForMessage(DROP)
            state += 1
            if state == 6:
                toggleInEvacuation()
    else:
        sendMessage(region)


def searchForTriangle(threshold, color):
    blobs = img.find_blobs(threshold, roi=(0, 90, 240, 150), area_threshold=500)
    if len(blobs) != 0:
        largest = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
        img.draw_rectangle(largest.rect(), color=color)
        x = largest.cx()
        y = largest.cy()
        img.draw_circle(x, y, 1)
        goToTriangle(x, y, largest.rect()[2], largest.rect()[3])


def searchForGreenTriangle():
    searchForTriangle(green_threshold, (0, 255, 0))


def searchForRedTriangle():
    searchForTriangle(red_threshold, (255, 0, 0))


def searchForRedLine():
    blobs = img.find_blobs(
        red_line_threshold, merge=True, roi=(20, 200, 280, 40), area_threshold=500
    )
    if len(blobs):
        uart.write("e")
        blob = max(blobs, key=lambda b: b.area())
        img.draw_rectangle(blob.rect(), color=(255,0,0))
        print("Saw red line")

def lookForAnyTriagle():
    red = img.find_blobs(red_threshold, roi=(0, 90, 320, 160))
    if(len(red)):
        redFiltered = list(filter(lambda b: b.rect()[2] / b.rect()[3] > 1, red))
        if(len(redFiltered)):
            sendMessage(TOGGLE_EVACUATION)
            l = max(red, key=lambda b: b.rect()[2] * b.rect()[3])
            img.draw_rectangle(l.rect(), color=(255, 0, 0))
            return
    green = img.find_blobs(green_threshold, roi=(0, 90, 320, 160))
    if(len(green)):
        greenFiltered = list(filter(lambda b: b.rect()[2] / b.rect()[3] > 1, green))
        if(len(greenFiltered)):
            sendMessage(TOGGLE_EVACUATION)
            l = max(green, key=lambda b: b.rect()[2] * b.rect()[3])
            img.draw_rectangle(l.rect(), color=(0, 255, 0))
            return

def handleEvacuation():
    global sentMessage
    print("State: ", state)
    if state == FIRST_SILVER_VICTIM or state == SECOND_SILVER_VICTIM:
        searchForSilverVictim()
    elif state == FIRST_GREEN_TRIANGLE or state == SECOND_GREEN_TRIANGLE:
        searchForGreenTriangle()
    elif state == BLACK_VICTIM:
        searchForBlackVictim()
    elif state == RED_TRIANGLE:
        searchForRedTriangle()
    if not sentMessage:
        if state % 2: # If it's searching for a triangle
            sendMessage(SEARCH_TRIANGLE)
        else: # Searching for a victim
            sendMessage(SEARCH)
    sentMessage = False

while True:
    if(readMessage() == TOGGLE_EVACUATION):
        print("Switch")
        inEvacuation = not inEvacuation
        state = 0
    if inEvacuation:
        img = sensor.snapshot().crop(roi=(40, 0, 240, 240))
        handleEvacuation()
    else:
        img = sensor.snapshot()
        if(recievedMessage == TOGGLE_CHECK_ENTRANCE):
            checkForEntrance = not checkForEntrance
        if checkForEntrance:
            lookForAnyTriagle()
        searchForRedLine()
        img.draw_rectangle(20, 200, 280, 40)
