// From : http://stackoverflow.com/questions/13124271/driving-beaglebone-gpio-through-dev-mem
//
// Read one gpio pin and write it out to another using mmap.
// Be sure to set -O3 when compiling.
// Modified by Mark A. Yoder  26-Sept-2013
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <signal.h>    // Defines signal-handling functions (i.e. trap Ctrl-C)
#include "beaglebone_gpio.h"

/****************************************************************
 * Global variables
 ****************************************************************/
int keepgoing = 1;    // Set to 0    when ctrl-c is pressed

/****************************************************************
 * signal_handler
 ****************************************************************/
void signal_handler(int sig);
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
    printf( "\nCtrl-C pressed, cleaning up and exiting...\n" );
	keepgoing = 0;
}

int main(int argc, char *argv[]) {
    volatile void *gpio_addr1;
    volatile unsigned int *gpio_oe_addr1;
    volatile unsigned int *gpio_datain1;
    volatile unsigned int *gpio_setdataout_addr1;
    volatile unsigned int *gpio_cleardataout_addr1;
    unsigned int reg1;
    volatile void *gpio_addr2;
    volatile unsigned int *gpio_oe_addr2;
    volatile unsigned int *gpio_datain2;
    volatile unsigned int *gpio_setdataout_addr2;
    volatile unsigned int *gpio_cleardataout_addr2;
    unsigned int reg2;

    // Set the signal callback for Ctrl-C
    signal(SIGINT, signal_handler);

    int fd = open("/dev/mem", O_RDWR);

    printf("Mapping %X - %X (size: %X)\n", GPIO1_START_ADDR, GPIO1_END_ADDR, 
                                           GPIO1_SIZE);
                                           
    printf("Mapping %X - %X (size: %X)\n", GPIO3_START_ADDR, GPIO3_END_ADDR, 
                                           GPIO3_SIZE);

    gpio_addr1 = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 
                        GPIO1_START_ADDR);
                        
    gpio_addr2 = mmap(0, GPIO3_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 
                        GPIO3_START_ADDR);


    gpio_oe_addr1           = gpio_addr1 + GPIO_OE;
    gpio_datain1           = gpio_addr1 + GPIO_DATAIN;
    gpio_setdataout_addr1   = gpio_addr1 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr1 = gpio_addr1 + GPIO_CLEARDATAOUT;
    
    gpio_oe_addr2           = gpio_addr2 + GPIO_OE;
    gpio_datain2           = gpio_addr2 + GPIO_DATAIN;
    gpio_setdataout_addr2   = gpio_addr2 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr2 = gpio_addr2 + GPIO_CLEARDATAOUT;

    if(gpio_addr1 == MAP_FAILED|gpio_addr2 == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    printf("GPIO mapped to %p\n", gpio_addr1);
    printf("GPIO OE mapped to %p\n", gpio_oe_addr1);
    printf("GPIO SETDATAOUTADDR mapped to %p\n", gpio_setdataout_addr1);
    printf("GPIO CLEARDATAOUT mapped to %p\n", gpio_cleardataout_addr1);
    
     // Set GPIO_57 to be an input pin
    // reg1 = *gpio_oe_addr1;
    // printf("GPIO1 configuration: %X\n", reg1);
    // reg1 |= GPIO_57;       // Set GPIO_57 bit to 1
    // *gpio_oe_addr1 = reg1;
    // printf("GPIO1 configuration: %X\n", reg1);   
    
    // Set GPIO_116 to be an input pin
    // reg2 = *gpio_oe_addr2;
    // printf("GPIO3 configuration: %X\n", reg2);
    // reg2 |= GPIO_116;       // Set GPIO_49 bit to 1
    // *gpio_oe_addr2 = reg2;
    // printf("GPIO3 configuration: %X\n", reg2); 

    printf("Start copying GPIO_57 to USR3 and GPIO_49 to USR2\n");
    while(keepgoing) {
    	if(*gpio_datain1& GPIO_57) {
    	    printf("off1\n");
            *gpio_cleardataout_addr1 = USR3;
    	} else {
    	        	    printf("on1\n");
            
            *gpio_setdataout_addr1 = USR3;
    	}
    	
    	if(*gpio_datain2& GPIO_116) {
    	    printf("off2\n");
            *gpio_setdataout_addr1= USR2;
    	} else {
    	     printf("on2\n");
            *gpio_cleardataout_addr1 = USR2;
    	}
        usleep(100000);
    }

    munmap((void *)gpio_addr1, GPIO1_SIZE);
    munmap((void *)gpio_addr2, GPIO3_SIZE);
    close(fd);
    return 0;
}

