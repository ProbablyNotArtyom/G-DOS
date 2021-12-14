 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // VT100 Emulation

#ifdef CONFIG_EMU_VT100
#define CONFIG_EMU_VT100

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <io.h>
	#include <tui.h>
	#include <mod/init.h>

	#include "VT100.h"

//---------------------------------------------------

struct vt_attrs defattrs = {
	.bold = false,
    .dim = false,
    .underline = false,
    .blink = false,
    .reverse = false,
    .invisible = false,
	.fg = VT_COLOR_DEFAULT,
	.bg = VT_COLOR_DEFAULT
};

const struct entry_info debug_menuentry = {
	.label = "(DEBUG) flush VT100 screen attrs",
	.action = &tui_cls
};

//---------------------------------------------------

charResult vt100_screen_init() {
	puts("VT100 emulation / NotArtyom / 02-04-21");
	register_bootmenu_entry(&debug_menuentry);
	return CH_OK;
}

static struct vt_state *create_vt() {
	struct vt_state *vt = (struct vt_state *)malloc(sizeof(struct vt_state));
	if (vt == NULL) return NULL;

	memcpy(&defattrs, &(vt->attrs), sizeof(struct vt_attrs));
	return vt;
}

static void vt_handle_sgr(struct vt_state *vt) {

}

//---------------------------------------------------

subsys_initcall(vt100_screen_init);

#endif
