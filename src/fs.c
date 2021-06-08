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
	#include <errno.h>

//---------------------------------------------------

const int const errNumbers[];

//-----------------------Main------------------------

void fsInit(void) {

	return;
}

f_error fs_seek(f_file *file, size_t bytes) {
	f_error err = f_lseek(&file, bytes);
	fs_putsError(err);
	return err;
}

f_error fs_read(f_file *file, void *buff, size_t byte, size_t *numReads) {
	f_error err = f_read(&file, *(uint8_t*)buff, byte, *numReads);
	fs_putsError(err);
	return err;
}

f_error fs_write(f_file *file, const void *buff, size_t bytes, size_t *numRead) {
	return RES_OK;
}

f_error fs_open(f_file *file, const char *path, f_mode mode) {
	f_error err = f_open (&file, *path, mode);
	fs_putsError(err);
	return err;
}

f_error fs_close(f_file *file) {
	return RES_OK;
}

f_error fs_openDir(f_dir *dir, const char *path) {
	return RES_OK;
}

f_error fs_closeDir(f_dir *dir, const char *path) {
	return RES_OK;
}

f_error fs_readDir(f_dir *dir, f_info *info) {
	return RES_OK;
}

f_error fs_getCWD(char *buff, uint16_t len) {
	return RES_OK;
}

f_error fs_getLabel(const uint8_t disknum, char *label) {
	return RES_OK;
}

f_error fs_chdir(const char* path) {
	return RES_OK;
}

f_error fs_mount(FATFS* fs, const TCHAR* path, uint8_t opt) {
	f_error err = f_mount(&fs, &path, opt);
	fs_putsError(err);
	return err;
}

f_error fs_mkfs(const char* path, uint8_t opt, uint32_t au, void* work, uint32_t len) {
	f_error err = f_mkfs(*path, opt, au, *(uint8_t*)work, len);
	fs_putsError(err);
	return err;
}

void fs_putsError(f_error error) {
	if (error <= 0) return;
	error--;
	puts(strerror(errNumbers[error]));
	return;
}

//----------------------Tables-----------------------

const int const errNumbers[] = {
	EPROTO,
	EPROTO,
	EBUSY,
	ENOENT,
	ENOENT,
	ENOENT,
	ENOMEM,
	EACCES,
	EFAULT,
	EROFS,
	ENODEV,
	EBUSY,
	ENOFS,
	EBUSY,
	ETIME,
	EDEADLK,
	ENOMALLOC,
	ENFILE,
	EINVAL
};
