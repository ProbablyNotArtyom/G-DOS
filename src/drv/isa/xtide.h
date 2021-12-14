//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// pata Header

#ifndef _DEV_ISA_XTIDE_HEADER
#define _DEV_ISA_XTIDE_HEADER

#define XTIDE_TIMEOUT	0x2FFFFF
#define	XTIDE_BASE		0x300

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x08
#define ATA_REG_FEATURES   0x08
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x0A
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x0C
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x0E
#define ATA_REG_STATUS     0x0E

//---------------------------------------------------

#define xtide_inb(addr)			inb( isa_io_vtor(addr))
#define xtide_outb(val, addr)	outb(val, isa_io_vtor(addr))
#define xtide_inw(addr)			inw( isa_io_vtor(addr))
#define xtide_outw(val, addr)	outw(val, isa_io_vtor(addr))

#define xtide_reg_outb(val, reg)	xtide_outb(val, XTIDE_BASE+reg)
#define xtide_reg_inb(reg)		xtide_inb(XTIDE_BASE+reg)
#define xtide_reg_outw(val, reg)	xtide_outb((val & 0xFF), XTIDE_BASE+1); \
								xtide_outb((val >> 8), XTIDE_BASE)

#define xtide_reg_inw(reg)		(xtide_inb(XTIDE_BASE) << 8 | (xtide_inb(XTIDE_BASE+1)))
#define xtide_reg_inw_le(reg)	(xtide_inb(XTIDE_BASE) | (xtide_inb(XTIDE_BASE+1) << 8))

//---------------------------------------------------

typedef struct {
	uint8_t		exists;
	uint8_t		dNum;
	uint16_t	signature;
	uint16_t	cylinders;
	uint16_t	heads;
	uint16_t	numSecsInTrack;
	uint32_t	size;				// size in sectors
} xtide_dev;

//-----------------Function Protos-------------------

diskStatus xtide_init(uint8_t drive);
diskStatus xtide_status(uint8_t drive);
diskResult xtide_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len);
diskResult xtide_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len);
diskResult xtide_ioctl(uint8_t drive, uint8_t cmd, void *buff);
static bool xtide_wait_bsy();
static bool xtide_wait_drq();
static bool xtide_wait_chk(uint8_t flag);
static void xtide_read_blk(uint16_t *buff);
static void xtide_read_blk_le(uint16_t *buff);
static void xtide_write_blk(uint16_t *buff);

#endif
