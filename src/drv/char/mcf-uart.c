 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // MCF coldfire uart driver

 #ifdef CONFIG_DEV_MCF_UART
 #define CONFIG_DEV_MCF_UART

	#include <std.h>
	#include <mod/init.h>
	#include <io.h>
	#include "mcf-uart.h"

#define UART0_BASE 		0xFC060000

//--------------------Functions----------------------

charResult mcf_uart_dev_write(char out){
	for (int i = 0; (i < 0x10000); i++) {
		if (readb(UART0_BASE + MCFUART_USR) & MCFUART_USR_TXREADY) break;
	}
	outb(out, UART0_BASE + MCFUART_UTB);
	for (int i = 0; (i < 0x10000); i++) {
		if (readb(UART0_BASE + MCFUART_USR) & MCFUART_USR_TXREADY)
			break;
	}
	return CH_OK;
}

char mcf_uart_dev_read(){
	return 'c';
}

charResult mcf_uart_dev_init(){
	outb(MCFUART_UCR_CMDRESETRX, UART0_BASE + MCFUART_UCR);
	outb(MCFUART_UCR_CMDRESETTX, UART0_BASE + MCFUART_UCR);
	outb(MCFUART_UCR_RXENABLE | MCFUART_UCR_TXENABLE, UART0_BASE + MCFUART_UCR);
	puts("\r\nColdfire UART driver / NotArtyom / 27-6-19");
	return CH_OK;
}

early_initcall(mcf_uart_dev_init);

 #endif
