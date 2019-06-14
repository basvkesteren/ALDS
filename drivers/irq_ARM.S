/*
    ALDS (ARM LPC Driver Set)

    irq.S:
          Low-level interrupt functions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Note: when these functions are called directly, the CPSR is modified.
             But, when called through a SWI call, SPSR is modified. This is tested
             with the RUNMODE_USER and HANDLE_SWI defines (which both must be 1).
             This means that if you set HANDLE_SWI, but clear RUNMODE_USER and then
             use one of the SWI calls to enable/disable interrupts, it wont work since
             these functions will write to the CPSR instead of the SPSR.

*/
#include <config.h>

/* The driverpackage contains code which can be used to get the functionname
   from a piece of code during runtime. It does this by storing this name in
   the function prologue. Since all the low-level IRQ code is written in
   assembler, this is not automaticly done here, so these are handcoded. */
#if CRASHACTION & C_TRACE
#define IRQ_FUNCTIONNAME    1
#else
#define IRQ_FUNCTIONNAME    0
#endif

/* If these functions are called through a SWI call, we have to modify the
   SPSR register instead of the CPSR */
#if RUNMODE_USER && HANDLE_SWI
#define STATUSREG   SPSR
#else
#define STATUSREG   CPSR
#endif

    .arm
    .align 2

    @ Defines for the interrupt bits in the CPSR register. '1' means the
    @ interrupt is disabled
    .set CPSR_I_BIT,     0x80
    .set CPSR_F_BIT,     0x40
    .set CPSR_INT_MASK,  0xFFFFFF3F

@@
@ Save interruptstatus, and then disable them
@  Bits I and F of the CPSR register are the only ones modified
#if IRQ_FUNCTIONNAME
__store_interrupts0:
    .ascii "__store_interrupts"
    .align
__store_interrupts1:
    .word 0xff000000 + (__store_interrupts1 - __store_interrupts0)
#endif

    .global __store_interrupts
__store_interrupts:
    mrs     r1, STATUSREG           @ Read interrupt status
    and     r2, r1, #CPSR_I_BIT | CPSR_F_BIT  @ We only want the IRQ and FIQ bits
    str     r2, [a1]                @ Store them in the given location (a1 == r0!!)
    orr     r1, r1, #CPSR_I_BIT | CPSR_F_BIT  @ Disable both IRQ and FIQ in copy of CPSR
    msr     STATUSREG, r1           @ Write modified status register back
    bx      lr                      @ And leave function

@@
@ Restore interruptstatus
@  Bits I and F of the CPSR register are the only ones modified
#if IRQ_FUNCTIONNAME
__restore_interrupts0:
    .ascii "__restore_interrupts"
    .align
__restore_interrupts1:
    .word 0xff000000 + (__restore_interrupts1 - __restore_interrupts0)
#endif

    .global __restore_interrupts
__restore_interrupts:
    mrs     r1, STATUSREG           @ Read interrupt status
    ldr     r0, [a1]                @ Load stored status (a1 == r0!!)
    mov     r2, #CPSR_INT_MASK
    orr     r0, r2, r0              @ Create bitmask for CPSR
    and     r1, r1, r0              @ CPSR &= bitmask
    msr     STATUSREG, r1           @ place modified status register back
    bx      lr                      @ And leave function

@@
@ Disable requested interrupts (IRQ, FIQ or both)
#if IRQ_FUNCTIONNAME
__disable_interrupts0:
    .ascii "__disable_interrupts"
    .align
__disable_interrupts1:
    .word 0xff000000 + (__disable_interrupts1 - __disable_interrupts0)
#endif

    .global __disable_interrupts
__disable_interrupts:
    mrs     r1, STATUSREG           @ Load interrupt status
    and     r0, a1, #CPSR_I_BIT | CPSR_F_BIT  @ First argument specifies which interrupttypes
                                              @ to disable. Make sure we do nothing else than
                                              @ that by masking out all other bits (NOTE: a1 == r0)
    orr     r1, r1, r0              @ Now disable requested interrupts by setting the right bits
    msr     STATUSREG, r1           @ Place modified status register back
    bx      lr                      @ And exit function

@@
@ Enable requested interrupts (IRQ, FIQ or both)
#if IRQ_FUNCTIONNAME
__enable_interrupts0:
    .ascii "__enable_interrupts"
    .align
__enable_interrupts1:
    .word 0xff000000 + (__enable_interrupts1 - __enable_interrupts0)
#endif

    .global __enable_interrupts
__enable_interrupts:
    mrs     r1, STATUSREG           @ Load interrupt status
    and     r0, a1, #CPSR_I_BIT | CPSR_F_BIT  @ First argument specifies which interrupttypes
                                              @ to enable. Make sure we do nothing else than
                                              @ that by masking out all other bits (NOTE: a1 == r0)
    bic     r1, r1, r0              @ And enable the requested interrupts by clearing the right bits
    msr     STATUSREG, r1           @ Place modified status register back
    bx      lr                      @ And exit function

    .end
