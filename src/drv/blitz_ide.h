//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// pata Header

#ifndef _DEV_BLITZ_IDE_HEADER
#define _DEV_BLITZ_IDE_HEADER

#define BLITZ_IDE_TIMEOUT	0x2FFFFF

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07

//---------------------------------------------------

#define blitz_ide_inb(addr)			inb(addr+0x10000)
#define blitz_ide_outb(val, addr)	outb(val, addr+0x10000)
#define blitz_ide_inw(addr)			inw(addr)
#define blitz_ide_outw(val, addr)	outw(val, addr)

#define blitz_ide_reg_outb(val, reg)	blitz_ide_outb(val, BLITZ_IDE_BASE+reg)
#define blitz_ide_reg_inb(reg)		blitz_ide_inb(BLITZ_IDE_BASE+reg)
#define blitz_ide_reg_outw(val, reg)	blitz_ide_outw(val, BLITZ_IDE_BASE+reg)

#define blitz_ide_reg_inw(reg)		(blitz_ide_inw(BLITZ_IDE_BASE+reg))
#define blitz_ide_reg_inw_le(reg)	(swapw(blitz_ide_inw(BLITZ_IDE_BASE+reg)))

//---------------------------------------------------

typedef struct {
	uint8_t		exists;
	uint8_t		dNum;
	uint16_t	signature;
	uint16_t	cylinders;
	uint16_t	heads;
	uint16_t	numSecsInTrack;
	uint32_t	size;				// size in sectors
} blitz_ide_dev;

//-----------------Function Protos-------------------

diskStatus blitz_ide_init(uint8_t drive);
diskStatus blitz_ide_status(uint8_t drive);
diskResult blitz_ide_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len);
diskResult blitz_ide_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len);
diskResult blitz_ide_ioctl(uint8_t drive, uint8_t cmd, void *buff);
static bool blitz_ide_wait_bsy();
static bool blitz_ide_wait_drq();
static bool blitz_ide_wait_chk(uint8_t flag);
static void blitz_ide_read_blk(uint16_t *buff);
static void blitz_ide_read_blk_le(uint16_t *buff);
static void blitz_ide_write_blk(uint16_t *buff);

#endif
