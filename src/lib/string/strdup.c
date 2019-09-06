//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <string.h>

	#include <sys/cdefs.h>

//---------------------------------------------------

__weak__ char *strdup(const char *s1) {
	char *s2 = (char *)malloc(strlen(s1));
	if (s2 == NULL)
		return NULL;
	strncpy(s2, s1, strlen(s1));
	return s2;
}
