//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Memory / IO access macros

#ifndef _HEADER_CPU_IO
#define _HEADER_CPU_IO

	#include <sys/cdefs.h>
	#include <types.h>

//---------------------------------------------------

#define outb(val, addr)		((*(__force volatile uint8_t *) (addr)) = (val))
#define outw(val, addr)		((*(__force volatile uint16_t *) (addr)) = (val))
#define outl(val, addr)		((*(__force volatile uint32_t *) (addr)) = (val))

#define inb(addr)			({ uint8_t __v = (*(__force volatile uint8_t *) (addr)); __v; })
#define inw(addr)			({ uint16_t __v = (*(__force volatile uint16_t *) (addr)); __v; })
#define inl(addr)			({ uint32_t __v = (*(__force volatile uint32_t *) (addr)); __v; })

//---------------------------------------------------

static inline uint16_t swapw(volatile uint16_t val){
	return ((val << 8) | (val >>8));
}

static inline uint32_t swapl(volatile uint32_t val){
	return ((val << 24) | ((val & 0xff00) << 8) | ((val & 0xff0000) >> 8) | (val >> 24));
}

static inline void io_outsb(size_t addr, const void* buff, size_t len){
	volatile uint8_t *ptr_a = (volatile uint8_t *)addr;
	uint8_t *ptr_b = (uint8_t *)buff;
	while (len--) *ptr_a = *ptr_b++;
}

static inline void io_outsw(size_t addr, const void* buff, size_t len){
	volatile uint16_t *ptr_a = (volatile uint16_t *)addr;
	uint16_t *ptr_b = (uint16_t *)buff;
	while (len--) *ptr_a = swapw(*ptr_b++);
}

static inline void io_outsl(size_t addr, const void* buff, size_t len){
	volatile uint32_t *ptr_a = (volatile uint32_t *)addr;
	uint32_t *ptr_b = (uint32_t *)buff;
	while (len--) *ptr_a = swapl(*ptr_b++);
}

static inline void io_insb(size_t addr, void* buff, size_t len){
	volatile uint8_t *ptr_a = (volatile uint8_t *)addr;
	uint8_t *ptr_b = (uint8_t *)buff;
	while (len--) *ptr_b++ = *ptr_a;
}

static inline void io_insw(size_t addr, void* buff, size_t len){
	volatile uint16_t *ptr_a = (volatile uint16_t *)addr;
	uint16_t *ptr_b = (uint16_t *)buff;
	while (len--) *ptr_b++ = swapw(*ptr_a);
}

static inline void io_insl(size_t addr, void* buff, size_t len){
	volatile uint32_t *ptr_a = (volatile uint32_t *)addr;
	uint32_t *ptr_b = (uint32_t *)buff;
	while (len--) *ptr_b++ = swapl(*ptr_a);
}

//---------------------------------------------------
#endif
