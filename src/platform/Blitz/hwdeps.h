//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BLITZ
#define	CONFIG_BLITZ


#define		RAMSTART		0x04000000		// The start of non-code RAM in memory
#define		RAMEND			0x04100000		// End of usable RAM
#define		__STACK			0x04100000		// Stack position in memory

#define 	MEMTEST 		false
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#endif
