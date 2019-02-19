//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// pata Header

#ifndef _DEV_ISA_HEADER
#define _DEV_ISA_HEADER

//---------------------------------------------------

#define isa_mem_vtor(addr)			DEV_ISA_MEM_START + addr
#define isa_io_vtor(addr)			DEV_ISA_IO_START + addr

#define isa_io_outb(val, addr)		outb(val, isa_io_vtor(addr))
#define isa_io_inb(addr)			inb( isa_io_vtor(addr))
#define isa_mem_outb(val, addr)		outb(val, isa_mem_vtor(addr))
#define isa_mem_inb(addr)			inb( isa_mem_vtor(addr))

//---------------------------------------------------

#endif
