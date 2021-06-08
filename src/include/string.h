//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard headers for tools n such

#ifndef _HEADER_STD_STRING
#define _HEADER_STD_STRING

	#include <stddef.h>
	#include <stdbool.h>
	#include <limits.h>
	#include <stdint.h>
	#include <char.h>

//---------------------------------------------------

#define index(string, c)		strchr(string, c)			// Compatability
#define rindex(string, c)		strrchr(string, c)			// Compatability

#define HAVE_STRCHRNUL
#define HAVE_STRTOD
#define HAVE_KILLPG

//---------------------------------------------------

void *memcpy(void *str1,const void *str2,size_t n);
void *memchr(const void *block,int c,size_t size);
void *memccpy(void *restrict to,const void *restrict from,int c,size_t size);
int memcmp(const void *p1,const void *p2,size_t n);
void *memmem(const void *haystack,size_t haystack_len,const void *needle,size_t needle_len);
void *memmove(void *to,const void *from,size_t len);
void *memrchr(const void *block,int c,size_t size);
void *memset(void *block,int c,size_t size);
void *rawmemchr(const void *block,int c);
int strcasecmp(const char *s1,const char *s2);
char *strcasestr(const char *haystack,const char *needle);
size_t strlen(const char *str);
char *strcat(char *dest,const char *src);
void *strchr(const void *block,int c);
char *strchrnul(const void *block,int c);
int strcmp(const char *s1,const char *s2);
int strcmpl(const char *s1,const char *s2,int len);
char *strcpy(char *to,const char *from);
size_t strcspn(const char *string,const char *stopset);
char *strncpy(char *restrict to,const char *restrict from,size_t size);
char *strdup(const char *s1);
int strtoerrno(const char *name);
char *strerror(int errnum);
char *strncat(char *dest,const char *src,size_t n);
int strncmp(const char *s1,const char *s2,size_t size);
size_t strnlen(const char *str,size_t size);
char *strndup(const char *s1,size_t size);
size_t strpbrk(const char *string,const char *stopset);
void *strrchr(const void *block,int c);
char *strstr(const char *haystack,const char *needle);
size_t strspn(const char *string,const char *skipset);
char *strtok(char *str,const char *delims);
size_t strxfrm(char *dest,const char *src,size_t n);

//---------------------------------------------------
#endif
