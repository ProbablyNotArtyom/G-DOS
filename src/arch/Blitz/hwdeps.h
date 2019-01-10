//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BLITZ
#define	CONFIG_BLITZ


#define		RAMSTART		0x00100000		// The start of non-code RAM in memory
#define		RAMEND			0x00200000		// End of usable RAM
#define		__STACK			0x00300000		// Stack position in memory

#define 	MEMTEST 		true
#define 	ALLOW_UNALIGNED	true			// Set true for anything above 68020

#endif
