//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// RAMdisk driver

#ifdef CONFIG_DEV_FATDISK

 	#include <std.h>
	#include <fs.h>
	#include <disk.h>
	#include <mod/init.h>
	#include <mod/device.h>
	#include "fatdisk.h"

	extern unsigned char _binary___romdisk_img_start;
	extern unsigned char _binary___romdisk_img_end;
	extern unsigned char _binary___romdisk_img_size;

	static const char drv_name[] = "fatdisk";
	const char drv_vendor[] = "notartyom";

	uint8_t *fatdisk_base;

	static uint8_t	*currentIndex;
	static uint32_t currentSec;
	static bool exists;

//--------------------Functions----------------------

void fatdisk_dev_register(){
	exists = false;
	puts("FAT-based romdisk driver / NotArtyom / 06-30-19");

	struct device_info *driver = (struct device_info *)malloc(sizeof(struct device_info));
	driver->driver_disk = (struct dev_disk *)malloc(sizeof(struct dev_disk));

	driver->name = &drv_name;
	driver->vendor = &drv_vendor;
	driver->type = DEVTYPE_BLOCK;

	driver->driver_disk->init = &fatdisk_init;
	driver->driver_disk->status = &fatdisk_status;
	driver->driver_disk->write = &fatdisk_write;
	driver->driver_disk->read = &fatdisk_read;
	driver->driver_disk->ioctl = &fatdisk_ioctl;
	driver->driver_disk->local_drive_num = 0;
	diskRegister(driver);
	return;
}

diskStatus fatdisk_init(uint8_t drive){
	if (exists == true) return STA_OK;
	else {
		fatdisk_base = &_binary___romdisk_img_start;
		currentIndex = fatdisk_base;
		currentSec = 0;
		exists = true;
		return STA_OK;
	}
}

diskStatus fatdisk_status(uint8_t drive){
	if (exists == true) return STA_OK;
	else return STA_NOINIT;
}

diskResult fatdisk_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len){
	return RES_WRPRT;
}

diskResult fatdisk_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len){
	//if (sector > FATDISK_SIZE / 512) return RES_PARERR;
	currentSec = sector;
	currentIndex = (uint32_t)fatdisk_base + sector * 512;
	memcpy(buff, currentIndex, len * 512);
	return RES_OK;
}

diskResult fatdisk_ioctl(uint8_t drive, uint8_t cmd, void *buff){
	switch (cmd){
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(uint32_t*)buff = _binary___romdisk_img_size * 2;
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

late_initcall(fatdisk_dev_register);

#endif
