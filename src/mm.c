//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Generic memory management

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <mod/init.h>

	#define MAX_ALLOCS		20

//---------------------------------------------------

extern size_t __heap_start;
extern size_t __heap_end;

static size_t mm_heap_begin;
static size_t mm_heap_end;

static struct alloc_entry {
	void *	length;
	void *	address;
	bool	used;
};

static struct alloc_entry allocs[MAX_ALLOCS];
static size_t num_allocs;

//---------------------------------------------------

void mm_init(){
	mm_heap_begin = &__heap_start;
	mm_heap_end = &__heap_end;
	for (int i = 0; i < MAX_ALLOCS; i++){
		allocs[i].length = NULL;
		allocs[i].address = NULL;
		allocs[i].used = false;
	}
	num_allocs = 0;
}

void* malloc(size_t bytes) {
	unsigned int index = 0;
	while (index < MAX_ALLOCS){
		if (allocs[index].used == false && allocs[index].length >= bytes){
			allocs[index].used = true;
			return allocs[index].address;
		}
		index++;
	}

	if (mm_heap_begin + bytes >= mm_heap_end) return NULL;
	index = 0;
	while (allocs[index].length != NULL && index < MAX_ALLOCS) index++;
	if (index == MAX_ALLOCS){
		puts("** [!] malloc() has run out of allocation slots **");
		return NULL;
	}
	allocs[index].length = bytes;
	allocs[index].address = mm_heap_begin;
	allocs[index].used = true;

	mm_heap_begin += bytes;
	return allocs[index].address;

}

void free(void *ptr) {
	unsigned int index = 0;
	while (index < MAX_ALLOCS){
		if (allocs[index].address == ptr) {
			allocs[index].used = false;
			return;
		}
		index++;
	}
}

early_initcall(mm_init);
