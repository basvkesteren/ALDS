/*
    ALDS (ARM LPC Driver Set)

    rtc.h:
          RTC driver definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
RTC driver definitions
*/
#ifndef RTC_H
#define RTC_H

/* Include global configuration */
#include <config.h>

/* This driver is known to work (or is very likely to do so) on all the LPC2000 MCU's (fingers crossed :-) )*/
#define RTC_ENABLED
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
/* On these MCU's an external clock is available */
#define RTC_HASEXTERNALCLK
#endif

#include <types.h>
#include "drivers/registers.h"

/* Macro's to get only one value of the RTC */
#define rtc_getseconds()         SEC
#define rtc_getminutes()         MIN
#define rtc_gethours()           HOUR
#define rtc_getdayofmonth()      DOM
#define rtc_getdayofweek()       DOW
#define rtc_getdayofyear()       DOY
#define rtc_getmonth()           MONTH
#define rtc_getyear()            YEAR

/* Macro's to set only one value of the RTC */
#define rtc_putseconds(value)    SEC=value
#define rtc_putminutes(value)    MIN=value
#define rtc_puthours(value)      HOUR=value
#define rtc_putdayofmonth(value) DOM=value
#define rtc_putdayofweek(value)  DOW=value
#define rtc_putdayofyear(value)  DOY=value
#define rtc_putmonth(value)      MONTH=value
#define rtc_putyear(value)       YEAR=value

/*! Returns the RTC tick counter (32768 ticks per second) */
#define rtc_getticks()           (CTC>>1)

/* Value's used for the rtc_setcounterinterrupts() and
   rtc_setalarm() functions */
#define RTC_BSEC                 (1<<0)
#define RTC_BMIN                 (1<<1)
#define RTC_BHOUR                (1<<2)
#define RTC_BDOM                 (1<<3)
#define RTC_BDOW                 (1<<4)
#define RTC_BDOY                 (1<<5)
#define RTC_BMON                 (1<<6)
#define RTC_BYEAR                (1<<7)

/* Function prototypes */
void rtc_init(const bool reset, const bool use_external_clk, const FUNCTION counter_callback, const FUNCTION alarm_callback);
void rtc_setcounterinterrupts(const unsigned char bits);
void rtc_setalarm(const rtc_t rtc, const unsigned char maskbits);
void rtc_get(rtc_t *rtc);
void rtc_put(rtc_t rtc);

#endif /* RTC_H */
