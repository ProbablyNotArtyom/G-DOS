
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <linkage.h>
	#include <syscall.h>

	#include <mod/init.h>

//---------------------------------------------------

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

extern size_t _end;

#ifdef CUSTOM_SPLASH
	extern char CUSTOM_SPLASH[];
#endif

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

const char const b_logo[];
const char const b_opts[];

//---------------------------------------------------

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

	for (level = 1; level <  ARRAY_SIZE(initcall_levels) - 1; level++){
		printf("Running initcall level: %s\n\r", initcall_level_names[level]);
		do_initcall_level(level);
	}
}

static void memtest_start(){
	size_t test_start, test_end;
	char chBuff[9];					// Buffer for ascii input
	fputs("\r\n[?] Start address : 0x");
	gets(chBuff, 9);
	if (chBuff[0] == '\r')
		test_start = &_end;
	else
		test_start = strtoul(chBuff, NULL, 16);

	fputs("\r\n[?] End address : 0x");
	gets(chBuff, 9);
	if (chBuff[0] == '\r')
		test_end = RAMEND;
	else
		test_end = strtoul(chBuff, NULL, 16);

	fputs("\r\n[?] Intense (y/n) : ");
	do_memtest(test_end, test_start, read());
}

int main(void){
	do_initcall_level(0);
	do_initcalls();
	puts(b_logo);
	#ifdef CUSTOM_SPLASH
	puts(CUSTOM_SPLASH);
	#endif

	register char tmp;
	while(true){
		fputs(b_opts);
		do {
			tmp = read();
		} while (tmp == NULL);
		switch (tmp){
			case '0':
				puts("");
				memtest_start();
				break;
			case '1':
			default:
				puts("");
				gdos();
				break;
			case '2':
				puts("");
				monBegin();
				break;
			case '3':
				INTERNAL_SYSCALL(syscall_putc, 1, 'K');
				break;
		}
	}
}

//---------------------------------------------------

const char const b_logo[] =
	"\r\n   ___   _   ___   ___  ___ \r\n"
	"  / __| ( ) |   \\ / _ \\/ __|\r\n"
	" | (_ | |/  | |) | (_) \\__ \\\r\n"
	"  \\___|     |___/ \\___/|___/\r\n\r\n";

const char const b_opts[] =
	" G'DOS Booter Menu\r\n"
	"=============================================\r\n"
	"  0)  RAM Test\r\n"
	"  1)  Shell  (default)\r\n"
	"  2)  G'mon\r\n"
	"  3)  Exit\r\n"
	"=============================================\r\n"
	"> ";
