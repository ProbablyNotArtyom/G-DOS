
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>

	#include "tcl.h"

//---------------------------------------------------

const char *tcl_string(tcl_value_t *val) { return val; }
int tcl_int(tcl_value_t *val) { return atoi(val); }
int tcl_length(tcl_value_t *val) { return (val == NULL ? 0 : strlen(val)); }
int tcl_is_whitespace(char chr) { return (chr == ' ' || chr == '\t'); }
int tcl_is_eol(char chr) { return (chr == '\n' || chr == '\r' || chr == '\0' || chr == ';'); }
int tcl_is_special(char chr, int q) {
	return (chr == '$' ||
		(!q && (chr == '{' || chr == '}' || chr == ';' || chr == '\r' || chr == '\n')) ||
        chr == '[' || chr == ']' || chr == '"' || chr == '\0');
}

tcl_value_t *tcl_alloc(const char *ptr, size_t len) { return tcl_append_string(NULL, ptr, len); }
tcl_value_t *tcl_dup(tcl_value_t *val) { return tcl_alloc(tcl_string(val), tcl_length(val)); }
tcl_value_t *tcl_list_alloc(void) { return tcl_alloc("", 0); }

tcl_value_t *tcl_append_string(tcl_value_t *val, const char *ptr, size_t len) {
	size_t tmp = tcl_length(val);
	val = realloc(val, tmp + len + 2);

	memset((char*)tcl_string(val) + tmp, 0, len + 1);
	strncpy((char*)tcl_string(val) + tmp, ptr, len);
	return val;
}

tcl_value_t *tcl_append(tcl_value_t *val, tcl_value_t *tail) {
	val = tcl_append_string(val, tcl_string(tail), tcl_length(tail));
	free(tail);
	return val;
}

int tcl_list_length(tcl_value_t *val) {
	int index = 0;
	tcl_each(tcl_string(val), tcl_length(val) + 1, 0)
		if (prse.token == TCL_WORD) index++;

	return index;
}

tcl_value_t *tcl_list_at(tcl_value_t *val, int index) {
	int i = 0;
	tcl_each(tcl_string(val), tcl_length(val) + 1, 0) {
		if (prse.token == TCL_WORD) {
			if (i == index) {
				if (prse.src[0] == '{') {
					return tcl_alloc(prse.src + 1, prse.dest - prse.src - 2);
				}
				return tcl_alloc(prse.src, prse.dest - prse.src);
			}
			i++;
		}
	}
	return NULL;
}

tcl_value_t *tcl_list_append(tcl_value_t *val, tcl_value_t *tail) {
	if (tcl_length(val) > 0) val = tcl_append(val, tcl_alloc(" ", 2));
	if (tcl_length(tail) > 0) {
		int q = 0;
		const char *ptr;
		for (ptr = tcl_string(tail); *ptr; ptr++) {
			if (tcl_is_whitespace(*ptr) || tcl_is_special(*ptr, 0)) {
				q = 1;
				break;
			}
		}
		if (q) val = tcl_append(val, tcl_alloc("{", 1));
		val = tcl_append(val, tcl_dup(tail));
		if (q) val = tcl_append(val, tcl_alloc("}", 1));
	} else {
		val = tcl_append(val, tcl_alloc("{}", 2));
	}
	return val;
}

//---------------------------------------------------

struct tcl_env *tcl_env_alloc(struct tcl_env *parent) {
	struct tcl_env *env = malloc(sizeof(*env));
	env->vars = NULL;
	env->parent = parent;
	return env;
}

struct tcl_var *tcl_env_var(struct tcl_env *env, tcl_value_t *name) {
	struct tcl_var *var = malloc(sizeof(struct tcl_var));
	var->name = tcl_dup(name);
	var->next = env->vars;
	var->value = tcl_alloc("", 0);
	env->vars = var;
	return var;
}

struct tcl_env *tcl_env_free(struct tcl_env *env) {
	struct tcl_env *parent = env->parent;
	while (env->vars) {
		struct tcl_var *var = env->vars;
		env->vars = env->vars->next;
    	free(var->name);
    	free(var->value);
    	free(var);
	}
	free(env);
	return parent;
}

tcl_value_t *tcl_var(struct tcl *tcl, tcl_value_t *name, tcl_value_t *v) {
	debug_printf("var(%s := %.*s)\n", tcl_string(name), tcl_length(v), tcl_string(v));
	struct tcl_var *var;
	for (var = tcl->env->vars; var != NULL; var = var->next)
		if (strcmp(var->name, tcl_string(name)) == 0) break;
	if (var == NULL) var = tcl_env_var(tcl->env, name);
	if (v != NULL) {
    	free(var->value);
    	var->value = tcl_dup(v);
    	free(v);
	}
	return var->value;
}

int tcl_result(struct tcl *tcl, int flow, tcl_value_t *result) {
	debug_printf("tcl_result %.*s, flow=%d\n", tcl_length(result), tcl_string(result), flow);
	free(tcl->result);
	tcl->result = result;
	return flow;
}

