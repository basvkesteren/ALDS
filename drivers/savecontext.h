/*
    ALDS (ARM LPC Driver Set)

    savecontext.h:
                  Low-level code to store the current context to memory

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Low-level code to store the current context to memory
*/
#ifndef SAVECONTEXT_H
#define SAVECONTEXT_H

/* Include global configuration */
#include <config.h>

/*! Points to a location in RAM used to store the crashdata */
extern unsigned int crashdata;

/*! Following struct defines the format of the saved crashdata */
typedef struct __packed {
    /* CPU registers */
    unsigned int r0,
                 r1,
                 r2,
                 r3,
                 r4,
                 r5,
                 r6,
                 r7,
                 r8,
                 r9,
                 r10,
                 r11,
                 r12,
                 lr;
    /* CPU status reguster */
    unsigned int cpsr;
    /* lr and sp as they where during the exception */
    unsigned int lr_c,
                 sp_c;
} crashdata_t;

#endif /* SAVECONTEXT_H */
