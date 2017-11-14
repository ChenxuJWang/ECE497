https://elinux.org/ECE497_Project_Web_Face_Recognition

Interrupt:
1. gcc hw07interrupt.c -o interrupt
2. sudo ./setoutput.sh
3. sudo ./interrupt

The cpu usage is about 0.7% according to htop.
The average response delay is about 56ms.

mmap:
./hw07mmap.py

The cpu usage is also about 0.7%.
The average response delay is much shorter and is about 1ms. 56 times faster than interrupt!
