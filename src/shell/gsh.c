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

	const result_t (*shFunctions[])();
	const char* const shFuncNames[];
	const char* const shErrors[];
	const uint8_t const shFuncLen[];

	static bool funcCmp(const char *s1, const char *s2);
	static void runCMD(shFunc_t func, char *buffer);

//-----------------------Main------------------------

void shellBegin(void){
	static char shellBuff[SHBUFFLEN];
	char *shParse;
	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell\n\r");
	while (true){
		fputs("# ");					// very very temp
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
			runCMD(*shFunctions[i], shParse);
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
		puts("Unimplemented function call.");
	}
	return;
}

//----------------------Tables-----------------------

const result_t (*shFunctions[])() = {
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
