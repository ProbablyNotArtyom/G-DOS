//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <stdint.h>
	#include <sys/cdefs.h>

static uint64_t seed;

//---------------------------------------------------

__weak_symbol void srand(unsigned seed_in) {
	seed = seed_in-1;
}

__weak_symbol int rand(void) {
	seed = 6364136223846793005ULL*seed + 1;
	return seed>>33;
}
