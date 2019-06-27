//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Diapason FPGA VGA Controller

#ifdef CONFIG_DEV_DPSN_VGA
#define CONFIG_DEV_DPSN_VGA

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <mod/init.h>
	#include <io.h>
	#include "dpsn-vga.h"

//--------------------Functions----------------------

charResult dpsn_vga_dev_init(){
	puts("---- / NotArtyom / 26-4-19");
	return CH_OK;
}

//---------------------------------------------------

device_initcall(dpsn_vga_dev_init);

#endif
