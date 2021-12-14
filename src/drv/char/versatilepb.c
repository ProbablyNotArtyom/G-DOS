 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // Diapason FPGA serial port Driver

 #ifdef CONFIG_DEV_VERSATILEPB_UART
 #define CONFIG_DEV_VERSATILEPB_UART

	#include <std.h>
	#include <mod/init.h>
	#include "versatilepb.h"

//---------------------------------------------------

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
volatile unsigned int * const UART0FR = (unsigned int *)0x101f1018;
char lastVal;

//--------------------Functions----------------------

charResult versatilepb_serial_dev_write(char out){
	*UART0DR = (unsigned int)(out);
	return CH_OK;
}

char versatilepb_serial_dev_read(){
	while(1){
		if (*UART0FR & (1<<6)) break;
	}
	return *UART0DR;
}

char versatilepb_serial_dev_peek(){
	if (*UART0FR & (1<<6)) return *UART0DR;
	else return NULL;
}

charResult versatilepb_serial_dev_init(){
	puts("\r\nQEMU-versatilepb UART driver / NotArtyom / 27-6-19");
	return CH_OK;
}

early_initcall(versatilepb_serial_dev_init);

 #endif
