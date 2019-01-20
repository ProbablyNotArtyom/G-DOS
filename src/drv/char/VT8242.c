 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // PS/2 Keyboard Driver

 #ifdef CONFIG_DEV_VT8242
 #define CONFIG_DEV_VT8242

	#include <std.h>
	#include <char.h>
	#include <keycodes.h>
	#include "VT8242.h"

//---------------------------------------------------

uint8_t	lastKey;
uint8_t	keyBuff;

static char* _qwertzuiop = "qwertzuiop";
static char* _asdfghjkl = "asdfghjkl";
static char* _yxcvbnm = "yxcvbnm";
static char* _nums = "123456789";

struct vt8242_regs *vt8242_device;

//--------------------Functions----------------------

charResult vt8242_dev_write(char out){
	return CH_NOTRDY;
}

char vt8242_dev_read(){
	keyBuff = VT8242_DATA_READ(vt8242_device);
	while (keyBuff == lastKey){
		keyBuff = VT8242_DATA_READ(vt8242_device);
	}
	lastKey = keyBuff;
	return scancode_to_ascii(keyBuff);
}

charResult vt8242_dev_init(){
	vt8242_device = DEV_VT8242_BASE;

	VT8242_CMD_READ(vt8242_device);
	VT8242_DATA_READ(vt8242_device);
	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_WRITECMDBYTE);
	VT8242_CMD_WRITE(vt8242_device, VT8242_CMDBYTE_DISABLE_INHIBIT);

	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_SELFTEST);
	if (VT8242_DATA_READ(vt8242_device) != 0x55){
		puts("[!] Controller failed self test");
		return;
	}
	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_KEYBOARDTEST);
	if (VT8242_DATA_READ(vt8242_device) != 0x00){
		puts("[?] Keyboard not found");
		return CH_OK;
	}
	puts("Keyboard detected.");

	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_ENABLEMOUSE);
	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_KEYBOARDTEST);
	if (VT8242_DATA_READ(vt8242_device) != 0x00){
		puts("[?] Mouse not found");
	} else {
		puts("Mouse detected.");
	}

	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_ENABLEKBD);
	VT8242_CMD_WRITE(vt8242_device, VT8242_CMD_ENABLEMOUSE);
	return CH_OK;
}

char scancode_to_ascii(uint8_t code){
	if (code == KB_ENTER_PRESSED) return '\n';
	if (code == KB_SPACE_PRESSED) return ' ';
	if (code == KB_BACKSPACE_PRESSED) return 0x7F;
	if (code == KB_PERIOD_PRESSED) return '.';
	if (code == KB_SLASH_PRESSED) return '/';
	if (code == KB_BACKSLASH_PRESSED) return '\\';
	if (code == KB_0_PRESSED) return '0';
	if (code >= KB_1_PRESSED && code <= KB_9_PRESSED)
		return _nums[code - KB_1_PRESSED];
	if (code >= 0x10 && code <= 0x1C)
		return _qwertzuiop[code - 0x10];
	else if (code >= 0x1E && code <= 0x26)
		return _asdfghjkl[code - 0x1E];
	else if (code >= 0x2C && code <= 0x32)
		return _yxcvbnm[code - 0x2C];
	return NULL;
}

//device_initcall(vt8242_dev_init);

 #endif