int tcl_subst(struct tcl *tcl, const char *ptr, size_t len) {
	debug_printf("subst(%.*s)\n", (int)len, ptr);
	if (len == 0) return tcl_result(tcl, FNORMAL, tcl_alloc("", 0));

	switch (ptr[0]) {
		case '{':
			if (len <= 1) return tcl_result(tcl, FERROR, tcl_alloc("", 0));
			else return tcl_result(tcl, FNORMAL, tcl_alloc(ptr + 1, len - 2));
		case '$': {
			if (len >= MAX_VAR_LENGTH) return tcl_result(tcl, FERROR, tcl_alloc("", 0));
			char buf[5 + MAX_VAR_LENGTH] = "set ";
		    strncat(buf, ptr + 1, len - 1);
		    return tcl_eval(tcl, buf, strlen(buf) + 1);
		}
		case '[': {
			tcl_value_t *expr = tcl_alloc(ptr + 1, len - 2);
		    int tmp = tcl_eval(tcl, tcl_string(expr), tcl_length(expr) + 1);
		    free(expr);
		    return tmp;
		}
		default:
			return tcl_result(tcl, FNORMAL, tcl_alloc(ptr, len));
	}
}

int tcl_eval(struct tcl *tcl, const char *ptr, size_t len) {
	tcl_value_t *list = tcl_list_alloc();
	tcl_value_t *current = NULL;
	tcl_each(ptr, len, 1) {
		switch (prse.token) {
			case TCL_ERROR:
				return tcl_result(tcl, FERROR, tcl_alloc("", 0));
			case TCL_WORD:
				if (current != NULL) {
					tcl_subst(tcl, prse.src, prse.dest - prse.src);
			      	tcl_value_t *part = tcl_dup(tcl->result);
			        current = tcl_append(current, part);
				} else {
					tcl_subst(tcl, prse.src, prse.dest - prse.src);
					current = tcl_dup(tcl->result);
				}
				list = tcl_list_append(list, current);
		        free(current);
		        current = NULL;
		        break;
			case TCL_PART:
				tcl_subst(tcl, prse.src, prse.dest - prse.src);
				tcl_value_t *part = tcl_dup(tcl->result);
		        current = tcl_append(current, part);
		        break;
			case TCL_CMD:
				if (tcl_list_length(list) == 0) {
					tcl_result(tcl, FNORMAL, tcl_alloc("", 0));
				} else {
					tcl_value_t *cmdname = tcl_list_at(list, 0);
					struct tcl_cmd *cmd = NULL;
					int r = FERROR;
					for (cmd = tcl->cmds; cmd != NULL; cmd = cmd->next) {
						if (strcmp(tcl_string(cmdname), tcl_string(cmd->name)) == 0) {
							if (cmd->arity == 0 || cmd->arity == tcl_list_length(list)) {
								r = cmd->fn(tcl, list, cmd->arg);
								break;
							}
						}
			        }
			        free(cmdname);
			        if (cmd == NULL || r != FNORMAL) {
						free(list);
						return r;
			        }
				}
				free(list);
				list = tcl_list_alloc();
				break;
		}
	}
	free(list);
	return FNORMAL;
}

//---------------------------------------------------

int tcl_do_next(const char *ptr, size_t len, const char **src, const char **dest, int *q) {
	unsigned int i = 0;
	int depth = 0;
    char open, close;

	/* Skip leading whitespace */
	for (; !*q && len > 0 && tcl_is_whitespace(*ptr); ptr++, len--);
	*src = ptr;										// Update the parser position

	if (!*q && len > 0 && tcl_is_eol(*ptr)) {		// Exit command if not quoted
		*dest = ptr + 1;
		return TCL_CMD;
	}

	if (*ptr == '$') {								// Check for the variable token
		if (tcl_is_whitespace(ptr[1]) || ptr[1] == '"') return TCL_ERROR;
		int mode = *q;
		*q = 0;
		int tmp = tcl_do_next(ptr + 1, len - 1, dest, dest, q);
		*q = mode;
		return ((tmp == TCL_WORD && *q) ? TCL_PART : tmp);
	}

	if (*ptr == '[' || (!*q && *ptr == '{')) {		// Check for substitutions
		open = *ptr;
		close = (open == '[' ? ']' : '}');
		for (i = 0, depth = 1; i < len && depth != 0; i++) {
			if (i > 0 && ptr[i] == open)
				depth++;
			else if (ptr[i] == close)
				depth--;
		}
	} else if (*ptr == '"') {						// Check for quoted strings
		*q = !*q;
		*src = *dest = ptr + 1;
		if (*q)
			return TCL_PART;
		if (len < 2 || (!tcl_is_whitespace(ptr[1]) && !tcl_is_eol(ptr[1])))
			return TCL_ERROR;
		*src = *dest = ptr + 1;
		return TCL_WORD;
	} else {
		while (i < len && (*q || !tcl_is_whitespace(ptr[i])) && !tcl_is_special(ptr[i], *q))
			i++;
	}

	*dest = ptr + i;
	if (i == len) return TCL_ERROR;
	if (*q) return TCL_PART;
	return (tcl_is_whitespace(ptr[i]) || tcl_is_eol(ptr[i])) ? TCL_WORD : TCL_PART;
}

//---------------------------------------------------
