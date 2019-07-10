
#ifndef	_HEADER_CDEFS
#define	_HEADER_CDEFS

/*
 * The __CONCAT macro is used to concatenate parts of symbol names, e.g.
 * with "#define OLD(foo) __CONCAT(old,foo)", OLD(foo) produces oldfoo.
 * The __CONCAT macro is a bit tricky -- make sure you don't put spaces
 * in between its arguments.  __CONCAT can also concatenate double-quoted
 * strings produced by the __STRING macro, but this only works with ANSI C.
 */
#define	__P(protos)	protos
#define	__CONCAT(x,y)	x ## y
#define	__STRING(x)	#x

#define __force 				__attribute__((force))
#define __inline__      		__attribute__((always_inline))
#define __noreturn__      		__attribute__((no_return))
#define __deprecated			__attribute__((deprecated))
#define __no_optimize			__attribute__((optimize("O0")))
#define __attribute_used__		__attribute__((__used__))
#define __attribute_const__		__attribute__((__const__))
#define __weak__				__attribute__((__weak__))
#define __must_check			__attribute__((warn_unused_result))

/* Unreachable code */
#ifndef unreachable
#define unreachable() do { } while (1)
#endif

#ifndef __section
#define __section(S) __attribute__ ((__section__(#S)))
#endif


#ifndef __cold
#define __cold
#endif

/* Simple shorthand for a section definition */
#ifndef __section
#define __section(S) __attribute__ ((__section__(#S)))
#endif

#ifndef __visible
#define __visible
#endif

/*
 * Assume alignment of return value.
 */
#ifndef __assume_aligned
#define __assume_aligned(a, ...)
#endif

#endif
