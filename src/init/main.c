
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <stdio.h>
	#include <linkage.h>
	#include <syscall.h>
	#include <flags.h>
	#include <errno.h>
	#include <sys/cdefs.h>

	#include <mod/init.h>
	#include <tui.h>
	#include <syscall.h>

//---------------------------------------------------

extern int monBegin(void);

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

extern const char _end;

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

#define EXTRA_BOOTMENUS_MAX		10
struct entry_info *extra_bootmenu_entries[EXTRA_BOOTMENUS_MAX];		// anything more than 10 extra entries would be absurd and shouldnt be allowed

int register_bootmenu_entry(const struct entry_info *entry);
int init_bootmenu_entrys(void);

//---------------------------------------------------

static int do_one_initcall(initcall_t fn) {
	return fn();
}

static void do_initcall_level(int level) {
	initcall_t *fn;
	for (fn = initcall_levels[level]; fn < initcall_levels[level+1]; fn++) do_one_initcall(*fn);
}

static void do_initcalls(void) {
	int level;

	for (level = 1; level <  ARRAY_SIZE(initcall_levels) - 1; level++) {
		printf( SET_COLOR_FG(C_LIGHTGREEN) "Running initcall level:" SET_COLOR_FG(C_WHITE) " %s" COLOR_RESET_FG "\n\r", initcall_level_names[level]);
		do_initcall_level(level);
	}
}

void print_boot_menu(void) {
	/* Display G'DOS logo */
	printf(COLOR_FG(C_LIGHTBLUE, "%s\r\n"), b_logo);

	/* Add a menu entry to toggle DEBUG and update it in real time to reflect DEBUG's current state */
	printf(COLOR_FG(C_YELLOW, "--=-========================================================-=--\r\n"));
	printf("  " COLOR_FG(C_MAGENTA, "D)") COLOR_FG(C_WHITE, "  Toggle DEBUG : ") "%s\r\n",
		(__global_flags[GLOBAL_FLAG_DEBUG])? COLOR_FG(C_LIGHTGREEN, "enabled") : COLOR_FG(C_LIGHTRED, "disabled"));

	/* Render the menu and it's extensions */
	printf(b_opts);
	if (extra_bootmenu_entries[0] != NULL) {
		int i = 0;
		while (extra_bootmenu_entries[i] != NULL) {
			printf("  " COLOR_FG(C_LIGHTBLUE, "%d)") COLOR_FG(C_WHITE, "  %s\r\n"), i+4, extra_bootmenu_entries[i]->label);
			i++;
		}
		puts(COLOR_FG(C_YELLOW, "--=-========================================================-=--\r\n"));
	}
}

int main(void) {
	do_initcall_level(0);
	init_bootmenu_entrys();
	do_initcalls();

#ifdef CUSTOM_SPLASH		// Print custom splash only on boot, if it exists
	puts(CUSTOM_SPLASH);
#endif

	char tmp = 0xFF;
	int retval = 0;
	while(true) {
		if (tmp) print_boot_menu();
		if (retval) printf("%s\n", strerror(errno));
		do {
			tmp = getchar();
		} while (!tmp);
		switch (tmp) {
			case '0':
				puts("");
				retval = memtest_start();
				break;
			case '1':
			case '\r':
				puts("");
				retval = shellBegin();
				break;
			case '2':
				puts("");
				retval = guiBegin();
				break;
			case '3':
				puts("");
				retval = monBegin();
				break;
			case 'D':
			case 'd':
				__global_flags[GLOBAL_FLAG_DEBUG] = !__global_flags[GLOBAL_FLAG_DEBUG];
				break;
			default:
				tmp = NULL;
				retval = 0;
				break;
		}
		if (tmp) tui_cls();
	}
}

void _exit(int status) {
	while (1);
}

//---------------------------------------------------

int register_bootmenu_entry(const struct entry_info *entry) {
	int i = 0;
	while (extra_bootmenu_entries[i] != NULL) i++;
	extra_bootmenu_entries[i] = entry;
	return 0;
}

int init_bootmenu_entrys(void) {
	for (int i = 0; i < EXTRA_BOOTMENUS_MAX; i++) extra_bootmenu_entries[i] = NULL;
	return 0;
}

//---------------------------------------------------

const char const b_logo[] =
	"\r\n"
	"     .oooooo.  o8o  oooooooooo.      .oooooo.     .ooooooo.  \r\n"
	"    d8P'  `Y8b `YP  `888'   `Y8b    d8P   `Y8b   d88'   `Y8b  \r\n"
	"   888      Y8'  '   888      888  888      888  Y88bo.       \r\n"
	"   888               888      888  888      888   `\"Y8888o.  \r\n"
	"   888    ooooo      888      888  888      888       `\"Y88b \r\n"
	"   `88.    .88'      888     d88'  `88b    d88'  o88.   .d8P  \r\n"
	"    `\"8bood8\"'      o888bood8\"'     `\"8bood8\"'    `\"88888P' \r\n"
	SET_COLOR_FG(C_WHITE) "\r\n"
	"                         Booter Menu                        \r\n";

const char const b_opts[] =
	COLOR_FG(C_YELLOW, "--=-========================================================-=--\r\n")
	COLOR_FG(C_LIGHTBLUE, "  0)") COLOR_FG(C_WHITE, "  RAM Test\r\n")
	COLOR_FG(C_LIGHTBLUE, "  1)") COLOR_FG(C_WHITE, "  Shell  (default)\r\n")
	COLOR_FG(C_LIGHTBLUE, "  2)") COLOR_FG(C_WHITE, "  TUI interface demo\r\n")
	COLOR_FG(C_LIGHTBLUE, "  3)") COLOR_FG(C_WHITE, "  G'mon\r\n")
	COLOR_FG(C_YELLOW, "--=-========================================================-=--\r\n");
