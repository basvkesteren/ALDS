/*
    ALDS (ARM LPC Driver Set)

    i2c_bits.h:
                I2C register bit definitions

    copyright:
              Copyright (c) 2006,2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several bit-definitions, used by the I2C drivers. They are meant for
             the I2C drivers only, not for global inclusion.

*/
/*!
\file
I2C register bit definitions
*/
#ifndef I2C_BITS_H
#define I2C_BITS_H

/* I2CONSET and I2CONCLR bit definitions */
#define CON_AA      (1<<2)
#define CON_SI      (1<<3)
#define CON_STO     (1<<4)
#define CON_STA     (1<<5)
#define CON_I2EN    (1<<6)

#endif /* I2C_BITS */
