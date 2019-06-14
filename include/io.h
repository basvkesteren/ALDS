/*
    ALDS (ARM LPC Driver Set)

    io.h:
         I/O macro's

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -There are LPC2000 versions which have more than 32 I/O pins and thus more than one IOxxx
             registerset. It would have been handy to make a general purpose set of functions here which
             would deal with that internally. However, that would add overhead, which I find unneeded.
             Thus, programmer has to think of that when using these functions.

*/
/*!
\file
I/O macro's
*/
#ifndef IO_H
#define IO_H

/* Include global configuration */
#include <config.h>

#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2141 && __MCU <= LPC2148)
/* On these MCU's fast I/O is supported */
#define IO_HASFASTIO
#endif

#include "drivers/registers.h"

/* SCS Register Bit Definitions, used to select GPIO mode */
#define BIT_SCS_GPIO0M      (1<<0)  // GPIO 0 mode
#define BIT_SCS_GPIO1M      (1<<1)  // GPIO 1 mode

/*! Initialise I/O. Does nothing when fast I/O is not enabled or unsupported.
    Note that we set both banks, even if the MCU has only one. */
#ifdef IO_HASFASTIO
#if IO_USEFASTIO
#define io_init()           SCS = (BIT_SCS_GPIO0M | BIT_SCS_GPIO1M); \
                            FIOMASK0 = 0; \
                            FIOMASK1 = 0;
#else
#define io_init()           SCS = 0;
#endif
#else
#define io_init()
#endif

/*! Return the bank (PINSEL register) of the given pin */
#define io_getbank(pin)     (pin>>5)

/*! This is TRUE when fast I/O is enabled, FALSE otherwise.
    Pass the GPIO bank to test as argument, OR-ed togheter when more than one. */
#ifdef IO_HASFASTIO
#define io_usingfastio(bank) SCS & (bank))
#else
#define io_usingfastio(bank) 0
#endif

/*! For compile-time detection of fast-I/O usage */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define IO_USINGFASTIO       1
#else
#define IO_USINGFASTIO       0
#endif

/*! Set outputs, pin 0 to 31 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define ioset0              FIOSET0
#else
#define ioset0              IOSET0
#endif

/*! Clear outputs, pin 0 to 31 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define ioclr0              FIOCLR0
#else
#define ioclr0              IOCLR0
#endif

/*! Get value of pin, pin 0 to 31 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define iopin0              FIOPIN0
#else
#define iopin0              IOPIN0
#endif

/*! Set direction of pin, pin 0 to 31. 0=input, 1=output */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define iodir0              FIODIR0
#else
#define iodir0              IODIR0
#endif

/*! Set outputs, pin 32 to 63 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define ioset1              FIOSET1
#else
#define ioset1              IOSET1
#endif

/*! Clear outputs, pin 32 to 63 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define ioclr1              FIOCLR1
#else
#define ioclr1              IOCLR1
#endif

/*! Get value of pin, pin 32 to 63 */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define iopin1              FIOPIN1
#else
#define iopin1              IOPIN1
#endif

/*! Set direction of pin, pin 32 to 63. 0=input, 1=output */
#if defined IO_HASFASTIO && IO_USEFASTIO
#define iodir1              FIODIR1
#else
#define iodir1              IODIR1
#endif

/*
  Some easy to use macro's
*/

/*! Make given pins input, pin 0 to 31 */
#define iodir0_in(pins)     iodir0 &= ~(pins)
/*! Make given pins output, pin 0 to 31 */
#define iodir0_out(pins)    iodir0 |= (pins)

/*! Make given pins input, pin 32 to 63 */
#define iodir1_in(pins)     iodir1 &= ~(pins)
/*! Make given pins output, pin 32 to 63 */
#define iodir1_out(pins)    iodir1 |= (pins)

/* Compatability functions for MCU's with only one IOxxx register set
   (so <=32 I/O pins) */
#define ioset               ioset0
#define ioclr               ioclr0
#define iopin               iopin0
#define iodir               iodir0
#define iodir_in            iodir0_in
#define iodir_out           iodir0_out

#endif /* IO_H */
