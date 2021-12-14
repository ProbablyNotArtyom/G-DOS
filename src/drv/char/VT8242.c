 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // PS/2 Keyboard Driver

 #ifdef CONFIG_DEV_VT8242
 #define CONFIG_DEV_VT8242

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <keycodes.h>
	#include <mod/init.h>
	#include "VT8242.h"

//---------------------------------------------------

uint8_t key_mode;
uint8_t state;

//--------------------Functions----------------------

char vt8242_dev_peek(){
	return NULL;
}

char vt8242_dev_read(){
	return vt8242_dev_getKey();
}

char vt8242_dev_getKey(){
	uint8_t register chr;
	uint8_t register tmp;
	while (true){
		vt8242_dev_wait_out();
		tmp = VT8242_DATA_READ(DEV_VT8242_BASE);

		if (tmp == 0xF0)
			state |= SCAN_MODE_BREAK;
		else if (tmp == 0xE0)
			state |= SCAN_MODE_MODIFIER;
		else {
			if (state & SCAN_MODE_BREAK){
				if (tmp == 0x12)
					state &= ~SCAN_MODE_SHIFT_L;
				else if (tmp == 0x59)
					state &= ~SCAN_MODE_SHIFT_R;
				else if (tmp == 0x11 && (state & SCAN_MODE_MODIFIER))
					state &= ~SCAN_MODE_ALTGR;
				state &= ~(SCAN_MODE_BREAK | SCAN_MODE_MODIFIER);
				continue;
			}
			if (tmp == 0x12) {
				state |= SCAN_MODE_SHIFT_L;
				continue;
			} else if (tmp == 0x59) {
				state |= SCAN_MODE_SHIFT_R;
				continue;
			} else if (tmp == 0x11 && (state & SCAN_MODE_MODIFIER)) {
				state |= SCAN_MODE_ALTGR;
			}

			if (tmp == 0x58) {
				key_mode ^= KEY_MODE_CAPSLOCK;
				vt8242_dev_set_leds(key_mode);
			} else if (tmp == 0x77) {
				key_mode ^= KEY_MODE_NUMLOCK;
				vt8242_dev_set_leds(key_mode);
			} else if (tmp == 0x7E) {
				key_mode ^= KEY_MODE_SCROLLLOCK;
				vt8242_dev_set_leds(key_mode);
			}

			chr = 0;
			if (state & SCAN_MODE_MODIFIER){
				switch (tmp){
					case 0x70: chr = PS2_INSERT;      	break;
					case 0x6C: chr = PS2_HOME;        	break;
					case 0x7D: chr = PS2_PAGEUP;      	break;
					case 0x71: chr = PS2_DELETE;      	break;
					case 0x69: chr = PS2_END;         	break;
					case 0x7A: chr = PS2_PAGEDOWN;    	break;
					case 0x75: chr = PS2_UPARROW;     	break;
					case 0x6B: chr = PS2_LEFTARROW;   	break;
					case 0x72: chr = PS2_DOWNARROW;   	break;
					case 0x74: chr = PS2_RIGHTARROW;  	break;
					case 0x4A: chr = '/';             	break;
					case 0x5A: chr = PS2_ENTER; 		break;
					default: break;
				}

			} else if (state & (SCAN_MODE_SHIFT_L | SCAN_MODE_SHIFT_R)){
				if (tmp < PS2_KEYMAP_SIZE) chr = ps2_keymap_shift[tmp];
			} else {
				if (tmp < PS2_KEYMAP_SIZE) chr = ps2_keymap[tmp];
			}

			if (key_mode & KEY_MODE_CAPSLOCK) chr = (char)toUpper(chr);

			state &= ~(SCAN_MODE_BREAK | SCAN_MODE_MODIFIER);
			if (chr != 0) {
				return chr;
			}
		}
	}
}

charResult vt8242_dev_init(){
	uint8_t register keyBuff;
	puts("\r\nVT8242 Keyboard driver / NotArtyom / 29-1-19");
	delay(0x4FFFF);
	vt8242_dev_flush();
	vt8242_dev_wait_in();

	keyBuff = vt8242_dev_cmd_ret(VT8242_CMD_SELFTEST);
	if (keyBuff != 0x55){
		printf("[!] Controller failed self test with value: 0x");
		printByte(keyBuff);
		puts("");
	}

	keyBuff = vt8242_dev_cmd_ret(VT8242_CMD_KEYBOARDTEST);
	if (keyBuff != 0x00){
		printf("[?] Keyboard not found: 0x");
		printByte(keyBuff);
		puts("");
	} else {
		puts("Keyboard detected.");
	}

	VT8242_CMD_WRITE(DEV_VT8242_BASE, VT8242_CMD_WRITECMDBYTE);
	vt8242_dev_wait_in();
	VT8242_DATA_WRITE(DEV_VT8242_BASE, 0b00101000);
	vt8242_dev_wait_in();

	keyBuff = vt8242_dev_kbdcmd_ret(KBCMD_ENABLE_SCANNING);
	if (keyBuff != KBRSP_ACK){
		printf("Keyboard enable failed: 0x");
		printByte(keyBuff);
		puts("");
	}

	vt8242_dev_flush();

	key_mode = 0x00;
	vt8242_dev_set_leds(key_mode);
	vt8242_dev_flush();
	state = 0x00;
	return CH_OK;
}

void vt8242_dev_flush(){
	uint8_t tmp;
	for (uint8_t i = 0; i < VT8242_BUFFER_SIZE; i++){
		tmp = VT8242_DATA_READ(DEV_VT8242_BASE);
	}
}

void vt8242_dev_wait_out(){
	uint8_t tmp;
	do {
		tmp = VT8242_CMD_READ(DEV_VT8242_BASE) & VT8242_STATUS_OUTBUFF_FULL;
	} while (tmp == 0x00);
}

void vt8242_dev_wait_in(){
	uint8_t tmp;
	do {
		tmp = VT8242_CMD_READ(DEV_VT8242_BASE) & VT8242_STATUS_INBUFF_FULL;
	} while (tmp != 0x00);
}

uint8_t vt8242_dev_cmd_ret(uint8_t cmd){
	VT8242_CMD_WRITE(DEV_VT8242_BASE, cmd);
	vt8242_dev_wait_out();
	return VT8242_DATA_READ(DEV_VT8242_BASE);
}

uint8_t vt8242_dev_kbdcmd_ret(uint8_t cmd){
	VT8242_DATA_WRITE(DEV_VT8242_BASE, cmd);
	vt8242_dev_wait_out();
	return VT8242_DATA_READ(DEV_VT8242_BASE);
}

void vt8242_dev_set_leds(uint8_t leds){
	VT8242_DATA_WRITE(DEV_VT8242_BASE, KBCMD_SET_LEDS);
	vt8242_dev_wait_out();
	if (VT8242_DATA_READ(DEV_VT8242_BASE) != KBRSP_ACK) return;
	VT8242_DATA_WRITE(DEV_VT8242_BASE, leds);
	vt8242_dev_wait_out();
	if (VT8242_DATA_READ(DEV_VT8242_BASE) != KBRSP_ACK) return;
}

device_initcall(vt8242_dev_init);

 #endif
