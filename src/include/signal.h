//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#ifndef _HEADER_SIGNAL
#define _HEADER_SIGNAL

//---------------------------------------------------

typedef int sig_atomic_t;

typedef void (*__sighandler_t) (int);

void (*signal(int sig, __sighandler_t func))(int);
int raise(int sig);

//---------------------------------------------------

#define SIGABRT 	0
#define SIGFPE 		1
#define SIGILL 		2
#define SIGINT 		3
#define SIGSEGV 	4
#define SIGTERM 	5

#define SIG_ERR ((__sighandler_t) -1)
#define SIG_DFL ((__sighandler_t)  0)
#define SIG_IGN ((__sighandler_t)  1)

#define raise(blah)		while(1)

//---------------------------------------------------

#endif
