// BUILTIN
// LS
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

//---------------------------------------------------

result_t shfunc_ls(char *argv[], int argc){

	f_error res;
    f_dir dir;
    static f_info info;

	if (argc == 0)
		res = f_opendir(&dir, ".");
	else
		res = f_opendir(&dir, argv[0]);

    if (res == FR_OK){
		char labelTmp[15];
		uint32_t sizeTmp;
		FATFS *fsTmp;

		/* grab a bunch of information about the current volume */
		res = f_getlabel("", &labelTmp, NULL);
		if (res != FR_OK) return RET_SYS;
		res = f_getfree("", &sizeTmp, &fsTmp);
		if (res != FR_OK) return RET_SYS;
		sizeTmp = (sizeTmp * fsTmp->csize) / 2;
		printf("Volume: %s\r\n", labelTmp);
		printf("Size: %dKB, %dKB free\r\n", ((fsTmp->n_fatent - 2) * fsTmp->csize) / 2 , sizeTmp);

		while (true){
            res = f_readdir(&dir, &info);
            if (res != FR_OK || info.fname[0] == 0) break;
            if (info.fattrib & AM_DIR){
				printf("  <DIR> %s", info.fname);
				for(int i = 14 - strlen(info.fname); i > 0; i--) putchar(' ');
				if (info.fsize / 1024 >= 1)
					if (info.fsize / 1048576 >= 1)
						printf(" %d MB\r\n", info.fsize / 1048576);
					else
						printf(" %d KB\r\n", info.fsize / 1024);
				else
					printf(" %d B\r\n", info.fsize);
			}
		}
		f_readdir(&dir, NULL);
		while (true){
            res = f_readdir(&dir, &info);
            if (res != FR_OK || info.fname[0] == 0) break;
            if (!(info.fattrib & AM_DIR)){
                printf("        %s", info.fname);
				for(int i = 14 - strlen(info.fname); i > 0; i--) putchar(' ');
				if (info.fsize / 1024 >= 1)
					if (info.fsize / 1048576 >= 1)
						printf(" %d MB\r\n", info.fsize / 1048576);
					else
						printf(" %d KB\r\n", info.fsize / 1024);
				else
					printf(" %d B\r\n", info.fsize);
            }
        }
        f_closedir(&dir);
    } else {
		fs_putsError(res);
	}
	return RET_OK;
}
