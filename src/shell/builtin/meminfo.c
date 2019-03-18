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

	nprintf("Blocks free: %d", mm_num_free());
	nprintf("Blocks used: %d", mm_num_used());
	nprintf("Blocks unused: %d\n", mm_num_fresh());

	tmp = ((size_t)&__heap_end - (size_t)&__heap_start);
	nprintf("Total reservable memory: %d kB (%d bytes)", tmp / 1024, tmp);

	tmp = (mm_addr_top() - (size_t)&__heap_start);
	nprintf("Memory used: %d kB (%d bytes)", tmp / 1024, tmp);

	tmp = ((size_t)&__heap_end - mm_addr_top());
	nprintf("Memory free: %d kB (%d bytes)", tmp / 1024, tmp);

	return RET_OK;
}
