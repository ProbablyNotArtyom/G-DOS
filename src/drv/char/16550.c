 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // ppc e500 uart driver

 #ifdef CONFIG_DEV_16550_UART
 #define CONFIG_DEV_16550_UART

	#include <std.h>
	#include <mod/init.h>
	#include <io.h>
	#include "16550.h"

#ifndef DEV_16550_UART_BASE
#error "[!] DEV_16550_UART_BASE must be defined."
#endif

#define THR(n) (DEV_16550_UART_BASE + UART_16550_REG_THR)
#define RDR(n) (DEV_16550_UART_BASE + UART_16550_REG_RDR)
#define BRDL(n) (DEV_16550_UART_BASE + UART_16550_REG_BRDL)
#define BRDH(n) (DEV_16550_UART_BASE + UART_16550_REG_BRDH)
#define IER(n) (DEV_16550_UART_BASE + UART_16550_REG_IER)
#define IIR(n) (DEV_16550_UART_BASE + UART_16550_REG_IIR)
#define FCR(n) (DEV_16550_UART_BASE + UART_16550_REG_FCR)
#define LCR(n) (DEV_16550_UART_BASE + UART_16550_REG_LCR)
#define MDC(n) (DEV_16550_UART_BASE + UART_16550_REG_MDC)
#define LSR(n) (DEV_16550_UART_BASE + UART_16550_REG_LSR)
#define MSR(n) (DEV_16550_UART_BASE + UART_16550_REG_MSR)
#define DLF(n) (DEV_16550_UART_BASE + UART_16550_REG_DLF)

//--------------------Functions----------------------

charResult uart_16550_dev_write(char out){
	while (inb(LSR()) & UART_16550_LSR_THRE == 0);
	outb(out, THR());
	return CH_OK;
}

char uart_16550_dev_read(){
	while (inb(LSR()) & UART_16550_LSR_RXRDY == 0);
	return inb(RDR());
}

charResult uart_16550_dev_init(){
	puts("\r\Generic 16550 UART driver / NotArtyom / 27-6-19");
	return CH_OK;
}

early_initcall(uart_16550_dev_init);

 #endif
