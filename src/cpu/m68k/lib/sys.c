// TRAP wrapper for syscalls

	#include <std.h>
	#include <cpu/syscall.h>
	#include <char.h>
	#include <fs.h>

FRESULT f_open (FIL* fp, const TCHAR* path, uint8_t mode){ FRESULT res = INTERNAL_SYSCALL(syscall_f_open, 3, fp, path, mode); return res; }
FRESULT f_close (FIL* fp){ FRESULT res = INTERNAL_SYSCALL(syscall_f_close, 1, fp); return res; }
FRESULT f_read (FIL* fp, void* buff, uint32_t btr, uint32_t* br){ FRESULT res = INTERNAL_SYSCALL(syscall_f_read, 4, fp, buff, btr, br); return res; }
FRESULT f_write (FIL* fp, const void* buff, uint32_t btw, uint32_t* bw){ FRESULT res = INTERNAL_SYSCALL(syscall_f_write, 4, fp, buff, btw, bw); return res; }
FRESULT f_lseek (FIL* fp, FSIZE_t ofs){ FRESULT res = INTERNAL_SYSCALL(syscall_f_lseek, 2, fp, ofs); return res; }
FRESULT f_truncate (FIL* fp);
FRESULT f_sync (FIL* fp);
FRESULT f_opendir (DIR* dp, const TCHAR* path);
FRESULT f_closedir (DIR* dp);
FRESULT f_readdir (DIR* dp, FILINFO* fno);
FRESULT f_mkdir (const TCHAR* path);
FRESULT f_unlink (const TCHAR* path);
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);
FRESULT f_stat (const TCHAR* path, FILINFO* fno){ FRESULT res = INTERNAL_SYSCALL(syscall_f_stat, 2, path, fno); return res; };
FRESULT f_chdir (const TCHAR* path);
FRESULT f_chdrive (const TCHAR* path);
FRESULT f_getcwd (TCHAR* buff, uint32_t len);
FRESULT f_getfree (const TCHAR* path, uint32_t* nclst, FATFS** fatfs);
FRESULT f_getlabel (const TCHAR* path, TCHAR* label, uint32_t* vsn);
FRESULT f_setlabel (const TCHAR* label);
FRESULT f_expand (FIL* fp, FSIZE_t szf, uint8_t opt);
FRESULT f_mount (FATFS* fs, const TCHAR* path, uint8_t opt);
FRESULT f_mkfs (const TCHAR* path, uint8_t opt, uint32_t au, void* work, uint32_t len);
int f_putc (TCHAR c, FIL* fp);
int f_puts (const TCHAR* str, FIL* cp);
int f_printf (FIL* fp, const TCHAR* str, ...);
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);

void *malloc(size_t size){ void *res = INTERNAL_SYSCALL(syscall_malloc, 1, size); return res; }
void *pmalloc(size_t size){ void *res = INTERNAL_SYSCALL(syscall_pmalloc, 1, size); return res; }
charResult putc(char out){ charResult res = INTERNAL_SYSCALL(syscall_putc, 1, out); return res; }
char read(void){ char res = INTERNAL_SYSCALL(syscall_read, 0); return res; }
