//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// RAMdisk Header

#ifndef _FATDISK_HEADER
#define _FATDISK_HEADER

//-----------------Function Protos-------------------

diskStatus fatdisk_init(uint8_t drive);
diskStatus fatdisk_status(uint8_t drive);
diskResult fatdisk_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len);
diskResult fatdisk_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len);
diskResult fatdisk_ioctl(uint8_t drive, uint8_t cmd, void *buff);

#endif
