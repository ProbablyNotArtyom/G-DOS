// BUILTIN
// MEMINFO
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

//---------------------------------------------------

extern size_t __heap_start;
extern size_t __heap_end;

result_t shfunc_meminfo(char *argv[], int argc){
	size_t tmp;

	printf("Blocks free: %d\n", mm_num_free());
	printf("Blocks used: %d\n", mm_num_used());
	printf("Blocks unused: %d\n\n", mm_num_fresh());

	tmp = ((size_t)&__heap_end - (size_t)&__heap_start);
	printf("Total reservable memory: %d kB (%d bytes)\n", tmp / 1024, tmp);

	tmp = (mm_addr_top() - (size_t)&__heap_start);
	printf("Memory used: %d kB (%d bytes)\n", tmp / 1024, tmp);

	tmp = ((size_t)&__heap_end - mm_addr_top());
	printf("Memory free: %d kB (%d bytes)\n", tmp / 1024, tmp);

	return RET_OK;
}
