/*
    ALDS (ARM LPC Driver Set)

    iap.h:
          In Application Programming

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
In Application Programming
*/
#ifndef IAP_H
#define IAP_H

/* Include global configuration */
#include <config.h>

/* This driver is known to work (or is very likely to do so) on all the LPC2000 MCU's from which we know the flashlayout(fingers crossed :-)
   See drivers/mcu_select.h for the flash variant detection. */
#if __MCU_FLASHTYPE > 0  || (defined __DOXYGEN__)

#define IAP_ENABLED

#include <types.h>

/* Function prototypes */
error_t iap_write(const unsigned int address, const unsigned int *buffer, const unsigned short length);
error_t iap_erase(const unsigned char start, const unsigned char end);
error_t iap_blankcheck(const unsigned char start, const unsigned char end);
unsigned int iap_getpartid(void);
unsigned short iap_getbootloaderversion(void);

#endif

#endif /* IAP_H */
