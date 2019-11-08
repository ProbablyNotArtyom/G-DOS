
#ifndef HEADER_TCLGO
#define HEADER_TCLGO

//---------------------------------------------------

#define debug_printf(fmt, ...) 	if (debug) printf(fmt, __VA_ARGS__)
#define debug_puts(str) 		if (debug) puts(str)
#define MAX_VAR_LENGTH		255

#define FUNC_BUILTIN(name, argc, fptr)	{name,argc,fptr}
#define tcl_each(ptr, len, skiperr)                                              									\
	for (struct tcl_parser prse = { NULL, NULL, (ptr), (ptr) + (len), 0, TCL_ERROR };        						\
		prse.start < prse.end &&                                                      								\
		(((prse.token = tcl_do_next(prse.start, prse.end - prse.start, &prse.src, &prse.dest, &prse.q)) != TCL_ERROR)	\
		|| (skiperr));                                                            										\
		prse.start = prse.dest)

#define TYPE_MASK(TYPE) 		((TYPE)&0x0000001F)
#define IS_PURE(TYPE) 			(((TYPE) & TCL_FLAG_PURE) != 0)
#define IS_FUNCTION(TYPE) 		(((TYPE) & TCL_FUNCTION0) != 0)
#define IS_CLOSURE(TYPE) 		(((TYPE) & TCL_CLOSURE0) != 0)
#define ARITY(TYPE) 			( ((TYPE) & (TCL_FUNCTION0 | TCL_CLOSURE0)) ? ((TYPE) & 0x00000007) : 0 )
#define NEW_EXPR(type, ...) 	tcl_new_expr((type), (const tcl_expr*[]){__VA_ARGS__})
#define TCL_FUNCTION(...) 		((double(*)(__VA_ARGS__))n->function)
#define TCL_DO_EVAL(e) 			tcl_eval_expr(n->parameters[e])

//---------------------------------------------------

typedef char tcl_value_t;
typedef int (*tcl_cmd_fn_t)(struct tcl *, tcl_value_t *, void *);

#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif

/* Define token constants */
enum {
	TCL_CMD,
	TCL_WORD,
	TCL_PART,
	TCL_WORD_ESC,
	TCL_ESC,
	TCL_STR,
	TCL_ERROR,
	TCL_EOL,
	TCL_SEP,
	TCL_VAR,
	TCL_NONE,
	TCL_DICTSUGAR,
	TCL_EXPRSUGAR,
	TCL_COMMENT
};

/* Define flow control constants */
enum {
	FERROR,
	FNORMAL,
	FRETURN,
	FBREAK,
	FAGAIN
};

/* Helper for parsing tokens */
struct tcl_parser {
	const char *src;
	const char *dest;
	const char *start;
	const char *end;
	int 		q;			// Non-zero if the next chars are a quoted string
	int			token;		// Token type
};

struct tcl {
	struct tcl_env 	*env;
	struct tcl_cmd 	*cmds;
	tcl_value_t 	*result;
};

struct tcl_cmd {
	tcl_value_t 	*name;
	int 			arity;
	tcl_cmd_fn_t 	fn;
	void 			*arg;
	struct tcl_cmd 	*next;
};

struct tcl_cmd_builtin {
	tcl_value_t 	*name;
	int 			arity;
	tcl_cmd_fn_t 	fn;
};

struct tcl_var {
	tcl_value_t 	*name;
	tcl_value_t 	*value;
	struct tcl_var 	*next;
};

typedef struct tcl_expr {
    int 			type;
    union {
		double 			value;
		const double 	*bound;
		const void 		*function;
	};
    void 			*parameters[1];
} tcl_expr;

struct tcl_env {
	struct tcl_var 	*vars;
	struct tcl_env 	*parent;
};

/* Internal for EVAL */
typedef struct tcl_variable {
    const char 		*name;
    const void 		*address;
    int 			type;
    void 			*context;
} tcl_variable;

typedef struct tcl_expr_state {
    const char 		*start;
    const char 		*next;
    int 			type;
    union {
		double 			value;
		const double 	*bound;
		const void 		*function;
	};
    void 			*context;

    const tcl_variable *lookup;
    int lookup_len;
} tcl_expr_state;

//---------------------------------------------------

size_t get_builtin_count(void);

int tcl_is_special(char chr, int q);
int tcl_is_whitespace(char chr);
int tcl_is_eol(char chr);
const char *tcl_string(tcl_value_t *val);
int tcl_int(tcl_value_t *val);
int tcl_length(tcl_value_t *val);

tcl_value_t *tcl_append_string(tcl_value_t *val, const char *ptr, size_t len);
tcl_value_t *tcl_append(tcl_value_t *val, tcl_value_t *tail);
tcl_value_t *tcl_alloc(const char *ptr, size_t len);
tcl_value_t *tcl_dup(tcl_value_t *val);
tcl_value_t *tcl_list_alloc(void);
int tcl_list_length(tcl_value_t *val);
tcl_value_t *tcl_list_at(tcl_value_t *val, int index);
tcl_value_t *tcl_list_append(tcl_value_t *val, tcl_value_t *tail);
struct tcl_env *tcl_env_alloc(struct tcl_env *parent);
struct tcl_var *tcl_env_var(struct tcl_env *env, tcl_value_t *name);
struct tcl_env *tcl_env_free(struct tcl_env *env);
tcl_value_t *tcl_var(struct tcl *tcl, tcl_value_t *name, tcl_value_t *v);
int tcl_result(struct tcl *tcl, int flow, tcl_value_t *result);
int tcl_subst(struct tcl *tcl, const char *ptr, size_t len);
int tcl_eval(struct tcl *tcl, const char *ptr, size_t len);

/* Function declarations for the builtin functions */
int tcl_cmd_set(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_subst(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_puts(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_proc(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_if(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_while(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_flow(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_incr(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_exit(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_math(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_eval(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_expr(struct tcl *tcl, tcl_value_t *args, void *arg);

//---------------------------------------------------

extern bool doExit;
extern bool debug;

//---------------------------------------------------

#endif
