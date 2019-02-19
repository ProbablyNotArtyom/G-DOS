//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// pata Header

#ifndef _DEV_ISA_PATA_HEADER
#define _DEV_ISA_PATA_HEADER

#define PATA_TIMEOUT	0x2FFFFF
#define	PATA_BASE		0x300

//---------------------------------------------------

#define pata_inb(addr)			inb( isa_io_vtor(addr))
#define pata_outb(val, addr)	outb(val, isa_io_vtor(addr))
#define pata_inw(addr)			inw( isa_io_vtor(addr))
#define pata_outw(val, addr)	outw(val, isa_io_vtor(addr))

#define pata_reg_outb(val, reg)	pata_outb(val, PATA_BASE+reg)
#define pata_reg_inb(reg)		pata_inb(PATA_BASE+reg)
#define pata_reg_outw(val, reg)	pata_outb((val >> 8), PATA_BASE+1); \
								pata_outb((val & 0xFF), PATA_BASE)

#define pata_reg_inw(reg)		(pata_inb(PATA_BASE) | (pata_inb(PATA_BASE+1) << 8))

#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01
/*
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
*/
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

#define FLAG_ATA_IS_EXIST	0x01
#define FLAG_ATA_IS_INIT	0x02

//---------------------------------------------------

typedef struct {
	uint8_t		exists;
	uint8_t		dNum;
	uint16_t	signature;
	uint16_t	cylinders;
	uint16_t	heads;
	uint16_t	numSecsInTrack;
	uint32_t	size;				// size in sectors
} pata_dev;

//-----------------Function Protos-------------------

diskStatus pata_init(uint8_t drive);
diskStatus pata_status(uint8_t drive);
diskResult pata_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len);
diskResult pata_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len);
diskResult pata_ioctl(uint8_t drive, uint8_t cmd, void *buff);
static bool pata_wait_bsy();
static bool pata_wait_drq();
static bool pata_wait_chk(uint8_t flag);
static void pata_read_blk(uint16_t *buff);
static void pata_write_blk(uint16_t *buff);

#endif
