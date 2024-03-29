//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	14/12/21
//
//---------------------------------------------------
// Dynamic memory allocator & block compactor

	#include "mm.h"
	#include <stdint.h>
	#include <std.h>
	#include <mod/init.h>
	#include <flags.h>
	#include <debug.h>

//---------------------------------------------------

#ifndef TA_ALIGN
#define TA_ALIGN 8
#endif

#ifndef TA_BASE
#define TA_BASE 0x400
#endif

#ifndef TA_HEAP_START
#define TA_HEAP_START (TA_BASE + sizeof(Heap))
#endif

#ifndef TA_HEAP_LIMIT
#define TA_HEAP_LIMIT (1 << 24)
#endif

#ifndef TA_HEAP_BLOCKS
#define TA_HEAP_BLOCKS 256
#endif

#ifndef TA_SPLIT_THRESH
#define TA_SPLIT_THRESH 16
#endif

//---------------------------------------------------

typedef struct Block Block;
struct Block {
    void *addr;
    Block *next;
    size_t size;
};

typedef struct {
    Block *free;   // first free block
    Block *used;   // first used block
    Block *fresh;  // first available blank block
    size_t top;    // top free addr
    Block blocks[TA_HEAP_BLOCKS];
} Heap;

//---------------------------------------------------

static Heap *heap = (Heap *)TA_BASE;

/* Memory compaction helper */
static void insert_block(Block *block) {
    Block *ptr  = heap->free;
    Block *prev = NULL;

	while (ptr != NULL) {
        if ((size_t)block->addr <= (size_t)ptr->addr) {
			debug_printf("[MM] insert 0x%x\n", (size_t)ptr);
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    if (prev != NULL) {
        if (ptr == NULL) puts_debug("[MM] new tail");
        prev->next = block;
    } else {
        puts_debug("[MM] new head");
        heap->free = block;
    }
    block->next = ptr;
}

static void release_blocks(Block *scan, Block *to) {
    Block *scan_next;

    while (scan != to) {
        puts_debug("[MM] release");
        puts_debug((size_t)scan);
        scan_next = scan->next;
        scan->next = heap->fresh;
        heap->fresh = scan;
        scan->addr = 0;
        scan->size = 0;
        scan = scan_next;
    }
}

static void compact() {
    Block *ptr = heap->free;
    Block *prev;
    Block *scan;

	while (ptr != NULL) {
        prev = ptr;
        scan = ptr->next;
        while (scan != NULL && (size_t)prev->addr + prev->size == (size_t)scan->addr) {
            debug_printf("[MM] merge 0x%x\n", (size_t)scan);
            prev = scan;
            scan = scan->next;
        }
        if (prev != ptr) {
            size_t new_size = (size_t)prev->addr - (size_t)ptr->addr + prev->size;
            Block *next = prev->next;

            debug_printf("[MM] new size 0x%x\n", new_size);
            ptr->size = new_size;

            release_blocks(ptr->next, prev->next);	// make merged blocks available
            ptr->next = next;						// relink
        }
        ptr = ptr->next;
    }
}

bool mm_init() {
	Block *block = heap->blocks;
	size_t i = TA_HEAP_BLOCKS - 1;

	heap->free = NULL;
    heap->used = NULL;
    heap->fresh = heap->blocks;
    heap->top = TA_HEAP_START;
	while (i--) {
        block->next = block + 1;
        block++;
    }

	block->next = NULL;
    return true;
}

void free(void *free) {
    Block *block = heap->used;
    Block *prev = NULL;

    while (block != NULL) {
        if (free == block->addr) {
            if (prev) prev->next = block->next;
            else heap->used = block->next;

			insert_block(block);
            compact();
            return;
        }
        prev  = block;
        block = block->next;
    }
    return;
}

static Block *alloc_block(size_t num) {
    Block *ptr = heap->free;
    Block *prev = NULL;
    size_t top = heap->top;

	num = (num + TA_ALIGN - 1) & -TA_ALIGN;
    while (ptr != NULL) {
        const int is_top = ((size_t)ptr->addr + ptr->size >= top) && ((size_t)ptr->addr + num <= TA_HEAP_LIMIT);
        if (is_top || ptr->size >= num) {
            if (prev != NULL) prev->next = ptr->next;
            else heap->free = ptr->next;

            ptr->next = heap->used;
            heap->used = ptr;

			if (is_top) {
                puts_debug("[MM] resize top block");
                ptr->size = num;
                heap->top = (size_t)ptr->addr + num;
            } else if (heap->fresh != NULL) {
                size_t excess = ptr->size - num;
                if (excess >= TA_SPLIT_THRESH) {
                    Block *split = heap->fresh;
                    ptr->size = num;
                    heap->fresh = split->next;
                    split->addr = (void *)((size_t)ptr->addr + num);

					debug_printf("[MM] split 0x%x\n", (size_t)split->addr);

					split->size = excess;
                    insert_block(split);
                    compact();
                }
            }
            return ptr;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    // no matching free blocks
    // see if any other blocks available
    size_t new_top = top + num;
    if (heap->fresh != NULL && new_top <= TA_HEAP_LIMIT) {
        ptr = heap->fresh;
        heap->fresh = ptr->next;
        ptr->addr = (void *)top;
        ptr->next = heap->used;
        ptr->size = num;
        heap->used = ptr;
        heap->top = new_top;
        return ptr;
    }
    return NULL;
}

static size_t block_size(void *ptr) {
	int i = 0;
	while (i < TA_HEAP_BLOCKS) {
		if (heap->blocks[i].addr == ptr) break;
		i++;
	}
	if (i >= TA_HEAP_BLOCKS) return NULL;
	else return heap->blocks[i].size;
}

void *malloc(size_t num) {
    Block *block = alloc_block(num);
    if (block != NULL) return block->addr;
    else return NULL;
}

void *pmalloc(size_t num) {
    Block *block = alloc_block(num);
    if (block != NULL) return block->addr;
    else return NULL;
}

static void memclear(void *ptr, size_t num) {
    size_t *ptrw = (size_t *)ptr;
    size_t numw = (num & -sizeof(size_t)) / sizeof(size_t);
    while (numw--) *ptrw++ = 0;

    num &= (sizeof(size_t) - 1);
    uint8_t *ptrb = (uint8_t *)ptrw;
    while (num--) *ptrb++ = 0;
}

void *calloc(size_t num, size_t size) {
    num *= size;
    Block *block = alloc_block(num);
    if (block != NULL) {
        memclear(block->addr, num);
        return block->addr;
    }
    return NULL;
}

void *realloc(void *ptr, size_t num) {
	void *newmem;
	if (!ptr) {
		newmem = malloc(num);
		if (!newmem) return NULL;
	} else {
		if (block_size(ptr) < num) {
			newmem = malloc(num);
			if (!newmem) return NULL;

			memcpy(newmem, ptr, block_size(num));
			free(ptr);
		} else {
			newmem = ptr;
		}
	}
	return newmem;
}

static size_t count_blocks(Block *ptr) {
    size_t num = 0;
    while (ptr != NULL) {
        num++;
        ptr = ptr->next;
    }
    return num;
}

size_t mm_num_free() {
    return count_blocks(heap->free);
}

size_t mm_num_used() {
    return count_blocks(heap->used);
}

size_t mm_num_fresh() {
    return count_blocks(heap->fresh);
}

size_t mm_addr_top() {
    return heap->top;
}

bool mm_check() {
    return TA_HEAP_BLOCKS == mm_num_free() + mm_num_used() + mm_num_fresh();
}

early_initcall(mm_init);
