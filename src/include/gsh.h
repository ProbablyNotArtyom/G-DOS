//---------------------------------------------------
//
//	G-DOS
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

	result_t shfunc_ls(int argc, char *argv[]);
	result_t shfunc_cd(int argc, char *argv[]);
	result_t shfunc_echo(int argc, char *argv[]);
	result_t shfunc_cat(int argc, char *argv[]);
	result_t shfunc_rm(int argc, char *argv[]);
	result_t shfunc_cp(int argc, char *argv[]);
	result_t shfunc_mv(int argc, char *argv[]);
	result_t shfunc_mkdir(int argc, char *argv[]);
	result_t shfunc_mon(int argc, char *argv[]);
	result_t shfunc_mount(int argc, char *argv[]);
	result_t shfunc_fputs(int argc, char *argv[]);
	result_t shfunc_mkfs(int argc, char *argv[]);
	result_t shfunc_pico(int argc, char *argv[]);
	result_t shfunc_meminfo(int argc, char *argv[]);
	result_t tckBegin(int argc, char *argv[]);
	result_t shfunc_expr(int argc, char *argv[]);

//---------------------------------------------------

	#define SHBUFFLEN	256
	#define MAXARGS		40

//---------------------------------------------------

#endif
