/*
    ALDS (ARM LPC Driver Set)

    assert.h:
             ASSERT implementation

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This is a rather basic assert implementation; It'll call dprint() when the expression is false.
             You might want to adapt this to your own wishes..?
*/
#ifndef ASSERT_H
#define ASSERT_H

#include <config.h>
#include <debug.h>

/*! ASSERT implementation; use this to check assumptions done in your code .
    \note While the 'normal' assert would stop (crash) the program if the given expression
          is false, this assert doesn't do that, since that's usually not the right thing
          to do for an embedded system... */
#if ASSERT_ENABLED
#if ASSERT_CALLBACK
#include "drivers/callback.h"

#define ASSERT(expr)    do { \
                            if(!expr) { \
                                callback_assert(__FILE__, __LINE__); \
                            } \
                        } while(0)
#else
#define ASSERT(expr)    do { \
                            if(!expr) { \
                                dprint("Assert at %s:%i\n\r",__FILE__, __LINE__); \
                            } \
                        } while(0)
/* An alternative which halts program execution could be done as follows:
#include "drivers/exceptions.h"
#include <err.h>
#define ASSERT(expr)    do { \
                            if(!expr) { \
                                dprint("Assert at %s:%i\n\r",__FILE__, __LINE__); \
                                __HaltExit(TERM); \
                            } \
                        } while(0)
*/
#endif
#else
#define ASSERT(expr)
#endif

#endif /* ASSERT_H */
