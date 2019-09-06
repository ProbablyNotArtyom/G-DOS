//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_EMU68K
#define	CONFIG_EMU68K

#define CONFIG_CPU_68020

#define		RAMSTART		0x00000000
#define		RAMEND			0x00400000		// End of usable RAM

#define 	MEMTEST 		true
#define 	ALLOW_UNALIGNED	false			// Set true for anything above 68020

#define		CUSTOM_SPLASH	emu68k_splash

//---------------------------------------------------

// ramdisk size in kilobytes
#define DEV_RAMDISK_SIZE	256

#define CONFIG_DEV_RAMDISK
#define CONFIG_DEV_FATDISK
#define CONFIG_DEV_EMU68KSERIAL

#define peek_def 		emu68k_dev_peek
#define putc_def 		emu68k_dev_write
#define read_def 		emu68k_dev_read
#define tty_init_def 	emu68k_dev_init

#define FF_DRIVER_STRS		"fatdisk", "ramdisk", NULL, NULL
#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		true
#endif
