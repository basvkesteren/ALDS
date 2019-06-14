/*
    LPC21xx core set - Board specific functions

    buzzer.h:
             Buzzer driver for HvU testboard 2.3 and 3.2

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Buzzer driver for HvU testboard 2.3 and 3.2
*/
#ifndef BUZZER_H
#define BUZZER_H

/* Include global configuration */
#include <config.h>
#include <board.h>

#ifdef BOARD_ENABLED
#define BOARD_HAS_BUZZER

/*! The frequency used by beep() */
#define BEEPPULS PLL_MUL*50

/*! The duration used by beep() */
#define BEEPTIME PLL_MUL*30

/*! Simple straightforward beep */
#define beep()  buzzer_put(BEEPPULS,BEEPTIME)

void buzzer_init(void);
void buzzer_put(const unsigned int frequency, unsigned int time);

#endif

#endif /* BUZZER_H */
