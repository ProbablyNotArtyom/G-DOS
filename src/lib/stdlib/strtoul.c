//---------------------------------------------------
//
//	G-DOS
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

__weak_symbol unsigned long int strtoul(const char *nptr, char **endptr, int base){
	unsigned long int num;
	if (base <= 0) base = 0;

	while (isSpace(*nptr)) nptr++;
	if (*nptr == '-' || *nptr == '+') nptr++;

	if (base == 0 && *nptr == '0'){
		if (toUpper(*(nptr+1)) == 'X'){
			base = 16;
			nptr += 2;
		} else {
			base = 8;
			nptr++;
		}
	}
	if (base == 0) base = 10;
	num = 0;
	while (isAscii(*nptr) && isAlnum(*nptr)){
		int chr = *nptr;
		if (isLower(chr)) chr = toUpper(chr);
		chr -= (chr<='9' ? '0' : 'A'-10);
		if (chr > base) break;

		num = chr + (num * base);
		nptr++;
	}

	if (endptr != NULL) *endptr = (char *)nptr;
	return num;
}
