//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// VT100 ANSI header

#ifndef _HEADER_TUI
#define _HEADER_TUI

//---------------------------------------------------

#define SGR_RESET		0
#define SGR_BOLD		1
#define SGR_FAINT		2
#define SGR_ITALIC		3
#define SGR_UNDERLINE	4
#define SGR_BLINK_SLOW	5
#define SGR_BLINK_FAST	6
#define SGR_INVERSE 	7

#define MSE_EVENT_MSE0	0
#define MSE_EVENT_MSE1	1
#define MSE_EVENT_MSE2	2

#define tui_cursor_off()		printf("\e[?25l")
#define tui_cursor_on()			printf("\e[?25h")
#define tui_cursor_home()		printf("\e[H")
#define tui_line_erase(x)		printf("\e[%dK", x)

#define tui_set_sgr(sgr)		printf("\e[%dm", sgr)
#define tui_unset_sgr(sgr)		printf("\e[%dm", sgr+20)

//---------------------------------------------------

typedef char  			*label_t;
typedef unsigned int	plot_t;
typedef void 			(*action_t)();

/* describes a single point in 2d space */
typedef struct {
	plot_t	x;
	plot_t	y;
} point_t;

/* describes the colors of a text cell */
typedef struct {
	unsigned char	fg;
	unsigned char	bg;
} color_t;

/* x and y are the coordinates of the lower left corner of the box */
struct gfx_box {
	plot_t	height;
	plot_t	width;
	plot_t	x;
	plot_t	y;
	char	fill;
};

// - - - - - - - - - - - - - - - - - - - - - - - - -

struct menu_entry {
	char 				*label;
	void 				(*action)(void);
	struct gfx_box		bg;
	struct menu_entry	*prev;
	struct menu_entry	*next;
};

struct entry_info {
	label_t 	label;
	action_t 	action;
};

// - - - - - - - - - - - - - - - - - - - - - - - - -

typedef enum {
	 ALLOW_MOUSE = 0b00000001
} tui_menu_cfg_t;

typedef struct {
	color_t				selected;
	color_t				normal;
	unsigned int		numEntries;
	tui_menu_cfg_t		flags;
	struct gfx_box		bounds;

	struct entry_info	entry[];
} menu_info_t;

typedef struct {
	color_t				selected;
	color_t				normal;
	tui_menu_cfg_t		flags;
	struct menu_entry	*start;			// First entry on the list
	struct menu_entry	*end;			// Last entry on the list
	struct gfx_box		bounds;			// boundries of the menu
	unsigned int		numEntries;		// Number of entries on the list
} menu_list_t;

//---------------------------------------------------

void tui_cls();
void tui_cursor_up(const int len);
void tui_cursor_down(const int len);
void tui_cursor_left(const int len);
void tui_cursor_right(const int len);
void tui_cursor_setpos(const int x, const int y);
void tui_mode_set(const int mode);
void tui_color_fg(unsigned char color, char *str);
void tui_color_bg(unsigned char color, char *str);
void tui_set_color_fg_bg(unsigned char fg, unsigned char bg);
void tui_reset_color_fg_bg();
void tui_reset_color_fg();
void tui_reset_color_bg();

menu_list_t *new_menu_list(unsigned int numEntries, plot_t height, plot_t width, plot_t x, plot_t y);
menu_list_t *new_menu_list_from(menu_info_t *info);
void free_menu_list(menu_list_t *menu);
struct menu_entry *traverse_menu_list(menu_list_t *list, unsigned int index);
void inspect_menu_list(menu_list_t *list);

action_t trigger_menu_list(menu_list_t *list);

void draw_menu_list(menu_list_t *list, int selected_index);
void draw_gfx_box(struct gfx_box *box, unsigned char fg, unsigned char bg);

//---------------------------------------------------

#endif
