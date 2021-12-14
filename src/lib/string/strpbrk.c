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

__weak_symbol size_t strpbrk (const char *string, const char *stopset) {
	size_t length = 0;
	while (*string++ != '\0') {
		bool do_increase = true;		// determines if we increase the length or not
		for (int i = 0; *(stopset+i) != '\0' && do_increase == true; i++) {
			if (*string == *(stopset+i))
				do_increase = false;
		}
		if (!do_increase)
			length++;
		else
		 	return string+length;		// return the pointer if one of the stopset chars matched the string char
	}
	return string+length;
}
