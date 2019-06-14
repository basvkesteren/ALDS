/*
    ALDS (ARM LPC Driver Set)

    irqstatus.h:
               Print IRQ/FIQ/VIC status on the debug console

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
IRQ/FIQ/VIC status debugging
*/
#ifndef IRQSTATUS_H
#define IRQSTATUS_H

/* Include global configuration */
#include <config.h>

/* Function prototypes */
void irqstatus(void);

#endif /* IRQSTATUS_H */
