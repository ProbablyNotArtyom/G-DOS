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
	#include <stdlib.h>
	#include <errno.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>
	#include <history.h>

//---------------------------------------------------

#define skip_blank(a)		while (*a == ' ') a++
#define skip_token(a)		while (*a != ' ' && *a != '\0') a++;

//-----------------Variable Protos-------------------

	static bool funcCmp(const char *s1, const char *s2);
	static void runCMD(shFunc_t func, char *buffer);
	static void putsPrompt();

	FATFS *fsMounts[4];
	history_t shHistory;
	uint8_t numMounts;
	uint8_t currentDisk;
	bool sh_doExit;

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
	tckBegin,
	shfunc_expr,
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
	"tclgo ",
	"expr ",
	"\0"
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
	"\0"
};

//-----------------------Main------------------------

static char *shellPrefix;
int shellBegin(void) {
	char *shellBuff, *shParse;

	shellBuff = (char *)malloc(GSH_BUFFLEN);
	if (!shellBuff) return __ret_errno(ENOMALLOC);
	shellPrefix = (char *)malloc(GSH_BUFFLEN+sizeof(GSH_BINDIR));
	if (!shellPrefix) return __ret_errno(ENOMALLOC);

	strncpy(shellPrefix, GSH_BINDIR, sizeof(GSH_BINDIR)-1);

	gsh_init();
	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell\n\r");

	history_init(&shHistory, GSH_HISTORY_SIZE);

	uint8_t i;
	while (sh_doExit == false) {
		putsPrompt();
		shParse = shellBuff;
		history_gets(&shHistory, shParse, GSH_BUFFLEN);

		skip_blank(shParse);
		for (i = 0; funcCmp(shParse, shFuncNames[i]) == false && i < ARRAY_SIZE(shFuncNames)-1; i++);	// Identify what function it is
		puts("");
		if (i == ARRAY_SIZE(shFuncNames)-1) {
			runCMD(NULL, shParse);
		} else {
			skip_token(shParse);
			runCMD(*shFunctions[i], shParse);
		}
	}

	free(shellBuff);
	free(shellPrefix);
	return 0;
}

static void runCMD(shFunc_t func, char *buffer) {
	char *ptr;
	char *arg[GSH_MAXARGS+1];
	int numArgs = 0;

	ptr = buffer;
	while(1) {
		if (numArgs == GSH_MAXARGS) {
			printf("[!] Too many arguments, %d maximum", GSH_MAXARGS);
			*ptr = NULL;
			return;
		}
		if (!*ptr) {
			arg[numArgs] = NULL;
			break;
		}
		skip_blank(ptr);
		if (*ptr == '\"') {
			ptr++;
			arg[numArgs] = ptr;
			numArgs++;
			while (*ptr && *ptr != '\"') ptr++;
			if (!*ptr) {
				shThrow(RET_BADSTR);
				continue;
			} else {
				*ptr = NULL;
				ptr++;
			}
		} else if (!isSpace(*ptr)) {
			arg[numArgs] = ptr;
			numArgs++;
			while (*ptr && !isSpace(*ptr)) ptr++;
			if (!*ptr) continue;
			while (isSpace(*ptr)) {
				*ptr = NULL;
				ptr++;
			}
		}
	}

	if (func != NULL) {
		shThrow(func(arg, numArgs));
	} else {
		f_file file;
		f_error res = f_open(&file, arg[0], FA_READ);

		if (res == FR_OK) {
			loadELF(numArgs-1, arg+1, &file);
		} else {
			strcpy(shellPrefix+sizeof(GSH_BINDIR)-1, arg[0]);
			res = f_open(&file, shellPrefix, FA_READ);
			fs_putsError(res);
			if (res != FR_OK) return;
			loadELF(numArgs-1, arg+1, &file);
		}
	}
	return;
}

/* attempt to mount the default disk */
void gsh_init(void) {
	sh_doExit = false;
	numMounts = 0;
	currentDisk = 0;

#ifdef DISK_AUTOMOUNT
	fsMounts[0] = malloc(sizeof(FATFS));
	f_error res = f_mount(fsMounts[0], "0", 1);
	if (res != FR_OK) puts("[!] Default disk could not be mounted");
	else numMounts++;
#endif
}

void shThrow(result_t err) {
	if (err == RES_OK) return;
	puts(shErrors[err]);
}

static bool funcCmp(const char *s1, const char *s2) {
	while (*s1 == *s2) {
		if (*s1++ == ' ') return true;
		*s2++;
	}
	if (*s1 == '\0' && *s2 == ' ') return true;
	return false;
}

static void putsPrompt() {
	f_error res;
	char promptBuff[GSH_BUFFLEN];

	printf(COLOR_FG(C_LIGHTGREEN,"("));
	printf(SET_COLOR_FG(C_WHITE));
	res = f_getcwd(promptBuff, sizeof(promptBuff));
	if ((res == FR_OK) && (numMounts != 0)) printf(promptBuff);
	printf(SET_COLOR_FG(C_LIGHTGREEN));
	printf(")" COLOR_RESET_FG ":" COLOR_FG(C_LIGHTGREEN, "> "));
}

result_t shfunc_exit(char *argv[], int argc) {
	sh_doExit = true;
	return RET_OK;
}
