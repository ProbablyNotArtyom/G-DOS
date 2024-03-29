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

result_t shfunc_mount(int argc, char *argv[]) {
	uint8_t diskNum;
	if (argc == 0) return RET_ARGS;
	diskNum = strtoul(argv[0], NULL, 10);

	if (numMounts >= sizeof(fsMounts)) {
		puts("[!] Mount limit reached");
		return RET_OK;
	}
	fsMounts[numMounts] = malloc(sizeof(FATFS));
	if (fsMounts[numMounts] == NULL) {
		puts("[!] Could not allocate mount header");
		return RET_SYS;
	}

    f_error res;
	res = f_mount(fsMounts[numMounts], argv[0], 1);
	fs_putsError(res);
	if (res != FR_OK) return RET_SYS;

	numMounts++;
	puts("Disk mounted");
	return RET_OK;
}
