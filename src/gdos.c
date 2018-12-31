//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>

//-----------------Variable Protos-------------------


//-----------------------Main------------------------

int main(void){
	if (init() == -1){
		puts("Init sequence failed!\n\rHaulting execution...");
		while (1);
	}

	/* Handle generic device inits */
	initcall_t *fn = &__start_initsec;
	while (fn < &__stop_initsec){
		(*fn)();
		fn++;
	}

	/* Init filesystems and mount disks */
	fsInit();
	diskInit();

	FATFS fsFat;
	char buf[20];

	fs_putsError(f_mount(&fsFat, "", 1));
	fs_putsError(f_getcwd(buf, 20));
	puts(buf);
	printByte(buf[0]);

	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell");
	monBegin();

	puts("End.");
	while(1);
}
