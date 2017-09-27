#!/usr/bin/env python3
# Write an 8x8 Red/Green LED matrix
# https://www.adafruit.com/product/902

import smbus
import time
import Adafruit_BBIO.GPIO as GPIO

rows_count = 8
cols_count = 8

intx=0
inty=0

buttonM="MODE"
BTN1="GP0_3"
BTN2="GP0_4"
BTN3="GP0_5"
BTN4="GP0_6"

GPIO.setup(BTN1, GPIO.IN)
GPIO.setup(BTN2, GPIO.IN)
GPIO.setup(BTN3, GPIO.IN)
GPIO.setup(BTN4, GPIO.IN)
GPIO.setup(buttonM, GPIO.IN)

bus = smbus.SMBus(1)  # Use i2c bus 1
matrix = 0x70         # Use address 0x70

delay = 1; # Delay between images in s

bus.write_byte_data(matrix, 0x21, 0)   # Start oscillator (p10)
bus.write_byte_data(matrix, 0x81, 0)   # Disp on, blink off (p11)
bus.write_byte_data(matrix, 0xe7, 0)   # Full brightness (page 15)

# The welcome screen is a 8-bit piggy

c0r = 0b00011110
c1r = 0b11100101
c2r = 0b01101011
c3r = 0b00101111
c4r = 0b00101111
c5r = 0b01101011
c6r = 0b11100101
c7r = 0b00011110

c0g = 0x00
c1g = 0b00000100
c2g = 0b00011010
c3g = 0b00001010
c4g = 0b00001010
c5g = 0b00011010
c6g = 0b00000100
c7g = 0x00

output = [c0g,c0r,c1g,c1r,c2g,c2r,c3g,c3r,c4g,c4r,c5g,c5r,c6g,c6r,c7g,c7r]


bus.write_i2c_block_data(matrix, 0, output)
print("Press MOD button on board to clean the matrix and start the game.")

for fade in range(0xef, 0xe0, -1):
    bus.write_byte_data(matrix, fade, 0)
    time.sleep(delay/10)

#update Matrix display    
def updateMatrix():
    global output
    global matrix
    global bus
    bus.write_i2c_block_data(matrix, 0, output)

    
def updateLED(channel):
    global intx
    global inty
    global output
    
    print("channel = " + channel)
    if intx==0 and inty==0:
        intx += 1
        inty += 1
        
    elif channel == "GP0_3":
        if intx <= cols_count-1:
            intx += 1
            
    elif channel == "GP0_4":
         if intx > 1:
            intx -= 1

    elif channel == "GP0_5":
        if inty <= rows_count-1:
            inty += 1
            
    else:
        if inty > 0:
            inty -= 1
            
    pointer = 2*intx-1
    output[pointer] = output[pointer]|0x01<<(8-inty)
    updateMatrix()

def shake(channel):
    global intx
    global inty

    print("LOADING------------------------------------")
    
    for x in range(0, 16):
            output[x] = 0x00
    bus.write_i2c_block_data(matrix, 0, output)
    intx = 0
    inty = 0

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
