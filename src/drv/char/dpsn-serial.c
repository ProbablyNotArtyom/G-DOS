 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // Diapason FPGA serial port Driver

 #ifdef CONFIG_DEV_DPSN_SERIAL
 #define CONFIG_DEV_DPSN_SERIAL

	#include <std.h>
	#include "dpsn-serial.h"

//---------------------------------------------------



//--------------------Functions----------------------

charResult dpsn_serial_dev_write(char out){
	return CH_NOTRDY;
}

char dpsn_serial_dev_read(){
	while(1);
}

charResult dpsn_serial_dev_init(){
	return CH_OK;
}

 #endif
