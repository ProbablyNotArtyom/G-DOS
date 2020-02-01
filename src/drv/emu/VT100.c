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

static void vt_handle_sgr(vt_state *state) {
	#define FGBG(c) *(P0(i) < 40? &vt->attrs.fg : &vt->attrs.bg) = c
    for (size_t i = 0; i < vt->npar; i++) switch (P0(i)){
        case  0: vt->attrs                    = defattrs;   break;
        case  1: case 22: vt->attrs.bold      = P0(0) < 20; break;
        case  2: case 23: vt->attrs.dim       = P0(0) < 20; break;
        case  4: case 24: vt->attrs.underline = P0(0) < 20; break;
        case  5: case 25: vt->attrs.blink     = P0(0) < 20; break;
        case  7: case 27: vt->attrs.reverse   = P0(0) < 20; break;
        case  8: case 28: vt->attrs.invisible = P0(0) < 20; break;
        case 10: case 11: vt->acs             = P0(0) > 10; break;
        case 30: case 40: FGBG(TMT_COLOR_BLACK);            break;
        case 31: case 41: FGBG(TMT_COLOR_RED);              break;
        case 32: case 42: FGBG(TMT_COLOR_GREEN);            break;
        case 33: case 43: FGBG(TMT_COLOR_YELLOW);           break;
        case 34: case 44: FGBG(TMT_COLOR_BLUE);             break;
        case 35: case 45: FGBG(TMT_COLOR_MAGENTA);          break;
        case 36: case 46: FGBG(TMT_COLOR_CYAN);             break;
        case 37: case 47: FGBG(TMT_COLOR_WHITE);            break;
        case 39: case 49: FGBG(TMT_COLOR_DEFAULT);          break;
    }
}

//---------------------------------------------------

 #endif
