//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BLITZ
#define	CONFIG_BLITZ

#define CONFIG_CPU_68030

#define		RAMSTART		0x08000000		// The start of non-code RAM in memory
#define		RAMEND			0x08400000		// End of usable RAM

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define 	CUSTOM_SPLASH	blitz_splash

// ---------- Driver Support ----------

#define CONFIG_DEV_ISA_CGA
#define DEV_ISA_MEM_START	0x04200000
#define DEV_ISA_IO_START	0x04000000

#define CONFIG_DEV_VT8242
#define DEV_VT8242_BASE 	0x00180000

#define CONFIG_DEV_DP8440
#define DEV_DP8440_BASE		0x0C000000
#define DEV_DP8440_MAGIC	0x0C34FC4D

// ---------- Disk Interfaces ----------

#define CONFIG_DEV_BLITZ_IDE
#define BLITZ_IDE_BASE		0x00100000

#define CONFIG_DEV_FATDISK
#define CONFIG_DEV_RAMDISK
#define DEV_RAMDISK_SIZE	256

#define peek_def 		vt8242_dev_peek
#define read_def 		vt8242_dev_read
#define putc_def 		isa_cga_dev_write
#define tty_init_def	isa_cga_dev_init

#define FF_DRIVER_STRS		"fatdisk", "ramdisk", NULL, NULL
#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		true

#endif
