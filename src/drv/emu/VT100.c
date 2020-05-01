 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // VT100 Emulation

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <io.h>
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

//---------------------------------------------------

static void vt_handle_sgr(struct vt_state *vt) {
	
}

//---------------------------------------------------
