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
	#include <stdbool.h>
	#include <fs.h>
	#include <disk.h>
	#include <mod/init.h>
	#include <io.h>

	#include "isa.h"
	#include "pata.h"

pata_dev *ide_disk[2];
struct dev_disk *pata_disk[2];

//--------------------Functions----------------------

void pata_dev_register(){

	puts("PATA Disk Driver / NotArtyom / 12-2-19");
	puts("Waiting for disks...");

	for (int i = 0; i < 2; i++){
		pata_wait_bsy();
		if (i == 0)
			pata_reg_outb(0xE0, ATA_REG_HDDEVSEL);
		else
			pata_reg_outb(0xF0, ATA_REG_HDDEVSEL);
		pata_wait_bsy();
		pata_reg_outb(0x00, ATA_REG_SECCOUNT0);
		pata_reg_outb(0x00, ATA_REG_LBA0);
		pata_reg_outb(0x00, ATA_REG_LBA1);
		pata_reg_outb(0x00, ATA_REG_LBA2);

		pata_reg_outb(ATA_CMD_IDENTIFY, ATA_REG_COMMAND);	// Send identify command
		delay(0xFFFF);

		if (pata_reg_inb(ATA_REG_STATUS) == 0){
			nprintf("[?] IDE disk %d not detected.", i);
		} else {
			nprintf("IDE disk %d detected.", i);
			ide_disk[i] = (pata_dev *)pmalloc(sizeof(pata_dev));
			pata_disk[i] = (struct dev_disk *)pmalloc(sizeof(struct dev_disk));
			ide_disk[i]->exists == 0x00;
			/* read in the identify buffer */
			uint16_t idBuff[256];
			if (!pata_wait_chk(ATA_SR_DRQ)) return RES_ERROR;
			pata_read_blk_le(&idBuff);

			ide_disk[i]->exists = FLAG_ATA_IS_EXIST;
			ide_disk[i]->dNum = 0;
			ide_disk[i]->signature = idBuff[0];
			ide_disk[i]->cylinders = idBuff[1];
			ide_disk[i]->heads = idBuff[3];
			ide_disk[i]->numSecsInTrack = idBuff[6];
			ide_disk[i]->size = ide_disk[i]->cylinders * ide_disk[i]->heads * ide_disk[i]->numSecsInTrack;

			fputs("model: ");
			putsl((&idBuff[27]), 20);
			putnl();
			nprintf("sig:        0x%X", ide_disk[i]->signature);
			nprintf("cylinders:  %d", ide_disk[i]->cylinders);
			nprintf("heads:      %d", ide_disk[i]->heads);
			nprintf("sectors:    %d", ide_disk[i]->size);
			nprintf("size:       %dMB", ide_disk[i]->size / 2048);
			delay(0x8FFFF);

			pata_disk[i]->init = &pata_init;
			pata_disk[i]->status = &pata_status;
			pata_disk[i]->write = &pata_write;
			pata_disk[i]->read = &pata_read;
			pata_disk[i]->ioctl = &pata_ioctl;
			diskRegister(pata_disk[i]);
		}
	}
	return;
}

diskStatus pata_init(uint8_t drive){
	if (ide_disk[drive]->exists & FLAG_ATA_IS_INIT) return STA_OK;
	if (ide_disk[drive]->exists == FLAG_ATA_IS_EXIST){
		pata_reg_outb((0xE0 | (drive << 4)), ATA_REG_HDDEVSEL);
		ide_disk[drive]->exists = ide_disk[drive]->exists | FLAG_ATA_IS_INIT;
		return STA_OK;
	} else {
		return (STA_NOINIT | STA_NODISK);
	}
}

diskStatus pata_status(uint8_t drive){
	if (ide_disk[drive]->exists == FLAG_ATA_IS_EXIST)
		return STA_NOINIT;
	else if (ide_disk[drive]->exists == (FLAG_ATA_IS_EXIST | FLAG_ATA_IS_INIT))
		return STA_OK;
	else
		return STA_NODISK;
}

