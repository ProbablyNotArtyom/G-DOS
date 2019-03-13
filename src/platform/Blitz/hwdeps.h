//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BLITZ
#define	CONFIG_BLITZ

#define CONFIG_CPU_68030

#define		RAMSTART		0x04000000		// The start of non-code RAM in memory
#define		RAMEND			0x04400000		// End of usable RAM

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define CUSTOM_SPLASH	blitz_splash

#define CONFIG_DEV_ISA_CGA
#define CONFIG_DEV_ISA_ADLIB
#define CONFIG_DEV_ISA_PATA
#define DEV_ISA_MEM_START	0x00100000
#define DEV_ISA_IO_START	0x00080000

#define CONFIG_DEV_AVRPTOS
#define CONFIG_DEV_VT8242
#define DEV_VT8242_BASE 0x00080060

#define peek vt8242_dev_peek
#define read vt8242_dev_read
#define putc isa_cga_dev_write
#define tty_init isa_cga_dev_init

#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		"hd0"

#endif
