OUTPUT_ARCH(arm)
SEARCH_DIR(.)

PROVIDE (memEnd = 0);

MEMORY
{
  ram (rwx) : ORIGIN = 0x00008000, LENGTH = 0x10000000
}

STACK_SIZE = 0x2000;

_ram_start = ORIGIN(ram);
_ram_end = ORIGIN(ram) + LENGTH(ram);

_stack_start = _ram_end - STACK_SIZE;
_stack_end = _ram_end;

HEAP_SIZE = _stack_start - _ram_start - (__data_end - __data_start) - (__text_end - __text_start) - (_end - __bss_start);

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

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{
 . = 0x8000;

 .text : {
    *(.vectors)
    . = ALIGN(0x4);
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
 } > ram

 .ARM.exidx : {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
 } > ram

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
	. = ALIGN(8);
	__heap_end = .;
 } > ram
}
