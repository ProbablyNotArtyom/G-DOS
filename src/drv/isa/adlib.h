//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _DEV_ISA_ADLIB_HEADER
#define _DEV_ISA_ADLIB_HEADER

//---------------------------------------------------

#define opl3_mem_vtor(addr)		DEV_ISA_MEM_START + addr
#define opl3_io_vtor(addr)		DEV_ISA_IO_START + addr

#define opl3_io_outb(val, addr)		outb(val, opl3_io_vtor(addr))
#define opl3_io_inb(addr)			inb( opl3_io_vtor(addr))
#define opl3_mem_outb(val, addr)		outb(val, opl3_mem_vtor(addr))
#define opl3_mem_inb(addr)			inb( opl3_mem_vtor(addr))


#define OPL3_REG_INDEX			0x388
#define OPL3_REG_DATA			0x389
#define OPL3_REG_SUB_INDEX		0x38A
#define OPL3_REG_SUB_DATA		0x38B

#define OPL3_INDEX_TEST			0x01
#define OPL3_T1_COUNT			0x02
#define OPL3_T2_COUNT			0x03
#define OPL3_TIMER_CTL			0x04
#define OPL3_INDEX_TEST			0x01

//-----------------Function Protos-------------------

#endif
