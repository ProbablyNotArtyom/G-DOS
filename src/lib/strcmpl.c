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

//---------------------------------------------------

int strcmpl(const char *s1, const char *s2, int len){
	while (*s1 == *s2++){
		if (*s1++ == 0 || len == 1) return (0);
		len--;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}
