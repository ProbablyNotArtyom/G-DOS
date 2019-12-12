
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>
	#include <errno.h>
	#include <limits.h>
	#include <math.h>

	#include "tcl.h"

//---------------------------------------------------

enum { TCL_CONSTANT = 1 };
enum {
    TCL_VARIABLE = 0,
    TCL_FUNCTION0 = 8, TCL_FUNCTION1, TCL_FUNCTION2, TCL_FUNCTION3,
    TCL_FUNCTION4, TCL_FUNCTION5, TCL_FUNCTION6, TCL_FUNCTION7,
    TCL_CLOSURE0 = 16, TCL_CLOSURE1, TCL_CLOSURE2, TCL_CLOSURE3,
    TCL_CLOSURE4, TCL_CLOSURE5, TCL_CLOSURE6, TCL_CLOSURE7,
    TCL_FLAG_PURE = 32
};

enum {
    TOKEN_NULL = TCL_CLOSURE7+1, TOKEN_ERROR, TOKEN_END, TOKEN_SEP,
    TOKEN_OPEN, TOKEN_CLOSE, TOKEN_NUMBER, TOKEN_VARIABLE, TOKEN_INFIX
};

// Declarations
double tcl_interp_expr(const char *expression, int *error);
static tcl_expr *tcl_compile_expr(const char *expression, const tcl_variable *variables, int var_count, int *error);
static double tcl_eval_expr(const tcl_expr *n);
static void free_expr(tcl_expr *n);
static void next_token(tcl_expr_state *s);
static void free_expr_parameters(tcl_expr *expr);
static double pi(void);
static double e(void);
static double fac(double a);
static double ncr(double n, double r);
static double npr(double n, double r);
static tcl_expr *list(tcl_expr_state *s);
static tcl_expr *expr(tcl_expr_state *s);
static tcl_expr *power(tcl_expr_state *s);
static tcl_expr *base(tcl_expr_state *s);

