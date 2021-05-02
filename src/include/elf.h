//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// ELF layout

#ifndef _ELF_STUFF
#define _ELF_STUFF

	#include <sys/cdefs.h>
	#include <std.h>

//---------------------------------------------------

#define ET_NONE		0			/* No file type */
#define ET_REL		1			/* Relocatable file */
#define ET_EXEC		2			/* Executable file */
#define ET_DYN		3			/* Shared object file */
#define ET_CORE		4			/* Core file */
#define	ET_NUM		5			/* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */

#define EM_NONE		 0	/* No machine */
#define EM_M32		 1	/* AT&T WE 32100 */
#define EM_SPARC	 2	/* SUN SPARC */
#define EM_386		 3	/* Intel 80386 */
#define EM_68K		 4	/* Motorola m68k family */
#define EM_88K		 5	/* Motorola m88k family */
#define EM_IAMCU	 6	/* Intel MCU */
#define EM_860		 7	/* Intel 80860 */
#define EM_MIPS		 8	/* MIPS R3000 big-endian */
#define EM_S370		 9	/* IBM System/370 */
#define EM_MIPS_RS3_LE	10	/* MIPS R3000 little-endian */
						/* reserved 11-14 */
#define EM_PARISC	15	/* HPPA */
						/* reserved 16 */
#define EM_VPP500	17	/* Fujitsu VPP500 */
#define EM_SPARC32PLUS	18	/* Sun's "v8plus" */
#define EM_960		19	/* Intel 80960 */
#define EM_PPC		20	/* PowerPC */
#define EM_PPC64	21	/* PowerPC 64-bit */
#define EM_S390		22	/* IBM S390 */
#define EM_SPU		23	/* IBM SPU/SPC */
						/* reserved 24-35 */
#define EM_V800		36	/* NEC V800 series */
#define EM_FR20		37	/* Fujitsu FR20 */
#define EM_RH32		38	/* TRW RH-32 */
#define EM_RCE		39	/* Motorola RCE */
#define EM_ARM		40	/* ARM */

#if defined(__powerpc__)
	#define CPU_ARCH	EM_PPC
#elif defined(__powerpc64__)
	#define CPU_ARCH	EM_PPC64
#elif defined(__arm__)
	#define CPU_ARCH	EM_ARM
#elif defined(__m68k__)
	#define CPU_ARCH	EM_68K
#else
	#define CPU_ARCH	EM_NONE
#endif

//---------------------------------------------------

#ifndef ARGBUFF
#define ARGBUFF 256
#endif

typedef enum {
    PT_NULL = 0,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
	PT_TLS
} progtype_t;

typedef enum {
	HDATA_NONE = 0,
	HDATA_LITTLE,
	HDATA_BIG
} endianness_t;

struct __attribute_packed ident_t {
	unsigned char	magic[4];
	unsigned char	class;
	unsigned char	data;
	unsigned char	version;
	unsigned char	osabi;
	unsigned char	abiversion;
	unsigned char	padding[7];
};

typedef struct __attribute_packed {
    struct ident_t	ident;
    unsigned short	type;
    unsigned short	machine;
    unsigned long	version;
    unsigned long	entry;
    unsigned long	progOffset;
    unsigned long	shoff;
    unsigned long	flags;
    unsigned short	ehsize;
    unsigned short	progSize;
    unsigned short	progNum;
    unsigned short	shentsize;
    unsigned short	shnum;
    unsigned short	shtrndx;
} elf32_header;

typedef struct __attribute_packed {
    unsigned long	type;
    unsigned long	offset;
    unsigned long	virtAddr;
    unsigned long	physAddr;
    unsigned long	fileSize;
    unsigned long	memSize;
    unsigned long	flags;
    unsigned long	align;
} elf32_program_header;

#endif
