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

	#include <elf.h>
	#include <fs.h>
	#include "boot.h"

//---------------------------------------------------

int loadELF(char* args[], uint8_t argCount, f_file *file){

	size_t highMem = 0;
	size_t lowMem = ~0;
	elf32_header header;
	elf32_program_header progHeader;
	struct bootversion *bVersion;
	struct memBlock	*memSeg;
	struct biRecord	*bootInfo;
	unsigned int progIndex = 0;

	f_seek(file, 0);
	f_read(file, &header, sizeof(header));

	if(header.magic[0] != 0x7F ||
		header.magic[1] != 'E' ||
		header.magic[2] != 'L' ||
		header.magic[3] != 'F'){
			puts("Malformed ELF header");
			return -1;
	}
	if(header.h_class != 1 ||
		header.h_abiversion != 0 ||
		header.h_osabi != 0 ||
		header.h_data != 2){
			puts("ELF file is not 32bit");
			return -1;
	}
	if(header.machine != 4){
		puts("ELF is not m68k native");
		return -1;
	}
	if(header.type != 2){
		puts("ELF is not executable");
		return -1;
	}

	while (progIndex < header.progNum){
		f_seek(file, progIndex * header.progSize + header.progOffset);
		f_read(file, &progHeader, sizeof(progHeader));
		switch (progHeader.type){
			case PHT_NULL:
			case PHT_PHDR:
			case PHT_NOTE:
				break;
			case PHT_DYNAMIC:
			case PHT_SHLIB:
				puts("Dynamically linked ELFs not supported");
				return -1;
			case PHT_LOAD:
				if(progHeader.physAddr < 0x100) {
					progHeader.offset += 0x100;
					progHeader.physAddr += 0x100;
					progHeader.memSize -= 0x100;
					progHeader.fileSize -= 0x100;
				}
				nprintf("Loading %d byte segment from offset 0x%x to address 0x%x",
					progHeader.fileSize, progHeader.offset, progHeader.physAddr);
				//fputs("Loading 0x");
				//printLong(progHeader.fileSize);
				//fputs(" byte segment from offset 0x");
				//printLong(progHeader.offset);
				//fputs(" to address 0x");
				//printLong(progHeader.physAddr);
				f_seek(file, progHeader.offset);
				f_read(file, progHeader.physAddr, progHeader.fileSize);
				if(progHeader.fileSize < progHeader.memSize)
					memset((uint8_t*)progHeader.physAddr + progHeader.fileSize,
						0, progHeader.memSize - progHeader.fileSize);
				if(progHeader.physAddr + progHeader.fileSize > highMem)
					highMem = progHeader.physAddr + progHeader.fileSize;
				if(progHeader.physAddr < lowMem)
					lowMem = progHeader.physAddr;
				break;
			case PHT_INTERP:
				puts("Interpreted ELFs are not supported");
				return -1;
		}
		progIndex++;
	}

	/* Check for a linux kernel */
	bVersion = (struct bootversion *)lowMem;
	if(bVersion->magic == BOOTINFOV_MAGIC){
		fputs("Linux kernel found");
		int i = 0;
		while(bVersion->machversions[i].machine != MACH_BLITZ){
			if(bVersion->machversions[i].machine == 0x0000){
				puts("[!] Blitz machine type not supported.");
				return -1;
			}
			i++;
		}
		if(bVersion->machversions[i].version != BLITZ_BOOTI_VERSION){
			puts("[!] bootinfo version mismatch");
			return -1;
		}

		nprintf("Creating kernel bootinfo at 0x%x", bootInfo);

		/* Machine type */
		bootInfo = (struct biRecord*)((highMem + 0xFFF) & ~0xFFF);
		bootInfo->size = sizeof(struct biRecord) + sizeof(long);
		bootInfo->tag = BI_MACHTYPE;
		bootInfo->entry[0] = MACH_BLITZ;

		/* CPU type */
		bootInfo = (struct biRecord *)((uint8_t*)bootInfo + bootInfo->size);
		bootInfo->size = sizeof(struct biRecord) + sizeof(long);
		bootInfo->tag = BI_CPUTYPE;
		bootInfo->entry[0] = CPU_68030;

		/* MMU type */
		bootInfo = (struct biRecord *)((uint8_t*)bootInfo + bootInfo->size);
		bootInfo->size = sizeof(struct biRecord) + sizeof(long);
		bootInfo->tag = BI_MMUTYPE;
		bootInfo->entry[0] = MMU_68030;

		/* FPU type */
		bootInfo = (struct biRecord *)((uint8_t*)bootInfo + bootInfo->size);
		bootInfo->size = sizeof(struct biRecord) + sizeof(long);
		bootInfo->tag = BI_FPUTYPE;
		bootInfo->entry[0] = FPU_68882;

		/* FPU type */
		bootInfo = (struct biRecord *)((uint8_t*)bootInfo + bootInfo->size);
		bootInfo->size = sizeof(struct biRecord) + sizeof(struct memBlock);
		bootInfo->tag = BI_MEMBLOCK;
		memSeg = (struct memBlock *)bootInfo->entry;
		memSeg->addr = 0x800000;		// Memory address
		memSeg->size = 0x500000;		// Memory size

		char kernelArgs[ARGBUFF];
		int argLen;
		int charsLeft = ARGBUFF - 1;
		for(i = 1; i < argCount; i++){
			if(kernelArgs[0] && charsLeft > 0){
				strncat(kernelArgs, " ", charsLeft);
				charsLeft -= 1;
			}
			argLen = strlen(args[i]);
			if(charsLeft >= argLen) {
				strncat(kernelArgs, args[i], charsLeft);
				charsLeft -= 1;
			} else {
				puts("[!] Kernel arguments exceed buffer length");
				return -1;
			}
		}

		/* -=-=-=-=-=-=-=-=-=- */
	}
}


int loadELF_flat(char* args[], uint8_t argCount, char *loadAddr){
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
