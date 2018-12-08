 //---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Loading ELFs into memory & linking

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <fs.h>

//-----------------Function Protos-------------------

int loadELF(struct fsFile file, uint32_t *address);

//---------------------------------------------------

int loadELF(struct fsFile file, uint32_t *address){
	char* loadAddr = file.data;
	uint32_t entry, progTable, secTable, progSize, secSize;
	uint32_t numProgs, numSecs, nameTable;
	uint8_t type;
	if (*(loadAddr+4) == 2) {
		puts("64-bit binaries are not supported!");
		return -1;
	}
	puts("ELF32-m68k ");
	if (*(loadAddr+5) == 1) {
		puts("Little-endian binaries are not supported!");
		return -1;
	} 
	puts("Big-endian");
	if (*(uint16_t*)(loadAddr+0x10) == 0x01){ puts("Relocatable binary"); type = 0;}
	else if (*(uint16_t*)(loadAddr+0x10) == 0x02){ puts("Executable binary"); type = 1;}
	else if (*(uint16_t*)(loadAddr+0x10) == 0x03){ puts("Dynamic binary"); type = 1;} //2
	
	entry = *(uint32_t*)(loadAddr+0x18);
	progTable = *(uint32_t*)(loadAddr+0x1C);
	secTable = *(uint32_t*)(loadAddr+0x20);
	progSize = *(uint16_t*)(loadAddr+0x2A);
	secSize = *(uint16_t*)(loadAddr+0x2E);
	numProgs = *(uint16_t*)(loadAddr+0x2C);
	numSecs = *(uint16_t*)(loadAddr+0x30);
	nameTable = *(uint16_t*)(loadAddr+0x32);

	if (type == 0){

	} else if (type == 1){
		uint8_t *paddr, *segmentPtr;
		while (numProgs > 0){
			paddr = *(uint32_t*)(progTable+loadAddr+0x0C);
			segmentPtr = *(uint32_t*)(progTable+loadAddr+0x04) + loadAddr;
			fputs("Loading segment from offset 0x");
			printLong(progTable);
			fputs(" to address 0x");
			printLong(paddr); 
			uint32_t segLen = *(uint32_t*)(progTable+loadAddr+0x10);
			while (segLen > 0){
				*paddr++ = *segmentPtr++;
				if ((segLen & 0x00000FFF) == 0x000000000) putc('.');
				segLen--;
			}
			puts(" OK");
			do {
				numProgs--;
				progTable += progSize;
			} while (*(uint32_t*)(progTable+loadAddr) != 0x00000001 && (numProgs > 0));
		}

	} else {
	}
	int (*entrypoint)(int, char*) = entry;
	(*entrypoint)(0, "");
}