//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// RAMdisk driver

#ifdef CONFIG_DEV_RAMDISK

 	#include <std.h>
	#include <fs.h>
	#include <disk.h>
	#include <mod/init.h>
	#include "ramdisk.h"

	uint8_t *ramdisk_base;

	static uint8_t	*currentIndex;
	static uint32_t currentSec;

	static struct dev_disk disk;
	static bool exists;

//--------------------Functions----------------------

void ramdisk_dev_register(){
	exists = false;
	puts("\n\rRAMdisk driver initializing....");
	disk.init = &ramdisk_init;
	disk.status = &ramdisk_status;
	disk.write = &ramdisk_write;
	disk.read = &ramdisk_read;
	disk.ioctl = &ramdisk_ioctl;
	disk.local_drive_num = 0;
	diskRegister(&disk);
	return;
}

diskStatus ramdisk_init(uint8_t drive){
	if (exists == true) return STA_OK;
	else {
		ramdisk_base = (uint8_t *)malloc(DEV_RAMDISK_SIZE*1024);
		if (ramdisk_base == NULL) return (STA_NOINIT | STA_NODISK);
		currentIndex = ramdisk_base;
		currentSec = 0;
		exists = true;
		return STA_OK;
	}
}

diskStatus ramdisk_status(uint8_t drive){
	if (exists == true) return STA_OK;
	else return STA_NOINIT;
}

diskResult ramdisk_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len){
	//if (sector * 512 + len > RAMDISK_SIZE) return RES_ERROR;
	currentSec = sector;
	currentIndex = (uint32_t)ramdisk_base + sector * 512;
	memcpy(currentIndex, buff, len * 512);
	return RES_OK;
}

diskResult ramdisk_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len){
	//if (sector > RAMDISK_SIZE / 512) return RES_PARERR;
	currentSec = sector;
	currentIndex = (uint32_t)ramdisk_base + sector * 512;
	memcpy(buff, currentIndex, len * 512);
	return RES_OK;
}

diskResult ramdisk_ioctl(uint8_t drive, uint8_t cmd, void *buff){
	switch (cmd){
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(uint32_t*)buff = DEV_RAMDISK_SIZE * 2;
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

late_initcall(ramdisk_dev_register);

#endif
