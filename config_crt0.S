/*
    ALDS (ARM LPC Driver Set)

    config_crt0.s:
                  configurable bootcode is found here

    copyright:
              Copyright (c) 2006-2009 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Here you'll find macro's which might need adjusting if, for example, you want to run an
             RTOS (usually requires special IRQ and SWI handling).
             Also included is the FIQ handling, which is completely unhandled by default

*/

    .macro IRQ_enter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ What to do right after the IRQ exception happend?
@ One usually saves the current context on the stack. But, when an OS is
@ running, some more housekeeping might be needed
    stmfd   sp!, {r0-r3,r12,lr}     @ Save current context on stack
    .endm


    .macro IRQ_leave
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ What to do right after the IRQ exception happend?
@ One usually restores the context from before the exception from the stack.
@ But, when an OS is running, some more housekeeping might be needed
    ldmfd   sp!, {r0-r3,r12,lr}
    subs    pc, lr, #4              @ return from IRQ-interrupt
    .endm


    .macro SWI_unhandled
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ What to do with a SWI exception? Note that this is only called when
@ HANDLE_SWI is 0 (see config.h)
    @ Dunno how to handle SWI; let's just hang..
    #ifdef __THUMB
    ldr     r2,= __HaltSwi
    bx      r2
    #else
    b       __HaltSwi
    #endif
    .endm


    .macro FIQ_unhandled
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ What to do with a FIQ exception?
    @ Dunno how to handle FIQ; let's just hang..
    #ifdef __THUMB
    ldr     r2,= __HaltFiq
    bx      r2
    #else
    b       __HaltFiq
    #endif
    .endm
