/*
    LPC21xx core set - Board specific functions

    keys.h:
           Key-driver, for the HvU 2.3 testboard

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Key-driver, for the HvU 2.3 testboard
*/
#ifndef KEYS_H
#define KEYS_H

/* Include global configuration */
#include <config.h>
#include <board.h>

#ifdef BOARD_ENABLED
#define BOARD_HAS_KEYS

void keys_init(void);
unsigned short keys_get(void);

#endif

#endif /* KEYS_H */
