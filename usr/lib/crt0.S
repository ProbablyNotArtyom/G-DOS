/* Startup file for the BSVC simulator suite */

	.title <startup.S>

/* These are defined in C code. */
	.extern SYM (main)
	.extern SYM (media_start)
	.extern SYM (exit)
	.extern SYM (atexit)

/*
 * These values are set in the linker script, so they must be
 * explicitly named here without SYM.
 */
	.extern __bss_start
	.extern _end

/*
 * Set things up so the application will run. For historical reasons
 * this is called 'start'.  We set things up to provide '_start'
 * as with other systems, but also provide a weak alias called
 * 'start' for compatibility with existing linker scripts.
 */
	.globl _start

_start:

1:
	/* Clear BSS */
 	leal   _end,%a0
 	movel  %a0,%d2
 	leal   __bss_start,%a1
 	subl   %a1,%a0
 	movel  %a0,%d0
 	jbra   3f
2:	clrb   (%a1)+
3:	dbra    %d0,2b

        pea     0
        pea     %sp@(4)
        pea     0
	pea	(%a1)
	jbsr	main /* Here the main(..) function is called */
	rts
