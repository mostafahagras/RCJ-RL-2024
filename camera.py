import sensor
import time
import math
from machine import UART

state = 0
search = 0
uart = UART(1, 9600)
uart.init(9600)
black_thresholds = [(0, 20, -10, 10, -10, 10), (1, 6, -15, 12, -13, 16)]
red_threshold = [(0, 20, 5, 50, 5, 50), (0, 50, 75, 17, -30, 47)]
green_threshold = [(10, 30, -50, 0, 5, 30)]#[(29, 67, -128, -9, -128, 127),(10, 30, -50, 0, 5, 30)]
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
max_distance = 15
max_blob_area = 5000
center = [80, 40]
last_case = None
rotations = ["R", "R", "L", "L"]
rotation = 3

# 0, 2 -> Silver
# 1, 3 -> Green Triangle
# 4 -> Black
# 5 -> Red Triangle
# 6 -> Exit
# match state:
#   case 0 | 2 | 4 if search == 1:
#       # Search
#   case 0 | 2:
#       # Silver
#   case 1 | 3:
#       # Green Triangle
#   case 4:
#       # Black
#   case 5:
#       # Red Triangle
#   case 6:
#       # Exit


def validCircle(blob):
    ratio = blob.rect()[2]/blob.rect()[3]
    if(ratio < 1.2 and ratio > 0.8): return True
    else: return False

def validSilver(circle):
    if(circle.y() / circle.r() > 2): return True
    else: return False

def distance(x1, y1, x2, y2):
    return math.sqrt(math.pow(x1-x2, 2) + math.pow(y1-y2, 2))

def goToTriangle(threshold):
    global state
    global search
    blobs = img.find_blobs(threshold, pixels_threshold=100, area_threshold=100)
    if(len(blobs) == 0):
        uart.write("R")
        print("R")
    else:
        blob = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
        img.draw_edges(blob.min_corners(), color=(255, 0, 0))
        img.draw_line(blob.major_axis_line(), color=(255, 0, 0))
        img.draw_line(blob.minor_axis_line(), color=(255, 0, 0))
        xOffset = blob.rect()[0] - 80
        yOffset = blob.rect()[1] - 40
        print(blob.rect()[2] * blob.rect()[3])
        if(state == 5 or search == 2): max_area = 6000
        else: max_area = 5000
        if(blob.rect()[2] * blob.rect()[3] > max_area):
            if(state % 2):
                uart.write("D")
                if(state == 1): state = 2
                elif(state == 3): state = 4
                elif(state == 5): state = 6
                print("D")
            else:
                uart.write("B")
                time.sleep_ms(1000)
                nextRotation = rotations[(rotation+1)%4]
                uart.write(nextRotation)
                time.sleep_ms(3000)
                if(search == 2): search = 1
                else: search = 2
                print(nextRotation)
        elif(blob.cx() < 100 and blob.cx() > 60):
            uart.write("F")
            print("F")
        elif(blob.cx() < 60):
            uart.write("L")
            print("L")
        elif(blob.cx() > 100):
            uart.write("R")
            print("R")

while True:
    clock.tick()
    print("State: ", state)
    img = sensor.snapshot().lens_corr(1.8)#.crop(roi=(0, 35, 160, 80))
