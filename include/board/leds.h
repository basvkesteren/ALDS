/*
    LPC21xx core set - Board specific functions

    leds.h:
           LED-driver HvU testboard 2.3 and 3.2

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
LED-driver HvU testboard 2.3 and 3.2
*/
#ifndef LEDS_H
#define LEDS_H

/* Include global configuration */
#include <config.h>
#include <board.h>

#ifdef BOARD_ENABLED
#define BOARD_HAS_LEDS

/*! To led_get() or not to led_get(). If enabled, a function led_get() is
    available which can be used to read the current state of the LED's */
#define LED_GET_ENABLED

void led_init(void);
void led_put(const unsigned char led_byte);
#ifdef LED_GET_ENABLED
unsigned short led_get(void);
#endif

#endif

#endif /* LEDS_H */
