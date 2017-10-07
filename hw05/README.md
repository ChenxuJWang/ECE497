When matrixLED.html starts, it calls the matrixLED in matrixLED.js to initialize the table.
In matrixLED.js connect() is called to make a connection between the browser and the bone. 
When the user click any LED icon, LEDclick(i,j) is called and toggle the LED at row i column j.
When the user drag the slider, the matrixLED.js sends the lightness value to board via i2cset.

3.background-color: green;

4.Having two matrixs. One controls green and another controls red.
