/*
    LPC21xx core set - Board specific functions

    leds.c:
           LED-driver HvU testboard 2.3 and 3.2

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

    used I/O:
            HvU board v. 2.3:
             P2 - RCK: Storage register clock
             P4 - SCK: Shift register clock
             P6 - SER: Data input
            HvU board v. 3.2:
             P16 - RCK: Storage register clock
             P4 - SCK: Shift register clock
             P6 - SER: Data input

*/
/*!
\file
LED-driver HvU testboard 2.3 and 3.2
*/
#include <board/leds.h>

#ifdef BOARD_ENABLED

#include <types.h>
#include <io.h>

#ifdef LED_GET_ENABLED
static unsigned short led_byte;
#endif

/* The I/O pin used by the LED-driver */
#if BOARD == BOARD_HVU_2_3
#define LED_RCK  2
#else
#define LED_RCK  16
#endif
#define LED_SCK  4
#define LED_SER  6

void led_init(void)
/*!
  Configure used I/O pins
*/
{
    #if BOARD == BOARD_HVU_2_3
    PINSEL0 &= (~(3<<(LED_RCK*2))) & (~(3<<(LED_SCK*2))) & (~(3<<(LED_SER*2)));
    #else
    PINSEL0 &= (~(3<<(LED_SCK*2))) & (~(3<<(LED_SER*2)));
    PINSEL1 &= (~(3<<((LED_RCK-16)*2)));
    #endif
    iodir0_out((1<<LED_RCK) | (1<<LED_SCK) | (1<<LED_SER));
    ioclr0 = (1<<LED_RCK) | (1<<LED_SCK) | (1<<LED_SER);

    #ifdef LED_GET_ENABLED
    led_byte=0;
    #endif
}

void led_put(const unsigned char state)
/*!
  Write given value to LED's
*/
{
    unsigned char i;

    #ifdef LED_GET_ENABLED
    led_byte=state;
    #endif

    /* Clock data to the shift-register */
    for(i=128; i>=1; i>>=1) {
        if(state & i) {
            ioset0 = (1<<LED_SER);
        }
        else {
            ioclr0 = (1<<LED_SER);
        }
        ioset0 = (1<<LED_SCK);
        ioclr0 = (1<<LED_SCK);
    }
    /* And place contents of shiftregister on outputs */
    ioset0 = (1<<LED_RCK);
    ioclr0 = (1<<LED_RCK);
}

#ifdef LED_GET_ENABLED
unsigned short led_get(void)
/*!
  Read current state of LED's (that is; current as far as we know; we don't really read the LED's.. )
*/
{
    return led_byte;
}
#endif

#endif /* BOARD_ENABLED */
