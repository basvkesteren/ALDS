/*
    ALDS (ARM LPC Driver Set)

    timer0.c:
             Timer0 driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Timer 0 is a 32 bit timer.
            -The timer interrupt-handler must clear the interruptflags using the following code:
              T0IR = 0xFF;
             Or, use one of the timerx_clearint() macro's (see timer.h)

*/
/*!
\file
Timer0 driver
*/
#include <timer.h>
#include <types.h>
#include <vic.h>
#include "registers.h"
#include "timer_bits.h"

void timer0_init(const unsigned int interval, const unsigned int prescaler, const FUNCTION handler)
/*!
  Initialize timer0.
  Calling this function with 'handler' as NULL will enable the timer with the interrupt disabled.
*/
{
    /* configure timer0 */
    T0TCR = 0;                                      /* stop timer */
    T0PR  = prescaler;                              /* set prescaler */
    T0MR0 = interval;                               /* set counter */
    T0MCR = BIT_MCR_MR0_I | BIT_MCR_MR0_R;          /* Interrupt on MR0, and reset */
    T0IR = 0xFF;                                    /* clear interrupt-flags */

    if(handler != NULL) {
        /* configure VIC */
        vic_setup(VIC_CH_TIMER0, IRQ, PRIO_TIMER0, handler);
    }

    /* start timer */
    T0TCR = BIT_TCR_ENABLE;
}
