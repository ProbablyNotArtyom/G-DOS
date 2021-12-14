//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Diapason FPGA serial port Header

#ifndef _DEV_DPSN_SERIAL_HEADER
#define _DEV_DPSN_SERIAL_HEADER

//---------------------------------------------------

struct dpsn_serial_regs {
	uint8_t	d_out;
	uint8_t	d_in;
	uint8_t	status;
	uint8_t config;
};

//---------------------------------------------------

charResult dpsn_serial_dev_write(char out);
char dpsn_serial_dev_read();
charResult dpsn_serial_dev_init();

//---------------------------------------------------

#endif
