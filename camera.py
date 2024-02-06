import sensor
import time
import math
import pyb
from machine import UART

state = 0
search = 1
changing_search = False
uart = UART(3, 9600)
uart.init(9600)
black_thresholds = [(0, 20, -10, 10, -10, 10), (1, 6, -15, 12, -13, 16)] # Black
triangles_thresholds = [
    ( 0, 20, 5, 50, 5, 50 ), # RED
    ( 10, 30, -50, 0, 5, 30 ) # GREEN
]
red_threshold = (0, 50, 75, 17, -30, 47)#(0, 20, 5, 50, 5, 50)
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
RED_CODE = 1
GREEN_CODE = 2
def distance(x1, y1, x2, y2):
    return math.sqrt(math.pow(x1-x2, 2) + math.pow(y1-y2, 2))

while True:
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8).crop(roi=(0, 35, 160, 80))
    if(state % 2 or search):
        if(state == 5 or search == 2): threshold = red_threshold
        else: threshold = green_threshold
        if(changing_search):
            uart.write("R")
            if(search == 2):
                threshold = green_threshold
                search = 1
            else:
                threshold = red_threshold
                search = 2
        blobs = img.find_blobs([threshold], pixels_threshold=100, area_threshold=100)
        if(len(blobs) == 0): changing_search = True
        else: changing_search = False
        for blob in blobs:
            if(blob.rect()[2]/blob.rect()[3]>3):
                if blob.elongation() > 0.5:
                    if(search):
                        xOffset = blob.rect()[0] - 80
                        yOffset = blob.rect()[1] - 40
#                        print(blob.cx(), ", ", blob.cy())
                        if(blob.cy() > 25):
                            uart.write("R")
                        elif(blob.cx() < 100 and blob.cx() > 60):
                            uart.write("F")
                        elif(blob.cx() < 60):
                            uart.write("L")
                        elif(blob.cx() > 100):
                            uart.write("R")

                    if(state != 5):
                        img.draw_edges(blob.min_corners(), color=(255, 0, 0))
                        img.draw_line(blob.major_axis_line(), color=(255, 0, 0))
                        img.draw_line(blob.minor_axis_line(), color=(255, 0, 0))
                    else:
                        img.draw_edges(blob.min_corners(), color=(0, 255, 0))
                        img.draw_line(blob.major_axis_line(), color=(0, 255, 0))
                        img.draw_line(blob.minor_axis_line(), color=(0, 255, 0))
    if(state % 2 == 0):
        blobs = img.find_blobs(black_thresholds, pixels_threshold=100, area_threshold=100)
        circles = img.find_circles(
           threshold=2500,
           x_margin=25,
           y_margin=25,
           r_margin=25,
           r_min=0,
           r_max=30,
           r_step=2,
        )
        foundAnyBall = False
        for c in circles:
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
                foundAnyBall = True
                search = 0
                changing_search = False
#                print(xOffset, ", ", yOffset)
                if(xOffset > -15 and xOffset < 15 and yOffset < 45):
                    uart.write("P")
#                    print("P")
                elif(xOffset > 15):
                    uart.write("L")
                elif(xOffset < -15):
                    uart.write("R")
                elif(yOffset > 45):
                    uart.write("F")
            if(isBlack):# and state == 4):
                foundAnyBall = True
                search = 0
                changing_search = False
#                print(xOffset, ", ", yOffset)
                if(xOffset > -15 and xOffset < 15 and yOffset < 45):
                    uart.write("P")
#                    print("P")
                elif(xOffset > 15):
                    uart.write("L")
                elif(xOffset < -15):
                    uart.write("R")
                elif(yOffset > 45):
                    uart.write("F")
        if(not foundAnyBall):
            if(not search): search = 1

