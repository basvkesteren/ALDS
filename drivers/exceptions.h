/*
    ALDS (ARM LPC Driver Set)

    exceptions.h:
                 ARM exception handlers, the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
ARM exception handlers, the definitions
*/
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/* Include global configuration */
#include <config.h>

#include <types.h>

/* CRASHACTION bitmasks */
#define C_REBOOT            (1<<0)
#define C_CALLBACK          (1<<1)
#define C_WARN              (1<<2)
#define C_CONTEXT           (1<<3)
#define C_TRACE             (1<<4)

/* Function prototypes */
void __HaltUndef(void) __noreturn;
void __HaltSwi(void) __noreturn;
void __HaltPabort(void) __noreturn;
void __HaltDabort(void) __noreturn;
void __HaltFiq(void) __noreturn;
void __HaltIrq(void) __noreturn;

void __HaltVICerr(void) __noreturn;

void __HaltExit(error_t exitcode) __noreturn;

#endif /* EXCEPTIONS_H */
