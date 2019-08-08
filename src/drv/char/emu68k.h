//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _DEV_EMU68KSERIAL_HEADER
#define _DEV_EMU68KSERIAL_HEADER

//-----------------Function Protos-------------------

char emu68k_dev_peek();
char emu68k_dev_read();
charResult emu68k_dev_write(char out);
charResult emu68k_dev_init();

#endif
