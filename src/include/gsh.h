//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Shell header

#ifndef _HEADER_GSHELL
#define _HEADER_GSHELL

	extern FATFS *fsMounts[];
	extern uint8_t numMounts;
	extern uint8_t currentDisk;

	void shellBegin(void);
	void shThrow(result_t err);

	result_t shfunc_ls(char *argv[], int argc);
	result_t shfunc_cd(char *argv[], int argc);
	result_t shfunc_echo(char *argv[], int argc);
	result_t shfunc_cat(char *argv[], int argc);
	result_t shfunc_rm(char *argv[], int argc);
	result_t shfunc_cp(char *argv[], int argc);
	result_t shfunc_mv(char *argv[], int argc);
	result_t shfunc_mkdir(char *argv[], int argc);
	result_t shfunc_mon(char *argv[], int argc);
	result_t shfunc_mount(char *argv[], int argc);
	result_t shfunc_fputs(char *argv[], int argc);

//---------------------------------------------------

	#define SHBUFFLEN	256
	#define MAXARGS		40

//---------------------------------------------------

#endif