static const tcl_variable const functions[] = {
    {"abs", fabs,     	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"acos", acos,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"asin", asin,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"atan", atan,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"atan2", atan2,  	TCL_FUNCTION2 | TCL_FLAG_PURE, 0},
    {"ceil", ceil,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"cos", cos,      	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"cosh", cosh,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"e", e,          	TCL_FUNCTION0 | TCL_FLAG_PURE, 0},
    {"exp", exp,      	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"fac", fac,      	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"floor", floor,  	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"ln", log,       	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"log", log10,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"log10", log10,  	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"ncr", ncr,      	TCL_FUNCTION2 | TCL_FLAG_PURE, 0},
    {"npr", npr,      	TCL_FUNCTION2 | TCL_FLAG_PURE, 0},
    {"pi", pi,        	TCL_FUNCTION0 | TCL_FLAG_PURE, 0},
    {"pow", pow,      	TCL_FUNCTION2 | TCL_FLAG_PURE, 0},
    {"sin", sin,      	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"sinh", sinh,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"sqrt", sqrt,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"tan", tan,      	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {"tanh", tanh,    	TCL_FUNCTION1 | TCL_FLAG_PURE, 0},
    {0, 0, 0, 0}
};

//---------------------------------------------------

int tcl_cmd_expr(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	tcl_value_t *var = tcl_list_at(args, 1);
	double expr_ret = tcl_interp_expr(var, 0);
	free(var);
	char tmp[64];

	if (expr_ret == INFINITY) {
		snprintf(tmp, sizeof(tmp)-1, "infinity");
	} else if (ceilf(expr_ret) != expr_ret) {
		snprintf(tmp, sizeof(tmp)-1, "%f", expr_ret);
	} else
		snprintf(tmp, sizeof(tmp)-1, "%d", (int)expr_ret);

	return tcl_result(tcl, FNORMAL, tcl_alloc(tmp, strlen(tmp)));
}

int tcl_cmd_math(struct tcl *tcl, tcl_value_t *args, void *arg) {
	(void)arg;
	char buf[64];
	tcl_value_t *opval = tcl_list_at(args, 0);
	tcl_value_t *aval = tcl_list_at(args, 1);
	tcl_value_t *bval = tcl_list_at(args, 2);
	const char *op = tcl_string(opval);
	int a = tcl_int(aval);
	int b = tcl_int(bval);
	int c = 0;

	if (op[0] == '+') c = a + b;
	else if (op[0] == '-') c = a - b;
	else if (op[0] == '*') c = a * b;
	else if (op[0] == '/') c = a / b;
	else if (op[0] == '>' && op[1] == '\0') c = a > b;
	else if (op[0] == '>' && op[1] == '=') c = a >= b;
	else if (op[0] == '<' && op[1] == '\0') c = a < b;
	else if (op[0] == '<' && op[1] == '=') c = a <= b;
	else if (op[0] == '=' && op[1] == '=') c = a == b;
	else if (op[0] == '!' && op[1] == '=') c = a != b;
	snprintf(buf, sizeof(buf)-1, "%d", c);
	free(opval);
	free(aval);
	free(bval);
	return tcl_result(tcl, FNORMAL, tcl_alloc(buf, strlen(buf)));
}

//---------------------------------------------------

static double pi(void) { return 3.14159265358979323846; }
static double e(void) { return 2.71828182845904523536; }
static double fac(double a) {
    if (a < 0.0) return NAN;
    if (a > UINT_MAX) return INFINITY;
    unsigned long int result = 1, i;
    for (i = 1; i <= (unsigned int)(a); i++) {
        if (i > ULONG_MAX / result) return INFINITY;
        result *= i;
    }
    return (double)result;
}

static double ncr(double n, double r) {
    if (n < 0.0 || r < 0.0 || n < r) return NAN;
    if (n > UINT_MAX || r > UINT_MAX) return INFINITY;
    unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
    unsigned long int result = 1;
    if (ur > un / 2) ur = un - ur;
    for (i = 1; i <= ur; i++) {
        if (result > ULONG_MAX / (un - ur + i)) return INFINITY;
        result *= un - ur + i;
        result /= i;
    }
    return result;
}
static double npr(double n, double r) { return ncr(n, r) * fac(r); }
static double add(double a, double b) { return a + b; }
static double sub(double a, double b) { return a - b; }
static double mul(double a, double b) { return a * b; }
static double divide(double a, double b) { return a / b; }
static double negate(double a) { return -a;}
static double comma(double a, double b) { (void)a; return b; }

//---------------------------------------------------

static tcl_expr *tcl_new_expr(const int type, const tcl_expr *params[]) {
	const int arity = ARITY(type);
    const int psize = sizeof(void*) * arity;
    const int size = (sizeof(tcl_expr) - sizeof(void*)) + psize +
		(IS_CLOSURE(type) ? sizeof(void*) : 0);

	tcl_expr *ret = (tcl_expr*)malloc(size);
	memset(ret, 0, size);
	if (arity && params) memcpy(ret->parameters, params, psize);
	ret->type = type;
    ret->bound = 0;
    return ret;
}

static void free_expr_parameters(tcl_expr *expr) {
    if (!expr) return;
    switch (TYPE_MASK(expr->type) & 0b10000) {						// Masked so that each case also matches its TCL_CLOSEx
        case TCL_FUNCTION7: free_expr(expr->parameters[6]);
        case TCL_FUNCTION6: free_expr(expr->parameters[5]);
        case TCL_FUNCTION5: free_expr(expr->parameters[4]);
        case TCL_FUNCTION4: free_expr(expr->parameters[3]);
        case TCL_FUNCTION3: free_expr(expr->parameters[2]);
        case TCL_FUNCTION2: free_expr(expr->parameters[1]);
        case TCL_FUNCTION1: free_expr(expr->parameters[0]);
    }
}

static const tcl_variable *find_builtin(const char *name, int len) {
    int imin = 0;
    int imax = sizeof(functions) / sizeof(tcl_variable) - 2;

    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        int c = strncmp(name, functions[i].name, len);

		if (!c) c = '\0' - functions[i].name[len];
        if (c == 0) return functions + i;
		else if (c > 0) imin = i + 1;
		else imax = i - 1;
    }
    return 0;
}

static const tcl_variable *find_lookup(const tcl_expr_state *state, const char *name, int len) {
    if (!state->lookup) return 0;

	int iters;
    const tcl_variable *var;
    for (var = state->lookup, iters = state->lookup_len; iters; ++var, --iters) {
        if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') return var;
    }
    return 0;
}

