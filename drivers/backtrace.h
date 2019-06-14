/*
    ALDS (ARM LPC Driver Set)

    backtrace.h:
                Backtrace generation during runtime, function definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Backtrace generation during runtime, function definitions
*/
#ifndef BACKTRACE_H
#define BACKTRACE_H

/* Include global configuration */
#include <config.h>

/* Function prototypes */
void backtrace(unsigned int framepointer);

#endif /* BACKTRACE_H */
