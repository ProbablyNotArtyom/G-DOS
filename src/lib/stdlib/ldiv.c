//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol ldiv_t ldiv(long n0, long n1) {
	return (ldiv_t){ n0 / n1, n0 % n1 };
}
