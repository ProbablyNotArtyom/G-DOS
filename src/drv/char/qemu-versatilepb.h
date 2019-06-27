//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Diapason FPGA serial port Header

#ifndef _DEV_QEMU_SERIAL_HEADER
#define _DEV_QEMU_SERIAL_HEADER

//---------------------------------------------------

charResult qemu_serial_dev_write(char out);
char qemu_serial_dev_read();
charResult qemu_serial_dev_init();

//---------------------------------------------------

#endif
