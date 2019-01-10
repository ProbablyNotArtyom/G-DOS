//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _FATROM_HEADER
#define _FATROM_HEADER

//-----------------Function Protos-------------------

diskStatus fatRom_init(uint8_t drive);
diskStatus fatRom_status(uint8_t drive);
diskResult fatRom_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint32_t len);
diskResult fatRom_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint32_t len);
diskResult fatRom_ioctl(uint8_t drive, uint8_t cmd, void *buff);

#endif
