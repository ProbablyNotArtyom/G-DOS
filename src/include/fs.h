//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Filesystem control header

#ifndef _HEADER_FSTOOLS
#define _HEADER_FSTOOLS

	#include <stdbool.h>
	#include <fat.h>

//-------------------Type Protos---------------------

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} diskResult;

typedef enum {
	fm_read = 0x01,
	fm_write = 0x02,
	fm_existing = 0x00,
	fm_new = 0x04,
	fm_new_always = 0x08,
	fm_open_always = 0x10,
	fm_open_append = 0x30
} f_mode;

typedef DSTATUS diskStatus;
typedef FRESULT f_error;
typedef FIL f_file;
typedef DIR f_dir;
typedef FILINFO f_info;

/*
f_error:
	FR_OK = 0,				(0) Succeeded
	FR_DISK_ERR,			(1) A hard error occurred in the low level disk I/O layer
	FR_INT_ERR,				(2) Assertion failed
	FR_NOT_READY,			(3) The physical drive cannot work
	FR_NO_FILE,				(4) Could not find the file
	FR_NO_PATH,				(5) Could not find the path
	FR_INVALID_NAME,		(6) The path name format is invalid
	FR_DENIED,				(7) Access denied due to prohibited access or directory full
	FR_EXIST,				(8) Access denied due to prohibited access
	FR_INVALID_OBJECT,		(9) The file/directory object is invalid
	FR_WRITE_PROTECTED,		(10) The physical drive is write protected
	FR_INVALID_DRIVE,		(11) The logical drive number is invalid
	FR_NOT_ENABLED,			(12) The volume has no work area
	FR_NO_FILESYSTEM,		(13) There is no valid FAT volume
	FR_MKFS_ABORTED,		(14) The f_mkfs() aborted due to any problem
	FR_TIMEOUT,				(15) Could not get a grant to access the volume within defined period
	FR_LOCKED,				(16) The operation is rejected according to the file sharing policy
	FR_NOT_ENOUGH_CORE,		(17) LFN working buffer could not be allocated
	FR_TOO_MANY_OPEN_FILES,	(18) Number of open files > FF_FS_LOCK
	FR_INVALID_PARAMETER	(19) Given parameter is invalid
*/

//-----------------Function Protos-------------------

void fsInit(void);

f_error fs_seek(f_file *file, size_t bytes);
f_error fs_read(f_file *file, void *buff, size_t bytes, size_t *numRead);
f_error fs_write(f_file *file, const void *buff, size_t bytes, size_t *numRead);
f_error fs_open(f_file *file, const char *path, f_mode mode);
f_error fs_close(f_file *file);
f_error fs_openDir(f_dir *dir, const char *path);
f_error fs_closeDir(f_dir *dir, const char *path);
f_error fs_readDir(f_dir *dir, f_info *info);
f_error fs_getCWD(char *buff, uint16_t len);
f_error fs_getLabel(const uint8_t disknum, char *label);
f_error fs_chdir(const char* path);
f_error fs_mount(FATFS* fs, const TCHAR* path, uint8_t opt);
f_error fs_mkfs(const char* path, uint8_t opt, uint32_t au, void* work, uint32_t len);

void fs_putsError(f_error error);

#endif
