//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Disk device handling

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>


	#include <disk.h>
	#include <fs.h>
	#include <mod/device.h>

//---------------------------------------------------

struct device_info *diskList[4];
const char const *diskMapping[] = { FF_DRIVER_STRS };

//-----------------------Main------------------------

void diskInit(void) {
	return;
}

void diskRegister(struct device_info *disk){
	printf("Driver: %s, Vendor: %s\n\r", disk->name, disk->vendor);
	for (int i = 0; i < ARRAY_SIZE(diskMapping); i++) {
		if (!strcmp(diskMapping[i], disk->name)) {
			diskList[i] = disk;
			printf("Added disk %d to disks\n\r", i);
			return;
		}
	}
	puts("[?] Unable to map disk to a drive.");
	return;
}

diskStatus disk_initialize(uint8_t drive){
	return diskList[drive]->driver_disk->init(diskList[drive]->driver_disk->local_drive_num);
}

diskStatus disk_status(uint8_t drive){
	return diskList[drive]->driver_disk->status(diskList[drive]->driver_disk->local_drive_num);
}

diskResult disk_read(uint8_t drive, uint8_t* buff, uint32_t sector, uint8_t count){
	return diskList[drive]->driver_disk->read(diskList[drive]->driver_disk->local_drive_num, buff, sector, count);
}

diskResult disk_write(uint8_t drive, const uint8_t* buff, uint32_t sector, uint8_t count){
	return diskList[drive]->driver_disk->write(diskList[drive]->driver_disk->local_drive_num, buff, sector, count);
}

diskResult disk_ioctl(uint8_t drive, uint8_t cmd, void* buff){
	return diskList[drive]->driver_disk->ioctl(diskList[drive]->driver_disk->local_drive_num, cmd, buff);
}
