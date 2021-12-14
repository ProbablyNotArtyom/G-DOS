//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <string.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol char *strtok(char *str, const char *delims) {
	static char *tmp = "";
	char **lasts = &tmp;

	char *s_start, *s_end;
	s_start = str ? str : *lasts;
	s_start += strspn(s_start, delims);
	if (*s_start == '\0') {
		*lasts = "";
		return NULL;
	}
	s_end = s_start + strcspn(s_start, delims);
	if (*s_end != '\0')
		*s_end++ = '\0';
	*lasts = s_end;
	return s_start;
}
