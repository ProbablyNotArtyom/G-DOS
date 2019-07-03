OUTPUT_ARCH(m68k)
SEARCH_DIR(.)
ENTRY(main)

MEMORY
{
  ram (rwx) : ORIGIN = 0x08100000, LENGTH = 0x00100000
}

/*
 * Initialize some symbols to be zero so we can reference them in the
 * crt0 without core dumping. These functions are all optional, but
 * we do this so we can have our crt0 always use them if they exist.
 * This is so BSPs work better when using the crt0 installed with gcc.
 * We have to initialize them twice, so we cover a.out (which prepends
 * an underscore) and coff object file formats.
 */
PROVIDE (main = 0);
PROVIDE (_main = 0);

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{

 .text : {

  __text_start = .;
    *(.text .text.*)
  . = ALIGN(0x4);
  __text_end = .;

 } > ram

  . = ALIGN(0x4);
 .rodata : {
 	*(.rodata .rodata.*)
 } > ram

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
    *(.bss.*)
    *(COMMON)
    _end = .;
 } > ram

}
