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
	#include <char.h>

//---------------------------------------------------

#if __SIZEOF_SIZE_T__ == 8
typedef int64_t ssize_t;
#elif __SIZEOF_SIZE_T__ == 4
typedef int32_t ssize_t;
#endif

//---------------------------------------------------

/* GCC is awesome. */
#define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]) \
     + sizeof(typeof(int[1 - 2 * \
           !!__builtin_types_compatible_p(typeof(arr), \
                 typeof(&arr[0]))])) * 0)


typedef enum {
	C_BLACK = 30,
	C_RED,
	C_GREEN,
	C_YELLOW,
	C_BLUE,
	C_PINK,
	C_CYAN,
	C_LIGHTGREY,
	C_DARKGREY = 90,
	C_LIGHTRED,
	C_LIGHTGREEN,
	C_LIGHTYELLOW,
	C_LIGHTBLUE,
	C_LIGHTPINK,
	C_LIGHTCYAN,
	C_WHITE
} color_t;

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
charResult putc(char out);
char read(void);

unsigned long int strtoul(const char *nptr, char **endptr, int base);
long int strtol(const char *nptr, char **endptr, int base);
int isSpace(int val);
int isLower(int val);
int isUpper(int val);
int isAlpha(int val);
int isDigit(int val);
int toUpper(int val);
int toLower(int val);

void setFG(color_t clr);
void setBG(color_t clr);
void setFG256(uint8_t clr);
void setBG256(uint8_t clr);

//---------------------------------------------------
#endif
