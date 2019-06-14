/*
    ALDS (ARM LPC Driver Set)

    ALDS.h:
           Main ALDS include file

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file is meant to be included once, from main.c

*/
/*!
\file
Main ALDS include file
*/
#ifndef ALDS_H
#define ALDS_H

/* Include some standard stuff */
#include <config.h>
#include <types.h>
#include <err.h>
#include <gcc.h>
#include <debug.h>
#include <io.h>
#include "../project.h"

/*! The version-number */
#define ALDS_VERSION    "2.3.2"

/*! To be used by usercode to tell if it can use ALDS or not */
#define __ALDS__

/* And the ALDS starting point (from the C point of view, that is..) */
error_t init(bool enableirq);

/* Call __HaltExit() if a stack overflow is detected */
#if STACKCANARY
void stack_canary_check(void);
#else
#define stack_canary_check()
#endif

/* Get system reset cause */
char getresetcause(void);

/* Reset system */
void reset(void);

/* Clock value range tests */
#if ((FOSC < __MCU_MIN_FOSC) || (FOSC > 25000000))
#error "FOSC is out of range"
#endif

#if ((PLL_MUL > 1) && ((FOSC < __MCU_MIN_PLL_FOSC) || (FOSC > __MCU_MAX_PLL_FOSC)))
#error "FOSC is out of range of PLL"
#endif

#if ((CCLK < __MCU_MIN_FOSC) || (CCLK > __MCU_MAX_CCLK))
#error "CCLK is out of range"
#endif

#if ((FCCO < 156000000) || (FCCO > 320000000))
#error "FCCO is out of range (156MHz-320MHz), Check the PLL calculation"
#endif

#if ((PBSD != 1) && (PBSD != 2) && (PBSD != 4))
#error "Pheripheral Bus Speed Divider (PBSD) illegal value (1, 2, or 4)"
#endif

/* A little warning, can save the user a lot of time... */
#if IO_USINGFASTIO
#warning "Fast I/O enabled, make sure you use the right register-set!"
#endif

/* And one more; JTAG and ISP reading disabling */
#if CODEPROTECTION && defined __RUN_FROM_ROM
#warning "Code protection enabled, JTAG interface and all ISP read commands won't work"
#endif

#endif /* ALDS_H */
