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

//---------------------------------------------------

struct vt8242_regs {
	uint8_t	data;
	uint8_t cmd;
};

//---------------------------------------------------

#define VT8242_CMD_WRITE(dev, byte)		(dev->cmd = (byte))
#define VT8242_CMD_READ(dev)			(dev->cmd)
#define VT8242_DATA_WRITE(dev, byte)	(dev->data = (byte))
#define VT8242_DATA_READ(dev)			(dev->data)

//-----------------Function Protos-------------------

char vt8242_dev_read();
charResult vt8242_dev_write(char out);
charResult vt8242_dev_init();
char scancode_to_ascii(uint8_t code);

#endif
