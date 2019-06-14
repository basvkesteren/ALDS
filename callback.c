/*
    ALDS (ARM LPC Driver Set)

    callback.c:
                Some system callbacks

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several functions which are called when certain 'things' happen.
             This gives you a way to handle this to your liking
*/
/*!
\file
Some system callbacks
*/
#include "drivers/callback.h"
#include "drivers/exceptions.h"
#include <debug.h>
#include <err.h>

inline void callback_crash(error_t exitcode)
/*!
  This function is called after a crash occured or if the program execution stopped, if configured to do so (See config.h, CRASHACTION)
*/
{

}

inline void callback_watchdogtimeout(void)
/*!
  This function is called when a watchdog-timeout occurs when running from RAM, if configured to do so (See config.h, WATCHDOG_TIMEOUT)
*/
{

}

void callback_brownoutdetect(void)
/*!
  This function is called when a first stage brown out detect is triggered, if this is enabled (See power.c, bod_init() and bod_enableinterrupt()
*/
{

}

#if ASSERT_CALLBACK
void callback_assert(const char* file, const int line)
/*!
  This function is called when an ASSERT() is executed with a non-true value, if configured to do so (See config.h, ASSERT_CALLBACK)
*/
{
    dprint("Assert at %s:%i\n\r",file, line);

    /* And we can halt the program like this: */
    __HaltExit(TERM);
}
#endif
