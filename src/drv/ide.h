//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// IDE Header

#ifndef _IDE_HEADER
#define _IDE_HEADER

//-----------------Function Protos-------------------

diskStatus ide_init(uint8_t drive);
diskStatus ide_status(uint8_t drive);
diskResult ide_write(uint8_t drive, const uint8_t *buff, uint32_t sector, int len);
diskResult ide_read(uint8_t drive, uint8_t *buff, uint32_t sector, int len);
diskResult ide_ioctl(uint8_t drive, uint8_t cmd, void *buff);

#endif
