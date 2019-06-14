/*
    ALDS (ARM LPC Driver Set)

    swi.h:
          Software interrupt handling, the definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Software interrupt handling, the definitions
*/
#ifndef SWI_H
#define SWI_H

/* Include global configuration */
#include <config.h>

/* SWI arguments */
#define SWI_INT_ENABLE      0
#define SWI_INT_DISABLE     1
#define SWI_INT_STORE       2
#define SWI_INT_RESTORE     3

/* No. of SWI commands */
#define SWI_MAX             4

/* SWI user functions */
#ifndef __ASSEMBLER__
/*! Enable interrupts (IRQ, FIQ or both) */
extern void swi__enable_interrupts(unsigned int IRQorFIQ);
/*! Disable interrupts (IRQ, FIQ or both) */
extern void swi__disable_interrupts(unsigned int IRQorFIQ);
/*! Save interruptstatus, then disable them */
extern void swi__store_interrupts(interrupt_t *status);
/*! Restore interruptstatus */
extern void swi__restore_interrupts(interrupt_t *status);
#endif

#endif /* SWI_H */
