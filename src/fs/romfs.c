//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Driver for the required ROMFS filesystem

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <fs.h>

//-----------------Variable Protos-------------------

extern char _binary___bin_romfs_dat_start;
extern char _binary___bin_romfs_dat_end;
extern char _binary___bin_romfs_dat_size;

#define _romfs_start _binary___bin_romfs_dat_start
#define _romfs_end _binary___bin_romfs_dat_end
#define _romfs_size _binary___bin_romfs_dat_size

uint32_t *currentDir;

//-----------------Function Protos-------------------

int romfsInit(void);
struct fsFile romfsExists(char *fname);
struct fsFile romfsQuery(uint32_t offset);
struct fsFile romfsLoad(char *fname);
int romfsChDir(char *fname);
void putsp(char *str, int len);

//-----------------------Main------------------------

int romfsInit(void){								// Init function
	puts("Checking ROMFS header... ");
	if (strcmpl(&_romfs_start, "-rom1fs-", 3) == 0) puts("PASS"); 
	else {
		puts("FAIL!");
		return -1;
	}
	memcpy(diskTable[nextDisk].label, &_romfs_start+16, 16);
	puts("Disk ");
	putsl(diskTable[nextDisk].label, 16);
	puts(" found\n\r");
	diskTable[nextDisk].query = romfsQuery;
	diskTable[nextDisk].exists = romfsExists;
	diskTable[nextDisk].load = romfsLoad;
	diskTable[nextDisk].chdir = romfsChDir;
	diskTable[nextDisk].size = &_romfs_start+8;
	currentDir =  &_romfs_start+32;
	return 1;
}

struct fsFile romfsExists(char *fname){						// Checks if a file exists in the current dir
													// returns( -1: not found, 1:file, 10:dir)
	uint32_t *index = currentDir;
	uint32_t newIndex = index;
	do {
		index = newIndex;
		newIndex = (*index & 0xFFFFFFF0) + &_romfs_start;
	} while (strcmp((uint32_t)index+16, fname) != 0 && ((uint32_t)index & 0xFFFFFFF0) != 0);
	if (strcmp((uint32_t)index+16, fname) == 0){
		struct fsFile query;
		strcpy(query.name, (uint32_t)index+16);
		query.size = *(index+2);
		query.type = (uint32_t)(index) & 0x00000007;
		query.data = NULL;
		return query;
	} else {
		struct fsFile query;
		query.size = NULL;
		query.type = NULL;
		query.data = NULL;
		return query;
	}
}

struct fsFile romfsLoad(char *fname){							// Loads a file into memory
	uint32_t *index = currentDir;
	uint32_t newIndex = index;
	do {
		index = newIndex;
		newIndex = (*index & 0xFFFFFFF0) + &_romfs_start;
	} while (strcmp((uint32_t)index+16, fname) != 0 && ((uint32_t)index & 0xFFFFFFF0) != 0);
	if (strcmp((uint32_t)index+16, fname) == 0){
		struct fsFile query;
		strcpy(query.name, (uint32_t)index+16);
		query.size = *(index+2);
		query.type = (uint32_t)(index) & 0x00000007;
		query.data = index+8;
		return query;
	} else {
		struct fsFile query;
		query.size = NULL;
		query.type = NULL;
		query.data = NULL;
		return query;
	}
}

struct fsFile romfsQuery(uint32_t offset){							// Loads a file into memory
	uint32_t *index = currentDir;
	while (offset > 0 && ((uint32_t)index & 0xFFFFFFF0) != 0){
		index = (*index & 0xFFFFFFF0) + &_romfs_start;
		offset--;
	}
	if (offset != 0){
		struct fsFile query;
		query.size = NULL;
		query.type = NULL;
		query.data = NULL;
		return query;
	} else {
		struct fsFile query;
		strcpy(query.name, (uint32_t)index+16);
		query.size = *(index+2);
		query.type = (uint32_t)(index) & 0x00000007;
		query.data = index+8;
		return query;
	}
}

int romfsChDir(char *fname){
	uint32_t *index = currentDir;
	uint32_t lastIndex;
	do {
		if ((*index & 0x00000006) == 0x00
		&& strcmp((char*)index+16, fname) == 0) {
			currentDir = *(index+1) + &_romfs_start;
			return 1;
		} else {
			lastIndex = *index;
			index = (lastIndex & 0xFFFFFFF0) + &_romfs_start;
		}
	} while ((lastIndex & 0xFFFFFFF0) != 0);
	return 1;
}

void putsp(char *str, int len){
	while (*str != NULL && len > 0){
		putc(*str++);
		len--;
	}
	while (len > 0){
		putc(' ');
		len--;
	}
	return;
}

setInit(romfsInit);