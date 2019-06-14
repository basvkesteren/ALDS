/*
    ALDS (ARM LPC Driver Set)

    watchdog.h:
               Watchdog driver definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Watchdog driver definitions
*/
#ifndef WATCHDOG_H
#define WATCHDOG_H

/* Include global configuration */
#include <config.h>

#include <types.h>
#include "drivers/registers.h"

/* Macro's */
/*! Return current value from the watchdog timer */
#define watchdog_getvalue()     WDTV

/* Function prototypes */
bool watchdog_init(const unsigned int interval);
void watchdog_clear(void);
void watchdog_trigger(void);

#endif /* WATCHDOG_H */
