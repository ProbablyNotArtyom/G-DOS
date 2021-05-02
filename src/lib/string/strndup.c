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

__weak_symbol char *strndup(const char *s1, size_t size) {
	size_t len = strnlen(s1, size);
	char *s2 = (char *)malloc(len + 1);
	if (s2 == NULL)
		return NULL;
	s2[len] = '\0';
	strncpy(s2, s1, size);
	return s2;
}