diskResult pata_write(uint8_t drive, const uint8_t *buff, uint32_t sector, uint8_t len){
	pata_reg_outb((0xE0 | (drive << 4)), ATA_REG_HDDEVSEL);
	pata_wait_bsy();
	//nprintf("[DEBUG] pata_write, sector: 0x%X, len: %d", sector, len);
	if (len == 0 || sector > 0xFFFFFFF) return RES_PARERR;
	if (ide_disk[drive]->exists != (FLAG_ATA_IS_EXIST | FLAG_ATA_IS_INIT)) return RES_NOTRDY;

	if (!pata_wait_chk(ATA_SR_DRDY)) return RES_ERROR;
	pata_reg_outb(((uint8_t)(sector >> 24) & 0x0F) | (0xE0 | (drive << 4)), ATA_REG_HDDEVSEL);
	if (!pata_wait_chk(ATA_SR_DRDY)) return RES_ERROR;
	pata_reg_outb((uint8_t)(sector >> 16), ATA_REG_LBA2);
	pata_reg_outb((uint8_t)(sector >> 8), ATA_REG_LBA1);
	pata_reg_outb((uint8_t)(sector), ATA_REG_LBA0);
	pata_reg_outb((uint8_t)(len), ATA_REG_SECCOUNT0);
	pata_reg_outb(ATA_CMD_WRITE_PIO, ATA_REG_COMMAND);
	uint16_t *nptr = buff;
	if (!pata_wait_chk(ATA_SR_DRQ)) return RES_ERROR;
	while (len > 0){
		pata_write_blk(nptr);
		nptr += 256;
		len--;
	};

	pata_reg_inb(ATA_REG_STATUS);
	return RES_OK;
}

diskResult pata_read(uint8_t drive, uint8_t *buff, uint32_t sector, uint8_t len){
	pata_reg_outb((0xE0 | (drive << 4)), ATA_REG_HDDEVSEL);
	pata_wait_bsy();
	//nprintf("[DEBUG] pata_read, sector: 0x%X, len: %d", sector, len);
	if (len == 0 || sector > 0xFFFFFFF) return RES_PARERR;
	if (ide_disk[drive]->exists != (FLAG_ATA_IS_EXIST | FLAG_ATA_IS_INIT)) return RES_NOTRDY;

	if (!pata_wait_chk(ATA_SR_DRDY)) return RES_ERROR;
	pata_reg_outb(((uint8_t)(sector >> 24) & 0x0F) | (0xE0 | (drive << 4)), ATA_REG_HDDEVSEL);
	if (!pata_wait_chk(ATA_SR_DRDY)) return RES_ERROR;
	pata_reg_outb((uint8_t)(sector >> 16), ATA_REG_LBA2);
	pata_reg_outb((uint8_t)(sector >> 8), ATA_REG_LBA1);
	pata_reg_outb((uint8_t)(sector), ATA_REG_LBA0);
	pata_reg_outb((uint8_t)(len), ATA_REG_SECCOUNT0);
	pata_reg_outb(ATA_CMD_READ_PIO, ATA_REG_COMMAND);
	uint16_t *nptr = buff;
	if (!pata_wait_chk(ATA_SR_DRQ)) return RES_ERROR;
	while (len > 0){
		pata_read_blk(nptr);
		nptr += 256;
		len--;
	};

	pata_reg_inb(ATA_REG_STATUS);
	return RES_OK;
}

diskResult pata_ioctl(uint8_t drive, uint8_t cmd, void *buff){
	//nprintf("[DEBUG] pata_ioctl, cmd: 0x%X", cmd);
	switch (cmd){
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(uint32_t*)buff = ide_disk[drive]->size;
			return RES_OK;
		case GET_SECTOR_SIZE:
			*(uint16_t*)buff = 512;
			return RES_OK;
		case GET_BLOCK_SIZE:
			*(uint32_t*)buff = 1;
			return RES_OK;
		case CTRL_TRIM:
			return RES_OK;
		default: return RES_OK;
	}
}

static bool pata_wait_bsy(){
	uint32_t i = 0;
	/* timeout if the drive doesn't respond */
	while (pata_reg_inb(ATA_REG_STATUS) & ATA_SR_BSY){
		if (i >= PATA_TIMEOUT) return false;
		i++;
	}
	return true;
}

static bool pata_wait_drq(){
	uint32_t i = 0;
	/* timeout if the drive doesn't respond */
	while (!(pata_reg_inb(ATA_REG_STATUS) & (ATA_SR_DRQ))){
		if (i >= PATA_TIMEOUT) {
			puts("[!] IDE device timeout");
			delay(0x8FFFF);
			return false;
		}
		i++;
	}
	return true;
}

static bool pata_wait_chk(uint8_t flag){
	if (pata_wait_bsy() == false) return false;
	if (!(pata_reg_inb(ATA_REG_STATUS) & flag)) return false;
	return true;
}

static void pata_read_blk(uint16_t *buff){
	for (int i = 0; i < 256; i++){
		*buff++ = pata_reg_inw(ATA_REG_DATA);
	}
}

static void pata_read_blk_le(uint16_t *buff){
	for (int i = 0; i < 256; i++){
		*buff++ = pata_reg_inw_le(ATA_REG_DATA);
	}
}

static void pata_write_blk(uint16_t *buff){
	for (int i = 0; i < 256; i++){
		pata_reg_outw(*buff, ATA_REG_DATA);
		buff++;
	}
}

//---------------------------------------------------

device_initcall(pata_dev_register);

#endif
