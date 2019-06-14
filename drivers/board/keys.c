/*
    LPC21xx core set - Board specific functions

    keys.c:
           Key-driver, for the HvU 2.3 board

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

    used I/O:
            P4 -  CLK:  Shift register clock
            P5 -  DATA: Data
            P15 - SHLD: Shift/load

*/
/*!
\file
Key-driver, for the HvU 2.3 board
*/
#include <board/keys.h>

#ifdef BOARD_ENABLED

#include <io.h>

/* Used I/O pins */
#define KEY_CLK  4
#define KEY_DATA 5
#define KEY_SHLD 15

void keys_init(void)
/*!
  Configure used I/O pins
*/
{
    PINSEL0 &= (~(3<<KEY_CLK*2)) & (~(3<<KEY_DATA*2)) & (~(3<<KEY_SHLD*2));
    iodir0_out((1<<KEY_CLK) | (1<<KEY_SHLD));
    iodir0_in(1<<KEY_DATA);
}

unsigned short keys_get(void)
/*!
  Read current key-state
*/
{
    unsigned short i;
    unsigned short result=0;

    /* lower clock */
    ioclr0 = (1<<KEY_CLK);
    /* load data */
    ioset0 = (1<<KEY_SHLD);
    ioclr0 = (1<<KEY_SHLD);
    ioset0 = (1<<KEY_SHLD);

    /* and read data */
    for (i=0; i<16; i++) {
        result = result<<1;

        /* read bit */
        if (iopin0 & (1<<KEY_DATA)) {
            result++;
        }

        /* advance to next bit by giving a clock-pulse */
        ioset0 = (1<<KEY_CLK);
        ioclr0 = (1<<KEY_CLK);
    }

    return result;
}

#endif /* BOARD_ENABLED */

