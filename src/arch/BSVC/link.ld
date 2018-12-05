OUTPUT_ARCH(m68k)
SEARCH_DIR(.)

MEMORY
{
  vectors (rx) : ORIGIN = 0x0000, LENGTH = 0x4213
  ram (rwx) : ORIGIN = 0x4214, LENGTH = 0x300000-0x4214
}

/*
 * allocate the stack
 */

PROVIDE (__stack = 0);

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
 . = 0x0000;
 .startup : {
 	*vectors.o (.data)
 	. = ALIGN(0x4);
 } > vectors
 
 . = 0x10000;
 .ro : {
  	*(.text)
  	*(.rodata)
 } > ram
 .init : {
 	__start_initsec = .;
 	*(initsec);
 	__stop_initsec = .;
 } > ram
 .rw : {
  	*(.data)
  	__bss_start = .;
  	*(.bss)
  	_end = .;
  	*(COMMON)
 } > ram
 . = ALIGN(8);
 heap_low = .; /* for _sbrk */
 . = . + 0x10000; /* 64kB of heap memory */
 heap_top = .; /* for _sbrk */
 . = . + 0x10000; /* 64kB of stack memory */
 stack_top = .; /* for startup.s */
}