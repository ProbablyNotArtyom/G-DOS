
	#include <std.h>
	#include <stdbool.h>

#define		RAMSTART		0x04000000					// The start of non-code RAM in memory
#define		RAMEND			0x04100000					// End of usable RAM
#define 	MEMTEST 		true
#define 	ALLOW_UNALIGNED	false

int init(void){
	#if MEMTEST == true
	fputs("Testing RAM");
	bool pass = true;
	#if ALLOW_UNALIGNED == false
		char *memPtr = RAMSTART;
		while ((uint32_t)memPtr & 0x0000000F != 0){
			*memPtr = 0xAA;
			if (*memPtr != 0xAA) pass = false;
			*memPtr = 0x55;
			if (*memPtr != 0x55) pass = false;
			*memPtr = 0x00;
			memPtr++;
		}
		long *memLPtr = memPtr;
		while (memLPtr <= RAMEND){
			*memLPtr = 0xAAAAAAAA;
			if (*memLPtr != 0xAAAAAAAA) pass = false;
			*memLPtr = 0x55555555;
			if (*memLPtr != 0x55555555) pass = false;
			*memLPtr = 0x00000000;
			memLPtr++;
			if (((uint32_t)memLPtr & 0x0000FFFF) == 0x00001000) putc('.');
		}
	#else
		long *memLPtr = memPtr;
		while (memLPtr <= RAMEND){
			*memLPtr = 0xAAAAAAAA;
			if (*memLPtr != 0xAAAAAAAA) pass = false;
			*memLPtr = 0x55555555;
			if (*memLPtr != 0x55555555) pass = false;
			*memLPtr = 0x00000000;
			memLPtr++;
			if ((memLPtr & 0x0000FFFF) == 0x00001000) putc('.');
		}
	#endif
		if (pass) puts(" PASS");
		else {
			puts(" FAIL!");
			return -1;
		}
		#endif

	return 1;
}

//----------------------Rescue-----------------------

void _BError(void){
	puts("\n\r[BUS ERROR]");
	while (1);
}

void _AError(void){
	puts("\n\r[ADDRESS ERROR]");
	while (1);
}

