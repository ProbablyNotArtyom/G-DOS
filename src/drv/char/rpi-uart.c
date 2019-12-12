 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // Raspberry pi UART driver

 #ifdef CONFIG_DEV_RPI_UART
 #define CONFIG_DEV_RPI_UART

	#include <std.h>
	#include <mod/init.h>
	#include <io.h>
	#include <platdep/rpi-gpio.h>
	#include <platdep/rpi-mailbox.h>
	#include "rpi-uart.h"

//--------------------Functions----------------------

charResult rpi_uart_dev_write(char out){
    while (inl(UART0_FR) & 0x20) asm("");	// Wait until we can send
    outl(out, UART0_DR);
	return CH_OK;
}

char rpi_uart_dev_read(){
	char tmp;

	while(inl(UART0_FR) & 0x10) asm("");	// Wait until something is in the buffer
    tmp = inb(UART0_DR);				// Input char
    return tmp;
}

charResult rpi_uart_dev_init() {
	unsigned int tmp;

    outl(0, UART0_CR);       			// turn off UART0

    /* set up clock */
    mbox[0] = (9 * 4);
    mbox[1] = MBOX_REQUEST;
    mbox[2] = MBOX_TAG_SETCLKRATE;		// set clock rate
    mbox[3] = 12;
    mbox[4] = 8;
    mbox[5] = 2;           				// UART clock
    mbox[6] = 4000000;     				// 4Mhz
    mbox[7] = 0;           				// clear turbo
    mbox[8] = MBOX_TAG_LAST;
    mbox_call(MBOX_CH_PROP);

    /* map UART0 to GPIO pins */
    tmp = inl(GPFSEL1);
    tmp &= ~((7 << 12) | (7 << 15)); 	// gpio14, gpio15
    tmp |= (4 << 12) | (4 << 15);    	// alt0
    outl(tmp, GPFSEL1);

	outl(0, GPPUD);            			// enable pins 14 and 15
    for (tmp = 500; tmp > 0; tmp--) asm("");
    outl(((1 << 14) | (1 << 15)), GPPUDCLK0);
    for (tmp = 500; tmp > 0; tmp--) asm("");
    outl(0, GPPUDCLK0);        			// flush GPIO setup

    outl(0x7FF, UART0_ICR);    			// clear interrupts
    outl(1, UART0_IBRD);       			// 115200 baud
    outl(40, UART0_FBRD);
    outl((1 << 4) | (1 << 5) | (1 << 6), UART0_LCRH);

	outl(0x7F2, UART0_IMSC);
    outl((1 << 0) | (1 << 8) | (1 << 9), UART0_CR);

	for (tmp = 500; tmp > 0; tmp--) asm("");
	puts("Raspberry pi UART driver / NotArtyom / 11-12-19");

	return CH_OK;
}

char rpi_uart_dev_peek() {
		return NULL;
}

early_initcall(rpi_uart_dev_init);

#endif
