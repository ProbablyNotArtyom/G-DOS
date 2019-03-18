
	#include <std.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>


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

#define CGA_MEMSIZE		0x4000

#define isa_mem_vtor(addr)			0x00100000 + addr
#define isa_io_vtor(addr)			0x00300000 + addr

#define isa_io_outb(val, addr)		outb(val, isa_io_vtor(addr))
#define isa_io_inb(addr)			inb( isa_io_vtor(addr))
#define isa_mem_outb(val, addr)		outb(val, isa_mem_vtor(addr))
#define isa_mem_inb(addr)			inb( isa_mem_vtor(addr))

//---------------------------------------------------

static const char const brot_help_txt[] = {
	"Usage: brot [number of brots]\r\n"
};

result_t main(char *argv[], int argc){
	uint8_t color = 0;
	for (int i = 0; i < CGA_MEMSIZE; i+=4){							// Setup actual CGA memory
		isa_mem_outb(0x55, 0xB8000+i);
		color++;
		isa_mem_outb(color, 0xB8000+i+1);
		isa_mem_outb(0x55, 0xB8000+i+2);
		color++;
		isa_mem_outb(color+3, 0xB8000+i+3);
	}
	isa_io_outb(0x06, CGA_REG_COLOR_CTL);

	isa_io_outb(0x05, CGA_REG_MODE_CTL);

	isa_io_outb(CGA_INDEX_MAX_SCAN_ADDR, CGA_REG_INDEX);
	isa_io_outb(0x01, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_HORIZ_SYNC_WIDTH, CGA_REG_INDEX);
	isa_io_outb(0x0F, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_START_ADDR_H, CGA_REG_INDEX);
	isa_io_outb(0x00, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_START_ADDR_L, CGA_REG_INDEX);
	isa_io_outb(0x00, CGA_REG_DATA);

	isa_io_outb(CGA_INDEX_VERT_TOTAL, CGA_REG_INDEX);
	isa_io_outb(127, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_DISP, CGA_REG_INDEX);
	isa_io_outb(100, CGA_REG_DATA);
	isa_io_outb(CGA_INDEX_VERT_SYNC_POS, CGA_REG_INDEX);
	isa_io_outb(112, CGA_REG_DATA);

	isa_io_outb(0x01, CGA_REG_MODE_CTL);
	isa_io_outb(0x09, CGA_REG_MODE_CTL);
}
