 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // DP8440 DRAM Controller

 #ifdef CONFIG_DEV_DP8440
 #define CONFIG_DEV_DP8440

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <mod/init.h>
	#include <io.h>
	#include "dp8440.h"

//--------------------Functions----------------------

charResult dp8440_dev_init(){
	puts("DP8440 DRAM Controller / NotArtyom / 26-4-19");
	nprintf("Current config value:   0x%08X", DEV_DP8440_MAGIC);
	nprintf("DRAM memory base:       0x%08X", DEV_DP8440_BASE);

	// Write a dummy byte to the address the DP8440 is listening at for configuration
	// The configuration settings are latched from the address bus
	outb(0xFF, DEV_DP8440_MAGIC);
	return CH_OK;
}

//---------------------------------------------------

device_initcall(dp8440_dev_init);

#endif
