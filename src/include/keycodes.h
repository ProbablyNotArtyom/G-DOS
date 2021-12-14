//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Shell header

#ifndef _HEADER_KEYCODES
#define _HEADER_KEYCODES

//---------------------------------------------------

#define KBRSP_ERR_DETECTION		0x00
#define KBRSP_SELFTEST_PASS		0xAA
#define KBRSP_ECHO				0xEE
#define KBRSP_ACK				0xFA
#define KBRSP_SELFTEST_FAIL		0xFC
#define KBRSP_SELFTEST_ERR		0xFD
#define KBRSP_RESEND			0xFE
#define KBRSP_ERR_OVERFLOW		0xFF

#define KBCMD_SET_LEDS			0xED
#define KBCMD_ECHO				0xEE
#define KBCMD_GET_KEYSET		0xF0
#define KBCMD_IDENTIFY			0xF2
#define KBCMD_SET_TYPEMATIC		0xF3
#define KBCMD_ENABLE_SCANNING	0xF4
#define KBCMD_DISABLE_SCANNING	0xF5
#define KBCMD_RESET				0xFF

//---------------------------------------------------

#define PS2_TAB				0x09
#define PS2_ENTER			'\r'
#define PS2_BACKSPACE		0x7F
#define PS2_ESC				0x1B
#define PS2_INSERT			0x00
#define PS2_DELETE			0x7F
#define PS2_HOME			0x00
#define PS2_END				0x00
#define PS2_PAGEUP			0x00
#define PS2_PAGEDOWN		0x00
#define PS2_UPARROW			0x00
#define PS2_LEFTARROW		0x00
#define PS2_DOWNARROW		0x00
#define PS2_RIGHTARROW		0x00
#define PS2_F1				0x00
#define PS2_F2				0x00
#define PS2_F3				0x00
#define PS2_F4				0x00
#define PS2_F5				0x00
#define PS2_F6				0x00
#define PS2_F7				0x00
#define PS2_F8				0x00
#define PS2_F9				0x00
#define PS2_F10				0x00
#define PS2_F11				0x00
#define PS2_F12				0x00
#define PS2_SCROLL			0x00


#define PS2_KEYMAP_SIZE 136

static const uint8_t const ps2_keymap[] = {
	0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7
};

static const uint8_t const ps2_keymap_shift[] = {
	0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7
};

//---------------------------------------------------

#endif
