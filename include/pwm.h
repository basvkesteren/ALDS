/*
    ALDS (ARM LPC Driver Set)

    pwm.h:
          PWM driver definitions

    copyright:
              Bruce McKenney, BMC Consulting, Selkirk, NY

              Some code-layour/cleaning up by me:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
PWM driver definitions
*/
#ifndef PWM_H
#define PWM_H

/* Include global configuration */
#include <config.h>

/* This driver is known to work (or is very likely to do so) on the following MCU's */
#if (__MCU >= LPC2104 && __MCU <= LPC2106) || (defined __DOXYGEN__)
#define PWM_ENABLED

#include <types.h>

/* The PWM channels */
#define PWM0    0
#define PWM1    1
#define PWM2    2
#define PWM3    3
#define PWM4    4
#define PWM5    5
#define PWM6    6

/* Function prototypes */
error_t pwm_set(const unsigned int chan, const unsigned int duty);
error_t pwm_chan_init(const unsigned int chan, const unsigned int initduty);
error_t pwm_init(const unsigned int nticks,const unsigned int tickcycles);

#endif

#endif /* PWM_H */
