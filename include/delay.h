/*
    ALDS (ARM LPC Driver Set)

    delay.h:
            Simple delay functions, the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Simple delay functions, the definitions
*/
#ifndef DELAY_H
#define DELAY_H

/* Include global configuration */
#include <config.h>

#if __FOSC == 10000
/* The following values have been calulated and tested with an oscilloscope,
   measuring the timing of a pulse generated on an I/O pin. They are more or less
   correct for all value's at all clock-frequencies */
#define DELAY_US    PLL_MUL/2
#define DELAY_MS    PLL_MUL*570
#define DELAY_S     PLL_MUL*570000
#elif __FOSC == 12000
/* Following values have been calculated from the above __FOSC == 10000 values;
   they have not been tested with an oscilloscope yet! */
#define DELAY_US    PLL_MUL/2
#define DELAY_MS    PLL_MUL*684
#define DELAY_S     PLL_MUL*684000
#else
#error "Don't know the right delay values for the selected __FOSC value!"
#endif

#define delay_s(t)      delay(t, DELAY_S)
#define delay_us(t)     delay(t, DELAY_US)
#define delay_ms(t)     delay(t, DELAY_MS)

void delay(volatile unsigned int time, volatile const unsigned int unit);

#endif /* DELAY_H */
