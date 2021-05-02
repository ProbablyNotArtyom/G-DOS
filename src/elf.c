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
	#include <flags.h>

	#include <elf.h>
	#include <fs.h>
	#include <boot.h>
	#include <debug.h>

//---------------------------------------------------

const char *const cpu_lookup[] = {
	"NONE",
	"M32",
	"SPARC",
	"386",
	"68K",
	"88K",
	"IAMCU",
	"860",
	"MIPS",
	"S370",
	"MIPS_RS3_LE",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"PARISC",
	"UNKNOWN",
	"VPP500",
	"SPARC32PLUS",
	"960",
	"PPC",
	"PPC64",
	"S390",
	"SPU",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"V800",
	"FR20",
	"RH32",
	"RCE",
	"ARM"
};

//---------------------------------------------------

static endianness_t endianness_detect(void) {
    const int i = 1;
    return ((const char *)(&i)[3] == 1)? HDATA_LITTLE : HDATA_BIG;
}

int loadELF(int argCount, char* args[], FIL *file) {
	size_t numBytes, highMem = 0, progIndex = 0, lowMem = ~0;
	elf32_header header;
	elf32_program_header progHeader;
	struct bootversion *bVersion;
	struct memBlock	*memSeg;
	struct biRecord	*bootInfo;

	f_lseek(file, 0);
	f_read(file, &header, sizeof(header), &numBytes);

	if (header.ident.magic[0] != 0x7F ||
		header.ident.magic[1] != 'E' ||
		header.ident.magic[2] != 'L' ||
		header.ident.magic[3] != 'F') {
			puts("[!] Not an ELF file");
			return -1;
	}

	if (header.ident.data != endianness_detect()) {
		if (header.ident.data == HDATA_LITTLE) puts("[!] Little endian ELFs are not supported on this machine");
		else if (header.ident.data == HDATA_BIG) puts("[!] Big endian ELFs are not supported on this machine");
		else puts("[!] ELF file has invalid endianness");
		return -1;
	}

	if (header.machine != CPU_ARCH) {
		printf("[!] Incompatible CPU arch: expected %s, but found %s\n", cpu_lookup[CPU_ARCH], cpu_lookup[header.machine]);
		return -1;
	}

	debug_printf("[.] CPU: %s\n", cpu_lookup[header.machine]);
	switch (header.type) {
		case ET_REL:
			puts_debug("[.] ELF type: relocatable");
			break;
		case ET_EXEC:
			puts_debug("[.] ELF type: executable");
			break;
		case ET_DYN:
			puts("[!] Cannot load ELF: loading shared objects is not supported");
			return -1;
		default:
			puts("[!] Cannot load ELF: invalid type flag (file probably corrupted)");
			return -1;
	}

	while (progIndex < header.progNum) {
		f_lseek(file, progIndex * header.progSize + header.progOffset);
		f_read(file, &progHeader, sizeof(progHeader), &numBytes);
		switch (progHeader.type) {
			case PT_NULL:
			case PT_PHDR:
			case PT_NOTE:
				break;
			case PT_DYNAMIC:
			case PT_SHLIB:
				puts("[!] Dynamically linked ELFs not supported");
				return -1;
			case PT_LOAD:
				debug_printf("[.] Loading %d byte segment from offset 0x%x to address 0x%x\n\r",
					progHeader.fileSize, progHeader.offset, progHeader.physAddr);
				f_lseek(file, progHeader.offset);
				f_read(file, progHeader.physAddr, progHeader.fileSize, &numBytes);
				if (progHeader.fileSize < progHeader.memSize)
					memset((uint8_t*)progHeader.physAddr + progHeader.fileSize, 0, progHeader.memSize - progHeader.fileSize);
				if (progHeader.physAddr + progHeader.fileSize > highMem) highMem = progHeader.physAddr + progHeader.fileSize;
				if (progHeader.physAddr < lowMem) lowMem = progHeader.physAddr + progHeader.align;
				break;
			case PT_INTERP:
				puts("[!] Interpreted ELFs are not supported");
				return -1;
		}
		progIndex++;
	}

	/* Check for a linux kernel */
	debug_printf("[.] Checking for bootversion at 0x%x\n\r", lowMem);
	bVersion = (struct bootversion *)lowMem;
	if (bVersion->magic == BOOTINFOV_MAGIC) {
		puts("[.] Linux kernel found");
		int i = 0;
		while(bVersion->machversions[i].machine != MACH_BLITZ) {
			// Check for a valid bootinfo header
			// Only iterate 200 times max, just in case the bootinfo header is mising the terminating word
			if (bVersion->machversions[i].machine == 0x0000 || i >= 200) {
				puts("[!] Blitz machine type not supported.");
				return -1;
			}
			i++;
		}
		if (bVersion->machversions[i].version != BLITZ_BOOTI_VERSION) {
			puts("[!] bootinfo version mismatch");
			return -1;
		}

		debug_printf("[.] Creating kernel bootinfo at 0x%x\n\r", &bootInfo);

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
		for(i = 1; i < argCount; i++) {
			if (kernelArgs[0] && charsLeft > 0) {
				strncat(kernelArgs, " ", charsLeft);
				charsLeft -= 1;
			}
			argLen = strlen(args[i]);
			if (charsLeft >= argLen) {
				strncat(kernelArgs, args[i], charsLeft);
				charsLeft -= 1;
			} else {
				puts("[!] Kernel arguments exceed buffer length");
				return -1;
			}
		}

		i = strlen(kernelArgs) + 1;
		i = (i + 3) & ~3;
		bootInfo->size = sizeof(struct biRecord) + i;
		bootInfo->tag = BI_CMDLINE;
		memcpy(bootInfo->entry, kernelArgs, i);

		/* Another bootinfo entry after the first one */
		bootInfo = (struct biRecord*)(((char*)bootInfo) + bootInfo->size);
		bootInfo->tag = BI_LAST;
		bootInfo->size = sizeof(struct biRecord);

		/* Execute Kernel */
		debug_printf("[.] Entering kernel at 0x%x\n\r", header.entry);
		void (*entrypoint)(void) = header.entry;
		(*entrypoint)();
		return;
	}

	debug_printf("[.] Jumping to elf entry point at 0x%x\n\r", header.entry);
	int (*entrypoint)(int, char**) = header.entry;
	return (*entrypoint)(argCount, args);
}
