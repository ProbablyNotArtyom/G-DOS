//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Shell UI

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <tui.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>

extern action_t monBegin;
extern action_t shellBegin;
static void absolutely_nothing();
void guiBegin(void);

//-----------------------Main------------------------

const struct entry_info const test_entry_info[5] = {
	{.label = "Start G'Mon", .action = &monBegin},
	{.label = "Start G'shell", .action = &shellBegin},
	{.label = "Do something", .action = &absolutely_nothing},
	{.label = "Do absolutely nothing", .action = &absolutely_nothing},
	{.label = "Exit", .action = NULL}
};

menu_info_t test_info;

//---------------------------------------------------

void guiBegin(void){
	test_info.selected.fg = C_WHITE;
	test_info.selected.bg = C_BLUE;
	test_info.normal.fg = C_DARKGREY;
	test_info.normal.bg = C_WHITE;
	test_info.flags = ALLOW_MOUSE;
	test_info.numEntries = 5;
	test_info.bounds.height = 5;
	test_info.bounds.width =  60;
	test_info.bounds.x = 1;
	test_info.bounds.y = 1;
	test_info.entry[0] = test_entry_info[0];
	test_info.entry[1] = test_entry_info[1];
	test_info.entry[2] = test_entry_info[2];
	test_info.entry[3] = test_entry_info[3];
	test_info.entry[4] = test_entry_info[4];

	tui_cls();
	menu_list_t *test_list = new_menu_list_from(&test_info);
	while (true) {
		action_t action = trigger_menu_list(test_list);
		tui_cls();
		if (action != NULL) {
			action();
		} else {
			return;
		}
	}
}

static void absolutely_nothing() {
	return;
}
