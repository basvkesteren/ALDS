/*
    ALDS (ARM LPC Driver Set)

    delay.c:
            Simple delay functions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -These are simple and small delay functions. The do not use any special hardware and can
             be used everywhere, at any time. This ofcourse has it's price; they are not that precise..
             Especially the delay_us() function is not too good at this..
             Note: timings will never be shorter than requested; longer on the other hand is very well
                   possible (and likely).

*/
/*!
\file
Simple delay functions
*/
#include <delay.h>

void delay(volatile unsigned int time, volatile const unsigned int unit)
/*!
  Delay for some time
*/
{
    volatile unsigned int i;

    while(time>0) {
        for(i=0;i<unit;i++);
        time--;
    }
}
