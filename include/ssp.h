/*
    ALDS (ARM LPC Driver Set)

    ssp.h:
          SSP-interface driver definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
SSP-interface driver definitions
*/
#ifndef SSP_H
#define SSP_H

/* Include global configuration */
#include <config.h>

/* This driver is known to work (or is very likely to do so) on the following MCU's */
#if (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
#define SSP_ENABLED

#include <types.h>

/* SSP configuration bits; ssp_init() accepts the argument 'config', which
   contains several configuration bits.
   Use the following defines, OR-ed together */
#define SSP_LOOPBACKMODE        (1<<0)
#define SSP_SLAVEMODE           (1<<1)
#define SSP_MASTERMODE          (0)
/* Following setting is only relevant when in slave mode */
#define SSP_SLAVEOUTPUTDISABLED (1<<2)
/* Following settings are for SPI-mode only (and ignored when used with another bustype) */
#define SSP_SAMPLEONFIRSTCLK    (0)
#define SSP_SAMPLEONSECONDCLK   (1<<3)
#define SSP_ACTIVELOW           (0)
#define SSP_ACTIVEHIGH          (1<<4)

/* Bus types */
#define SSP_SPI                 0
#define SSP_SSI                 1
#define SSP_MICROWIRE           2

/* Interface speed. clockspeed is calculated with PCLK / (CPSDVSR * SCR+1) */
#define SSP_MAXSPEED            2
#define SSP_MINPRESCALER        2   //In master-mode, this is indeed 2. However, in slave-mode this should be 12. The ssp_init() function will handle this

/* Function prototypes */
error_t ssp_init(const char type, const char datasize, const unsigned char config, const unsigned char speed, const unsigned char prescaler, const FUNCTION RXhandler);
error_t ssp_put(const unsigned short data);
unsigned short ssp_putnget(const unsigned char data);

#endif

#endif /* SSP_H */
