//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------


#ifndef _HEADER_SETJMP
#define _HEADER_SETJMP

typedef unsigned long jmp_buf[6];
int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif
