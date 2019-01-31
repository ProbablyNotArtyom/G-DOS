
	#include <mod/init.h>
	#include <stddef.h>
	#include <std.h>

//----------------------Rescue-----------------------

void _BError(void){
	puts("\n\r[BUS ERROR]");
	while (1);
}

void _AError(void){
	puts("\n\r[ADDRESS ERROR]");
	while (1);
}
