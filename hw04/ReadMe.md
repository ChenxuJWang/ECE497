# Comments from Prof. Yoder3
# Your ReadMe file is missing.  Please fill in the details.
# Grade:  00/10

I previously had trouble toggling LED in which case the LED can only be toggle once(off to on) and will remain on the whole time. I have finally figured out why... I have a typo in my code.... Anyway, it should work just fine now.

I have two button one connected to gpio0_3 and another one connected to gpio0_5. Each one of these buttons control a LED on board. I have gpio0_3 toggled USR3 and gpio0_5 toggled USR2. To run the program, first
gcc HW4gpio -o gpio
sudo ./gpio
