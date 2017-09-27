#!/usr/bin/env python3
import Adafruit_BBIO.GPIO as GPIO
import time
import smbus
bus = smbus.SMBus(1)

Thigh=0x1b
Tlow=0x19

addressL=0x4A
addressR=0x48

LED1="GREEN"
LED2="RED"
ALRL="GP1_3"
ALRR="GP1_4"

GPIO.setup(LED1,GPIO.OUT)
GPIO.setup(LED2,GPIO.OUT)
GPIO.setup(ALRL, GPIO.IN)
GPIO.setup(ALRR, GPIO.IN)

GPIO.output(LED1, 1)
GPIO.output(LED2, 1)

LEDmap = {ALRL: LED1, ALRR: LED2}
ALRmap = {ALRL: addressL, ALRR: addressR}

def updateLED(channel):
    print("channel = " + channel)
    state = GPIO.input(channel)
    GPIO.output(LEDmap[channel], state)
    reading = tempread(ALRmap[channel])
    if reading >= Thigh:
        print(LEDmap[channel] + " Toggled")
        print("Too hot!!! Current temprature:" + str(reading))
    elif reading < Tlow:
        print(LEDmap[channel] + " Toggled")
        print("WEEEEEE! We are safe now. Current temprature:" + str(reading))
    else:
        print(LEDmap[channel] + " Toggled")
        print("Current temprature:" + str(reading))
    
def tempread(address):
    reading = bus.read_byte_data(address, 0)
    return reading

GPIO.add_event_detect(ALRL, GPIO.BOTH, callback=updateLED)
GPIO.add_event_detect(ALRR, GPIO.BOTH, callback=updateLED)

try:
    while True:
        time.sleep(100)
        
except KeyboardInterrupt:
    print("Cleaning Up")
    GPIO.cleanup()
GPIO.cleanup()
