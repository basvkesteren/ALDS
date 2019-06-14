/*
    ALDS (ARM LPC Driver Set)

    spi.h:
          SPI-interface driver definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
SPI-interface driver definitions
*/
#ifndef SPI_H
#define SPI_H

/* Include global configuration */
#include <config.h>

/*! When this is enabled, the global errorflag (err) is set with the SPI status. This
    is required when SPI_ERRORRECOVERY is enabled (see spi.c). */
#define SPI_SETERRORFLAG    1

/* This driver is known to work (or is very likely to do so) on all the LPC2000 MCU's (fingers crossed :-) )*/
#define SPI_ENABLED
/* However, some small things differ */
#if (__MCU >= LPC2104 && __MCU <= LPC2106) || (defined __DOXYGEN__)
/* On these MCU's the SSEL pin is reserved; it cannot be used when the SPI interface is in use */
#define SPI_SSELRESERVED
#endif
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
/* On these MCU's the transferwidth is configurable */
#define SPI_TRANSFERWIDTHSETTABLE
#endif

#include <types.h>
#include "drivers/registers.h"

/* SPI configuration bits; SPI_init() accepts one byte,
   the configuration. Use the following defines, OR-ed together */
#define SPI_SAMPLEONFIRSTCLK    0
#define SPI_SAMPLEONSECONDCLK   (1<<3)
#define SPI_ACTIVELOW           0
#define SPI_ACTIVEHIGH          (1<<4)
#define SPI_SLAVEMODE           0
#define SPI_MASTERMODE          (1<<5)
#define SPI_MSBFIRST            0
#define SPI_LSBFIRST            (1<<6)
#define SPI_INTOFF              0
#define SPI_INTON               (1<<7)


/* SPI interface error codes */
#define SPI_GOOD                0x80        /* All is good */
#define SPI_ABRT                0x08        /* Slave abort */
#define SPI_MODF                0x10        /* Mode fault */
#define SPI_ROVR                0x20        /* Read overrun */
#define SPI_WCOL                0x40        /* Write collision */

/* Macro's */

/*! SPI maximum speed */
#define SPI_MAXSPEED           8

/*! Read from the SPI-interface without any wait-code. Use only when you're
    sure the SPI-interface is ready */
#define spi_getdirect()         SPDR

/*! Write to the SPI-interface without any wait-code. Use only when you're
    sure the SPI-interface is ready */
#define spi_putdirect(d)        do { SPDR=d; } while(0)

/*! Clear SPI interrupt flags */
#define spi_clearint()          SPINT |= 0x01;

/* Function prototypes */
error_t spi_init(const unsigned char config, const unsigned char speed, const FUNCTION inthandler);
void spi_put(const unsigned char data);
unsigned char spi_putnget(const unsigned char data);

/*
  Check for invalid settings
*/
#if SPI_ERRORRECOVERY && !SPI_SETERRORFLAG
#error "SPI_SETERRORFLAG must be enabled when SPI_ERRORRECOVERY is set"
#endif

#endif /* SPI_H */
