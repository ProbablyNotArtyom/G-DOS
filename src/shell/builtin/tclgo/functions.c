
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>

	#include "tcl.h"

//---------------------------------------------------

const struct tcl_cmd_builtin const builtin_commands[] = {
	FUNC_BUILTIN("set",0,&tcl_cmd_set),
	FUNC_BUILTIN("subst",2,&tcl_cmd_subst),
	FUNC_BUILTIN("puts",2,&tcl_cmd_puts),
	FUNC_BUILTIN("proc",4,&tcl_cmd_proc),
	FUNC_BUILTIN("if",0,&tcl_cmd_if),
	FUNC_BUILTIN("while",3,&tcl_cmd_while),
	FUNC_BUILTIN("return",0,&tcl_cmd_flow),
	FUNC_BUILTIN("break",1,&tcl_cmd_flow),
	FUNC_BUILTIN("continue",1,&tcl_cmd_flow),
	FUNC_BUILTIN("incr",0,&tcl_cmd_incr),
	FUNC_BUILTIN("exit",0,&tcl_cmd_exit),
	FUNC_BUILTIN("eval",2,&tcl_cmd_eval),
	FUNC_BUILTIN("expr",0,&tcl_cmd_expr)
};

void tcl_register(struct tcl *tcl, const char *name, tcl_cmd_fn_t fn, int arity, void *arg) {
	struct tcl_cmd *cmd = malloc(sizeof(struct tcl_cmd));
	cmd->name = tcl_alloc(name, strlen(name));
	cmd->fn = fn;
	cmd->arg = arg;
	cmd->arity = arity;
	cmd->next = tcl->cmds;
	tcl->cmds = cmd;
}

size_t get_builtin_count(void) {
	return (sizeof builtin_commands / sizeof builtin_commands[0]);
}

//---------------------------------------------------

int tcl_cmd_set(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *var = tcl_list_at(args, 1);
	tcl_value_t *val = tcl_list_at(args, 2);
	int r = tcl_result(tcl, FNORMAL, tcl_dup(tcl_var(tcl, var, val)));
	free(var);
	return r;
}

int tcl_cmd_subst(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *s = tcl_list_at(args, 1);
	int r = tcl_subst(tcl, tcl_string(s), tcl_length(s));
	free(s);
	return r;
}

int tcl_cmd_puts(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *text = tcl_list_at(args, 1);
	puts(tcl_string(text));
	return tcl_result(tcl, FNORMAL, text);
}

int tcl_user_proc(struct tcl *tcl, tcl_value_t *args, void *arg) {
	tcl_value_t *code = (tcl_value_t *)arg;
	tcl_value_t *params = tcl_list_at(code, 2);
	tcl_value_t *body = tcl_list_at(code, 3);
	tcl->env = tcl_env_alloc(tcl->env);
	for (int i = 0; i < tcl_list_length(params); i++) {
		tcl_value_t *param = tcl_list_at(params, i);
		tcl_value_t *val = tcl_list_at(args, i + 1);
		tcl_var(tcl, param, val);
		free(param);
	}
	tcl_eval(tcl, tcl_string(body), tcl_length(body) + 1);
	tcl->env = tcl_env_free(tcl->env);
	free(params);
	free(body);
	return FNORMAL;
}

int tcl_cmd_proc(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *name = tcl_list_at(args, 1);
	tcl_register(tcl, tcl_string(name), tcl_user_proc, 0, tcl_dup(args));
	free(name);
	return tcl_result(tcl, FNORMAL, tcl_alloc("", 0));
}

int tcl_cmd_if(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	int i = 1;
	int n = tcl_list_length(args);
	int r = FNORMAL;
	while (i < n) {
		tcl_value_t *cond = tcl_list_at(args, i);
		tcl_value_t *branch = NULL;
		if (i + 1 < n) branch = tcl_list_at(args, i + 1);
		r = tcl_eval(tcl, tcl_string(cond), tcl_length(cond) + 1);
		free(cond);
		if (r != FNORMAL) {
			free(branch);
			break;
		}
		if (tcl_int(tcl->result)) {
			r = tcl_eval(tcl, tcl_string(branch), tcl_length(branch) + 1);
			free(branch);
			break;
		}
		i = i + 2;
		free(branch);
	}
	return r;
}

int tcl_cmd_flow(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	int r = FERROR;
	tcl_value_t *flowval = tcl_list_at(args, 0);
	const char *flow = tcl_string(flowval);

	if (strcmp(flow, "break") == 0) r = FBREAK;
	else if (strcmp(flow, "continue") == 0) r = FAGAIN;
	else if (strcmp(flow, "return") == 0) r = tcl_result(tcl, FRETURN, tcl_list_at(args, 1));

	free(flowval);
	return r;
}

int tcl_cmd_while(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *cond = tcl_list_at(args, 1);
	tcl_value_t *loop = tcl_list_at(args, 2);
	int r;
	for (;;) {
		r = tcl_eval(tcl, tcl_string(cond), tcl_length(cond) + 1);
		if (r != FNORMAL) {
			free(cond);
			free(loop);
			return r;
		}
		if (!tcl_int(tcl->result)) {
			free(cond);
			free(loop);
			return FNORMAL;
		}
		int r = tcl_eval(tcl, tcl_string(loop), tcl_length(loop) + 1);
		switch (r) {
			case FBREAK:
				free(cond);
				free(loop);
				return FNORMAL;
			case FRETURN:
				free(cond);
				free(loop);
				return FRETURN;
			case FAGAIN:
				continue;
			case FERROR:
				free(cond);
				free(loop);
				return FERROR;
		}
	}
}

int tcl_cmd_incr(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	char buf[5 + MAX_VAR_LENGTH] = "set ";

	tcl_value_t *var = tcl_list_at(args, 1);			// Get the first argument string
	strncat(buf, tcl_string(var), strlen(var));
	strncat(buf, " [+ 1 $", 8);
	strncat(buf, tcl_string(var), strlen(var));
	strncat(buf, "]", 2);
	tcl_eval(tcl, buf, 15);

	free(var);
	return FNORMAL;
}

int tcl_cmd_exit(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	doExit = true;
	return FRETURN;
}

int tcl_cmd_eval(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *s = tcl_list_at(args, 1);

	int r = tcl_eval(tcl, tcl_string(s), tcl_length(s) + 1);
	free(s);
	return r;
}
