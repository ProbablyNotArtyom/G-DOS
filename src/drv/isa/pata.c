//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// IDE Driver

#ifdef CONFIG_DEV_ISA_PATA

 	#include <std.h>
	#include <fs.h>
	#include <disk.h>
	#include <mod/init.h>
	#include "pata.h"

	static uint8_t *currentIndex;
	static uint16_t currentSec;

//--------------------Functions----------------------

void ide_dev_register(){
	struct dev_disk disk;
	puts("\n\rIDE driver initializing....");
	disk.init = &ide_init;
	disk.status = &ide_status;
	disk.write = &ide_write;
	disk.read = &ide_read;
	disk.ioctl = &ide_ioctl;
	diskRegister(&disk);
	return;
}

diskStatus ide_init(uint8_t drive){


	//currentIndex = &_binary_fatROM_start;
	//currentSec = 0;

	//memcpy(disk->buff, currentIndex, 512);
	//memset(disk->buff, 0, 512);
	//disk->buffSector = 0;

	return STA_NODISK;
}

diskStatus ide_status(uint8_t drive){
	return STA_NODISK;
}

diskResult ide_write(uint8_t drive, const uint8_t *buff, uint32_t sector, int len){
	return RES_NOTRDY;
}

diskResult ide_read(uint8_t drive, uint8_t *buff, uint32_t sector, int len){
	//if (sector > _binary_fatROM_size / 512) return DSK_ARGS;
	//currentSec = sector;
	//currentIndex = &_binary_fatROM_start + sector * 512;
	//memset(buff, 0, len);
	//memcpy(buff, currentIndex, len);
	return RES_NOTRDY;
}

diskResult ide_ioctl(uint8_t drive, uint8_t cmd, void *buff){

	return RES_NOTRDY;
}

device_initcall(ide_dev_register);

#endif