static void next_token(tcl_expr_state *s) {
    s->type = TOKEN_NULL;
    do {
        if (!*s->next){
            s->type = TOKEN_END;
            return;
        }

        /* Try reading a number. */
        if (isdigit(s->next[0]) || s->next[0] == '.') {
            s->value = strtod(s->next, (char**)&s->next);
            s->type = TOKEN_NUMBER;
        } else {
            /* Look for a variable or builtin function call. */
            if (s->next[0] >= 'a' && s->next[0] <= 'z') {
                const char *start = s->next;
                while (isalpha(s->next[0]) || isdigit(s->next[0]) || (s->next[0] == '_')) s->next++;

                const tcl_variable *var = find_lookup(s, start, s->next - start);
                if (!var) var = find_builtin(start, s->next - start);

                if (!var) {
                    s->type = TOKEN_ERROR;
                } else if (TYPE_MASK(var->type) == TOKEN_VARIABLE) {
					s->type = TOKEN_VARIABLE;
					s->bound = var->address;
				} else if (TYPE_MASK(var->type) >= TCL_CLOSURE0 && TYPE_MASK(var->type) <= TCL_CLOSURE7) {
					s->context = var->context;
				} else if (TYPE_MASK(var->type) >= TCL_FUNCTION0 && TYPE_MASK(var->type) <= TCL_FUNCTION7) {
					s->type = var->type; s->function = var->address; break;
                }

            } else {
                /* Look for an operator or special character. */
                switch (s->next++[0]) {
					default: 	s->type = TOKEN_ERROR; break;
                    case '+': 	s->type = TOKEN_INFIX; s->function = add; break;
                    case '-': 	s->type = TOKEN_INFIX; s->function = sub; break;
                    case '*': 	s->type = TOKEN_INFIX; s->function = mul; break;
                    case '/': 	s->type = TOKEN_INFIX; s->function = divide; break;
                    case '^': 	s->type = TOKEN_INFIX; s->function = pow; break;
                    case '%': 	s->type = TOKEN_INFIX; s->function = fmod; break;
                    case '(': 	s->type = TOKEN_OPEN; break;
                    case ')': 	s->type = TOKEN_CLOSE; break;
                    case ',': 	s->type = TOKEN_SEP; break;
					case ' ':
					case '\t':
					case '\n':
					case '\r':	break;
                }
            }
        }
    } while (s->type == TOKEN_NULL);	// Skip all the whitespace
}

static void free_expr(tcl_expr *expr) {
    if (!expr) return;
    free_expr_parameters(expr);
    free(expr);
}

