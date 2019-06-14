/*
    ALDS (ARM LPC Driver Set)

    debug.h:
            Versatile debug function(s), the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This code is meant to be as efficient as possible, so it's macro's all the way.

*/
/*!
\file
Versatile debug function(s), the definitions
*/
#ifndef DEBUG_H
#define DEBUG_H

#include "../config-debug.h"

#include <config.h>

/* These define the debug levels. No need to change it, unless
   you like to mess things up.. */
#define D_NONE          0
#define D_ERROR         1
#define D_WARN          2
#define D_INFO          3
#define D_XINFO         4

#if ! DEBUG

/* Debug is disabled, so only provide an empty macro to satisfy the compiler */
#define dprint(args...)

#endif /* DEBUG */

#endif /* DEBUG_H */
