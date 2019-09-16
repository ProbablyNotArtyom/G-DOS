OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(main)
SEARCH_DIR(.)

PROVIDE (__SDATA_START__ = 0);
PROVIDE (__SBSS_END__ = 0);
PROVIDE (__SDATA2_START__ = 0);
PROVIDE (__SBSS2_END__ = 0);

MEMORY
{
  ram (rwx) : ORIGIN = 0x01000000, LENGTH = 0x01000000
}

SECTIONS
{
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

 .text : {
    . = ALIGN(0x4);
  __text_start = .;
     *(.text .text.*)
     . = ALIGN(0x4);
     *(.rodata .rodata.*)
  __text_end = .;
 } > ram

}
