
	#include <std.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>
	#include <stdint.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

	#include "tcl.h"

//---------------------------------------------------

const char const tcl_help_txt[] = {
	"A lightweight TCL interpreter for embedded systems\r\n"
	"Usage: tcl [-id] [-c \"command\"] [source] \r\n"
	"\r\n"
	"    -i           runs the interreter in interactive mode\r\n"
	"    -c           interpret and execute a single line of code\r\n"
	"    -d           Enables verbose debugging information\r\n"
};

extern struct tcl_cmd_builtin builtin_commands[];
bool interactive;
bool doExit, debug;
int docmd;

//----------------------Main-------------------------

static unsigned char linebuff[256];
static int line_index;
static bool read_line;

result_t tckBegin(char *argv[], int argc) {
	f_file file;
	f_info info;
	f_error res;
	doExit = false, debug = false, interactive = false;
	docmd = NULL;

	/* Default to starting in interactive mode if no args are passed */
	if (argc < 1) {
		debug = true;
		interactive = true;
	}
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				default:
				case 'h':
					puts(tcl_help_txt);
					return RET_OK;
				case 'i':
					interactive = true;
					break;
				case 'c':
					if (i < argc - 1) {
						docmd = i;
						i++;
					} else {
						printf("[?] No command specified\n");
						return;
					}
					break;
				case 'd':
					debug = true;
					break;
			}
		} else {
			res = f_stat(argv[i], &info);
			if (res != FR_OK) {
				printf("[?] File could not be found\n");
				return RES_OK;
			}
			f_open(&file, argv[i], FA_READ);
			f_lseek(&file, 0);
		}
	}

	struct tcl tcl;
	tcl_init(&tcl);

	if (docmd != NULL) {
		if (tcl_eval(&tcl, argv[docmd+1], strlen(argv[docmd+1])+1) != FERROR) {
    		if (*(tcl.result) != '\0') printf("> %s\n", tcl_string(tcl.result));
		} else {
			printf("[?] Syntax\n");
		}
		tcl_destroy(&tcl);
		return;
	}

	printf("TCL Command Shell v0.8\n");
	printf("NotArtyom 6/10/19\n");

	size_t buffer_size;
	char *buff;
	if (interactive) {
		buffer_size = 1024;
		buff = malloc(buffer_size);
	} else {
		buffer_size = info.fsize;
		buff = malloc(buffer_size);
		size_t bytes;
		f_read(&file, buff, buffer_size, &bytes);
		if (buffer_size != bytes) {
			printf("[?] File could not be loaded\n");
			return -1;
		}
	}

	char *buff_backup = buff;
	while (!doExit) {
		int line_size;
		if (interactive) {
			buff = buff_backup;
			memset(buff, '\0', 1024);
			gets(buff, 1024);
		}
		if (tcl_eval(&tcl, buff, strlen(buff)) != FERROR) {
    		printf("%.*s\n", tcl_length(tcl.result), tcl_string(tcl.result));
		}
	}
	free(buff);
	//if (file != NULL) fclose(file);
	return RET_OK;
}

//---------------------------------------------------

void tcl_init(struct tcl *tcl) {
	tcl->env = tcl_env_alloc(NULL);
    tcl->result = tcl_alloc("", 0);
    tcl->cmds = NULL;

	/* Walk throuth the builtin command array and install each one */
	for (int i = 0; i < get_builtin_count(); i++)
		tcl_register(tcl, builtin_commands[i].name, builtin_commands[i].fn, builtin_commands[i].arity, NULL);

		char *math[] = {"+", "-", "*", "/", ">", ">=", "<", "<=", "==", "!="};
		for (unsigned int i = 0; i < (sizeof(math) / sizeof(math[0])); i++)
			tcl_register(tcl, math[i], tcl_cmd_math, 3, NULL);
}

void tcl_destroy(struct tcl *tcl) {
	while (tcl->env) tcl->env = tcl_env_free(tcl->env);
	while (tcl->cmds) {
		struct tcl_cmd *cmd = tcl->cmds;
		tcl->cmds = tcl->cmds->next;
		free(cmd->name);
		free(cmd->arg);
		free(cmd);
	}
	free(tcl->result);
}
