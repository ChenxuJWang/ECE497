#!/usr/bin/env python3
import Adafruit_BBIO.GPIO as GPIO
import time

# setup input and output GPIO
button = "GP1_3"
led = "GP1_4"
GPIO.setup(button, GPIO.IN)
GPIO.setup(led, GPIO.OUT)

map = {button: led}

def identify(channel):
    state = GPIO.input(channel)
    GPIO.output(map[channel],state)
    
print("Running...")

GPIO.add_event_detect(button, GPIO.FALLING, callback=identify)

try:
	while True:
		time.sleep(100)

except KeyboardInterrupt:
	print("Cleaning Up")
	GPIO.cleanup()
GPIO.cleanup()



