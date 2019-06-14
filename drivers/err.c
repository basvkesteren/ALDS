/*
    ALDS (ARM LPC Driver Set)

    err.c:
          Error codes and stuff..

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
Error codes and stuff..
*/
#include <err.h>
#include <types.h>

/*! The global error-buffer. */
error_t err;
