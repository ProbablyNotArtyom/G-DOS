
#ifndef	_HEADER_CDEFS
#define	_HEADER_CDEFS


#ifdef __ELF__
# define _C_LABEL(x)	x
#else
# ifdef __STDC__
#  define _C_LABEL(x)	_ ## x
# else
#  define _C_LABEL(x)	_/**/x
# endif
#endif
#define	_ASM_LABEL(x)	x
#ifdef __STDC__
# define __CONCAT(x,y)	x ## y
# define __STRING(x)	#x
#else
# define __CONCAT(x,y)	x/**/y
# define __STRING(x)	"x"
#endif
#ifndef _ALIGN_TEXT
# define _ALIGN_TEXT .align 0
#endif

/*
 * The __CONCAT macro is used to concatenate parts of symbol names, e.g.
 * with "#define OLD(foo) __CONCAT(old,foo)", OLD(foo) produces oldfoo.
 * The __CONCAT macro is a bit tricky -- make sure you don't put spaces
 * in between its arguments.  __CONCAT can also concatenate double-quoted
 * strings produced by the __STRING macro, but this only works with ANSI C.
 */
#define	__P(protos)		protos
#define __DOTS			, ...
#define	__CONCAT(x,y)	x ## y
#define	__STRING(x)		#x

#define __force 				__attribute__((force))
#define __always_inline 		__inline __attribute__ ((__always_inline__))
#define __attribute_const__		__attribute__((__const__))
#define __attribute_pure__		__attribute__((__pure__))
#define __attribute_used__		__attribute__((__used__))
#define	__attribute_noinline__	__attribute__((__noinline__))
#define	__attribute_packed		__attribute__((__packed__))
#define __weak_symbol			__attribute__((__weak__))
#define __no_optimize			__attribute__((optimize("O0")))

/* Unreachable code */
#ifndef unreachable
#define unreachable() do { } while (1)
#endif

#ifndef __section
#define __section(S) __attribute__ ((__section__(#S)))
#endif

/* Simple shorthand for a section definition */
#ifndef __section
#define __section(S) __attribute__ ((__section__(#S)))
#endif

#ifndef __aligned
#define	__aligned(S) __attribute__((__aligned__(#S)))
#endif

#endif
