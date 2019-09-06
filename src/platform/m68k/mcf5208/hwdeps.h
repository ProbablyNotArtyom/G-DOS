//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_MCF5208
#define	CONFIG_MCF5208

#define CONFIG_CPU_MCF5208

#define		RAMSTART		0x08000000		// The start of non-code RAM in memory
#define		RAMEND			0x08400000		// End of usable RAM

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define CUSTOM_SPLASH	mcf_splash

// ---------- Driver Support ----------

#define CONFIG_DEV_DUMMY
#define CONFIG_DEV_MCF_UART

// ---------- Disk Interfaces ----------

#define CONFIG_DEV_FATDISK
#define CONFIG_DEV_RAMDISK
#define DEV_RAMDISK_SIZE	256

#define peek dummy_dev_peek
#define read mcf_uart_dev_read
#define putc mcf_uart_dev_write
#define tty_init mcf_uart_dev_init

#define FF_DRIVER_STRS		"fatdisk", "ramdisk", NULL, NULL
#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		true

#endif
