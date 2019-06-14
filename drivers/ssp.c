/*
    ALDS (ARM LPC Driver Set)

    spi.c:
          SSP driver

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This driver is still somewhat incomplete; the basic stuff works, but the SSP interface
             can do more than this. Most noticably, interrupt based transfers or better FiFo handling
             are missing.
*/
/*!
\file
SSP driver
*/
#include <ssp.h>

#ifdef SSP_ENABLED

#include <err.h>
#include <vic.h>
#include "registers.h"

/* SSP register bits */
//SSPCR0 (Control register 0)
#define CPOL    (1<<6)
#define CPHA    (1<<7)
//SSPCR1 (Control register 1)
#define LBM     (1<<0)
#define SSE     (1<<1)
#define MS      (1<<2)
#define SOD     (1<<3)
//SSPSR (Status register)
#define TFE     (1<<0)
#define TNF     (1<<1)
#define RNE     (1<<2)
#define RFF     (1<<3)
#define BSY     (1<<4)
//SSPIMSC (Interrupt mask set/clear register)
#define RORIM   (1<<0)
#define RTIM    (1<<1)
#define RXIM    (1<<2)
#define TXIM    (1<<3)
//SSPRIS (Raw interrupt status register)
#define RORRIS  (1<<0)
#define RTRIS   (1<<1)
#define RXRIS   (1<<2)
#define TXRIS   (1<<3)
//SSPMIS (Masked interrupt status register)
#define RORMIS  (1<<0)
#define RTMIS   (1<<1)
#define RXMIS   (1<<2)
#define TXMIS   (1<<3)
//SSPICR (Interrupt clear register)
#define RORIC   (1<<0)
#define RTIC    (1<<1)

//static void ssp_intHandler(void);

error_t ssp_init(const char type, const char datasize, const unsigned char config, const unsigned char speed, const unsigned char prescaler, const FUNCTION RXhandler)
/*!
  Initialise SSP interface.
  'type' sets the interface type; this can be SSP_SPI, SSP_SSI or SSP_MICROWIRE
  'datasize' sets the number of bits per transfer; values from 4 up to (and including) 16 are supported
  'speed' sets the clock rate of the interface; actual bit frequency results from PCLK / (CPSDVSR * speed+1) (where CPSDVSR is the SSP clock prescaler)
  'config' sets several options; it's a bitmask of the SSP_* defines found in ssp.h
*/
{
    unsigned short dummyread;

    /* Disable interface */
    SSPCR1 = 0;

    /* Configure I/O (SCK, MISO, MOSI and SSEL) */
    PINSEL1 &= ~((3<<2) | (3<<4) | (3<<6) | (3<<8));
    PINSEL1 |= ((2<<2) | (2<<4) | (2<<6) | (2<<8));

    /* Set bustype, data size and speed */
    SSPCR0 = ((datasize-1) & 0xF) | (type << 4) | ((speed & 0xFF)<<8);
    /* And option bits */
    if(config & SSP_SAMPLEONSECONDCLK) {
        SSPCR0 |= CPOL;
    }
    if(config & SSP_ACTIVEHIGH) {
        SSPCR0 |= CPHA;
    }

    /* Some more misc. options */
    if(config & SSP_LOOPBACKMODE) {
        SSPCR1 |= LBM;
    }
    if(config & SSP_SLAVEMODE) {
        /* Set prescaler; minimal value in slave-mode is 12 */
        if(prescaler < 12) {
            SSPCPSR = 12;
        }
        else {
            SSPCPSR = prescaler;
        }
        SSPCR1 |= MS;
        if(config & SSP_SLAVEOUTPUTDISABLED) {
            SSPCR1 |= SOD;
        }
    }
    else {
        /* Set prescaler; minimal value in master-mode is 2 */
        if(prescaler < 2) {
            SSPCPSR = 2;
        }
        else {
            SSPCPSR = prescaler;
        }
    }

    /* Clear Rx FiFo */
    while(SSPSR & RNE) {
        dummyread = SSPDR;
    }

    /* Enable the interface */
    SSPCR1 |= SSE;

    /* Install a vector in the VIC */
    //vic_setup(VIC_CH_SSP, IRQ, PRIO_SSP, ssp_intHandler);

    return GOOD;
}

error_t ssp_put(const unsigned short data)
/*!
  Send data on the SSP interface.
  Note that data is written to the FiFo first, then transmitted on the actual interface.
  If the FiFo is full, BUSY is returned (and no data is written).
*/
{
    unsigned short dummyread;

    /* Wait until there's room in the FiFo */
    while((SSPSR & TNF)==0);
    SSPDR = data;
    /* Wait until data is received */
    while((SSPSR & RNE)==0);
    /* Clear just received byte from Rx FiFo */
    dummyread = SSPDR;

    return GOOD;
}

unsigned short ssp_putnget(const unsigned char data)
/*!

*/
{
  //  if(SSPSR.TNF) {
        /* FiFo is full! */
  //      err = BUSY;
  //      return 0;
   // }
    /* Wait until there's room in the FiFo */
    while((SSPSR & TNF)==0);
    SSPDR = data;
    /* Wait until data is received */
    while((SSPSR & RNE)==0);

    return SSPDR;
}

#if 0
static void ssp_intHandler()
/*
  SSP interrupt handling
*/
{
    switch(SSPMIS) {
        case RORMIS:
            /* Rx FiFo overrun */
            SSPICR |= RORIC;
            break;
        case RTMIS:
            /* Receive timeout (Rx FiFo is not empty, and now new data has
               been received for 32 clocks) */
            SSPICR |= RTIC;
            break;
        case RXMIS:
            /* Rx FiFo is atleast half full */
            break;
        case TXMIS:
            /* Tx FiFo is atleast half empty */
            break;
    }
}
#endif
#else
#warning "Driver disabled"
#endif /* SSP_ENABLED */
