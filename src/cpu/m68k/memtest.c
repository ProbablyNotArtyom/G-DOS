
	#include <std.h>
	#include <stdbool.h>
	#include <mod/init.h>

	extern char __bss_start;
	extern char _end;

int init(void){
	#if MEMTEST == true
	fputs("Testing RAM");
	bool pass = true;
		char *memPtr = &__bss_start;
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
		if (pass) puts(" PASS");
		else {
			puts(" FAIL!");
			return -1;
		}
	#endif

	return 1;
}

early_initcall(init);

//----------------------Rescue-----------------------

void _BError(void){
	puts("\n\r[BUS ERROR]");
	while (1);
}

void _AError(void){
	puts("\n\r[ADDRESS ERROR]");
	while (1);
}
