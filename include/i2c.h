/*
    ALDS (ARM LPC Driver Set)

    i2c.h:
          I2C-interface driver definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
I2C-interface driver definitions
*/
#ifndef I2C_H
#define I2C_H

/* Include global configuration */
#include <config.h>

#include <types.h>

/*
  General I2C stuff
*/

#define I2C_READ    1
#define I2C_WRITE   0

/* Interface return codes */
#define I2C_START           0x08        /* start condition send */
#define I2C_REPSTART        0x10        /* repeated start condition send */
#define I2C_ADDRW_OK        0x18        /* Slave address + write send, received ACK */
#define I2C_ADDRW_ERR       0x20        /* Slave address + write send, no ACK received */
#define I2C_DATAT_OK        0x28        /* data byte send, received ACK */
#define I2C_DATAT_ERR       0x30        /* data byte send, no ACK received */
#define I2C_ARBLOST         0x38        /* arbitration lost */
#define I2C_ADDRR_OK        0x40        /* Slave adres + read send, received ACK */
#define I2C_ADDRR_ERR       0x48        /* Slave adres + read send, no ACK received */
#define I2C_DATAR_OK        0x50        /* data byte received, ACK send */
#define I2C_DATAR_ERR       0x58        /* data byte received, no ACK send */
#define I2C_GOOD            0xF8        /* All is good */

/* Interface speed values */
#define I2C_SPEEDMAX_H          4
#define I2C_SPEEDMAX_L          4
#define I2C_SPEED400KHZ_H       ((PCLK / 400000) / 2)
#define I2C_SPEED400KHZ_L       ((PCLK / 400000) / 2)
#define I2C_SPEED100KHZ_H       ((PCLK / 100000) / 2)
#define I2C_SPEED100KHZ_L       ((PCLK / 100000) / 2)

/*
  I2C0 functions
*/
/* Like, well, you know, all LPC2000 MCU's have a I2C0 interface. And this code should work on all of them */
#define I2C0_ENABLED
void i2c0_init(const unsigned short speedH, const unsigned short speedL);
error_t i2c0_put(const unsigned char data);
unsigned char i2c0_get(const unsigned char ack);
error_t i2c0_start(void);
error_t i2c0_stop(void);
error_t i2c0_status(void);

/* I2C == I2C0 */
#define i2c_init        i2c0_init
#define i2c_put         i2c0_put
#define i2c_get         i2c0_get
#define i2c_start       i2c0_start
#define i2c_stop        i2c0_stop
#define i2c_status      i2c0_status

/*
  I2C1 functions
*/
/* The following MCU's have a second I2C interface */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
#define I2C1_ENABLED
void i2c1_init(const unsigned short speedH, const unsigned short speedL);
error_t i2c1_put(const unsigned char data);
unsigned char i2c1_get(const unsigned char ack);
error_t i2c1_start(void);
error_t i2c1_stop(void);
#endif

#endif /* I2C_H */