#        if(changing_search):
#            uart.write("R")
#            if(search == 2):
#                threshold = green_threshold
#                search = 1
#            else:
#                threshold = red_threshold
#                search = 2
#        blobs = img.find_blobs([threshold], pixels_threshold=100, area_threshold=100)
#        print(threshold)
#        if(len(blobs) == 0):
#            uart.write("R")
#        else:
#            blob = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
#            xOffset = blob.rect()[0] - 80
#            yOffset = blob.rect()[1] - 40
#            if(blob.rect()[2] * blob.rect()[3] > 4000):
#                print("STATE: ", state)
#                if(state % 2):
#                    uart.write("D")
#                    if(state == 1): state = 2
#                    elif(state == 3): state = 4
#                    elif(state == 5): state = 6
#                    print("D")
#                else:
#                    uart.write("R")
#                    print("R")
#            elif(blob.cx() < 100 and blob.cx() > 60):
#                uart.write("F")
#                print("F")
#            elif(blob.cx() < 60):
#                uart.write("L")
#                print("L")
#            elif(blob.cx() > 100):
#                uart.write("R")
#                print("R")
#            if(state != 5):
#                img.draw_edges(blob.min_corners(), color=(255, 0, 0))
#                img.draw_line(blob.major_axis_line(), color=(255, 0, 0))
#                img.draw_line(blob.minor_axis_line(), color=(255, 0, 0))
#            else:
#                img.draw_edges(blob.min_corners(), color=(0, 255, 0))
#                img.draw_line(blob.major_axis_line(), color=(0, 255, 0))
#                img.draw_line(blob.minor_axis_line(), color=(0, 255, 0))
    if(state % 2 == 0):
        foundAnyBall = False
        blobs = img.find_blobs(black_thresholds, pixels_threshold=100, area_threshold=100)
        if(state == 4):
            blobs = list(filter(validCircle, img.find_blobs(black_thresholds, area_threshold=20)))
            if(blobs):
                blob = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
                img.draw_rectangle(blob.rect())
                foundAnyBall = True
                search = 0
                xOffset = center[0] - blob.cx()
                yOffset = center[1] - blob.cy()
                if(xOffset > -15 and xOffset < 15 and yOffset < -5 and yOffset > -10):
                    uart.write("P")
                    state = 5
                    print("P")
                elif(xOffset > 15):
                    uart.write("L")
                    print("L")
                elif(xOffset < -15):
                    uart.write("R")
                    print("R")
                elif(yOffset > -5):
                    uart.write("F")
                    print("F")
                else:
                    uart.write("B")
        else:
            circles = img.find_circles(
                threshold=1800,
                x_margin=6,
                y_margin=6,
                r_margin=10,
                r_min=6,
                r_max=15,
                r_step=2,
            )
            if(circles):
                filteredCircles = list(filter(validSilver, circles))
                if(len(filteredCircles) == 0):
                    timestamp = time.time()
                    if(last_case and timestamp - last_case[1] < 2):
                        uart.write(last_case[0])
                        foundAnyBall = True
                        print("last case" , last_case[0])
                else:
                    c = max(filteredCircles, key=lambda b: b.r())
                    isBlack = False
                    xOffset = center[0] - c.x()
                    yOffset = center[1] - c.y()
                    for blob in blobs:
                        d = distance(blob.cx(), blob.cy(), c.x(), c.y())
                        rect = blob.rect()
                        if(d < max_distance):
                            if(rect[2] * rect[3] < max_blob_area):
                                isBlack = True
                    if(not isBlack and state < 4):
                        img.draw_circle(c.x(), c.y(), c.r(), (255,0,0))
                        foundAnyBall = True
                        search = 0
                        timestamp = time.time()
                        if(xOffset > -15 and xOffset < 15 and yOffset < -5 and yOffset > -10):
                            uart.write("P")
                            last_case = None
                            if(state == 0): state = 1
                            elif(state == 2): state = 3
                            print("P")
                        elif(xOffset > 15):
                            uart.write("L")
                            last_case = ("L", timestamp)
                            print("L")
                        elif(xOffset < -15):
                            uart.write("R")
                            last_case = ("R", timestamp)
                            print("R")
                        elif(yOffset > -5):
                            uart.write("F")
                            last_case = ("F", timestamp)
                            print("F")
                        else:
                            uart.write("B")
            else:
                timestamp = time.time()
                if(last_case and timestamp - last_case[1] < 2):
                    uart.write(last_case[0])
                    foundAnyBall = True
                    print("last case" , last_case[0])
#                if(isBlack and state == 4):
#    #                print(xOffset, " , " , yOffset)
#                    img.draw_circle(c.x(), c.y(), c.r(), (255,0,255))
#                    foundAnyBall = True
#                    search = 0
#                    if(xOffset > -15 and xOffset < 15 and yOffset < 45 and yOffset > 40):
#                        uart.write("P")
##                        if(state == 0): state = 1
##                        elif(state == 2): state = 3
#                        state = 5
#                        print("P")
#                    elif(xOffset > 15):
#                        uart.write("L")
#                        print("L")
#                    elif(xOffset < -15):
#                        uart.write("R")
#                        print("R")
#                    elif(yOffset > 45):
#                        uart.write("F")
#                        print("F")
#                    else:
#                        uart.write("B")
#                        print("B")
        if(not foundAnyBall):
            if(not search):
#                print("N")
                search = 1
    if(state % 2 or search):
        if(state == 5 or search == 2): threshold = red_threshold
        else: threshold = green_threshold
        goToTriangle(threshold)

