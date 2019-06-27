
#ifndef _HEADER_COMPILER
#define _HEADER_COMPILER

# define __force __attribute__((force))

# define __inline__      		__attribute__((always_inline))
# define __deprecated			__attribute__((deprecated))
# define __no_optimize			__attribute__((optimize("O0")))
# define __attribute_used__		__attribute__((__used__))
# define __attribute_const__	__attribute__((__const__))
# define __must_check			__attribute__((warn_unused_result))

/* Unreachable code */
#ifndef unreachable
# define unreachable() do { } while (1)
#endif

#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif


#ifndef __cold
#define __cold
#endif

/* Simple shorthand for a section definition */
#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
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
