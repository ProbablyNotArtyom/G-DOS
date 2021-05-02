//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Constants required for bootstraping Linux

#ifndef _BOOT_HEADER
#define _BOOT_HEADER

	#include <sys/cdefs.h>
	
#define MACH_BLITZ		14
#define CPUB_68030		1
#define CPU_68030		(1 << CPUB_68030)
#define MMUB_68030		1
#define MMU_68030		(1 << MMUB_68030)
#define FPUB_68881		0
#define FPUB_68882		1
#define FPU_68881		(1 << FPUB_68881)
#define FPU_68882		(1 << FPUB_68882)

#define BOOTINFOV_MAGIC				0x4249561A
#define	MK_BI_VERSION(major, minor)	(((major) << 16) + (minor))
#define	BI_VERSION_MAJOR(v)			(((v) >> 16) & 0xffff)
#define	BI_VERSION_MINOR(v)			((v) & 0xffff)

#define BLITZ_BOOTI_VERSION	MK_BI_VERSION(2, 0)

enum boot_tags {
	BI_LAST = 0x0000,
	BI_MACHTYPE,
	BI_CPUTYPE,
	BI_FPUTYPE,
	BI_MMUTYPE,
	BI_MEMBLOCK,
	BI_RAMDISK,
	BI_CMDLINE
};

struct memBlock {
	uint32_t addr;		// Start of memory blocks
	uint32_t size;		// Size of block
};

struct biRecord {
	uint16_t tag;
	uint16_t size;		// Record length in bytes
	uint32_t entry[0];	// Param
};

struct __attribute_packed bootversion {
	uint16_t	branch;
	uint32_t	magic;
	struct {
		uint32_t machine;
		uint32_t version;
	} machversions[0];
};
#endif
