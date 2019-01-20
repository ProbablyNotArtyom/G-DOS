
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <mod/init.h>

extern void gdos(void);

extern void init_IRQ(void);
extern void init_traps(void);

extern initcall_t __initcall_start[];
extern initcall_t __initcall0_start[];
extern initcall_t __initcall1_start[];
extern initcall_t __initcall2_start[];
extern initcall_t __initcall3_start[];
extern initcall_t __initcall4_start[];
extern initcall_t __initcall5_start[];
extern initcall_t __initcall6_start[];
extern initcall_t __initcall7_start[];
extern initcall_t __initcall_end[];

static const initcall_t *initcall_levels[] = {
	__initcall_start,
	__initcall0_start,
	__initcall1_start,
	__initcall2_start,
	__initcall3_start,
	__initcall4_start,
	__initcall5_start,
	__initcall6_start,
	__initcall7_start,
	__initcall_end,
};

static const char const *initcall_level_names[] = {
	"early",
	"pure",
	"core",
	"postcore",
	"arch",
	"subsys",
	"fs",
	"device",
	"late",
};

static int do_one_initcall(initcall_t fn){
	return fn();
}

static void do_initcall_level(int level){
	initcall_t *fn;
	int index;
	for (fn = initcall_levels[level]; fn < initcall_levels[level+1]; fn++)
		do_one_initcall(*fn);
}

static void do_initcalls(void){
	int level;

	for (level = 0; level <  ARRAY_SIZE(initcall_levels) - 1; level++){
		printf("Running initcall level: %s\n\r", initcall_level_names[level]);
		do_initcall_level(level);
	}
}

int main(void){
	tty_init();
	do_initcalls();
	gdos();
}
