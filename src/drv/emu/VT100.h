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
#define TAB				4
#define MAX(x, y) (((size_t)(x) > (size_t)(y)) ? (size_t)(x) : (size_t)(y))
#define MIN(x, y) (((size_t)(x) < (size_t)(y)) ? (size_t)(x) : (size_t)(y))
#define CLINE(vt) (vt)->screen.lines[MIN((vt)->curs.row, (vt)->screen.numlines - 1)]

#define P0(x) (vt->params[x])
#define P1(x) (vt->params[x]? vt->params[x] : 1)
#define CB(vt, m, a) ((vt)->callback? (vt)->callback(m, vt, a, (vt)->p) : (void)0)
#define INESC ((vt)->state)

#ifndef VT_INVALID_CHAR
#define VT_INVALID_CHAR ((wchar_t)0xfffd)
#endif

//-----------------VT100 Emulation-------------------

typedef enum {
    VT_COLOR_DEFAULT = -1,
    VT_COLOR_BLACK = 0,
    VT_COLOR_RED,
    VT_COLOR_GREEN,
    VT_COLOR_YELLOW,
    VT_COLOR_BLUE,
    VT_COLOR_MAGENTA,
    VT_COLOR_CYAN,
    VT_COLOR_WHITE,
    VT_COLOR_MAX
} vt_color_t;

struct vt_attrs {
    bool 		bold;
    bool 		dim;
    bool 		italic;
    bool 		underline;
    bool 		blink;
    bool 		reverse;
    bool 		invisible;
    vt_color_t 	fg;
    vt_color_t 	bg;
};

struct vt_point {
    size_t		row;
    size_t		column;
};

struct vt_char {
    wchar_t 			chr;
    struct vt_attrs 	attrs;
};

struct vt_line {
    bool 			dirty;
    struct vt_char 	chars[];
};

struct vt_screen {
    size_t 			numlines;
    size_t 			numcols;
    struct vt_line **lines;
};

struct vt_state {
	struct vt_point		curs, last_curs;
	struct vt_attrs		attrs, last_attrs;
	struct vt_screen	screen;
	struct vt_line		*tabs;

	bool	dirty, acs, ignored;

	size_t		paramslen;
	size_t		params[VT_PARAM_MAX];
	size_t		arg;

	enum { TS_NULL, TS_ESC, TS_ARG } state;
};

//-----------------Function Protos-------------------


#endif
