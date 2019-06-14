/*
    ALDS (ARM LPC Driver Set)

    rtc.c:
          RTC driver

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -If you need only a few values of the RTC, use the rtc_get* macro's (see rtc.h) instead
             of rtc_get().

*/
/*!
\file
RTC driver
*/
#include <rtc.h>

#ifdef RTC_ENABLED

#include <types.h>
#include <vic.h>
#include "registers.h"

static void (* rtccounterintHandler)(void);
static void (* rtcalarmintHandler)(void);

static void rtc_intHandler(void);

void rtc_init(const bool reset, const bool use_external_clk, const FUNCTION counter_callback, const FUNCTION alarm_callback)
/*!
  Configure and start the RTC.
  'use_external_clk' is ignored when RTC_HASEXTERNALCLK is undefined
*/
{
    /* Clear interrupt flags */
    ILR = 0x03;     /* Clear both Counter(0) and Alarm(1) flag */
    /* And disable all interrupts */
    CIIR = 0;

    if(counter_callback != NULL || alarm_callback != NULL) {
        /* Set callback functions */
        rtccounterintHandler = counter_callback;
        rtcalarmintHandler = alarm_callback;
    }

    /* And install an interrupt vector */
    vic_setup(VIC_CH_RTC, IRQ, PRIO_RTC, rtc_intHandler);

    if(reset) {
        /* stop clock and reset counter */
        CCR = 2;

        /* reset clock */
        SEC = 0;
        MIN = 0;
        HOUR = 0;
        DOM = 1;
        DOW = 0;
        DOY = 1;
        MONTH = 1;
        YEAR = 0;
    }

    #ifdef RTC_HASEXTERNALCLK
    if(use_external_clk) {
        /* Use external oscillator */
        CCR = 0x11;
    }
    else {
    #endif
        /* integer-part of the RTC prescaler. Must be Pclk/32768 */
        PREINT = (PLL_MUL * FOSC) >> 15;

        /* rest of the RTC prescaler. Must be pclk -(PREINT+1) * 32768 */
        PREFRAC = (PLL_MUL * FOSC) - ((PREINT+1) << 15);

        /* Start clock */
        CCR = 1;
    #ifdef RTC_HASEXTERNALCLK
    }
    #endif
}

void rtc_setcounterinterrupts(const unsigned char bits)
/*!
  Set RTC Counter interrupt bits;
  Setting a bit enables it's interrupt; bits is a OR-ed value
  of the RTC_B* defines in rtc.h
  DO NOT use this function when the callback function for the counter interrupt isn't set.
*/
{
    CIIR = bits;
}

void rtc_setalarm(const rtc_t rtc, const unsigned char maskbits)
/*!
  Set RTC Alarm;
  The rtc_t struct contains the alarm time. The maskbits argument can be used to
  mask out certain values; this is a OR-ed value of the RTC_B* defines in rtc.h
  Note that to mask out a value, it's bit must be set.
  DO NOT use this function when the callback function for the alarm interrupt isn't set.
*/
{
    /* First we mask all alarm bits */
    AMR = 0xFF;
    /* Then we set the alarm */
    ALSEC = rtc.seconds;
    ALMIN = rtc.minutes;
    ALHOUR = rtc.hours;
    ALDOM = rtc.dayofmonth;
    ALDOW = rtc.dayofweek;
    ALDOY = rtc.dayofyear;
    ALMONTH = rtc.month;
    ALYEAR = rtc.year;
    /* And now enable the bits wanted */
    AMR = maskbits;
}

void rtc_get(rtc_t *rtc)
/*!
  Get all the values of the RTC. use as follows:
  \code
  rtc_t time;
  rtc_get(time);
  \endcode
  The RTC values can then be accessed from the 'time' struct.
*/
{
    unsigned int T0=CT1ME0;
    unsigned int T1=CT1ME1;
    unsigned int T2=CT1ME2;

    /* read CT1ME0 */
    rtc->dayofweek = (T0 & 0x7000000) >> 24;
    rtc->hours = (T0 & 0x1F0000) >> 16;
    rtc->minutes = (T0 & 0x3F00) >> 8;
    rtc->seconds = T0 & 0x3F;

    /* read CT1ME1 */
    rtc->year = (T1 & 0xFFF0000) >> 16;
    rtc->month = (T1 & 0xF00) >> 8;
    rtc->dayofmonth = T1 & 0x1F;

    /* read CT1ME2 */
    rtc->dayofyear = T2 & 0xFFF;
}

void rtc_put(rtc_t rtc)
/*!
  Set the time
*/
{
    SEC = rtc.seconds;
    MIN = rtc.minutes;
    HOUR = rtc.hours;
    DOM = rtc.dayofmonth;
    DOW = rtc.dayofweek;
    DOY = rtc.dayofyear;
    MONTH = rtc.month;
    YEAR = rtc.year;
}

static void rtc_intHandler(void)
/*
  RTC interrupt handling
*/
{
    /* Counter */
    if(ILR & (1<<0)) {
        /* Call callback function */
        rtccounterintHandler();
        /* Clear interrupt flag */
        ILR = (1<<0);
    }
    /* Alarm */
    if(ILR & (1<<1)) {
        /* Call callback function */
        rtcalarmintHandler();
        /* Clear interrupt flag */
        ILR = (1<<1);
    }
}

#else
#warning "Driver disabled"
#endif /* RTC_ENABLED */
