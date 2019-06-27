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

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <tui.h>
	#include <gsh.h>

//-----------------Variable Protos-------------------



//-----------------------Main------------------------

void guiBegin(void){
	tui_cls();
	tui_cursor_setpos(10, 10);
	puts("cool.");
}
