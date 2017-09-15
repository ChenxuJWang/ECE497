#!/usr/bin/env python3
import Adafruit_BBIO.GPIO as GPIO
import time

rows_count = 10
cols_count = 10

intx=0
inty=0

two_d_array = [['O' for j in range(cols_count)] for i in range(rows_count)]


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
GPIO.setup(buttonM, GPIO.IN)

GPIO.output(LED1, 1)
GPIO.output(LED2, 1)
GPIO.output(LED3, 1)
GPIO.output(LED4, 1)

# Map buttons to LEDs
map = {BTN1: LED1, BTN2: LED2,BTN3: LED3,BTN4: LED4}

def updateLED(channel):
    global intx
    global inty
    global rows_count
    global cols_count
    print("channel = " + channel)
    state = GPIO.input(channel)
    GPIO.output(map[channel], state)
    if channel == "GP0_3":
        if intx < cols_count-1:
            intx += 1
    elif channel == "GP0_4":
         if intx > 0:
            intx -= 1
    elif channel == "GP0_5":
        if inty < rows_count-1:
            inty += 1
    else:
        if inty > 0:
            inty -= 1
    
    two_d_array[intx][inty] = 'X'
    
    print(map[channel] + " Toggled")
    
    for y in range(0, cols_count):
        print(two_d_array[:][y])
print("Running...")

def shake(channel):
    global intx
    global inty
    global rows_count
    global cols_count
    
    print("LOADING------------------------------------")
    
    for x in range(0, rows_count):
        for y in range(0, cols_count):
            two_d_array[x][y] = 'O'
    
    for y in range(0, cols_count):
        print(two_d_array[:][y])
    intx = 0
inty = 0
    


for y in range(0, cols_count):
    print(two_d_array[:][y])

GPIO.add_event_detect(BTN1, GPIO.FALLING, callback=updateLED)
GPIO.add_event_detect(BTN2, GPIO.FALLING, callback=updateLED)
GPIO.add_event_detect(BTN3, GPIO.FALLING, callback=updateLED)
GPIO.add_event_detect(BTN4, GPIO.FALLING, callback=updateLED)
GPIO.add_event_detect(buttonM, GPIO.FALLING, callback=shake)


try:
    while True:
        time.sleep(100)
        
except KeyboardInterrupt:
    print("Cleaning Up")
    GPIO.cleanup()
GPIO.cleanup()


