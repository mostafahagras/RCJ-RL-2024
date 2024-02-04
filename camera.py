import sensor
import time
import math
import pyb
from machine import UART

uart = UART(3, 9600)
uart.init(9600)
thresholds = [(0, 20, -10, 10, -10, 10), (1, 6, -15, 12, -13, 16)] # Black
red_led = pyb.LED(1)
blue_led = pyb.LED(3)
red_led.off()
blue_led.off()
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

while True:
    red_led.off()
    blue_led.off()
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8).crop(roi=(0, 42, 160, 80))
    blobs = img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100)
    circles = img.find_circles(
       threshold=2500,
       x_margin=25,
       y_margin=25,
       r_margin=25,
       r_min=1,
       r_max=30,
       r_step=2,
    )
    if(len(circles) == 0): uart.write("N")
    for c in circles:
#        img.draw_circle(c.x(), c.y(), c.r(), color=(0,0,255))
        isBlack = False
        xOffset = center[0] - c.x()
        yOffset = center[1] - c.y()
        if(xOffset > -10 and xOffset < 10 and yOffset < 10):
            print("Pick ball")
        elif(xOffset > 10):
            print("Turn left")
        elif(xOffset < -10):
            print("Turn right")
        elif(yOffset > 10):
            print("Go forward")
#        print("x: ", xOffset,  " y: ", yOffset)
        for blob in blobs:
            d = distance(blob.cx(), blob.cy(), c.x(), c.y())
            rect = blob.rect()
            if(d < max_distance):
                if(rect[2] * rect[3] < max_blob_area):
                    isBlack = True
                    img.draw_circle(c.x(), c.y(), c.r())
        if(not isBlack):
            img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 255))
            uart.write("S")
        if(isBlack):
            uart.write("B")
#    print(clock.fps())
