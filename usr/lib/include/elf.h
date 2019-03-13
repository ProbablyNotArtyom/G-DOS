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

#ifndef ARGBUFF
#define ARGBUFF 256
#endif

enum {
    PHT_NULL,
    PHT_LOAD,
    PHT_DYNAMIC,
    PHT_INTERP,
    PHT_NOTE,
    PHT_SHLIB,
    PHT_PHDR
};

typedef struct __attribute__ ((__packed__)) {
    unsigned char  magic[4];
    unsigned char  h_class;
    unsigned char  h_data;
    unsigned char  h_version;
    unsigned char  h_osabi;
    unsigned char  h_abiversion;
    unsigned char  padding[7];
    unsigned short type;
    unsigned short machine;
    unsigned long  version;
    unsigned long  entry;
    unsigned long  progOffset;
    unsigned long  shoff;
    unsigned long  flags;
    unsigned short ehsize;
    unsigned short progSize;
    unsigned short progNum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shtrndx;
} elf32_header;

typedef struct __attribute__ ((__packed__)) {
    unsigned long type;
    unsigned long offset;
    unsigned long virtAddr;
    unsigned long physAddr;
    unsigned long fileSize;
    unsigned long memSize;
    unsigned long flags;
    unsigned long align;
} elf32_program_header;

#endif
