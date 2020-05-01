 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // CGAterm

 #ifdef CONFIG_DEV_ISA_CGA
 #define CONFIG_DEV_ISA_CGA

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <io.h>
	#include <mod/init.h>

	#include "isa.h"
	#include "cga.h"

//---------------------------------------------------

#define CGA_MEMSIZE		0x4000
#define FAST_SCROLLING					// Defining this enables use of the flickerfree scrolling routine
										// This routine is fast, but clobbers the entire VRAM as a result
										// This should generally be enabled

//---------------------------------------------------

#define getCursor()			((vram_cursor_y * 80) + vram_cursor_x)

#ifndef FAST_SCROLLING
uint8_t vram_local[2000];				// local copy of video memory because CGA is slow
#endif
uint8_t 	vram_cursor_x;
uint8_t		vram_cursor_y;
uint16_t	vram_current;
bool 		isInit = false;

//---------------------------------------------------

static bool ignored = false;
static char clense_escapes(char chr) {
	if (chr == '\033' && !ignored) {
		ignored = true;
		return NULL;
	} else if (ignored) {
		switch (chr) {
			case 'H':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'J':
			case 'P':
			case 'm':
			case 'h':
			case 'l':
			case 'I':
			case 'O':
				ignored = false;
				return NULL;
			default:
				return NULL;
		}
	}
	return chr;
}

char isa_cga_dev_read(){
	while(1);
}

charResult isa_cga_dev_write(char out){
	if (isInit == false) return CH_OK;
	out = clense_escapes(out);
	if (out == NULL) return CH_OK;
	if (vram_cursor_x >= 80) {
		vram_cursor_x = 0;
		if (vram_cursor_y <= 24){
			cReturn();
		}
	}

	if (out == '\r'){
		vram_cursor_x = 0;
	} else if (out == '\n'){
		vram_cursor_x = 0;
		cReturn();
	} else if (out == 0x7F){
#ifndef FAST_SCROLLING
		isa_mem_outb(' ', 0xB8000 + (getCursor()*2)-1);
#else
		write_frame(' ', getCursor()*2-2);
#endif
		vram_cursor_x--;
		setCursor();
	} else if (out == '\b'){
		vram_cursor_x--;
		setCursor();
	} else {
#ifndef FAST_SCROLLING
		vram_local[getCursor()] = out;
		isa_mem_outb(out, 0xB8000 + getCursor()*2);
#else
		write_frame(out, getCursor()*2);
#endif
		vram_cursor_x++;
	}

	setCursor();
	return CH_OK;
}

charResult isa_cga_dev_init(){

	isa_io_outb(0x00, CGA_REG_MODE_CTL);

	/* init CGA card registers to PC BIOS defaults for 80x25 text mode*/
	isa_io_outb(CGA_INDEX_HORIZ_TOTAL, CGA_REG_INDEX);
	isa_io_outb(0x71, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_HORIZ_DISP, CGA_REG_INDEX);
	isa_io_outb(0x50, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_HORIZ_SYNC_POS, CGA_REG_INDEX);
	isa_io_outb(0x5A, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_HORIZ_SYNC_WIDTH, CGA_REG_INDEX);
	isa_io_outb(0x0A, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_TOTAL, CGA_REG_INDEX);
	isa_io_outb(0x1F, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_TOTAL_ADJ, CGA_REG_INDEX);
	isa_io_outb(0x06, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_DISP, CGA_REG_INDEX);
	isa_io_outb(0x19, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_SYNC_POS, CGA_REG_INDEX);
	isa_io_outb(0x1C, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_INTERLACE_MODE, CGA_REG_INDEX);
	isa_io_outb(0x02, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_MAX_SCAN_ADDR, CGA_REG_INDEX);
	isa_io_outb(0x07, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_CURSOR_START, CGA_REG_INDEX);
	isa_io_outb(0x06, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_CURSOR_END, CGA_REG_INDEX);
	isa_io_outb(0x07, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_START_ADDR_H, CGA_REG_INDEX);
	isa_io_outb(0x00, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_START_ADDR_L, CGA_REG_INDEX);
	isa_io_outb(0x00, CGA_REG_DATA);


	isa_io_outb(0x00, CGA_REG_MODE_CTL);
	isa_io_outb(0x00, CGA_REG_COLOR_CTL);
	isa_io_outb(0x0D, CGA_REG_MODE_CTL);
	isa_io_outb(0x0D, CGA_REG_MODE_CTL);

#ifndef FAST_SCROLLING
	for (int i = 0; i < 2000; i++) vram_local[i] = ' ';			// Zero our local
#else
	vram_current = 0x0000;
#endif
	for (int i = 0; i < CGA_MEMSIZE; i+=2){							// Setup actual CGA memory
		isa_mem_outb(0x20, 0xB8000+i);
		isa_mem_outb(0x0F, 0xB8000+i+1);
	}
	vram_cursor_y = 0x00;
	vram_cursor_x = 0x00;
	isInit = true;

	return CH_OK;
}

static void drawFrame(){
#ifndef FAST_SCROLLING
	for (int i = 0; i < 2000; i++){
		isa_mem_outb(vram_local[i], (0xB8000+(i*2)));
	}
#endif
}

static void cReturn(){
	if (vram_cursor_y < 24){
		vram_cursor_y++;
	} else {
#ifndef FAST_SCROLLING
		for (int i = 0; i < 1920; i++){
			vram_local[i] = vram_local[i+80];
		}
		for (int i = 1920; i < 2000; i++){
			vram_local[i] = ' ';
		}
		drawFrame();
#else
		if (vram_current+160 >= CGA_MEMSIZE)
			vram_current = (vram_current+160)-CGA_MEMSIZE;
		else
			vram_current += 160;
		for (int i = 1920; i < 2000; i++){
			write_frame(' ', i*2);
		}
		isa_io_outb(CGA_INDEX_START_ADDR_H, CGA_REG_INDEX);
		isa_io_outb((uint8_t)((vram_current/2 >> 8) & 0xFF), CGA_REG_DATA);
		isa_io_outb(CGA_INDEX_START_ADDR_L, CGA_REG_INDEX);
		isa_io_outb((uint8_t)(vram_current/2 & 0xFF), CGA_REG_DATA);
#endif
	}
	return;
}

static void setCursor(){
#ifndef FAST_SCROLLING
	uint16_t cPos = getCursor();
#else
	uint16_t cPos;
	if (vram_current+(getCursor()*2) >= CGA_MEMSIZE)
		cPos = (((vram_current) + getCursor()*2)-CGA_MEMSIZE)/2;
	else
		cPos = ((vram_current) + getCursor()*2)/2;
#endif
	isa_io_outb(CGA_INDEX_CURSOR_ADDR_L, CGA_REG_INDEX);
	isa_io_outb((uint8_t)(cPos & 0xFF), CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_CURSOR_ADDR_H, CGA_REG_INDEX);
	isa_io_outb((uint8_t)((cPos >> 8) & 0xFF), CGA_REG_DATA);
}

#ifdef FAST_SCROLLING
static void write_frame(uint8_t byte, uint16_t offset){
	if (vram_current+offset >= CGA_MEMSIZE)
		isa_mem_outb(byte, 0xB8000+(vram_current+offset)-CGA_MEMSIZE);
	else
		isa_mem_outb(byte, 0xB8000 + vram_current+offset);
}
#endif

//--------------------Functions----------------------

early_initcall(isa_cga_dev_init);

 #endif
