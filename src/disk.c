//---------------------------------------------------
//
//	GBoot v0.0
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

//---------------------------------------------------

uint8_t numDisk = 0;
struct dev_disk diskList[4];

//-----------------------Main------------------------

void diskInit(void) {

	diskinitcall_t *fn = &__start_diskinitsec;
	while (fn < &__stop_diskinitsec) {
		(*fn)(&diskList[numDisk]);
		numDisk++;
		fn++;
	}
	return;
}

diskStatus disk_initialize(uint8_t drive){
	if (drive > numDisk) return STA_NODISK;
	return diskList[drive].init(0);
}

diskStatus disk_status(uint8_t drive){
	if (drive > numDisk) return STA_NODISK;
	return diskList[drive].status(0);
}

diskResult disk_read(uint8_t drive, uint8_t* buff, uint32_t sector, uint32_t count){
	if (drive > numDisk) return RES_NOTRDY;
	return diskList[drive].read(0, buff, sector, count);
}

diskResult disk_write(uint8_t drive, const uint8_t* buff, uint32_t sector, uint32_t count){
	if (drive > numDisk) return RES_NOTRDY;
	return diskList[drive].write(0, buff, sector, count);
}

diskResult disk_ioctl(uint8_t drive, uint8_t cmd, void* buff){
	if (drive > numDisk) return RES_NOTRDY;
	return diskList[drive].ioctl(0, cmd, buff);
}
