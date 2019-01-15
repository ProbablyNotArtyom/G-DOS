
#ifndef _HEADER_COMPILER
#define _HEADER_COMPILER


/* Optimization barrier */
#ifndef barrier
# define barrier() __memory_barrier()
#endif

#ifndef barrier_data
# define barrier_data(ptr) barrier()
#endif

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
