//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// VT100 Emulation Header

#ifndef _EMU_VT100_HEADER
#define _EMU_VT100_HEADER

//---------------------------------------------------

#define VT_BUF_MAX		100
#define VT_PARAM_MAX	8

//-----------------VT100 Emulation-------------------

typedef enum{
    VT_COLOR_DEFAULT = -1,
    VT_COLOR_BLACK = 1,
    VT_COLOR_RED,
    VT_COLOR_GREEN,
    VT_COLOR_YELLOW,
    VT_COLOR_BLUE,
    VT_COLOR_MAGENTA,
    VT_COLOR_CYAN,
    VT_COLOR_WHITE,
    VT_COLOR_MAX
} vt_color_t;

typedef struct {
    bool 		bold;
    bool 		dim;
    bool 		underline;
    bool 		blink;
    bool 		reverse;
    bool 		invisible;
    vt_color_t 	fg;
    vt_color_t 	bg;
} vt_attrs;

typedef struct {
    size_t		row;
    size_t		column;
} vt_point;

typedef struct {
    wchar_t 	chr;
    vt_attrs 	attrs;
} vt_char;

typedef struct {
    bool 		dirty;
    vt_char 	chars[];
} vt_line;

struct vt_state {
	vt_point	curs, last_curs;
	vt_attrs	attrs, last_attrs;

	size_t		buflen;
	char		buf[VT_BUF_MAX];
	size_t		paramslen;
	size_t		params[VT_PARAM_MAX];
	size_t		arg;

	enum { TS_NULL, TS_ESC, TS_ARG } vt_state;
}

//-----------------Function Protos-------------------


#endif
