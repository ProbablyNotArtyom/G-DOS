OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(_start)
SEARCH_DIR(.)

VECSTART = 0xFFF00000;
VECSIZE = 0x00002FFF;
ROMSIZE = 0x01000000;
ROMSTART = 0xFF000000;

PROVIDE (__SDATA_START__ = 0);
PROVIDE (__SBSS_END__ = 0);
PROVIDE (__SDATA2_START__ = 0);
PROVIDE (__SBSS2_END__ = 0);

MEMORY
{
	ram (rwx)		: ORIGIN = 0x00000000, LENGTH = 0x40000000
	rom (rx)		: ORIGIN = ROMSTART, LENGTH = ROMSIZE - VECSIZE
	vectors (rx)	: ORIGIN = VECSTART, LENGTH = VECSIZE
}

STACK_SIZE = 0x2000;

_ram_start = ORIGIN(ram);
_ram_end = ORIGIN(ram) + LENGTH(ram);

_stack_start = _ram_end - STACK_SIZE;
_stack_end = _ram_end;

//HEAP_SIZE = _stack_start - _end;
HEAP_SIZE = 0x20000;

PROVIDE (__STACK = _stack_end);
PROVIDE (__RAM_START = _ram_start);
PROVIDE (__RAM_END = _ram_end);

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{
	. = _ram_start;
	.data : {
		. = ALIGN(0x4);
		__data_start = . ;
		*(.data);
		__data_end = . ;
	} > ram

	.bss : {
		. = ALIGN(0x4);
		__bss_start = .;
		*(.bss)
		*(.bss*)
		*(COMMON)
		_end = .;
	} > ram

	.heap (NOLOAD): {
		. = ALIGN(8);
		__heap_start = .;
		. = . + HEAP_SIZE;
		. = ALIGN(8);
		__heap_end = .;
	} > ram

	. = ROMSTART;
	.text : {
		. = ALIGN(4096);
		__initcall_start = .;
			*(.initcallearly.init)
			__initcall0_start = .;
			*(.initcall0.init)
			__initcall1_start = .;
			*(.initcall1.init)
			__initcall2_start = .;
			*(.initcall2.init)
			__initcall3_start = .;
			*(.initcall3.init)
			__initcall4_start = .;
			*(.initcall4.init)
			__initcall5_start = .;
			*(.initcall5.init)
			__initcall6_start = .;
			*(.initcall6.init)
			__initcall7_start = .;
			*(.initcall7.init)
		__initcall_end = .;
			*(.eh_frame)
		__text_start = .;
		*(.text .text.*)
	. = ALIGN(0x4);
		*(.rodata .rodata.*)
		__text_end = .;

		__data_rom = .;
	} > rom

	. = VECSTART;
	.vectors : {
		*(.vectors)
	} > vectors
}
