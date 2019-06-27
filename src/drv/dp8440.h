//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#ifndef _DEV_DP8440_HEADER
#define _DEV_DP8440_HEADER

// Must be defined: DEV_DP8440_BASE			the start of the dram it is controlling
// Must be defined: DEV_DP8440_MAGIC		the magic being written through the address to the controller config register

//-----------------Function Protos-------------------

charResult dp8440_dev_init();

#endif
