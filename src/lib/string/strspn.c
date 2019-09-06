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

__weak__ size_t strspn (const char *string, const char *skipset) {
	size_t length = 0;
	while (*string++ != '\0') {
		bool do_increase = false;		// determines if we increase the length or not
		for (int i = 0; *(skipset+i) != '\0' && do_increase == false; i++) {
			if (*string == *(skipset+i))
				do_increase = true;
		}
		if (do_increase)
			length++;
		else
		 	return length;				// return the length if none of the one of the skipset chars matched the string char
	}
	return length;
}
