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

__weak_symbol lldiv_t lldiv(long long n0, long long n1) {
	return (lldiv_t){ n0 / n1, n0 % n1 };
}
