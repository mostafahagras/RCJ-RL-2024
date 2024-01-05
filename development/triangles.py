import sensor
import time
import math

thresholds = [
    ( 0, 20, 5, 50, 5, 50 ), # RED
    ( 10, 30, -50, 0, 5, 30 ) # GREEN 
]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)  # must be turned off for color tracking
sensor.set_auto_whitebal(False)  # must be turned off for color tracking
clock = time.clock()
RED_CODE = 1

while True:
    clock.tick()
    img = sensor.snapshot().crop(roi=(0, 60, 320, 180))
    blobs = img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100)
    for blob in blobs:
        if(blob.rect()[2]/blob.rect()[3]>3):
            if blob.elongation() > 0.5:
                if(blob.code() == RED_CODE):
                    img.draw_edges(blob.min_corners(), color=(255, 0, 0))
                    img.draw_line(blob.major_axis_line(), color=(255, 0, 0))
                    img.draw_line(blob.minor_axis_line(), color=(255, 0, 0))
                else:
                    img.draw_edges(blob.min_corners(), color=(0, 255, 0))
                    img.draw_line(blob.major_axis_line(), color=(0, 255, 0))
                    img.draw_line(blob.minor_axis_line(), color=(0, 255, 0))
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_keypoints(
                [(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20
            )
    print(clock.fps())
