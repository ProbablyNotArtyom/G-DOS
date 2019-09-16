OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(_start)
SEARCH_DIR(.)

VECSTART = 0xFFF00000;
VECEND = 0xFFF02FFF;
ROMSIZE = 0x01000000;
ROMSTART = 0xFF000000;

PROVIDE (__SDATA_START__ = 0);
PROVIDE (__SBSS_END__ = 0);
PROVIDE (__SDATA2_START__ = 0);
PROVIDE (__SBSS2_END__ = 0);

MEMORY
{
  ram (rwx) : ORIGIN = 0x00000000, LENGTH = 0x02000000
  rom (rx)	: ORIGIN = ROMSTART, LENGTH = ROMSIZE
}

STACK_SIZE = 0x2000;

_ram_start = 0x00000000;
_ram_end = 0x01000000;

_stack_start = _ram_end - STACK_SIZE;
_stack_end = _ram_end;

HEAP_SIZE = _stack_start - _end;

PROVIDE (__STACK = _stack_end);

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{
 . = _ram_start;
 .data : {
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

  __text_start = .;
     *(.text .text.*)
     . = ALIGN(0x4);
     *(.rodata .rodata.*)
  __text_end = .;
  . = VECSTART - ROMSTART;
  *(.vectors)
 } > rom

}
