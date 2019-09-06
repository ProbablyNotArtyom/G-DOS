//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <std.h>
	#include <string.h>
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>

	#include <sys/cdefs.h>

//---------------------------------------------------

__weak__ long int strtol(const char *nptr, char **endptr, int base){
	const char *ptr;
	long int num;
	unsigned short neg;

	ptr = nptr;
	while (isSpace(*ptr)) ptr++;

	neg = 0;
	if (*ptr == '-') neg = 1;

	num = (long int)strtoul(nptr, endptr, base);
	return (neg ? -num:num);
}
