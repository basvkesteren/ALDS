/*
    ALDS (ARM LPC Driver Set)

    board.h:
            Board support package

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -
*/
/*!
\file
Board support package
*/
#ifndef BOARD_H
#define BOARD_H

/* Include global configuration */
#include <config.h>

/* The supported boards's */
#define BOARD_HVU_2_3   23
#define BOARD_HVU_3_2   32

#define BOARD_ENABLED

#if BOARD == BOARD_HVU_2_3
#include <board/keys.h>
#include <board/leds.h>
#include <board/buzzer.h>
#include <board/lcd_hd44780.h>
#elif BOARD == BOARD_HVU_3_2
#include <board/leds.h>
#include <board/buzzer.h>
#include <board/lcd_hd44780.h>
#else
#undef BOARD_ENABLED
#warning "Board code disabled"
#endif

#endif /* BOARD_H */
