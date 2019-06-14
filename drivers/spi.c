/*
    ALDS (ARM LPC Driver Set)

    spi.c:
          SPI driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:

*/
/*!
\file
SPI driver
*/
#include <spi.h>

#ifdef SPI_ENABLED

#include <err.h>
#include <vic.h>
#include "registers.h"

/*! If the following define is set ('1'), the SPI waitloop will check for a timeout.
    If this is disabled, this loop will never exit if the SPI-interface locks up.
    This is relatively rare, and usually caused by a wrongly configured SPI-interface
    (or a not-yet-configured interface..), or a hardware fault.
    Note that enabling this will cause the througput to go down.. If your hardware
    is reliable, there should not be any reason to enable this. */
#define SPI_TIMEOUTRECOVERY 1

/*! With the following define set ('1'), the SPI errorflags are checked every
    SPI-transfer. When an error is found, the err flag is set (see err.h)
    This will slow the interface down, but not as much as the timeout detection. */
#define SPI_ERRORRECOVERY   1

#if SPI_TIMEOUTRECOVERY
static unsigned char SPIconfig;
static unsigned char SPIspeed;
static FUNCTION SPIhandler;
#endif

static inline void spi_wait(void);

error_t spi_init(const unsigned char config, const unsigned char speed, const FUNCTION inthandler)
/*!
  Initialise SPI
      config is a bitmask with the following values:
        0:1 not used
        2
          if SPI_TRANSFERWIDTHSETTABLE
            0: 8 bits per transfer
            1: >8 per transfer (depends on bits 8:11)
          else
            not used
        3   0: data is sampled on the first clock flank of SCK
            1: data is sampled on the first clock flank of SCK
        4   0: SCK is active low
            1: SCK is active high
        5   0: slave mode
            1: master mode
        6   0: data MSB first
            1: data LSB first
        7   0: SPI interrupt disabled
            1: SPI interrupt enabled
        8-11
          if SPI_TRANSFERWIDTHSETTABLE
            When bit 2 is set, these bits set the transferwidth:
            1000 = 8 ... 1111 = 15, 0000 = 16 bits per transfer
          else
            not used

    NOTE: 'speed' is ignored when operating in slave mode

    To enable interrupts, set the interrupt enable bit in 'config' and give a
    valid functionname for 'inthandler'
*/
{
    /* Configure I/O */
    #ifdef SPI_SSELRESERVED
    /* On this MCU the SSEL pin must be configured as SSEL, and pulled high */
    PINSEL0 &= ~((3<<8) | (3<<10) | (3<<12) | (3<<14));
    PINSEL0 |= ((1<<8) | (1<<10) | (1<<12) | (1<<14));
    #else
    if(config & SPI_MASTERMODE) {
        /* On all others the SSEL pin can be used as GPIO */
        PINSEL0 &= ~((3<<8) | (3<<10) | (3<<12));
        PINSEL0 |= ((1<<8) | (1<<10) | (1<<12));
    }
    else {
        /* Obviously not when using the interface in slavemode.. */
        PINSEL0 &= ~((3<<8) | (3<<10) | (3<<12) | (3<<14));
        PINSEL0 |= ((1<<8) | (1<<10) | (1<<12) | (1<<14));
    }
    #endif
    if(config & SPI_MASTERMODE) {
        /* Set speed of the interface. Is calculated with VPB clock/SPCCR value (MHz) */
        SPCCR = speed;
    }
    /* Set SPI config */
    SPCR = config;

    #if SPI_TIMEOUTRECOVERY
    /* We save the config and speed settings in two seperate variables to be able to reconfigure
       the interface when needed. */
    SPIconfig = config;
    SPIspeed = speed;
    SPIhandler = inthandler;
    #endif

    /* Configure interrupts if requested */
    if(config & SPI_INTON && inthandler!=NULL) {
        vic_setup(VIC_CH_SPI, IRQ, PRIO_SPI, inthandler);
    }
    return GOOD;
}

static inline void spi_wait(void)
/*
  Wait until the SPI-interface is ready for new data
*/
{
    #if SPI_ERRORRECOVERY || SPI_TIMEOUTRECOVERY
    unsigned short timeout=0;
    #endif

    /* Wait until the interface is ready */
    #if SPI_SETERRORFLAG
    while(!(err = (SPSR & 0xF8))) {
    #else
    while(!(SPSR & 0xF8)) {
    #endif
        #if SPI_TIMEOUTRECOVERY
        timeout++;
        if (!timeout) {
            /* A timeout is most often caused by a crashed(?) SPI-interface. We
               restart it here to make sure this doesn't happen again */
            spi_init(SPIconfig,SPIspeed,SPIhandler);
            err = TIMEOUT;
            return;
        }
        #endif
    }

    #if SPI_ERRORRECOVERY
    if (err & SPI_MODF) {
        /* Write to the config-register to clear this error-flag */
        SPCR=SPCR;
    }
    if (err & SPI_WCOL) {
        /* Read data-register to clear this error-flag */
        timeout=SPDR;
    }
    #endif
}

void spi_put(const unsigned char data)
/*!
  Send data on the SPI-interface
*/
{
    /* OK, send data */
    SPDR = data;

    /* Now wait until the interface is ready */
    spi_wait();
}

unsigned char spi_putnget(const unsigned char data)
/*!
  A very often occuring action when using the SPI-interface is
  writing a byte, and immediatly reading another from it.

  This function does exactly that.
*/
{
    /* OK, send data */
    SPDR = data;

    /* Wait until it's done */
    spi_wait();

    return spi_getdirect();
}

#else
#warning "Driver disabled"
#endif /* SPI_ENABLED */
