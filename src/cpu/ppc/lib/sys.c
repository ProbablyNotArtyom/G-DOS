// TRAP wrapper for syscalls

	#include <std.h>
	#include <cpu/syscall.h>
	#include <char.h>
	#include <fs.h>

_syscall3(FRESULT, f_open, FIL*, fp, const TCHAR*, path, uint8_t, mode)
_syscall1(FRESULT, f_close, FIL*, fp)
_syscall4(FRESULT, f_read, FIL*, fp, void*, buff, uint32_t, btr, uint32_t*, br)
_syscall4(FRESULT, f_write, FIL*, fp, const void*, buff, uint32_t, btw, uint32_t*, bw)
_syscall2(FRESULT, f_lseek, FIL*, fp, FSIZE_t, ofs)
FRESULT f_truncate (FIL* fp);
FRESULT f_sync (FIL* fp);
FRESULT f_opendir (DIR* dp, const TCHAR* path);
FRESULT f_closedir (DIR* dp);
FRESULT f_readdir (DIR* dp, FILINFO* fno);
FRESULT f_mkdir (const TCHAR* path);
FRESULT f_unlink (const TCHAR* path);
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);
_syscall2(FRESULT, f_stat, const TCHAR*, path, FILINFO*, fno)
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

_syscall1(void*, malloc, size_t, size)
_syscall1(void*, pmalloc, size_t, size)
_syscall1(charResult, putchar, char, out)
_syscall0(char, getchar)
