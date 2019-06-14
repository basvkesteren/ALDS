/*
    ALDS (ARM LPC Driver Set)

    callback.h:
                Some system callbacks

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Some system callbacks
*/
#ifndef CALLBACK_H
#define CALLBACK_H

/* Include global configuration */
#include <config.h>

#include <types.h>

/* Function prototypes */
void callback_crash(error_t exitcode);
void callback_watchdogtimeout(void);
void callback_brownoutdetect(void);
#if ASSERT_CALLBACK
void callback_assert(const char* file, const int line);
#endif

#endif /* CALLBACK_H */
