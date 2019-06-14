/*
    ALDS (ARM LPC Driver Set)

    types.h:
            Global type definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
Global type definitions
*/
#ifndef TYPES_H
#define TYPES_H

/* Some (hopefully) usefull defines */
#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef NULL
#define NULL        ((void *)0)
#endif

/*! When running from ROM it can be usefull to place some functions in RAM to
    make them run faster.
    Define a function to be runned from RAM as follows:
    \code
    RAMFUNC void bla(void)
    \endcode
    Only use this for the function prototype, not the actual function! */
#ifdef __RUN_FROM_ROM
#define RAMFUNC __attribute__ ((long_call, section (".ramfunc")))
#else
#define RAMFUNC
#endif

/* Following are used for the 'IRQorFIQ' argument from vic_setup(),
   and the __enable_interrupts() call */
#define IRQ         0x80
#define FIQ         0x40

/* Reset causes (as returned by getresetcause(), init.c) */
#define RESET_POWERUP   0
#define RESET_WATCHDOG  1
#define RESET_SOFT      2

/* Timing stuff; actually quite usefull when using PWM or timer stuff */
/*! System clock */
#define PCLK                ((PLL_MUL * __FOSC * 1000) / PBSD)
/*! One uSec takes .. cycles */
#define PCLK_ONE_USEC       (PCLK/1000000L)
/*! One MSec takes .. cycles */
#define PCLK_ONE_MSEC       (PCLK_ONE_USEC*1000)
/*! One Sec takes .. cycles */
#define PCLK_ONE_SEC        (PCLK_ONE_MSEC*1000)
/*! Return amount of cycles needed for a '_us' uSec delay */
#define PCLK_USEC(_us)      ((_us)*PCLK_ONE_USEC)
/*! Return amount of cycles needed for a '_ms' MSec delay */
#define PCLK_MSEC(_ms)      ((_ms)*PCLK_ONE_MSEC)
/*! Return amount of cycles needed for a '_us' Sec delay */
#define PCLK_SEC(_s)        ((_s)*PCLK_ONE_SEC)

/*! Boolean (for the TRUE and FALSE defines) */
typedef char bool;

/*! Errors (see drivers/err.h for error values) */
typedef unsigned char error_t;

/*! To store the interrupt status, use the following */
typedef unsigned int interrupt_t;

/*! Typedef used for function address passing (that is, functions whwithit *no* arguments,
    as is used in quite a lot of places in the default driverpackage code) */
typedef void (* FUNCTION)(void);

/*! RTC-type, for use with the RTC driver */
typedef struct rtctime {
    unsigned char month, dayofmonth, dayofweek, hours, minutes, seconds;
    unsigned short year, dayofyear;
} rtc_t;

/*
  And some general stuff
*/
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned char byte;
typedef unsigned int word;

#endif /* TYPES_H */
