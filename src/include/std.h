//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard headers for tools n such

#ifndef _HEADER_STDTOOLS
#define _HEADER_STDTOOLS

	#include <stddef.h>
	#include <limits.h>
	#include <stdint.h>

//---------------------------------------------------

#if __SIZEOF_SIZE_T__ == 8
typedef int64_t ssize_t;
#elif __SIZEOF_SIZE_T__ == 4
typedef int32_t ssize_t;
#endif

//---------------------------------------------------

typedef void (*initcall_t)(void);
extern initcall_t __start_initsec, __stop_initsec;
typedef void (*fsinitcall_t)(struct f_handler *fs);
extern fsinitcall_t __start_fsinitsec, __stop_fsinitsec;
typedef void (*diskinitcall_t)(struct dev_disk *disk);
extern diskinitcall_t __start_diskinitsec, __stop_diskinitsec;

#define data_attr         __attribute__ ((section ("initsec")))
#define setInit(fn)  	initcall_t _##fn data_attr = fn
#define fs_data_attr      __attribute__ ((section ("fsinitsec")))
#define setFSInit(fn)  	fsinitcall_t _##fn fs_data_attr = fn
#define disk_data_attr      __attribute__ ((section ("diskinitsec")))
#define setDiskInit(fn)  	diskinitcall_t _##fn disk_data_attr = fn

//---------------------------------------------------

void putnl(void);
void putsl(char *str, int len);
void fputs(char *str);
void puts(char *str);
char *gets(char *buff, int len);
char *sgets(char *buff, int len);
void *memcpy (void *str1, const void *str2, size_t n);
void printHex(char num);
void printByte(char num);
void printWord(uint16_t num);
void printLong(uint32_t num);
void *memset(void *s, int c, size_t n);
void *malloc(size_t size);
int strcmp(const char *s1, const char *s2);
int strcmpl(const char *s1, const char *s2, int len);

/* ser.c */
void putc(char out);
char read(void);
char query(void);

//---------------------------------------------------
#endif
