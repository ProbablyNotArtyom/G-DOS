OUTPUT_ARCH(m68k)
SEARCH_DIR(.)

PROVIDE (memEnd = 0);

MEMORY
{
  vectors (rx) : ORIGIN = 0x0000, LENGTH = 0x4213
  rom (rx) : ORIGIN = 0x4214, LENGTH = 0x80000-0x4214
  ram (rwx) : ORIGIN = 0x00100000, LENGTH = 0x00200000
}



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
PROVIDE (main = 0);
PROVIDE (_main = 0);

PROVIDE (software_init_hook = 0);
PROVIDE (_software_init_hook = 0);

SECTIONS
{

 .startup : {
 	. = 0x0000;
 	*vectors.o
 	. = ALIGN(0x4);
 } > vectors



 .text : {
 	__start_initsec = .;
 	*(initsec);
 	__stop_initsec = .;

 	__text_start = .;
  	*(.text .text.*)
  	*(.rodata .rodata.*)
	__text_end = .;
	__data_rom = .;
 } > rom


 .data :
 AT ( ADDR(.text) + SIZEOF (.text) ) {

	 __data_start = . ;
	 *(.data);
	 __data_end = . ;
 } > ram


 .bss : {

	__bss_start = .;
  	*(.bss)
  	_end = .;
 } > ram

}
