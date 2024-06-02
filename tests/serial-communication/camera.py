import sensor, image

from machine import UART

uart = UART(1, 9600)
uart.init(9600)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

def sendMessage(message):
    uart.write(message)
    print("Sent: ", message)

def readMessage():
    if(not uart.any()):
        print("Recieved: ")
        return ""
    try:
        message = uart.readline()
        if message:
            print("Recieved: ", message.decode("utf-8").strip())
            return message.decode("utf-8").strip()
        else:
            print("Recieved: ")
            return ""
    except Exception as e:
        print("Error: ", e)
        return ""

while(True):
    sendMessage("T")
    readMessage()
    img = sensor.snapshot()
