/*
    ALDS (ARM LPC Driver Set)

    timer_bits.h:
                Timer register bit definitions

    copyright:
              Copyright (c) 2006,2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several bit-definitions, used by the timer drivers. They are meant for
             the timer drivers only, not for global inclusion.

*/
/*!
\file
Timer register bit definitions
*/
#ifndef TIMER_BITS_H
#define TIMER_BITS_H

/* Timer Interrupt Register Bit Definitions */
#define BIT_IR_MR0      (1 << 0)
#define BIT_IR_MR1      (1 << 1)
#define BIT_IR_MR2      (1 << 2)
#define BIT_IR_MR3      (1 << 3)
#define BIT_IR_CR0      (1 << 4)
#define BIT_IR_CR1      (1 << 5)
#define BIT_IR_CR2      (1 << 6)
#define BIT_IR_CR3      (1 << 7)

/* Timer Control Register Bit Definitions */
#define BIT_TCR_ENABLE  (1 << 0)
#define BIT_TCR_RESET   (1 << 1)

/* Timer Match Control Register Bit Definitions */
#define BIT_MCR_MR0_I   (1 << 0)                                        // Enable Interrupt when MR0 matches TC
#define BIT_MCR_MR0_R   (1 << 1)                                        // Enable Reset of TC upon MR0 match
#define BIT_MCR_MR0_S   (1 << 2)                                        // Enable Stop of TC upon MR0 match
#define BIT_MCR_MR1_I   (1 << 3)                                        // Enable Interrupt when MR1 matches TC
#define BIT_MCR_MR1_R   (1 << 4)                                        // Enable Reset of TC upon MR1 match
#define BIT_MCR_MR1_S   (1 << 5)                                        // Enable Stop of TC upon MR1 match
#define BIT_MCR_MR2_I   (1 << 6)                                        // Enable Interrupt when MR2 matches TC
#define BIT_MCR_MR2_R   (1 << 7)                                        // Enable Reset of TC upon MR2 match
#define BIT_MCR_MR2_S   (1 << 8)                                        // Enable Stop of TC upon MR2 match
#define BIT_MCR_MR3_I   (1 << 9)                                        // Enable Interrupt when MR3 matches TC
#define BIT_MCR_MR3_R   (1 << 10)                                       // Enable Reset of TC upon MR3 match
#define BIT_MCR_MR3_S   (1 << 11)                                       // Enable Stop of TC upon MR3 match

#endif /* TIMER_BITS */