/* <base>      =    <constant> | <variable> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
static tcl_expr *base(tcl_expr_state *s) {
    tcl_expr *ret;
    int arity;

    switch (TYPE_MASK(s->type)) {
        case TOKEN_NUMBER:
            ret = tcl_new_expr(TCL_CONSTANT, 0);
            ret->value = s->value;
            next_token(s);
            break;
        case TOKEN_VARIABLE:
            ret = tcl_new_expr(TCL_VARIABLE, 0);
            ret->bound = s->bound;
            next_token(s);
            break;
        case TCL_FUNCTION0:
        case TCL_CLOSURE0:
            ret = tcl_new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[0] = s->context;
            next_token(s);
            if (s->type == TOKEN_OPEN) {
                next_token(s);
                if (s->type != TOKEN_CLOSE) s->type = TOKEN_ERROR;
                else next_token(s);
            }
            break;
        case TCL_FUNCTION1:
        case TCL_CLOSURE1:
            ret = tcl_new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[1] = s->context;
            next_token(s);
            ret->parameters[0] = power(s);
            break;
        case TCL_FUNCTION2: case TCL_FUNCTION3: case TCL_FUNCTION4:
        case TCL_FUNCTION5: case TCL_FUNCTION6: case TCL_FUNCTION7:
        case TCL_CLOSURE2: case TCL_CLOSURE3: case TCL_CLOSURE4:
        case TCL_CLOSURE5: case TCL_CLOSURE6: case TCL_CLOSURE7:
            arity = ARITY(s->type);

            ret = tcl_new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[arity] = s->context;
            next_token(s);

            if (s->type != TOKEN_OPEN) {
                s->type = TOKEN_ERROR;
            } else {
				int i;
                for(i = 0; i < arity; i++) {
                    next_token(s);
                    ret->parameters[i] = expr(s);
                    if(s->type != TOKEN_SEP) break;
                }
                if(s->type != TOKEN_CLOSE || i != arity - 1) s->type = TOKEN_ERROR;
                else next_token(s);
            }
            break;
        case TOKEN_OPEN:
            next_token(s);
            ret = list(s);
            if (s->type != TOKEN_CLOSE) s->type = TOKEN_ERROR;
            else next_token(s);
            break;
        default:
            ret = tcl_new_expr(0, 0);
            s->type = TOKEN_ERROR;
            ret->value = NAN;
            break;
    }
    return ret;
}

static double tcl_eval_expr(const tcl_expr *n) {
    if (!n) return NAN;

    switch(TYPE_MASK(n->type)) {
        case TCL_CONSTANT: return n->value;
        case TCL_VARIABLE: return *n->bound;

        case TCL_FUNCTION0: case TCL_FUNCTION1: case TCL_FUNCTION2: case TCL_FUNCTION3:
        case TCL_FUNCTION4: case TCL_FUNCTION5: case TCL_FUNCTION6: case TCL_FUNCTION7:
            switch(ARITY(n->type)) {
                case 0: return TCL_FUNCTION(void)();
                case 1: return TCL_FUNCTION(double)(TCL_DO_EVAL(0));
                case 2: return TCL_FUNCTION(double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1));
                case 3: return TCL_FUNCTION(double, double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2));
                case 4: return TCL_FUNCTION(double, double, double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3));
                case 5: return TCL_FUNCTION(double, double, double, double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4));
                case 6: return TCL_FUNCTION(double, double, double, double, double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4), TCL_DO_EVAL(5));
                case 7: return TCL_FUNCTION(double, double, double, double, double, double, double)(TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4), TCL_DO_EVAL(5), TCL_DO_EVAL(6));
                default: return NAN;
            }

        case TCL_CLOSURE0: case TCL_CLOSURE1: case TCL_CLOSURE2: case TCL_CLOSURE3:
        case TCL_CLOSURE4: case TCL_CLOSURE5: case TCL_CLOSURE6: case TCL_CLOSURE7:
            switch(ARITY(n->type)) {
                case 0: return TCL_FUNCTION(void*)(n->parameters[0]);
                case 1: return TCL_FUNCTION(void*, double)(n->parameters[1], TCL_DO_EVAL(0));
                case 2: return TCL_FUNCTION(void*, double, double)(n->parameters[2], TCL_DO_EVAL(0), TCL_DO_EVAL(1));
                case 3: return TCL_FUNCTION(void*, double, double, double)(n->parameters[3], TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2));
                case 4: return TCL_FUNCTION(void*, double, double, double, double)(n->parameters[4], TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3));
                case 5: return TCL_FUNCTION(void*, double, double, double, double, double)(n->parameters[5], TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4));
                case 6: return TCL_FUNCTION(void*, double, double, double, double, double, double)(n->parameters[6], TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4), TCL_DO_EVAL(5));
                case 7: return TCL_FUNCTION(void*, double, double, double, double, double, double, double)(n->parameters[7], TCL_DO_EVAL(0), TCL_DO_EVAL(1), TCL_DO_EVAL(2), TCL_DO_EVAL(3), TCL_DO_EVAL(4), TCL_DO_EVAL(5), TCL_DO_EVAL(6));
                default: return NAN;
            }
        default: return NAN;
    }
}

static void optimize(tcl_expr *n) {
    if (n->type == TCL_CONSTANT || (n->type == TCL_VARIABLE)) return;

    /* Only optimize out functions flagged as pure. */
    if (IS_PURE(n->type)) {
        const int arity = ARITY(n->type);
        int known = 1;
        for (int i = 0; i < arity; ++i) {
            optimize(n->parameters[i]);
            if (((tcl_expr*)(n->parameters[i]))->type != TCL_CONSTANT) known = 0;
        }
        if (known) {
            const double value = tcl_eval_expr(n);
            free_expr_parameters(n);
            n->type = TCL_CONSTANT;
            n->value = value;
        }
    }
}

