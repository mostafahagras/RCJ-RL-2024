import sensor
import time
import math
from machine import UART

state = 0
search = 0
changing_search = False
uart = UART(3, 9600)
uart.init(9600)
black_thresholds = [(0, 20, -10, 10, -10, 10), (1, 6, -15, 12, -13, 16)] # Black
red_threshold = (0, 20, 5, 50, 5, 50)#(0, 50, 75, 17, -30, 47)#
green_threshold = (10, 30, -50, 0, 5, 30)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
max_distance = 15
max_blob_area = 5000
center = [80, 90]

def distance(x1, y1, x2, y2):
    return math.sqrt(math.pow(x1-x2, 2) + math.pow(y1-y2, 2))

def goToTriangle(threshold):
    global state
    global search
    blobs = img.find_blobs([threshold], pixels_threshold=100, area_threshold=100)
    if(len(blobs) == 0): uart.write("R")
    else:
        blob = max(blobs, key=lambda b: b.rect()[2] * b.rect()[3])
#        img.draw_edges(blob.min_corners(), color=(255, 0, 0))
#        img.draw_line(blob.major_axis_line(), color=(255, 0, 0))
#        img.draw_line(blob.minor_axis_line(), color=(255, 0, 0))
        xOffset = blob.rect()[0] - 80
        yOffset = blob.rect()[1] - 40
        if(blob.rect()[2] * blob.rect()[3] > 4000):
            if(state % 2):
                uart.write("D")
                if(state == 1): state = 2
                elif(state == 3): state = 4
                elif(state == 5): state = 6
                print("D")
            else:
                uart.write("R")
                time.sleep_ms(1500)
                if(search == 2): search = 1
                else: search = 2
                print("R")
        elif(blob.cx() < 100 and blob.cx() > 60):
            uart.write("F")
            print("F")
        elif(blob.cx() < 60):
            uart.write("L")
            print("L")
        elif(blob.cx() > 100):
            uart.write("R")
            print("R")
#def goToBall():
# if(state == 1 or state == 3):
#   goToTriangle(green_threshold)
# if(state == 5):
#   goToTriangle(red_threshold)
while True:
    clock.tick()
#    message = uart.readchar()
#    print("State: ", state)
#    if(message > -1 and message < 6):
#        state = message
    img = sensor.snapshot().lens_corr(1.8).crop(roi=(0, 35, 160, 80))
    if(state % 2 or search):
        if(state == 5 or search == 2): threshold = red_threshold
        else: threshold = green_threshold
        if(changing_search):
            changing_search = False
            uart.write("R")
            if(search == 2):
                threshold = green_threshold
                search = 1
            else:
                threshold = red_threshold
                search = 2
        goToTriangle(threshold)
#        blobs = img.find_blobs([threshold], pixels_threshold=100, area_threshold=100)
#        print(threshold)
#        if(len(blobs) == 0):
#            uart.write("R")
#        else:
#            changing_search = False
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
#                    changing_search = True
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
        blobs = img.find_blobs(black_thresholds, pixels_threshold=100, area_threshold=100)
        if(state == 4):
            circles = img.find_circles(
               threshold=2500,
               x_margin=25,
               y_margin=25,
               r_margin=25,
               r_min=0,
               r_max=30,
               r_step=2,
            )
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
        foundAnyBall = False
        if(circles):
            c = max(circles, key=lambda b: b.r())
            print(c.r())
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
#                print(yOffset)
                img.draw_circle(c.x(), c.y(), c.r(), (255,0,0))
                foundAnyBall = True
                search = 0
                changing_search = False
                if(xOffset > -15 and xOffset < 15 and yOffset < 45 and yOffset > 40):
                    uart.write("P")
                    if(state == 0): state = 1
                    elif(state == 2): state = 3
                    print("P")
                elif(xOffset > 15):
                    uart.write("L")
                    print("L")
                elif(xOffset < -15):
                    uart.write("R")
                    print("R")
                elif(yOffset > 45):
                    uart.write("F")
                    print("F")
                else:
                    uart.write("R")
#                    print("R")
            if(isBlack and state == 4):
#                print(xOffset, " , " , yOffset)
                img.draw_circle(c.x(), c.y(), c.r(), (255,0,255))
                foundAnyBall = True
                search = 0
                changing_search = False
                if(xOffset > -15 and xOffset < 15 and yOffset < 45 and yOffset > 40):
                    uart.write("P")
                    if(state == 0): state = 1
                    elif(state == 2): state = 3
                    print("P")
                elif(xOffset > 15):
                    uart.write("L")
                    print("L")
                elif(xOffset < -15):
                    uart.write("R")
                    print("R")
                elif(yOffset > 45):
                    uart.write("F")
                    print("F")
                else:
                    uart.write("B")
                    print("B")
        if(not foundAnyBall):
            if(not search):
#                print("N")
                search = 1

