//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Filesystem support generics

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <fs.h>

//-------------------Type Protos---------------------


//-----------------Variable Protos-------------------


//-----------------Function Protos-------------------


//-----------------------Main------------------------

void fsInit(void){
	for (int i = 0; i <= 3; i++) memset(diskTable[i].label, ' ', 16);	// Set all labels blank
	currentDisk = 0;
	nextDisk = 0;
	
	return;
}

int fdChDisk(int num){
	if (num >= nextDisk) return -1;
	currentDisk = num;
	return 1;
}

struct fsFile fsLoad(char *fname){
	return diskTable[currentDisk].load(fname);
}

struct fsFile fsQuery(uint32_t index){
	return diskTable[currentDisk].query(index);
}

struct fsFile fsExists(char *fname){
	return diskTable[currentDisk].exists(fname);
}