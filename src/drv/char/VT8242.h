//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _DEV_VT8242_HEADER
#define _DEV_VT8242_HEADER

//---------------------------------------------------

# define __force __attribute__((force))

#define VT8242_BUFFER_SIZE			16

#define VT8242_CMD_READCMDBYTE		0x20
#define VT8242_CMD_WRITECMDBYTE		0x60
#define VT8242_CMD_GETVERSION		0xA1
#define VT8242_CMD_CHECKPASS		0xA4
#define VT8242_CMD_DISABLEMOUSE		0xA7
#define VT8242_CMD_ENABLEMOUSE		0xA8
#define VT8242_CMD_MOUSETEST		0xA9
#define VT8242_CMD_SELFTEST			0xAA
#define VT8242_CMD_KEYBOARDTEST		0xAB
#define VT8242_CMD_DISABLEKBD		0xAD
#define VT8242_CMD_ENABLEKBD		0xAE
#define VT8242_CMD_GETMODE			0xCA
#define VT8242_CMD_WRITEOUTPORT		0xD1
#define VT8242_CMD_WRITEKBDOUT		0xD2
#define VT8242_CMD_WRITEMOUSEOUT	0xD3
#define VT8242_CMD_WRITEMOUSE		0xD4
#define VT8242_CMD_READTEST			0xE0

#define VT8242_STATUS_OUTBUFF_FULL	0b00000001
#define VT8242_STATUS_INBUFF_FULL	0b00000010
#define VT8242_STATUS_SYSFLAG		0b00000100
#define VT8242_STATUS_LASTACCESS	0b00001000
#define VT8242_STATUS_KEYLOCK		0b00010000
#define VT8242_STATUS_MOUSEOBF		0b00100000
#define VT8242_STATUS_TIMEOUT		0b01000000
#define VT8242_STATUS_PARITYERR		0b10000000

#define VT8242_CMDBYTE_OBFENABLE		0b00000001
#define VT8242_CMDBYTE_MOUSEOBF			0b00000010
#define VT8242_CMDBYTE_SYSFLAG			0b00000100
#define VT8242_CMDBYTE_DISABLE_INHIBIT	0b00001000
#define VT8242_CMDBYTE_DISABLE_KBD		0b00010000
#define VT8242_CMDBYTE_DISABLE_MOUSE	0b00100000
#define VT8242_CMDBYTE_ENABLE_TRANSLATE	0b01000000


#define SCAN_MODE_BREAK			0x01
#define SCAN_MODE_MODIFIER		0x02
#define SCAN_MODE_SHIFT_L		0x04
#define SCAN_MODE_SHIFT_R		0x08
#define SCAN_MODE_ALTGR 		0x10

#define KEY_MODE_SCROLLLOCK		0x01
#define KEY_MODE_NUMLOCK		0x02
#define KEY_MODE_CAPSLOCK		0x04


//---------------------------------------------------

struct vt8242_regs {
	uint8_t	data;
	uint8_t cmd;
};

//---------------------------------------------------

#define VT8242_CMD_WRITE(dev, byte)		((*(__force volatile uint8_t *) (dev+1)) = (byte))
#define VT8242_CMD_READ(dev)			({ uint8_t __v = (*(__force volatile uint8_t *) (dev+1)); __v; })
#define VT8242_DATA_WRITE(dev, byte)	((*(__force volatile uint8_t *) (dev)) = (byte))
#define VT8242_DATA_READ(dev)			({ uint8_t __v = (*(__force volatile uint8_t *) (dev)); __v; })

//-----------------Function Protos-------------------

char vt8242_dev_peek();
char vt8242_dev_read();
char vt8242_dev_getKey();
charResult vt8242_dev_init();
void vt8242_dev_flush();
void vt8242_dev_wait_out();
void vt8242_dev_wait_in();
uint8_t vt8242_dev_cmd_ret(uint8_t cmd);
uint8_t vt8242_dev_kbdcmd_ret(uint8_t cmd);
void vt8242_dev_set_leds(uint8_t leds);

#endif
