/*
    ALDS (ARM LPC Driver Set)

    pwm.c:
          PWM driver

    copyright:
              Bruce McKenney, BMC Consulting, Selkirk, NY

              Some code-layout/cleaning up by me:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
PWM driver
*/
#include <pwm.h>

#ifdef PWM_ENABLED

#include <types.h>
#include <err.h>
#include "registers.h"

/*! When this is enabled, each function accessing a PWM channel will first
    check if the requested channel is actually a valid one. If your sure
    you'r using only valid channels, this is unneeded */
#define PWM_CHECKCHANNEL    0

/* Bit positions in PWMTCR: */
#define PWMTCR_COUNTERENABLE    (1<<0)
#define PWMTCR_RESET            (1<<1)
#define PWMTCR_PWMENABLE        (1<<3)

/* Syntax macro to set the appropriate PWM MRx and latch, mainly
   so we don't forget to do both.
   It's unclear whether the latch is strictly needed for MR0. */
#define PWMMRSET(_pwm,_val)     do {                                \
                                    *MRaddr[_pwm] = (_val);         \
                                    PWMLER |= (1 << (_pwm));        \
                                } while(0)

/* This table is used by the macro above. It is a concession to the
   fact that the MRn registers aren't consecutive. */
static volatile unsigned long * const MRaddr[7] = {
    &PWMMR0, &PWMMR1, &PWMMR2, &PWMMR3, &PWMMR4, &PWMMR5, &PWMMR6
};

#if PWM_CHECKCHANNEL
static inline unsigned int pwm_check(const unsigned int chan)
/*
  Check for a valid channel.
*/
{
    switch(chan) {
        case PWM2:
            /* fall through */
        case PWM4:
            /* fall through */
        case PWM6:
            return 1;
        default:
            return 0;
    }
}
#endif

static inline unsigned int pwm_pin(const unsigned int chan)
/*
  Return the P0.x pin associated with the given channel.
  The astute reader will observe that this is 6+(chan/2) for the
  even-numbered PWMs, but it's also only used in pwm_chan_init,
  so cost doesn't matter much.
*/
{
    switch(chan) {
        case PWM2:
            /* P0.7 */
            return 7;
        case PWM4:
            /* P0.8 */
            return 8;
        case PWM6:
            /* P0.9 */
            return 9;
        default:
            /* unknown, return an invalid channel value */
            return 99;
    }
}

error_t pwm_set(const unsigned int chan, const unsigned int duty)
/*!
  Set duty output for channel
  The duty value is expected to be [0..nticks] (setting > nticks
  results in a 100% duty cycle, since no match ever happens).
  The way the prescaler is set, this value can be used without scaling.
*/
{
    #if PWM_CHECKCHANNEL
    if (!pwm_check(chan)) {
        return INVALID;
    }
    #endif
    /* store MRx */
    PWMMRSET(chan,duty);

    return GOOD;
}

error_t pwm_chan_init(const unsigned int chan, const unsigned int initduty)
/*!
  Start up a particular channel.
  initduty is the initial duty cycle setting (a la pwm_set). 0 is a
  fine value, but e.g. servos might like the center pulse instead,
  and doing it here avoids glitches.
*/
{
    unsigned int pin;

    #if PWM_CHECKCHANNEL
    if (!pwm_check(chan)) {
        return INVALID;
    }
    #endif
    pin = pwm_pin(chan);

    /* Set initial duty cycle */
    PWMMRSET(chan, initduty);

    /* Set for alternate function: PWM.
      (We know all even-numbered PWMs are in PINSEL0.) */
    PINSEL0 = (PINSEL0 & ~(3 << (2*pin)))   /* all bits off */
                       |  (2 << (2*pin));   /* value=2      */

    /* PWMSELn = 0 (single edge); PWMENAn = 1 (enable driver) */
    PWMPCR = (PWMPCR & ~(1 << chan)) | (1 << (8+chan));

    return GOOD;
}

error_t pwm_init(const unsigned int nticks,const unsigned int tickcycles)
/*!
  Starts the PWM timer
  nticks is number of divisions in the range
  tickcycles is the size of the division (pclk/CPU clocks)
  (The PWM period is thus nticks*tickcycles.)
*/
{
    /* disable timer initially */
    PWMTCR = 0;

    /* Safety first: Since these are decremented, 0 values are anomalous. */
    if (nticks == 0 || tickcycles == 0) {
        return INVALID;
    }

    /* Clear the Prescale Counter, for no real good reason. */
    PWMPC = 0;

    /* Clear the Timer Counter, so we don't miss the first match
       and wait 60+ seconds for the TC to cycle. */
    PWMTC = 0;

    /* The prescaler is set to the time (in pclk(=CPU) cycles)
       for one tick. This allows us to store the pwm_set argument
       directly in the MRx without any scaling. */
    PWMPR = tickcycles - 1;

    /* Set PWM period (MR0) as the number of ticks
       Despite the diagram in the UM which suggests the match value
       needs to be decremented, experiment indicates this usage gives
       the correct result. Go figure. */
    PWMMRSET(0, nticks);

    /* Set to clear TC on a PWMMR0 match (so it cycles as we want). */
    PWMMCR = (PWMMCR & ~(7 << (3*0)))     /* all bits off */
                       |  (2 << (3*0));     /* clear-on-match option */

    /* Two-step sequence to start the timer: First enables and resets,
       the second turns off the reset so it runs. */
    PWMTCR = PWMTCR_PWMENABLE | PWMTCR_COUNTERENABLE | PWMTCR_RESET;

    PWMTCR = PWMTCR_PWMENABLE | PWMTCR_COUNTERENABLE ;

    return GOOD;
}

#else
#warning "Driver disabled"
#endif /* PWM_ENABLED */
