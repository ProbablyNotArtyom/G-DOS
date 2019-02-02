// BUILTIN
// MOUNT
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

extern FATFS *fsMounts[4];
extern uint8_t numMounts;

//---------------------------------------------------

result_t shfunc_mount(char *argv[], int argc){
	uint8_t diskNum;
	if (argc == 0) return RET_ARGS;
	diskNum = strtoul(argv[0], NULL, 10);

	if (numMounts >= sizeof(fsMounts)){
		puts("[!] Mount limit reached");
		return RET_OK;
	}
	fsMounts[numMounts] = malloc(sizeof(FATFS));
	if (fsMounts[numMounts] == NULL){
		puts("[!] Could not allocate mount header");
		return RET_SYS;
	}

	f_file fil;            /* File object */
    f_error res;        /* API result code */
	uint8_t work[512];
	res = f_mkfs("", FM_ANY | FM_FAT, 0, work, sizeof(work));
	fs_putsError(res);

	if (f_mount(fsMounts[numMounts], "", 1) != FR_OK){
		puts("[!] Disk failed to mount; probably an invalid FAT filesystem");
		return;
	}

	numMounts++;
	puts("Disk mounted");
	return RET_OK;
}
