 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // CGAterm

 #ifdef CONFIG_DEV_ISA_ADLIB
 #define CONFIG_DEV_ISA_ADLIB

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <io.h>
	#include <mod/init.h>

	#include "isa.h"
	#include "adlib.h"

//---------------------------------------------------

//---------------------------------------------------

char isa_adlib_dev_read(){
	while(1);
}

charResult isa_adlib_dev_write(char out){

	return CH_OK;
}

charResult isa_adlib_dev_init(){
	puts("Adlib sound card driver / NotArtyom / 6-2-19");

	/* Detect sound card */
	isa_io_outb(OPL3_TIMER_CTL, OPL3_REG_INDEX);
	isa_io_outb(0x60, OPL3_REG_DATA);
	isa_io_outb(OPL3_TIMER_CTL, OPL3_REG_INDEX);
	isa_io_outb(0x80, OPL3_REG_DATA);
	uint8_t pre = isa_io_inb(OPL3_REG_INDEX) & 0xE0;
	isa_io_outb(OPL3_T1_COUNT, OPL3_REG_INDEX);
	isa_io_outb(0xFF, OPL3_REG_DATA);
	isa_io_outb(OPL3_TIMER_CTL, OPL3_REG_INDEX);
	isa_io_outb(0x21, OPL3_REG_DATA);
	delay(0x1FFFF);
	uint8_t post = isa_io_inb(OPL3_REG_INDEX) & 0xE0;
	isa_io_outb(OPL3_TIMER_CTL, OPL3_REG_INDEX);
	isa_io_outb(0x60, OPL3_REG_DATA);
	isa_io_outb(OPL3_TIMER_CTL, OPL3_REG_INDEX);
	isa_io_outb(0x80, OPL3_REG_DATA);

	if (pre == 0x00 && post == 0xC0){
		puts("Adlib or compatible detected");
	} else {
		puts("Adlib not detected.");
	}

	return CH_OK;
}

//--------------------Functions----------------------

device_initcall(isa_adlib_dev_init);

 #endif
