/*
    ALDS (ARM LPC Driver Set)

    init.c:
           ALDS initialization code

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This wraps the whole thing together, and should be the first thing to call from main()

*/
/*!
\file
ALDS initialization code
*/
#include <ALDS.h>
#include <err.h>
#include <irq.h>
#include <vic.h>
#include <pll.h>
#include <watchdog.h>
#include <io.h>
#include <debug.h>
#include "exceptions.h"

static char resetcause;

#if CODEPROTECTION
/* By writing a special value to a special location (...) all ISP code
   reading functions AND the complete JTAG interface are disabled. */
const unsigned int codeprotection __attribute__ ((section(".codeprotection"))) = 0x87654321;
#endif

error_t init(bool enableirq)
/*!
  Initialize the system.
  When you want to add your own code to this, make sure you add it after the
  debug_init() line; that way the debug-system will be active so any possible
  crashes can be identified.
*/
{
    extern unsigned int powerloss;

    #if STACKCANARY != 0
    unsigned int *p;

    #if STACKSIZE_FIQ > 4
    /* Last byte of FIQ stack */
    extern unsigned int __stack_start_irq__;
    p = &__stack_start_irq__-1;
    *p = STACKCANARY;
    #endif
    /* Last byte of IRQ stack */
    extern unsigned int __stack_start_supervisor__;
    p = &__stack_start_supervisor__-1;
    *p = STACKCANARY;
    #if STACKSIZE_SVC > 4
    /* Last byte of SVC stack */
    extern unsigned int __stack_start_abort__;
    p = &__stack_start_abort__-1;
    *p = STACKCANARY;
    #endif
    #if 0
    /* Last byte of UND stack */
    extern unsigned int __stack_start_undefined__;
    p = &__stack_start_undefined__-1;
    *p = STACKCANARY;
    /* Last byte of ABT stack */
    extern unsigned int __stack_start_sys__;
    p = &__stack_start_sys__-1;
    *p = STACKCANARY;
    #endif
    /* Last byte of SYS stack */
    extern unsigned int __stack_end__;
    p = &__stack_end__-1;
    *p = STACKCANARY;
    #endif

    /* Clear the error-flag */
    err = GOOD;
    /* Initialize the PLL and MAM; after this we're running at the speed set in config.h */
    pll_init();
    /* Initialize the VIC */
    vic_init(NULL);
    /* Start the debug-system. After this the basic system is online;
       add any custom initialization-code after these calls */
    #if DEBUG
    debug_init();
    #endif

    /* Configure I/O; this enables fast I/O if that's supported */
    io_init();

    /* Set-up the watchdog, with an interval of tPCLK x interval x 4.
       When this function returns TRUE, the current boot was initiated by a watchdog timeout. */
    if(watchdog_init(WATCHDOG_TIMEOUT)) {
        resetcause = RESET_WATCHDOG;
    }
    else {
        #if POWERLOSSDETECTION
        if(powerloss != 0x55AA55AA) {
            powerloss = 0x55AA55AA;
            resetcause = RESET_POWERUP;
        }
        else {
            resetcause = RESET_SOFT;
        }
        #else
        resetcause = RESET_POWERUP;
        #endif
    }

    if(enableirq) {
        /* OK, enable interrupts (IRQ only, FIQ stays off). This is really the most
           tricky part; faulty interrupt-settings will cause the system to crash
           after this point.. */
        __enable_interrupts(IRQ);
    }

    /* OK, we're good to go */
    dprint("\n\rHardware initialized\n\r");

    /* And return; we're ready to roll */
    return err;
}

#if STACKCANARY
void stack_canary_check()
{
    unsigned int *p;

#if STACKSIZE_FIQ > 4
    /* FIQ, only checked when enabled */
    extern unsigned int __stack_start_irq__;
    p = &__stack_start_irq__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
#endif
    /* IRQ */
    extern unsigned int __stack_start_supervisor__;
    p = &__stack_start_supervisor__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
#if STACKSIZE_SVC > 4
    /* SVC, only checked when enabled */
    extern unsigned int __stack_start_abort__;
    p = &__stack_start_abort__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
#endif
#if 0
    /* ABT and UND, not checked */
    extern unsigned int __stack_start_undefined__;
    p = &__stack_start_undefined__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
    extern unsigned int __stack_start_sys__;
    p = &__stack_start_sys__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
#endif
    /* SYS */
    extern unsigned int __stack_end__;
    p = &__stack_end__-1;
    if(*p != STACKCANARY) {
        __HaltExit(OVERFLOW);
    }
}
#endif

char getresetcause(void)
/*!
  Return cause of the reset. Can be RESET_WATCHDOG, RESET_POWERUP or RESET_SOFT.
  Note that RESET_SOFT only works when POWERLOSSDETECTION is set in config.h.
*/
{
    return resetcause;
}

void reset(void)
/*!
  Restart the system
*/
{
    asm volatile("ldr r2,=_start\n"
                 "bx r2\n");
}