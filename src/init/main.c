
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <linkage.h>
	#include <syscall.h>
	#include <flags.h>

	#include <mod/init.h>
	#include <tui.h>

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
struct entry_info *extra_bootmenu_entries[10];		// anything more than 10 extra entries would be absurd and shouldnt be allowed

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
		printf( SET_COLOR_FG(C_LIGHTGREEN) "Running initcall level:" SET_COLOR_FG(C_WHITE) " %s" COLOR_RESET_FG "\n\r", initcall_level_names[level]);
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

void print_boot_menu(void) {
	fputs(b_opts);
	if (extra_bootmenu_entries[0] != NULL) {
		int i = 0;
		while (extra_bootmenu_entries[i] != NULL) {
			printf("  " COLOR_FG(C_LIGHTBLUE, "%d)") COLOR_FG(C_WHITE, "  $s\r\n"), i+5, extra_bootmenu_entries[i]->label);
			i++;
		}
		puts(COLOR_FG(C_YELLOW, "=============================================\r\n"));
	}
	fputs(COLOR_RESET_BG COLOR_RESET_FG "> ");
}

int main(void){
	delay(0x1FFFF);

	do_initcall_level(0);
	tui_cls();
	do_initcalls();
	puts(b_logo);
	#ifdef CUSTOM_SPLASH
	puts(CUSTOM_SPLASH);
	#endif

	register char tmp;
	while(true){
		print_boot_menu();
		do {
			tmp = read();
		} while (tmp == NULL);
		switch (tmp){
			case '0':
				puts("");
				memtest_start();
				break;
			case '1':
			case '\r':
				puts("");
				shellBegin();
				break;
			case '2':
				puts("");
				guiBegin();
				break;
			case '3':
				puts("");
				monBegin();
				break;
			case '4':
				return;
				break;
			default:
				break;
		}
	}
}

//---------------------------------------------------

int register_bootmenu_entry(const struct entry_info *entry) {
	int i = 0;
	while (extra_bootmenu_entries[i] != NULL) i++;
	extra_bootmenu_entries[i] = entry;
	return 1;
}

//---------------------------------------------------

const char const b_logo[] =
	"\r\n   ___   _   ___   ___  ___ \r\n"
	"  / __| ( ) |   \\ / _ \\/ __|\r\n"
	" | (_ | |/  | |) | (_) \\__ \\\r\n"
	"  \\___|     |___/ \\___/|___/\r\n\r\n";

const char const b_opts[] =
	" G'DOS Booter Menu\r\n"
	COLOR_FG(C_YELLOW, "=============================================\r\n")
	"  " COLOR_FG(C_LIGHTBLUE, "0)") COLOR_FG(C_WHITE, "  RAM Test\r\n")
	"  " COLOR_FG(C_LIGHTBLUE, "1)") COLOR_FG(C_WHITE, "  Shell  (default)\r\n")
	"  " COLOR_FG(C_LIGHTBLUE, "2)") COLOR_FG(C_WHITE, "  Shell  (TUI)\r\n")
	"  " COLOR_FG(C_LIGHTBLUE, "3)") COLOR_FG(C_WHITE, "  G'mon\r\n")
	"  " COLOR_FG(C_LIGHTBLUE, "4)") COLOR_FG(C_WHITE, "  Exit\r\n")
	COLOR_FG(C_YELLOW, "=============================================\r\n");
