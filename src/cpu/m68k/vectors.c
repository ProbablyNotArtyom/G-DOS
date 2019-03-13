
	#include <std.h>
	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>

extern const size_t _start;
extern const size_t __STACK;

asmlinkage void addrerr(void);
asmlinkage void buserr(void);
asmlinkage void trap(void);
asmlinkage void syscall(void);

__attribute__((section ("vectors"))) const uint32_t _vectors[255] = {
	(uint32_t)&__STACK,
	(uint32_t)&_start,
	(uint32_t)buserr,
	(uint32_t)addrerr,
	(uint32_t)syscall,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)syscall,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)NULL,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall,
	(uint32_t)syscall
};
