/*
To test that the Linux framebuffer is set up correctly, and that the device permissions
are correct, use the program below which opens the frame buffer and draws a gradient-
filled red square:

retrieved from:
Testing the Linux Framebuffer for Qtopia Core (qt4-x11-4.2.2)

http://cep.xor.aps.anl.gov/software/qt4-x11-4.2.2/qtopiacore-testingframebuffer.html
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "/opt/source/Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "/opt/source/Robotics_Cape_Installer/libraries/roboticscape.h"

int main(int argc, char **argv)
{   int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 1;       // Make it so the it runs before the encoder is moved
    int xold = 0, yold = 0;
    long int location = 0;
    int redd = 0;
    int greenn = 0;
    int bluee = 0;
    int br = 13;
    int bg = 47;
    int bb = 17;
    if(argc < 2) // no arguments were passed
    {
        redd = 1;
        greenn = 2;
        bluee = 3;
    }
    else
    {   if(argc == 2){
            if(strcmp("-b", argv[1]) == 0){
                br = 0;
                bg = 0;
                bb = 0;
                redd = 31;
                greenn = 63;
                bluee = 31;
            }
            else if(strcmp("-w", argv[1]) == 0){
                br = 31;
                bg = 63;
                bb = 31;
                redd = 0;
                greenn = 0;
                bluee = 0;
            }
            else{
                printf("Unsupported command.\n");
                return 0;
            }
        }
        else if(argc == 4){
            if(atoi(argv[1]) == 0 || atoi(argv[2]) == 0 || atoi(argv[3]) == 0){
                printf("Line color only accept integer RGB values.\n");
                return 0;
            }
            else
            {
                if(atoi(argv[1]) <= 31 && atoi(argv[2]) <= 63 && atoi(argv[3]) <= 31){
                    redd = atoi(argv[1]);
                    greenn = atoi(argv[2]);
                    bluee = atoi(argv[3]);   
                }
                else{
                    printf("RGB value(s) overflow. The range of red and blue is 1-31. The range of green is 1-63.\nPlease check your input.\n");
                    return 0;
                }
            }
        }
        else if(argc == 5){
            if(strcmp("-b", argv[1]) == 0){
                br = 0;
                bg = 0;
                bb = 0;
                redd = 31;
                greenn = 63;
                bluee = 31;
                if(atoi(argv[2]) == 0 || atoi(argv[3]) == 0 || atoi(argv[4]) == 0){
                    printf("Line color only accept integer RGB values.\n");
                    return 0;
                }
                else
                {
                    if(atoi(argv[1]) <= 31 && atoi(argv[2]) <= 63 && atoi(argv[3]) <= 31){
                        redd = atoi(argv[1]);
                        greenn = atoi(argv[2]);
                        bluee = atoi(argv[3]);   
                    }
                    else{
                        printf("RGB value(s) overflow. The range of red and blue is 1-31. The range of green is 1-63.\nPlease check your input.\n");
                        return 0;
                    }
                }
            }
            else if(strcmp("-w", argv[1]) == 0){
                br = 31;
                bg = 63;
                bb = 31;
                redd = 0;
                greenn = 0;
                bluee = 0;
                if(atoi(argv[2]) == 0 || atoi(argv[3]) == 0 || atoi(argv[4]) == 0){
                    printf("Line color only accept integer RGB values.\n");
                    return 0;
                }
                else
                {
                    if(atoi(argv[1]) <= 31 && atoi(argv[2]) <= 63 && atoi(argv[3]) <= 31){
                        redd = atoi(argv[1]);
                        greenn = atoi(argv[2]);
                        bluee = atoi(argv[3]);   
                    }
                    else{
                        printf("RGB value(s) overflow. The range of red and blue is 1-31. The range of green is 1-63.\nPlease check your input.\n");
                        return 0;
                }
                }
            }
            else{
                printf("Unsupported command.\n");
                return 0;
            }
        }
        else{
            printf("Please check your input.\n");
            return 0;
        }
    }
   
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if(fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    printf("Offset: %dx%d, line_length: %d\n", vinfo.xoffset, vinfo.yoffset, finfo.line_length);
    
    if(vinfo.bits_per_pixel != 16) {
        printf("Can't handle %d bpp, can only do 16.\n", vinfo.bits_per_pixel);
        exit(5);
    }

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    // initialize hardware first
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to run rc_initialize(), are you root?\n");
		return -1;
	}

// 	printf("\nRaw encoder positions\n");
// 	printf("   E1   |");
// 	printf("   E2   |");
// 	printf("   E3   |");
// 	printf("   E4   |");
// 	printf(" \n");
	
	// Black out the screen
	short color = (br<<11) | (bg << 5) | bb;  // RGB
	for(int i=0; i<screensize; i+=2) {
	    fbp[i  ] = color;      // Lower 8 bits
	    fbp[i+1] = color>>8;   // Upper 8 bits
	}

	while(rc_get_state() != EXITING) {
// 		printf("\r");
// 		for(int i=1; i<=4; i++){
// 			printf("%6d  |", rc_get_encoder_pos(i));
// 		}
		fflush(stdout);
        // Update framebuffer
        // Figure out where in memory to put the pixel
        x = (rc_get_encoder_pos(1)/4 + vinfo.xres) % vinfo.xres;
        y = (rc_get_encoder_pos(3)/4 + vinfo.yres) % vinfo.yres;
        // printf("xpos: %d, xres: %d\n", rc_get_encoder_pos(1), vinfo.xres);
        
        if((x != xold) || (y != yold)) {
            // printf("Updating location to %d, %d\n", x, y);
            // Set old location to green
            location = (xold+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                      
                       (yold+vinfo.yoffset) * finfo.line_length;
            int r = redd;     // 5 bits
            int g = greenn;      // 6 bits
            int b = bluee;      // 5 bits
            unsigned short int t = r<<11 | g << 5 | b;
            *((unsigned short int*)(fbp + location)) = t;
            // Set new location to white
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
    
            *((unsigned short int*)(fbp + location)) = 0xff;
            xold = x;
            yold = y;
        }
		
		rc_usleep(5000);
	}
	
	rc_cleanup();
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
