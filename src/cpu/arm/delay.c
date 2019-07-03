
	#include <std.h>

void delay(uint32_t loops){
	for (long i = 0; i < 10000000; i++) asm("");
	return;
}
