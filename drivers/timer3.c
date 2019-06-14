/*
    ALDS (ARM LPC Driver Set)

    timer3.c:
             Timer3 driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Timer 3 is a 16 bit timer.
            -The timer interrupt-handler must clear the interruptflags using the following code:
              T3IR = 0xFF;
             Or, use one of the timerx_clearint() macro's (see timer.h)

*/
/*!
\file
Timer3 driver
*/
#include <timer.h>

#ifdef TIMER3_ENABLED

#include <types.h>
#include <vic.h>
#include "registers.h"
#include "timer_bits.h"

void timer3_init(const unsigned short interval, const unsigned short prescaler, const FUNCTION handler)
/*!
  Initialise timer3.
  Calling this function with 'handler' as NULL will enable the timer with the interrupt disabled.
*/
{
    /* configure timer3 */
    T3TCR = 0;                                      /* stop timer */
    T3PR  = prescaler;                              /* set prescaler */
    T3MR0 = interval;                               /* set counter */
    T3MCR = BIT_MCR_MR0_I | BIT_MCR_MR0_R;          /* Interrupt on MR0, and reset */
    T3IR = 0xFF;                                    /* clear interrupt-flags */

    if(handler != NULL) {
        /* configure VIC */
        vic_setup(VIC_CH_TIMER3, IRQ, PRIO_TIMER3, handler);
    }

    /* start timer */
    T2TCR = BIT_TCR_ENABLE;
}

#else
#warning "Driver disabled"
#endif /* TIMER3_ENABLED */
