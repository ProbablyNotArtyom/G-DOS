//---------------------------------------------------
// Hardware Configuration

#ifndef CONFIG_BSVC
#define	CONFIG_BSVC


#define		RAMEND			0x00400000		// End of usable RAM
#define		__STACK			0x00800000		// Stack position in memory

#define 	MEMTEST 		true
#define 	ALLOW_UNALIGNED	false			// Set true for anything above 68020

#endif
