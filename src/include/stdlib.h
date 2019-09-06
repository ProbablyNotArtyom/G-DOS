//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#ifndef _HEADER_STDLIB
#define _HEADER_STDLIB

	#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX (0x7FFFFFFF)

#define MB_CUR_MAX 4

//---------------------------------------------------

typedef int (*__compare)(const void *, const void *);
typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;


void _Exit(int status);
void exit(int status);
int atexit(void (*func)(void));
void abort(void);
int system(const char *command);
char *getenv(const char *name);

void *malloc(size_t num);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

double atof(const char *str);
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
float strtof(const char *str, char **end);
double strtod(const char *str, char **end);
long double strtold(const char *str, char **end);
long strtol(const char *str, char **end, int radix);
long long strtoll(const char *str, char **end, int radix);
unsigned long strtoul(const char *str, char **end, int radix);
unsigned long long strtoull(const char *str, char **end, int radix);

div_t div(int n0, int n1);
ldiv_t ldiv(long n0, long n1);
lldiv_t lldiv(long long n0, long long n1);
int abs(int val);
long labs(long val);
long long llabs(long long val);

int mblen(const char *src, size_t max);
int mbtowc(wchar_t *dest, const char *src, size_t max);
size_t mbstowcs(wchar_t *dest, const char *src, size_t max);
int wctomb(char *dest, wchar_t wc);
size_t wcstombs(char* dest, const wchar_t* src, size_t max);

void *bsearch(const void *key, const void *base, size_t num, size_t width, int (*comp)(const void *, const void *));
void qsort(void *base, unsigned num, unsigned width, int (*comp)(const void *, const void *));

//---------------------------------------------------

#endif
