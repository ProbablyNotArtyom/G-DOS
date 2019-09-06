
	#include <std.h>

void delay(uint32_t loops){
	__asm__ __volatile__ (
		"1: subql #1,%0\n\t"
		"jcc 1b"
		: "=d" (loops)
		: "0" (loops));
}
