OUTPUT_ARCH(m68k)
SEARCH_DIR(.)

PROVIDE (memEnd = 0);

MEMORY
{
  vectors (rx) : ORIGIN = 0x0000, LENGTH = 0x3FF
  rom (rx) : ORIGIN = 0x400, LENGTH = 0x7FFFF-0x400
  ram (rwx) : ORIGIN = 0x08000000, LENGTH = 0x00400000
}

STACK_SIZE = 0x2000;

_ram_start = ORIGIN(ram);
_ram_end = ORIGIN(ram) + LENGTH(ram);

_stack_start = _ram_end - STACK_SIZE;
_stack_end = _ram_end;

HEAP_SIZE = (_stack_start - _end);

/*
 * allocate the stack
 */

/*
 * Initialize some symbols to be zero so we can reference them in the
 * crt0 without core dumping. These functions are all optional, but
 * we do this so we can have our crt0 always use them if they exist.
 * This is so BSPs work better when using the crt0 installed with gcc.
 * We have to initialize them twice, so we cover a.out (which prepends
 * an underscore) and coff object file formats.
 */
PROVIDE (__STACK = _stack_end);
PROVIDE (__RAM_START = _ram_start);
PROVIDE (__RAM_END = _ram_end);

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{

 .startup : {
  . = 0x0000;
  *(.vectors)
  . = ALIGN(0x4);
 } > vectors

 .text : {
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

  __data_rom = .;
 } > rom


 .data :
 AT ( ADDR(.text) + SIZEOF (.text) ) {

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

}
