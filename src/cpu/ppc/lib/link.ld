OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
SEARCH_DIR(.)
ENTRY(main)

PROVIDE (__init = 0);
PROVIDE (__SDATA_START__ = 0);
PROVIDE (__SBSS_END__ = 0);
PROVIDE (__SDATA2_START__ = 0);
PROVIDE (__SBSS2_END__ = 0);

MEMORY
{
  ram (rwx) : ORIGIN = 0x100000, LENGTH = 0x01000000
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

 .ARM.exidx : {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
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
