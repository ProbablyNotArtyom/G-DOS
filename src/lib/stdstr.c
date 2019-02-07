//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Standard things for string manipulation

	#include <std.h>
	#include <stddef.h>
	#include <stdarg.h>

//---------------------------------------------------

char *strncat(char *dest, const char *src, size_t n){
    char *ret = dest;
    while (*dest) dest++;
    while (n--) if (!(*dest++ = *src++)) return ret;
    *dest = 0;
    return ret;
}

unsigned long int strtoul(const char *nptr, char **endptr, int base){
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

long int strtol(const char *nptr, char **endptr, int base){
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

int isSpace(int val){
	return ((char)val == ' ') ||
		((char)val == '\n') ||
		((char)val == '\r') ||
		((char)val == '\t');
}

int isLower(int val){
	return ((char)val <= 'z' && (char)val >= 'a');
}

int isUpper(int val){
	return ((char)val <= 'Z' && (char)val >= 'A');
}

int isAlpha(int val){
	return (isUpper(val) || isLower(val));
}

int isDigit(int val){
	return ((char)val <= '9' && (char)val >= '0');
}

int toUpper(int val){
	if ((val >= 'a') && (val <= 'z')){
		return (val - 0x20);
	}
	return val;
}

int toLower(int val){
	if ((val >= 'A') && (val <= 'Z')){
		return (val + 0x20);
	}
	return val;
}

int isAscii(int val)
{
	return !(( char)val & 0x80);
}

int isAlnum(int val)
{
	return isDigit(val) || isAlpha(val);
}
