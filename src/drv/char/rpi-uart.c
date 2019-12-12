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
	#include <platdep/rpi-gpio.h>
	#include <platdep/rpi-mailbox.h>
	#include "rpi-uart.h"

//--------------------Functions----------------------

charResult rpi_uart_dev_write(char out){
    while (*UART0_FR & 0x20) asm("");	// Wait until we can send
    *UART0_DR = out;
	return CH_OK;
}

char rpi_uart_dev_read(){
	char tmp;

	while(*UART0_FR & 0x10) asm("");	// Wait until something is in the buffer
    tmp = (char)(*UART0_DR);			// Input char
    return tmp;
}

charResult rpi_uart_dev_init(){
	unsigned int tmp;

    *UART0_CR = 0;         				// turn off UART0

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
    tmp = *GPFSEL1;
    tmp &= ~((7 << 12) | (7 << 15)); 	// gpio14, gpio15
    tmp |= (4 << 12) | (4 << 15);    	// alt0
    *GPFSEL1 = tmp;

	*GPPUD = 0;            				// enable pins 14 and 15
    for (tmp = 300; tmp > 0; tmp--) asm("");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (tmp = 300; tmp > 0; tmp--) asm("");
    *GPPUDCLK0 = 0;        				// flush GPIO setup

    *UART0_ICR = 0x7FF;    				// clear interrupts
    *UART0_IBRD = 2;       				// 115200 baud
    *UART0_FBRD = 0xB;
    *UART0_LCRH = 0b11 << 5;			// 8n1
    *UART0_CR = 0x301;     				// enable Tx, Rx, FIFO

	return CH_OK;
}

char rpi_uart_dev_peek() {
		return NULL;
}

#endif
