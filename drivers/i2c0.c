/*
    ALDS (ARM LPC Driver Set)

    i2c.c:
          I2C-interface driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This driver uses the I2C register names as used by devices with only one I2C interface

*/
/*!
\file
I2C0-interface driver
*/
#include <i2c.h>

#ifdef I2C0_ENABLED

#include <types.h>
#include <err.h>
#include "registers.h"
#include "i2c_bits.h"

/*! If the following define is set ('1'), every I2C waitloop is checked for a timeout.
    This means that if something goes wrong causing the I2C-interface to hang, the CPU
    will not wait forever.. */
#define I2C0_TIMEOUTRECOVERY 1

static error_t i2c0_laststatus;

static error_t i2c0_getstatus(void);

void i2c0_init(unsigned short speedH, unsigned short speedL)
/*!
  Initialize I2C
*/
{
    /* Configure I/O */
    PINSEL0 &= ~((3<<4) | (3<<6));
    PINSEL0 |= ((1<<4) | (1<<6));

    /* Clear flagbits */
    I2CONCLR = CON_AA | CON_SI | CON_STO | CON_STA | CON_I2EN;

    /* start interface */
    I2CONSET = CON_I2EN;

    /* Set speed */
    I2SCLH = speedH;
    I2SCLL = speedL;

    i2c0_laststatus = GOOD;
}

static error_t i2c0_getstatus(void)
/*
  Wait for a change on the I2C interface
*/
{
    #if I2C0_TIMEOUTRECOVERY
    unsigned short timeout=0;
    #endif

    while(!(I2CONSET & CON_SI)) {
        #if I2C0_TIMEOUTRECOVERY
        timeout++;
        if (timeout==0) {
            return TIMEOUT;
        }
        #endif
    }

    return I2STAT;
}

error_t i2c0_put(const unsigned char data)
/*!
  Write one byte on the I2C-bus. If succeeded (an acknowledge was received),
  0 is returned. Otherwise the error-code is returned (which will always be >0).
*/
{
    error_t returnvalue;

    if(!(I2CONSET & CON_SI)) {
        /* Device is busy */
        return BUSY;
    }

    /* Write data */
    I2DAT = data;
    /* Clear SI flag */
    I2CONCLR = CON_SI;

    /* Wait until it's done */
    while(1) {
        /* Read the status register */
        returnvalue=i2c0_getstatus();

        /* Check it's value */
        if( returnvalue == I2C_ADDRW_OK || returnvalue == I2C_ADDRR_OK || returnvalue == I2C_DATAT_OK ) {
            /* Data was successfully transmitted */
            i2c0_laststatus = GOOD;
            return GOOD;
        }
        else if (i2c0_laststatus != I2C_GOOD) {
            /* Something went wrong.. */
            i2c0_laststatus = returnvalue;
            return returnvalue;
        }
        else
        {
            /* Clear SI flag */
            I2CONCLR = CON_SI;
        }
    }
}

unsigned char i2c0_get(const unsigned char ack)
/*!
  Read one byte from the I2C-bus
*/
{
    #if I2C0_TIMEOUTRECOVERY
    unsigned short timeout=0;
    #endif

    if(!(I2CONSET & CON_SI)) {
        /* Device is busy */
        err=BUSY;
        return 0;
    }
    else {
        if(ack) {
            /* Send an ACK, slave should send next byte */
            I2CONSET = CON_AA;
        }
        else {
            /* Send a NACK, slave should stop sending */
            I2CONCLR = CON_AA;
        }
        /* Clear SI flag */
        I2CONCLR = CON_SI;

        /* Read data. First wait until it has arrived. */
        while(!(I2CONSET & CON_SI)) {
            #if I2C0_TIMEOUTRECOVERY
            timeout++;
            if (timeout==0) {
                i2c0_laststatus = TIMEOUT;
                return 0;
            }
            #endif
        }

        /* OK, it has arrived! */
        i2c0_laststatus = GOOD;
        return I2DAT;
    }
}


error_t i2c0_start(void)
/*!
  Generate startcondition
*/
{
    error_t returnvalue;

    /* Clear SI flag */
    I2CONCLR = CON_SI;

    /* Send start */
    I2CONSET = CON_STA;

    /* Wait until it's done */
    while(1) {
        /* Read the status register */
        returnvalue=i2c0_getstatus();

        /* Check it's value */
        if(returnvalue == I2C_START || returnvalue == I2C_REPSTART) {
            /* Startcondition was send, exit waitloop */
            returnvalue = GOOD;
            break;
        }
        else if (returnvalue != I2C_GOOD) {
            /* Something went wrong.. */
            break;
        }
        else
        {
            /* Clear SI flag */
            I2CONCLR = CON_SI;
        }
    }

    /* Clear start-flag */
    I2CONCLR = CON_STA;

    i2c0_laststatus = returnvalue;
    return returnvalue;
}

error_t i2c0_stop(void)
/*!
  Generate stopcondition
*/
{
    #if I2C0_TIMEOUTRECOVERY
    unsigned short timeout=0;
    #endif

    /* Send stop */
    I2CONSET = CON_STO;
    /* Clear SI flag */
    I2CONCLR = CON_SI;

    /* Wait until it's done */
    while(I2CONSET & CON_STO) {
        #if I2C0_TIMEOUTRECOVERY
        timeout++;
        if (timeout==0) {
            i2c0_laststatus = TIMEOUT;
            return TIMEOUT;
        }
        #endif
    }

    i2c0_laststatus = GOOD;
    return GOOD;
}

error_t i2c0_status(void)
{
    return i2c0_laststatus;
}

#else
#warning "Driver disabled"
#endif /* I2C0_ENABLED */
