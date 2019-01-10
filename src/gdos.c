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
	#include "gdos.h"
	#include "monitor/mon.h"

//-----------------Variable Protos-------------------

uint8_t buff[512];

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

	//fs_putsError(f_mount(&fsFat, "", 1));
	//fs_putsError(f_getcwd(buf, 20));
	//puts(buf);
	//printByte(buf[0]);

	fs_putsError(f_mkfs("", FM_SFD | FM_FAT, 0, buff, 512));

	shellBegin();
	//monBegin();

	puts("End.");
	while(1);
}

static void shellBegin(void){
	static char shellBuff[SHBUFFLEN];
	char *shParse;
	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell");
	while (true){
		fputs("\n\r# ");					// very very temp
		shParse = shellBuff;
		gets(shellBuff, SHBUFFLEN);
		uint8_t i;
		for (i = 0; shFuncNames[i] != NULL && funcCmp(shParse, shFuncNames[i]) == false; i++);	// Identify what function it is

		if (shFuncNames[i] != NULL){
			(*shFunctions[i])();
		}
	}
}

//------------------Shell Functions------------------

static void shfunc_ls(char *argv[], int argc){

}

static void shfunc_cd(char *argv[], int argc){

}

static void shfunc_echo(char *argv[], int argc){

}

static void shfunc_cat(char *argv[], int argc){

}

static void shfunc_rm(char *argv[], int argc){

}

static void shfunc_cp(char *argv[], int argc){

}

static void shfunc_mv(char *argv[], int argc){

}

static void shfunc_mkdir(char *argv[], int argc){

}

static void shfunc_mon(char *argv[], int argc){
	monBegin();
}

//----------------------Tables-----------------------

const void (*shFunctions[])() = {
	shfunc_ls,
	shfunc_cd,
	shfunc_echo,
	shfunc_cat,
	shfunc_rm,
	shfunc_cp,
	shfunc_mv,
	shfunc_mkdir,
	shfunc_mon,
	NULL
};

const char* const shFuncNames[] = {
	"ls ",
	"cd ",
	"echo ",
	"cat ",
	"rm ",
	"cp ",
	"mv ",
	"mkdir ",
	"mon ",
	NULL
};
