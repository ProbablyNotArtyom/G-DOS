//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Diapason FPGA serial port Header

#ifndef _DEV_SERIAL_HEADER
#define _DEV_SERIAL_HEADER

//---------------------------------------------------

charResult versatilepb_serial_dev_write(char out);
char versatilepb_serial_dev_read();
char versatilepb_serial_dev_peek();
charResult versatilepb_serial_dev_init();

//---------------------------------------------------

#endif
