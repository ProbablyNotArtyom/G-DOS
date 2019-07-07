//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Shell

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

//-----------------Variable Protos-------------------

	static bool funcCmp(const char *s1, const char *s2);
	static void runCMD(shFunc_t func, char *buffer);
	static void putsPrompt();

	const result_t const (* const shFunctions[])();
	const char* const shFuncNames[];
	const char* const shErrors[];
	const uint8_t const shFuncLen[];


	FATFS *fsMounts[4];
	uint8_t numMounts;
	uint8_t currentDisk;
	bool sh_doExit;

//-----------------------Main------------------------

void shellBegin(void){
	sh_doExit = false;
	numMounts = 0;
	currentDisk = 0;
	static char shellBuff[SHBUFFLEN];
	char *shParse;
	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell\n\r");

#ifdef DISK_AUTOMOUNT
	/* attempt to mount the default disk */
	fsMounts[0] = malloc(sizeof(FATFS));

	f_error res;
	res = f_mount(fsMounts[0], "0", 1);
	if (res != FR_OK){
		fs_putsError(res);
		puts("[!] Default disk could not be mounted");
	} else {
		numMounts++;
	}
#endif
	while (sh_doExit == false){
		putsPrompt();
		shParse = shellBuff;
		gets(shellBuff, SHBUFFLEN);
		while (isSpace(*shParse)) shParse++;
		uint8_t i;
		for (i = 0; shFuncNames[i] != NULL && funcCmp(shParse, shFuncNames[i]) == false; i++);	// Identify what function it is
		puts("");
		if (shFunctions[i] != NULL){
			shParse += shFuncLen[i];
			runCMD(*shFunctions[i], shParse);
		} else {
			runCMD(NULL, shParse);
		}
	}
}

void shThrow(result_t err){
	if (err == RES_OK) {
		//puts("");
		return;
	}
	puts(shErrors[err]);
}

static bool funcCmp(const char *s1, const char *s2){
	while (*s1 == *s2){
		if (*s1++ == ' ') return true;
		*s2++;
	}
	if (*s1 == '\0' && *s2 == ' ') return true;
	return false;
}

static void putsPrompt(){
	f_error res;
	char promptBuff[256];

	fputs(COLOR_FG(C_LIGHTGREEN,"("));
	fputs(SET_COLOR_FG(C_WHITE));
	res = f_getcwd(promptBuff, sizeof(promptBuff));
	if ((res == FR_OK) && (numMounts != 0))
		fputs(promptBuff);
	fputs(SET_COLOR_FG(C_LIGHTGREEN));
	fputs(")" COLOR_RESET_FG ":" COLOR_FG(C_LIGHTGREEN, "> "));
}

static void runCMD(shFunc_t func, char *buffer){
	char *ptr;
	char *arg[MAXARGS+1];
	int numArgs = 0;

	ptr = buffer;
	while(1){
		if (numArgs == MAXARGS){
			printf("[!] Too many arguments, %d maximum", MAXARGS);
			*ptr = NULL;
			return;
		}
		if (!*ptr){
			arg[numArgs] = NULL;
			break;
		}
		while (isSpace(*ptr)) ptr++;
		if (*ptr == '\"'){
			ptr++;
			arg[numArgs] = ptr;
			numArgs++;
			while (*ptr && *ptr != '\"') ptr++;
			if (!*ptr){
				shThrow(RET_BADSTR);
				continue;
			} else {
				*ptr = NULL;
				ptr++;
			}
		} else if (!isSpace(*ptr)){
			arg[numArgs] = ptr;
			numArgs++;
			while (*ptr && !isSpace(*ptr)) ptr++;
			if (!*ptr) continue;
			while (isSpace(*ptr)){
				*ptr = NULL;
				ptr++;
			}
		}
	}

	if (func != NULL){
		shThrow(func(arg, numArgs));
	} else {
		f_error res;
		f_file file;
		res = f_open(&file, arg[0], FA_READ);
		fs_putsError(res);
		if (res != FR_OK) return;

		loadELF(arg+1, numArgs-1, &file);
	}
	return;
}

result_t shfunc_exit(char *argv[], int argc){
	sh_doExit = true;
	return RET_OK;
}

//----------------------Tables-----------------------

const result_t const (* const shFunctions[])() = {
	shfunc_ls,
	shfunc_cd,
	shfunc_echo,
	shfunc_cat,
	shfunc_rm,
	shfunc_cp,
	shfunc_mv,
	shfunc_mkdir,
	shfunc_mon,
	shfunc_mount,
	shfunc_fputs,
	shfunc_exit,
	shfunc_mkfs,
	shfunc_pico,
	shfunc_meminfo,
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
	"mount ",
	"fputs ",
	"exit ",
	"mkfs ",
	"pico ",
	"meminfo ",
	NULL
};

const uint8_t const shFuncLen[] = {
	2,
	2,
	4,
	3,
	2,
	2,
	2,
	5,
	3,
	5,
	5,
	4,
	4,
	4,
	7,
	NULL
};

const char* const shErrors[] = {
	"",
	"[?] Formatting error.",
	"[?] Invalid arguments.",
	"",
	"",
	"",
	"",
	"",
	"[?] Invalid string argument",
	NULL
};
