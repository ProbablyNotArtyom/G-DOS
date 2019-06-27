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

#define TEXT_1HEIGHT_1WIDTH		5
#define TEXT_1HEIGHT_2WIDTH		6

//---------------------------------------------------

#define tui_cls()				printf("\ec")						// Clears the screen and homes to the top of the terminal
#define tui_cursor_off()		printf("\e[?25l")
#define tui_cursor_on()			printf("\e[?25h")

#define tui_cursor_up(x)		printf("\e["#x"A")
#define tui_cursor_down(x)		printf("\e["#x"B")
#define tui_cursor_left(x)		printf("\e["#x"D")
#define tui_cursor_right(x)		printf("\e["#x"C")
#define tui_cursor_home()		printf("\e[H")
#define tui_cursor_setpos(x,y)	printf("\e["#y";"#x"H")

#define tui_line_erase()		printf("\e[2K")
#define tui_line_erase()		printf("\e[2K")

#define tui_mode_set(x)			printf("\e#"#x)

//---------------------------------------------------

#endif
