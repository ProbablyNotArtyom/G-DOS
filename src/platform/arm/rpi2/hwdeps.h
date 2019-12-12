//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_RPI2
#define	CONFIG_RPI2

#define CONFIG_CPU_ARM
#define __ARM_EABI__

#define		RAMSTART		0x00000000		// The start of non-code RAM in memory
#define		RAMEND			0x01000000		// End of usable RAM

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define CUSTOM_SPLASH	rpi_splash

// ---------- Driver Support ----------

#define CONFIG_DEV_DUMMY
#define CONFIG_DEV_RPI_UART

// ---------- Disk Interfaces ----------


#define CONFIG_DEV_FATDISK
#define CONFIG_DEV_RAMDISK
#define DEV_RAMDISK_SIZE	256

#define peek_def 		rpi_uart_dev_peek
#define read_def		rpi_uart_dev_read
#define putc_def		rpi_uart_dev_write
#define tty_init_def	rpi_uart_dev_init

#define FF_DRIVER_STRS		"fatdisk", "ramdisk", NULL, NULL
#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		true
#endif
