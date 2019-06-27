 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // Diapason FPGA serial port Driver

 #ifdef CONFIG_DEV_QEMU_VERSATILEPB_UART
 #define CONFIG_DEV_QEMU_VERSATILEPB_UART

	#include <std.h>
	#include <mod/init.h>
	#include "qemu-versatilepb.h"

//---------------------------------------------------

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
volatile unsigned int * const UART0FR = (unsigned int *)0x101f1018;

//--------------------Functions----------------------

charResult qemu_serial_dev_write(char out){
	*UART0DR = (unsigned int)(out);
	return CH_OK;
}

char qemu_serial_dev_read(){
	while(1){
		if (*UART0FR & (1<<6)) break;
	}
	return *UART0DR;
}

charResult qemu_serial_dev_init(){
	puts("\r\nQEMU-versatilepb UART driver / NotArtyom / 27-6-19");
	return CH_OK;
}

late_initcall(qemu_serial_dev_init);

 #endif
