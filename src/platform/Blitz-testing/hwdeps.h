//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BLITZ
#define	CONFIG_BLITZ


#define		RAMSTART		0x04000000		// The start of non-code RAM in memory
#define		RAMEND			0x04100000		// End of usable RAM
#define		__STACK			0x04100000		// Stack position in memory

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define CUSTOM_SPLASH	blitz_splash

#define CONFIG_DEV_ISA_CGA
#define DEV_ISA_MEM_START	0x00100000
#define DEV_ISA_IO_START	0x00300000

#define CONFIG_DEV_AVRPTOS
#define CONFIG_DEV_VT8242
#define DEV_VT8242_BASE 0x00300060

#define read vt8242_dev_read
#define putc isa_cga_dev_write
#define tty_init isa_cga_dev_init

#endif
