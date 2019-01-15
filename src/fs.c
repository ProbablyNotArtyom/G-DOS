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

//---------------------------------------------------

//uint8_t numFS = 0;
//struct f_handler fsList[4];
const char* const errStrings[];

//-----------------------Main------------------------

void fsInit(void){
	/*
	fsinitcall_t *fn = &__start_fsinitsec;
	while (fn < &__stop_fsinitsec) {
		(*fn)(&fsList[numFS]);
		numFS++;
		fn++;
	}
	*/

	return;
}

f_error fs_seek(f_file *file, size_t bytes){

	f_error err = f_lseek(&file, bytes);
	fs_putsError(err);
	return err;
}

f_error fs_read(f_file *file, void *buff, size_t byte, size_t *numReads){

	f_error err = f_read(&file, *(uint8_t*)buff, byte, *numReads);
	fs_putsError(err);
	return err;
}

f_error fs_write(f_file *file, const void *buff, size_t bytes, size_t *numRead){

	return RES_OK;
}

f_error fs_open(f_file *file, const char *path, f_mode mode){

	f_error err = f_open (&file, *path, mode);
	fs_putsError(err);
	return err;
}

f_error fs_close(f_file *file){

	return RES_OK;
}

f_error fs_openDir(f_dir *dir, const char *path){

	return RES_OK;
}

f_error fs_closeDir(f_dir *dir, const char *path){

	return RES_OK;
}

f_error fs_readDir(f_dir *dir, f_info *info){

	return RES_OK;
}

f_error fs_getCWD(char *buff, uint16_t len){

	return RES_OK;
}

f_error fs_getLabel(const uint8_t disknum, char *label){

	return RES_OK;
}

f_error fs_chdir(const char* path){

	return RES_OK;
}

f_error fs_mount(FATFS* fs, const TCHAR* path, uint8_t opt){

	f_error err = f_mount(&fs, &path, opt);
	fs_putsError(err);
	return err;
}

f_error fs_mkfs(const char* path, uint8_t opt, uint32_t au, void* work, uint32_t len){

	f_error err = f_mkfs(*path, opt, au, *(uint8_t*)work, len);
	fs_putsError(err);
	return err;
}

void fs_putsError(f_error error){
	if (error <= 0) return;
	error--;
	puts(errStrings[error]);
	return;
}

//----------------------Tables-----------------------

const char* const errStrings[] = {
	"[!] Driver Error\n\r",
	"[!] Assertation failed\n\r",
	"[!] Drive unable to function\n\r",
	"[?] Unable to find file\n\r",
	"[?] Unable to follow path\n\r",
	"[?] Path is invalid\n\r",
	"[!] Access denied: Directory may be full\n\r",
	"[!] Access denied\n\r",
	"[?] Invalid file/dir object\n\r",
	"[!] Drive is write-protected\n\r",
	"[?] Invalid drive number\n\r",
	"[!] Volume has no work area\n\r",
	"[!] FAT filesystem not detected\n\r",
	"[!] MKFS aborted\n\r",
	"[!] Access timeout\n\r",
	"[!] Access locked\n\r",
	"[!] Not enough memory to allocate LFN buffer\n\r",
	"[!] Too many open files\n\r",
	"[?] Invalid parameter\n\r"
};
