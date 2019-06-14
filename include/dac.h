/*
    ALDS (ARM LPC Driver Set)

    dac.h:
          Digital to analog converter functions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -The DAC is rather simple, so this driver is really only some macro's...

*/
/*!
\file
Digital to analog converter functions
*/
#ifndef DAC_H
#define DAC_H

/* Include global configuration */
#include <config.h>

/* This driver is known to work (or is very likely to do so) on the following MCU's */
#if (__MCU >= LPC2132 && __MCU <= LPC2138) || (__MCU >= LPC2142 && __MCU <= LPC2148) || (defined __DOXYGEN__)
#define DAC_ENABLED

#include "drivers/registers.h"
#include <delay.h>

/*! Max. DAC value */
#define DAC_MAXVALUE                0x3FF

/*! Initialise DAC by configuring the output pin */
#define dac_init()                  do { \
                                        PINSEL1 |= (2<<18); \
                                    } while(0)

/*! Write 10 bits value to the DAC. 'fast' controls the BIAS bit:
    fast = FALSE (0) -> settling time=2.5uS, DAC can deliver 700uA
    fast = TRUE  (1) -> settling time=1uS, DAC can deliver 350uA
    The value on the output will be 'value'/1024 * Vref

    This will return after the normal settling time (which is valid with a max. load of 100pF) */
#define dac_put(value,fast)         do { \
                                        DACR = ( ((value&DAC_MAXVALUE)<<6) | ((fast&0x01)<<16) ); \
                                        delay_us(1); \
                                        if(!fast) { \
                                            delay_us(2); \
                                        } \
                                    } while(0)

/*! Like dac_put, but without the settling time delay */
#define dac__putdirect(value,fast)  do { \
                                        DACR = ( ((value&DAC_MAXVALUE)<<6) | ((fast&0x01)<<16) ); \
                                    }
#else
#warning "Driver disabled"
#endif
#endif /* DAC_H */
