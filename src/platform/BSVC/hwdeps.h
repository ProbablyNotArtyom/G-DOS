//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BSVC
#define	CONFIG_BSVC

#define		RAMEND			0x00400000		// End of usable RAM
#define		__STACK			0x00800000		// Stack position in memory

#define 	MEMTEST 		true
#define 	ALLOW_UNALIGNED	false			// Set true for anything above 68020

//---------------------------------------------------

#define CONFIG_DEV_RAMDISK
#define CONFIG_DEV_BSVCSERIAL

#define default_tty DEV_BSVCSERIAL

#define putc bsvc_dev_write
#define read bsvc_dev_read
#define tty_init bsvc_dev_init

#endif
