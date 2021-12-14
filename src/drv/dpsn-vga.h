//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Diapason FPGA VGA Controller

#ifndef _DEV_DPSN_VGA_HEADER
#define _DEV_DPSN_VGA_HEADER

// Must be defined: DEV_DP8440_BASE			the start of the dram it is controlling
// Must be defined: DEV_DP8440_MAGIC		the magic being written through the address to the controller config register

//-----------------Function Protos-------------------

charResult dpsn_vga_dev_init();

#endif
