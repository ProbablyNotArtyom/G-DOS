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




//-----------------------Main------------------------

void fsInit(void){

	return;
}

f_error f_seek(f_file *file, uint32_t bytes) {
	return ER_OK;
}

f_error f_read(f_file *file, void *buff, size_t bytes) {
	return ER_OK;
}

f_error f_write(f_file *file, const void *buff, size_t bytes, size_t offset) {
	return ER_OK;
}

f_error f_open(f_file *file, const char *path) {
	return ER_OK;
}

f_error f_close(f_file *file) {
	return ER_OK;
}

f_error f_openDir(struct f_dir *dir, const char *path) {
	return ER_OK;
}

f_error f_closeDir(struct f_dir *dir, const char *path) {
	return ER_OK;
}

f_error f_readDir(struct f_dir *dir, f_info info) {
	return ER_OK;
}

f_error f_getCWD(char *buff, uint16_t len) {
	return ER_OK;
}

f_error f_getLabel(const uint8_t disknum, char *label) {
	return ER_OK;
}

f_error f_chdir(const char* path) {
	return ER_OK;
}
