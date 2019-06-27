//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_QEMU_VERSATILEPB
#define	CONFIG_QEMU_VERSATILEPB

#define CONFIG_CPU_ARM

#define		RAMSTART		0x00000000		// The start of non-code RAM in memory
#define		RAMEND			0x01000000		// End of usable RAM

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#define CUSTOM_SPLASH	qemu_splash

// ---------- Driver Support ----------

#define CONFIG_DEV_DUMMY
#define CONFIG_DEV_QEMU_VERSATILEPB_UART

// ---------- Disk Interfaces ----------

#define CONFIG_DEV_RAMDISK
#define DEV_RAMDISK_SIZE	256

#define peek dummy_dev_peek
#define read qemu_serial_dev_read
#define putc qemu_serial_dev_write
#define tty_init qemu_serial_dev_init

#define FF_VOLUME_STRS		"hd0", "hd1", "fd0", "fd1"
#define DISK_AUTOMOUNT		"hd0"

#endif
