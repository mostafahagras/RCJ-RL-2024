# Edge Impulse - OpenMV Object Detection Example
#v11
import sensor, image, time, os, tf, math, uos, gc
from machine import UART

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)   # Let the camera adjust.
uart = UART(1, 9600)
uart.init(9600)
state = 9;
net = None
labels = None
min_confidence = 0.9
sent_message = False
black_threshold = [(0, 11, -11, 1, -4, 7)]#[(0, 20, -10, 10, -10, 10), (1, 6, -15, 12, -13, 16)]
exit_line_threshold = [(0, 33, -6, 4, -16, 5)]
green_threshold = [(12, 36, -48, -21, 24, 37), (12, 60, -61, -3, 17, 56)]#[(10, 30, -50, 0, 5, 30)]
red_threshold = [(0, 20, -1, 22, 8, 27), (0, 13, -1, 29, -3, 11)]#[(0, 20, 5, 50, 5, 50), (0, 50, 75, 17, -30, 47)]
red_line_threshold = [(27, 100, 15, 127, 15, 127), (22, 100, 14, 127, 6, 127)]
last_case = None
message = ""
red_line_threshold = [(27, 100, 15, 127, 15, 127)]
try:
    # Load built in model
    labels, net = tf.load_builtin_model('trained')
except Exception as e:
    raise Exception(e)

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,   0,   0),
    (  0, 255,   0),
    (255, 255,   0),
    (  0,   0, 255),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

def readMessage():
    try:
        msg = uart.readline()
        if(msg):
            print(msg)
            return msg.decode("utf-8").strip()
        else: return ""
    except Exception as e:
        print("Error: ", e)
        return ""

def goToBall(x, y):
    global last_case, sent_message, state
    if x >= 150:
        uart.write("r")
        last_case = ("r", time.time(), (x, y, 12))
        print("r")
        sent_message = True
    elif x <= 70:
        uart.write("l")
        last_case = ("l", time.time(), (x, y, 12))
        print("l")
        sent_message = True
    elif y <= 170:
        uart.write("F")
        print("F")
        sent_message = True
    else:
#        message = uart.readline()
        uart.write("P")
        print("P")
        i =0
        while (readMessage() != "P"):
#            message = uart.readline()
            i+=1
            print(i)
            print("Verifying pick")
        print("Pick verified")
        last_case = None
        sent_message = True
        state += 1

def goToTriangle(x, y, w, h):
    global last_case, sent_message, state
    if x >= 150:
        uart.write("r")
        last_case = ("r", time.time(), largest.rect())
        print("r")
        sent_message = True
    elif x <= 70:
        uart.write("l")
        last_case = ("l", time.time(), largest.rect())
        print("l")
        sent_message = True
    elif y <= 170:
        uart.write("F")
        print("F")
        sent_message = True
    elif(w * h > 6000):
        uart.write("D")
        print("D")
        while (readMessage() != "D"):
            print("Verifying drop")
        print("Drop verified")
        last_case = None
        sent_message = True
        state += 1

clock = time.clock()
while(True):
    clock.tick()
    print("State", state)
    img = sensor.snapshot()
    print("Message: ", readMessage())
    # detect() returns all objects found in the image (splitted out per class already)
    # we skip class index 0, as that is the background, and then draw circles of the center
    # of our objects
    if state == 0 or state == 2:
        for i, detection_list in enumerate(net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])):
            if (i != 2): continue # background class
            if (len(detection_list) == 0): continue # no detections for this class?
            filtered = list(filter(lambda b: b.rect()[1] > 100, detection_list))
            if(len(filtered) == 0): continue
            closest = max(filtered, key=lambda b: b.rect()[1]+b.rect()[3]/2)
#
#            print("********** %s **********" % labels[i])
    #        for d in filtered:
            [x, y, w, h] = closest.rect()
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            print('x %d\ty %d' % (center_x, center_y))
            img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)
            goToBall(center_x, center_y)
        if(not sent_message):
            if(last_case != None and time.time() - last_case[1] < 500):
                uart.write(last_case[0])
                img.draw_circle(last_case[2])
                print(last_case[0])
            else:
                uart.write("R")
                print("R")
        sent_message = False
    elif state == 1 or state == 3:
        blobs = img.find_blobs(green_threshold, roi=(0, 120, 240, 120), area_threshold=500)
        if(len(blobs) != 0):
            largest = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
            img.draw_rectangle(largest.rect())
            x = largest.cx()
            y = largest.cy()
            goToTriangle(x, y, largest.rect()[2], largest.rect()[3])
        else:
            if(last_case != None and time.time() - last_case[1] < 500):
                uart.write(last_case[0])
                img.draw_rectangle(last_case[2])
                print(last_case[0])
            else:
                uart.write("R")
                print("R")
    elif state == 4:
        blobs = img.find_blobs(black_threshold, roi=(0, 120, 240, 120))
        if(len(blobs) != 0):
            filtered = list(filter(lambda b: b.rect()[2] / b.rect()[3] < 1.2 and b.rect()[2] /b.rect()[3] > 0.8, blobs))
            if(len(filtered) != 0):
                largest = max(filtered, key=lambda b: b.rect()[2] * b.rect()[3])
                img.draw_rectangle(largest.rect())
                x = largest.cx()
                y = largest.cy()
                goToBall(x, y)
            else:
                if(last_case != None and time.time() - last_case[1] < 500):
                    uart.write(last_case[0])
                    img.draw_circle(last_case[2])
                    print(last_case[0])
                else:
                    uart.write("R")
                    print("R")
        else:
            if(last_case != None and time.time() - last_case[1] < 500):
                uart.write(last_case[0])
                img.draw_circle(last_case[2])
                print(last_case[0])
            else:
                uart.write("R")
                print("R")
    elif state == 5:
        blobs = img.find_blobs(red_threshold, roi=(0, 120, 240, 120), area_threshold=500)
        if(len(blobs) != 0):
            largest = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
            img.draw_rectangle(largest.rect())
            x = largest.cx()
            y = largest.cy()
            goToTriangle(x, y, largest.rect()[2], largest.rect()[3])
        else:
            if(last_case != None and time.time() - last_case[1] < 500):
                uart.write(last_case[0])
                print(last_case[0])
            else:
                uart.write("R")
                print("R")
    elif state == 6:
        uart.write("W")
        print("W")
        while (readMessage() != "W"):
            print("Verifying Exit")
        print("Exit verified")
        state = 7
    elif state == 7:
        blobs = img.find_blobs(black_threshold, roi=(0, 200, 240, 40))
        if(len(blobs) != 0):
            filtered = list(filter(lambda b: (b.rect()[2] / b.rect()[3] > 3) and (b.rect()[2] * b.rect()[3] > 5000), blobs))
            if(len(filtered) != 0):
                largest = max(filtered, key=lambda b: b.rect()[2] * b.rect()[3])
                uart.write("E")
                img.draw_rectangle(largest.rect())
    if state == 9:
        blobs = img.find_blobs(red_line_threshold, merge=True, roi=(20, 200, 200, 40), aree_threshold=500)
        if len(blobs):
            uart.write("e")
            filtered = list(filter(lambda b: b.rect()[2] / b.rect()[3] > 3, blobs))
            if len(filtered):
                blob = max(filtered, key=lambda b: b.rect()[2] * b.rect()[3])
                img.draw_rectangle(blob.rect())
                print("SAW RED")
#        print("Done Evacuation")
#    print(clock.fps(), "fps", end="\n\n")
