#!/usr/bin/env python3
import Adafruit_BBIO.GPIO as GPIO
import time

buttonM="MODE"

LED1="GREEN"
LED2="RED"
LED3="GP1_3"
LED4="GP1_4"

BTN1="GP0_3"
BTN2="GP0_4"
BTN3="GP0_5"
BTN4="GP0_6"


GPIO.setup(LED1,GPIO.OUT)
GPIO.setup(LED2,GPIO.OUT)
GPIO.setup(LED3,GPIO.OUT)
GPIO.setup(LED4,GPIO.OUT)
GPIO.setup(BTN1, GPIO.IN)
GPIO.setup(BTN2, GPIO.IN)
GPIO.setup(BTN3, GPIO.IN)
GPIO.setup(BTN4, GPIO.IN)

GPIO.output(LED1, 1)
GPIO.output(LED2, 1)
GPIO.output(LED3, 1)
GPIO.output(LED4, 1)

# Map buttons to LEDs
map = {BTN1: LED1, BTN2: LED2,BTN3: LED3,BTN4: LED4}

def updateLED(channel):
    print("channel = " + channel)
    state = GPIO.input(channel)
    GPIO.output(map[channel], state)
    print(map[channel] + " Toggled")
    
print("Running...")
GPIO.add_event_detect(BTN1, GPIO.BOTH, callback=updateLED)
GPIO.add_event_detect(BTN2, GPIO.BOTH, callback=updateLED)
GPIO.add_event_detect(BTN3, GPIO.BOTH, callback=updateLED)
GPIO.add_event_detect(BTN4, GPIO.BOTH, callback=updateLED)

try:
    while True:
        time.sleep(100)
        
except KeyboardInterrupt:
    print("Cleaning Up")
    GPIO.cleanup()
GPIO.cleanup()
