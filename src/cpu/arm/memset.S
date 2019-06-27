
#include <asm.h>

        /*
         * Optimized memset() for ARM.
         *
         * memset() returns its first argument.
         */

ENTRY(memset)
        /* compute the offset to align the destination
         * offset = (4-(src&3))&3 = -src & 3
         */
        .save       {r0, r4-r7, lr}
        stmfd       sp!, {r0, r4-r7, lr}
        rsb         r3, r0, #0
        ands        r3, r3, #3
        cmp         r3, r2
        movhi       r3, r2
        /* splat r1 */
        mov         r1, r1, lsl #24
        orr         r1, r1, r1, lsr #8
        orr         r1, r1, r1, lsr #16
        movs        r12, r3, lsl #31
        strcsb      r1, [r0], #1    /* can't use strh (alignment unknown) */
        strcsb      r1, [r0], #1
        strmib      r1, [r0], #1
        subs        r2, r2, r3
        ldmlsfd     sp!, {r0, r4-r7, lr}    /* return */
        bxls        lr
        /* align the destination to a cache-line */
        mov         r12, r1
        mov         lr, r1
        mov         r4, r1
        mov         r5, r1
        mov         r6, r1
        mov         r7, r1
        rsb         r3, r0, #0
        ands        r3, r3, #0x1C
        beq         3f
        cmp         r3, r2
        andhi       r3, r2, #0x1C
        sub         r2, r2, r3
        /* conditionally writes 0 to 7 words (length in r3) */
        movs        r3, r3, lsl #28
        stmcsia     r0!, {r1, lr}
        stmcsia     r0!, {r1, lr}
        stmmiia     r0!, {r1, lr}
        movs        r3, r3, lsl #2
        strcs       r1, [r0], #4
3:
        subs        r2, r2, #32
        mov         r3, r1
        bmi         2f
1:      subs        r2, r2, #32
        stmia       r0!, {r1,r3,r4,r5,r6,r7,r12,lr}
        bhs         1b
2:      add         r2, r2, #32
        /* conditionally stores 0 to 31 bytes */
        movs        r2, r2, lsl #28
        stmcsia     r0!, {r1,r3,r12,lr}
        stmmiia     r0!, {r1, lr}
        movs        r2, r2, lsl #2
        strcs       r1, [r0], #4
        strmih      r1, [r0], #2
        movs        r2, r2, lsl #2
        strcsb      r1, [r0]
        ldmfd       sp!, {r0, r4-r7, lr}
        bx          lr
END(memset)
