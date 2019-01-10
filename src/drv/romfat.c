//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// IDE Driver

 	#include <std.h>
	#include <fs.h>
	#include <disk.h>
	#include "romfat.h"

	extern size_t _binary_fatROM_bin_start;
	extern size_t _binary_fatROM_bin_size;
	extern size_t _binary_fatROM_bin_end;

	static uint8_t *currentIndex;
	static uint16_t currentSec;

//--------------------Functions----------------------

void fatRom_dev_register(struct dev_disk *disk){
	fputs("rom-FAT driver initializing....");
	disk->init = &fatRom_init;
	disk->status = &fatRom_status;
	disk->write = &fatRom_write;
	disk->read = &fatRom_read;
	disk->ioctl = &fatRom_ioctl;
	puts(" OK");
	return;
}

diskStatus fatRom_init(uint8_t drive){

	if (drive != 0) return STA_NODISK;

	currentIndex = &_binary_fatROM_bin_start;
	currentSec = 0;

	puts(" OK");
	return;
}

diskStatus fatRom_status(uint8_t drive){
	if (drive != 0) return STA_NODISK;
	return STA_OK;
}

diskResult fatRom_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint32_t len){
	return RES_NOTRDY;
}

diskResult fatRom_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint32_t len){
	//if (sector > _binary_fatROM_bin_size / 512) return RES_PARERR;
	currentSec = sector;
	currentIndex = &_binary_fatROM_bin_start + sector * 512;
	memcpy(buff, currentIndex, len * 512);
	return RES_OK;
}

diskResult fatRom_ioctl(uint8_t drive, uint8_t cmd, void *buff){
	switch (cmd){
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(uint32_t*)buff = 256;
			return RES_OK;
		case GET_SECTOR_SIZE:
			*(uint16_t*)buff = 512;
			return RES_OK;
		case GET_BLOCK_SIZE:
			*(uint32_t*)buff = 1;
			return RES_OK;
		case CTRL_TRIM:
			return RES_OK;
		default: return RES_OK;
	}
}

setDiskInit(fatRom_dev_register);
