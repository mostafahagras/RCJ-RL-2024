import sensor
import time
import math

thresholds = [(0, 20, -10, 10, -10, 10)] # Black

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
ditance_threshold = 15
area_threshold = 5000

def distance(x1, y1, x2, y2):
    return math.sqrt(math.pow(x1-x2, 2) + math.pow(y1-y2, 2))

while True:
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8).erode(2)
    blobs = img.find_blobs(thresholds, pixels_threshold=200, area_threshold=200)
    for c in img.find_circles(
        threshold=2500,
        x_margin=25,
        y_margin=25,
        r_margin=25,
        r_min=2,
        r_max=30,
        r_step=2,
    ):
        isBlack = False
        for blob in blobs:
            d = distance(blob.cx(), blob.cy(), c.x(), c.y())
            rect = blob.rect()
            if(d < ditance_threshold):
                if(rect[2] * rect[3] < area_threshold):
                    isBlack = True
                    img.draw_circle(c.x(), c.y(), c.r())
        if(not isBlack):
            img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 255))
