#ifndef _HEADER_TINYALLOC
#define _HEADER_TINYALLOC

	#include <stdbool.h>
	#include <stddef.h>

bool mm_init();
void *malloc(size_t num);
void *calloc(size_t num, size_t size);
bool free(void *ptr);

size_t mm_num_free();
size_t mm_num_used();
size_t mm_num_fresh();
size_t mm_addr_top();
bool mm_check();

extern size_t __heap_start;
extern size_t __heap_end;

char alloc_reserved[4096];

	#define TA_ALIGN		8
	#define TA_BASE			&alloc_reserved
	#define TA_HEAP_START	&__heap_start
	#define TA_HEAP_LIMIT	&__heap_end
	#define TA_HEAP_BLOCKS	256
	#define TA_SPLIT_THRESH 16

#endif