static tcl_expr *tcl_compile_expr(const char *expression, const tcl_variable *variables, int var_count, int *error) {
    tcl_expr_state state;
    state.start = state.next = expression;
    state.lookup = variables;
    state.lookup_len = var_count;

    next_token(&state);
    tcl_expr *root = list(&state);

    if (state.type != TOKEN_END) {
        free_expr(root);
        if (error) {
            *error = (state.next - state.start);
            if (*error == 0) *error = 1;
        }
        return 0;
    } else {
        optimize(root);
        if (error) *error = 0;
        return root;
    }
}

double tcl_interp_expr(const char *expression, int *error) {
    tcl_expr *n = tcl_compile_expr(expression, 0, 0, error);
    double ret;
    if (n) {
        ret = tcl_eval_expr(n);
        free_expr(n);
    } else {
        ret = NAN;
    }
    return ret;
}

//---------------------------------------------------
// These handle the evaluation of different possible mathematical operations

/* <power>     =    {("-" | "+")} <base> */
static tcl_expr *power(tcl_expr_state *s) {
    int sign = 1;
	tcl_expr *ret;
    while (s->type == TOKEN_INFIX && (s->function == add || s->function == sub)) {
        if (s->function == sub) sign = -sign;
        next_token(s);
    }

    if (sign == 1) {
        ret = base(s);
    } else {
        ret = NEW_EXPR(TCL_FUNCTION1 | TCL_FLAG_PURE, base(s));
        ret->function = negate;
    }
    return ret;
}

#ifdef TCL_POW_FROM_RIGHT
/* <factor>    =    <power> {"^" <power>} */
static tcl_expr *factor(tcl_expr_state *s) {
    tcl_expr *ret = power(s);
    int neg = 0;
    tcl_expr *insertion = 0;

    if (ret->type == (TCL_FUNCTION1 | TCL_FLAG_PURE) && ret->function == negate) {
        tcl_expr *se = ret->parameters[0];
        free(ret);
        ret = se;
        neg = 1;
    }

    while (s->type == TOKEN_INFIX && (s->function == pow)) {
		t = s->function;
        next_token(s);

        if (insertion) {
            /* Make exponentiation go right-to-left. */
            tcl_expr *insert = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, insertion->parameters[1], power(s));
            insert->function = t;
            insertion->parameters[1] = insert;
            insertion = insert;
        } else {
            ret = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, ret, power(s));
            ret->function = t;
            insertion = ret;
        }
    }

    if (neg) {
        ret = NEW_EXPR(TCL_FUNCTION1 | TCL_FLAG_PURE, ret);
        ret->function = negate;
    }
    return ret;
}
#else
/* <factor>    =    <power> {"^" <power>} */
static tcl_expr *factor(tcl_expr_state *s) {
    tcl_expr *ret = power(s);

    while (s->type == TOKEN_INFIX && (s->function == pow)) {
        double(*t)(double, double) = s->function;
        next_token(s);
        ret = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, ret, power(s));
        ret->function = t;
    }
    return ret;
}
#endif


/* <term>      =    <factor> {("*" | "/" | "%") <factor>} */
static tcl_expr *term(tcl_expr_state *s) {
    tcl_expr *ret = factor(s);

    while (s->type == TOKEN_INFIX && (s->function == mul || s->function == divide || s->function == fmod)) {
        double(*t)(double, double) = s->function;
        next_token(s);
        ret = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, ret, factor(s));
        ret->function = t;
    }

    return ret;
}

/* <expr>      =    <term> {("+" | "-") <term>} */
static tcl_expr *expr(tcl_expr_state *s) {
    tcl_expr *ret = term(s);

    while (s->type == TOKEN_INFIX && (s->function == add || s->function == sub)) {
        double(*t)(double, double) = s->function;
        next_token(s);
        ret = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, ret, term(s));
        ret->function = t;
    }

    return ret;
}

/* <list>      =    <expr> {"," <expr>} */
static tcl_expr *list(tcl_expr_state *s) {
    tcl_expr *ret = expr(s);

    while (s->type == TOKEN_SEP) {
        next_token(s);
        ret = NEW_EXPR(TCL_FUNCTION2 | TCL_FLAG_PURE, ret, expr(s));
        ret->function = comma;
    }

    return ret;
}
