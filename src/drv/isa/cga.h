//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _DEV_ISA_CGA_HEADER
#define _DEV_ISA_CGA_HEADER

//---------------------------------------------------

#define CGA_REG_INDEX			0x03D4
#define CGA_REG_DATA			0x03D5
#define CGA_REG_MODE_CTL		0x03D8
#define CGA_REG_COLOR_CTL		0x03D9
#define CGA_REG_STATUS			0x03DA

#define CGA_INDEX_HORIZ_TOTAL		0
#define CGA_INDEX_HORIZ_DISP		1
#define CGA_INDEX_HORIZ_SYNC_POS	2
#define CGA_INDEX_HORIZ_SYNC_WIDTH	3
#define CGA_INDEX_VERT_TOTAL		4
#define CGA_INDEX_VERT_TOTAL_ADJ	5
#define CGA_INDEX_VERT_DISP			6
#define CGA_INDEX_VERT_SYNC_POS		7
#define CGA_INDEX_INTERLACE_MODE	8
#define CGA_INDEX_MAX_SCAN_ADDR		9
#define CGA_INDEX_CURSOR_START		10
#define CGA_INDEX_CURSOR_END		11
#define CGA_INDEX_START_ADDR_H		12
#define CGA_INDEX_START_ADDR_L		13
#define CGA_INDEX_CURSOR_ADDR_H		14
#define CGA_INDEX_CURSOR_ADDR_L		15
#define CGA_INDEX_LP_H				16
#define CGA_INDEX_LP_L				17

//-----------------VT100 Emulation-------------------

enum {
	TSTATE_NULL,
	TSTATE_ESC,
	TSTATE_ARG
} vt_state;

//-----------------Function Protos-------------------

char isa_cga_dev_read();
charResult isa_cga_dev_write(char out);
charResult isa_cga_dev_init();
static void drawFrame();
static void cReturn();
static void setCursor();
static void write_frame(uint8_t byte, uint16_t offset);
static char clense_escapes(char chr);

#endif
