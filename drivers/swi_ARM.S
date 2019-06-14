/*
    ALDS (ARM LPC Driver Set)

    swi.S:
          Low-level software interrupt handling

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:

*/
#include <config.h>

@ Thumb bit; when set, we're in thumb mode
.set CPSR_T_BIT,     0x20

@ SWI arguments
.set INT_ENABLE,     0
.set INT_DISABLE,    1
.set INT_STORE,      2
.set INT_RESTORE,    3

@ No. of SWI commands
.set SWI_MAX,        4

    .arm
    .align 2

@ SWI user functions
    .global swi__enable_interrupts
#ifdef __THUMB
    .thumb
    .thumb_func
#endif
swi__enable_interrupts:
    swi     INT_ENABLE
    bx      lr

    .global swi__disable_interrupts
#ifdef __THUMB
    .thumb
    .thumb_func
#endif
swi__disable_interrupts:
    swi     INT_DISABLE
    bx      lr

    .global swi__store_interrupts
#ifdef __THUMB
    .thumb
    .thumb_func
#endif
swi__store_interrupts:
    swi     INT_STORE
    bx      lr

    .global swi__restore_interrupts
#ifdef __THUMB
    .thumb
    .thumb_func
#endif
swi__restore_interrupts:
    swi     INT_RESTORE
    bx      lr

    .arm
    .align 2

@ SWI command table
__swi_table:
.word   __enable_interrupts
.word   __disable_interrupts
.word   __store_interrupts
.word   __restore_interrupts

@@
@ Handle a SWI exception by calling the requested function from __swi_table
@ When giving an unknown SWI argument ( > SWI_MAX), this function returns
@ without calling anything.
@ NOTE: this function does not modify r0. Thus, one can call a function
@       with one argument through a SWI call by loading the argument in
@       r0 before doing the actual SWI.
@
    .global __software_interrupt
__software_interrupt:
    stmfd   sp!, {r1,r2,lr}         @ Store the stuff we're about to mess up
#ifdef __THUMB
    mrs     r1, SPSR                @ Load SPSR to..
    tst     r1, #CPSR_T_BIT         @ Check if we're in thumb mode or not
    ldrneh  r1, [lr,#-2]            @ THUMB: load SWI instruction argument (halfword)
    bicne   r1, r1, #0xff00         @        and clear top 8 bits (remove opcode)
    ldreq   r1, [lr, #-4]           @ ARM: load SWI instruction argument (word)
    biceq   r1, r1, #0xff000000     @      and clear top 8 bits (remove opcode)
#else
    ldr     r1, [lr, #-4]           @ Load SWI instruction argument (word)
    bic     r1, r1, #0xff000000     @ and clear top 8 bits (remove opcode)
#endif
    adr     r2, __swi_table         @ Get table start address
    cmp     r1, #SWI_MAX            @ Compare the given SWI argument with
                                    @ the max. SWI argument;
    mov     lr, pc
    ldrls   pc, [r2,r1,lsl #2]      @ If < SWI_MAX, load table entry address
                                    @  ( pc = __swi_table + (offset * 4 ) )
    b       __software_interrupt_exit @ Otherwise restore context and return
                                      @ without calling anything

@@
@ Return from SWI
@
    .global __software_interrupt_exit
__software_interrupt_exit:
    ldmfd   sp!, {r1,r2,pc}^       @ Note the '^': we want the PSR restored aswell

.end
