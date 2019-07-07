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

//---------------------------------------------------

unsigned long int strtoul(const char *nptr, char **endptr, int base);
long int strtol(const char *nptr, char **endptr, int base);

int strcmp(const char *s1, const char *s2);
int strcmpl(const char *s1, const char *s2, int len);
size_t strlen (const char *str);
int strcasecmp(const char *s1, const char *s2);
void * strchr (const void *block, int c);
void * strchrnul (const void *block, int c);
void * strrchr (const void *block, int c);
char * strstr (const char *haystack, const char *needle);
char * strcasestr (const char *haystack, const char *needle);
size_t strspn (const char *string, const char *skipset);
size_t strcspn (const char *string, const char *stopset);
size_t strpbrk (const char *string, const char *stopset);

int isSpace(int val);
int isLower(int val);
int isUpper(int val);
int isAlpha(int val);
int isDigit(int val);
int toUpper(int val);
int toLower(int val);

//---------------------------------------------------
#endif
