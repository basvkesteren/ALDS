/*
    ALDS (ARM LPC Driver Set)

    watchdog.c:
               Watchdog driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -The watchdog is only partly enabled when running from JTAG; the cpu will not be reset.
            -When running code from JTAG, make sure the code in flash does not enable the watchdog;
             this can cause some 'weird' problems...
            -When in idle or sleep mode (cpu_poweridle() or cpu_powersleep()), the watchdog doesn't run.

*/
/*!
\file
Watchdog driver
*/
#include <watchdog.h>
#include <types.h>
#include <irq.h>
#include <vic.h>
#include "registers.h"
#if __RUN_FROM_RAM
#include <debug.h>
#include "callback.h"
#endif
#include <debug.h>

/* Register bit definitions */
//WDMOD (Watchdog Mode Register)
#define WDEN    (1<<0)
#define WDRESET (1<<1)
#define WDTOF   (1<<2)
#define WDINT   (1<<3)

#if __RUN_FROM_RAM
static __noreturn void watchdog_intHandler(void);
#endif

bool watchdog_init(const unsigned int interval)
/*!
  Initialise watchdog.
  'interval' is used as the value for the watchdog timer, which
  is calcated with tPCLK x interval x 4 (this gives the watchdog timeout in seconds).
  Returns TRUE when the watchdog caused the reset, FALSE otherwise.
*/
{
    bool resetcaused;

    /* Did we, or did we not cause this reset? */
    if(WDMOD & WDTOF) {
        resetcaused = TRUE;
        WDMOD &= ~WDTOF;
    }
    else {
        resetcaused = FALSE;
    }

    /* Do not enable the watchdog when interval is zero */
    if(interval == 0) {
        /* We cannot disable the watchdog from software, so no attempts are done here to do so..
           (Even better, trying to disable it with WDMOD=0 seems to hang the CPU...) */
        return resetcaused;
    }

    /* Set timeout value */
    WDTC = interval;

    #if __RUN_FROM_RAM
    /* When running from RAM, we set the watchdog interrupt */
    vic_setup(VIC_CH_WDT, IRQ, PRIO_WDT, watchdog_intHandler);
    #endif

    /* set watchdog mode */
    #ifdef __RUN_FROM_RAM
    /* Watchdog is enabled, but will not reset the CPU */
    WDMOD = WDEN;
    #else
    /* Watchdog is fully enabled (will reset the CPU) */
    WDMOD = WDEN | WDRESET;
    #endif

    /* Start watchdog by clearing it */
    WDFEED = 0xAA;
    WDFEED = 0x55;

    return resetcaused;
}

void watchdog_clear(void)
/*!
  Clear the watchdog counter
*/
{
    __store_interrupts(&__interrupt_status);
    WDFEED = 0xAA;
    WDFEED = 0x55;
    __restore_interrupts(&__interrupt_status);
}

void watchdog_trigger(void)
/*!
  Trigger the watchdog
*/
{
    WDFEED = 0xAA;
    WDFEED = 0x00;
}

#if __RUN_FROM_RAM
static __noreturn void watchdog_intHandler(void)
/*
  This is only used when running from JTAG; when running from flash the CPU is reset before this gets
  executed.
*/
{
    dprint("watchdog timeout, hard reset needed to clear watchdog\n\r");

    callback_watchdogtimeout();

    /* And hang.. */
    while(1);
}
#endif
