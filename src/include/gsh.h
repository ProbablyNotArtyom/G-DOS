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

	int shellBegin(void);
	void gsh_init(void);
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
	result_t shfunc_mkfs(char *argv[], int argc);
	result_t shfunc_pico(char *argv[], int argc);
	result_t shfunc_meminfo(char *argv[], int argc);
	result_t tckBegin(char *argv[], int argc);
	result_t shfunc_expr(char *argv[], int argc);

	result_t shfunc_exit(char *argv[], int argc);

//---------------------------------------------------

	#define GSH_BUFFLEN			512
	#define GSH_MAXARGS			40
	#define GSH_HISTORY_SIZE	128			// The maximum number of lines to save in the history buffer

	#define GSH_BINDIR			"/bin/"		// The path to the default binary dir

//---------------------------------------------------

#endif
