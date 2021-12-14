//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// RAMdisk Header

#ifndef _RAMDISK_HEADER
#define _RAMDISK_HEADER

//-----------------Function Protos-------------------

diskStatus ramdisk_init(uint8_t drive);
diskStatus ramdisk_status(uint8_t drive);
diskResult ramdisk_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len);
diskResult ramdisk_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len);
diskResult ramdisk_ioctl(uint8_t drive, uint8_t cmd, void *buff);

#endif
