/*
    ALDS (ARM LPC Driver Set)

    irq.h:
          Low-level interrupt functions, the definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Low level interrupt functions, the definitions
*/
#ifndef IRQ_H
#define IRQ_H

/* Include global configuration */
#include <config.h>

#include <types.h>
#if RUNMODE_USER && HANDLE_SWI
#include <swi.h>
#endif

/*! This variable is used by __store_interrupts() and __restore_interrupts() to
    store the interrupt-status */
extern unsigned int __interrupt_status;

#if RUNMODE_USER
#if HANDLE_SWI
/* 'Normal' code is running in usermode, thus the interrupt functions
   have to go through a SWI */
#define __enable_interrupts     swi__enable_interrupts
#define __disable_interrupts    swi__disable_interrupts
#define __store_interrupts      swi__store_interrupts
#define __restore_interrupts    swi__restore_interrupts
#else
/* Erm.. We're running in usermode, but SWI-handling is disabled? */
#error "Cannot do interrupt enable/disable without SWI handling!"
#endif
#else
/*! Enable interrupts (IRQ, FIQ or both) */
extern void __enable_interrupts(unsigned int IRQorFIQ);
/*! Disable interrupts (IRQ, FIQ or both) */
extern void __disable_interrupts(unsigned int IRQorFIQ);
/*! Save interruptstatus, then disable them */
extern void __store_interrupts(interrupt_t *status);
/*! Restore interruptstatus */
extern void __restore_interrupts(interrupt_t *status);
#endif

/* And two macro's for nested interrupts. The LPC2000 MCU's by themselves don't support nested
   interrupts, but with a little software trick it's still possible (the ARM lpc book contains
   some info about this, see page 75).
   There are some things you should know;
   -First off, don't call these functions in anything else but IRQ mode.
   -They should always be called in pairs (e.g. every __enable_nested_interrupts()
    should be accompanied by a __disable_nested_interrupts())
   -Be warned that as soon as __enable_nested_interrupts() is called, *every* interrupt can be triggered,
    so that includes the interrupt being handled at that time. This means that you have to clear
    all interrupt flags before calling __enable_nested_interrupts() */

/* Enable nested interrupts:
    1. Save current status (IRQ mode)..
    2. ..on the stack, and update stackpointer
    3. Enable IRQ (FIQ stays off!!). Switch to system mode while we're at it
    4. And save System mode linkregister on stack */
#define __enable_nested_interrupts()    asm volatile ("mrs       lr, SPSR\n"       \
                                                      "stmfd     sp!, {lr}\n"      \
                                                      "msr       CPSR_c, #0x1F\n"  \
                                                      "stmfd     sp!, {lr}\n")

/* Disable nested interrupts:
    1. Restore our linkregister
    1. Disable IRQ and FIQ
    2. Get status from stack (thereby updating the stackpointer)..
    3. ..and restore it */
#define __disable_nested_interrupts()   asm volatile ("ldmfd     sp!, {lr}\n"      \
                                                      "msr       CPSR_c, #0x92\n"  \
                                                      "ldmfd     sp!, {lr}\n"      \
                                                      "msr       SPSR_cxsf, lr\n")

#endif /* IRQ_H */
