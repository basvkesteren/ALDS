/*
    LPC21xx core set - Board specific functions

    buzzer.c:
             Buzzer driver, for the HvU 2.3 and 3.2 boards

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

    used I/O:
            P8 - buzzer output (BUZZER_PIN)

*/
/*!
\file
Buzzer driver, for the HvU 2.3 and 3.2 boards
*/
#include <board/buzzer.h>

#ifdef BOARD_ENABLED

#include <delay.h>
#include <io.h>

/* The I/O pin used by the buzzer */
#define BUZZER_PIN 8

void buzzer_init(void)
/*!
  Configure the used I/O pin
*/
{
    PINSEL0 &= ~(3<<(BUZZER_PIN*2));
    iodir0_out(1<<BUZZER_PIN);
}

void buzzer_put(const unsigned int frequency, unsigned int time)
/*!
  Make some noise.

  Note: 'frequency' is the high/low time of the puls used to drive the buzzer,
        'time' is the duration of this pulse-train in microseconds.
*/
{
    while(time>0) {
        delay_us(frequency);
        ioset0 = (1<<BUZZER_PIN);
        delay_us(frequency);
        ioclr0 = (1<<BUZZER_PIN);
        time--;
    }
}

#endif /* BOARD_ENABLED */
