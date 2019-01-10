//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// RAMdisk driver
#define CONFIG_DEV_RAMDISK
#ifdef CONFIG_DEV_RAMDISK

 	#include <std.h>
	#include <fs.h>
	#include <disk.h>
	#include "ramdisk.h"

	#define RAMDISK_SIZE		131072
	uint8_t ramdisk[RAMDISK_SIZE];

	static uint8_t	*currentIndex;
	static uint32_t currentSec;

//--------------------Functions----------------------

void ramdisk_dev_register(struct dev_disk *disk){
	fputs("RAMdisk driver initializing....");
	disk->init = &ramdisk_init;
	disk->status = &ramdisk_status;
	disk->write = &ramdisk_write;
	disk->read = &ramdisk_read;
	disk->ioctl = &ramdisk_ioctl;
	puts(" OK");
	return;
}

diskStatus ramdisk_init(uint8_t drive){

	if (drive != 0) return STA_NODISK;

	currentIndex = &ramdisk;
	currentSec = 0;

	return;
}

diskStatus ramdisk_status(uint8_t drive){
	if (drive != 0) return STA_NODISK;
	return STA_OK;
}

diskResult ramdisk_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint32_t len){
	//if (sector * 512 + len > RAMDISK_SIZE) return RES_ERROR;
	currentSec = sector;
	currentIndex = (uint32_t)&ramdisk + sector * 512;
	memcpy(currentIndex, buff, len * 512);
	return RES_OK;
}

diskResult ramdisk_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint32_t len){
	//if (sector > RAMDISK_SIZE / 512) return RES_PARERR;
	currentSec = sector;
	currentIndex = (uint32_t)&ramdisk + sector * 512;
	memcpy(buff, currentIndex, len * 512);
	return RES_OK;
}

diskResult ramdisk_ioctl(uint8_t drive, uint8_t cmd, void *buff){
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

setDiskInit(ramdisk_dev_register);

#endif
