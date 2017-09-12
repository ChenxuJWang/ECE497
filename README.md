# ECE497
Embedded System
pygame is needed to run this program
To get pygame, use sudo apt-get install python-pygame
Use arrow keys to move cursor and space key to clean canvas
To exit the program, press Esc.

// Comment from Prof. Yoder
// See README requirements
// Please put in a directory called 'hw01'
// I'm having trouble with your code.  Please demo it in class.
// Grade:  3/10

./etch.py 
ALSA lib pcm_hw.c:1713:(_snd_pcm_hw_open) Invalid value for card
Traceback (most recent call last):
  File "./etch.py", line 7, in <module>
    screen = pygame.display.set_mode((600,400))
pygame.error: Unable to open a console terminal
debian@bone-0834:~/studentWork/wangChenxu$ sudo ./etch.py 
sudo: unable to resolve host bone-0834
ALSA lib confmisc.c:767:(parse_card) cannot find card '0'
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_card_driver returned error: No such file or directory
ALSA lib confmisc.c:392:(snd_func_concat) error evaluating strings
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_concat returned error: No such file or directory
ALSA lib confmisc.c:1246:(snd_func_refer) error evaluating name
ALSA lib conf.c:4528:(_snd_config_evaluate) function snd_func_refer returned error: No such file or directory
ALSA lib conf.c:5007:(snd_config_expand) Evaluate error: No such file or directory
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM default
Traceback (most recent call last):
  File "./etch.py", line 7, in <module>
    screen = pygame.display.set_mode((600,400))
pygame.error: No video mode large enough for 600x400
