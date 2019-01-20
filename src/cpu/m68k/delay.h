
#ifndef _M68K_DELAY_H
#define _M68K_DELAY_H


#define	HZSCALE		(268435456 / (1000000 / HZ))

static inline void __delay(unsigned long loops)
{
	__asm__ __volatile__ (
		DELAY_ALIGN
		"1: subql #1,%0\n\t"
		"jcc 1b"
		: "=d" (loops)
		: "0" (loops));
}

static inline void __udelay(unsigned long usecs)
{
	__const_udelay(usecs);
}

#define udelay(n) (__builtin_constant_p(n) ? \
	((n) > 20000 ? __bad_udelay() : __const_udelay(n)) : __udelay(n))

#endif
