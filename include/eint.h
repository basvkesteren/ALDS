/*
    ALDS (ARM LPC Driver Set)

    eint.h:
           External interrupt initialisation definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -
*/
/*!
\file
External interrupt initialisation definitions
*/
#ifndef EINT_H
#define EINT_H

/* Include global configuration */
#include <config.h>

#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2104 && __MCU <= LPC2106) || \
    (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)

#define EINT_ENABLED

#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
/* On these MCU's the external interrupt pins have level and edge mode */
#define EXINT_HASMODEANDEDGE
#endif

#include <types.h>
#include "drivers/registers.h"

#ifdef EXINT_HASMODEANDEDGE

/* The mode to use */
#define EINT_LEVEL  0
#define EINT_EDGE   1

/* And the edge or level (depending on the mode) */
#define EINT_LOW        0
#define EINT_HIGH       1
#define EINT_FALLING    0
#define EINT_RISING     1

#endif

/* Different MCU's, different extpin settings. Following array is used to store all the
   required parameters. Each element of the array describes one pin, using the extpin_t
   struct defined above. */
typedef struct extpin {
    /* The eint pin */
    unsigned char no;
    /* PINSEL register */
    volatile unsigned long * const reg;
    /* Bitmask for the PINSEL register */
    unsigned char mask;
    /* Shift for bitmask */
    unsigned char shift;
} extpin_t;

extern const extpin_t extpin0[];
extern const extpin_t extpin1[];
extern const extpin_t extpin2[];
extern const extpin_t extpin3[];

/* Clear interrupts with the following macro's.
   NOTE: When using the pin in level mode, these macro's will only do something when the pin is in it's inactive state. */
#define eint0_clearint()    do { \
                                EXTINT |= (1<<0); \
                            } while(0)
#define eint1_clearint()    do { \
                                EXTINT |= (1<<1); \
                                } while(0)
#define eint2_clearint()    do { \
                                EXTINT |= (1<<2); \
                            } while(0)
#define eint3_clearint()    do { \
                                EXTINT |= (1<<3); \
                            } while(0)

/* Function prototypes */
error_t eint_init(const extpin_t pin, const char mode, const char edge, const FUNCTION handler);
error_t eint_disable(const extpin_t pin);

#endif

#endif /* EINT_H */
