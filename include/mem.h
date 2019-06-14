/*
    ALDS (ARM LPC Driver Set)

    mem.h:
          lpc2000 memory info

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
lpc2000 memory info
*/
#ifndef MEM_H
#define MEM_H

/* Include global configuration */
#include <config.h>

#include "drivers/mcu_select.h"     /* Contains __MCU_RAM_BASE and __MCU_ROM_BASE defines */

/* Get some values from the Linkerscript for the actual memory usage info */
extern unsigned int __ro_end__;
extern unsigned int __data_beg__;
extern unsigned int __data_end__;
extern unsigned int __data_beg_src__;
extern unsigned int __bss_beg__;
extern unsigned int __bss_end__;
extern unsigned int __stack_start__;
extern unsigned int __stack_end__;

/* And add some more userfriendly stuff */
#if RUN_FROM_RAM
#define __PROGRAMDATA_START __MCU_RAM_BASE
#else
#define __PROGRAMDATA_START __MCU_ROM_BASE
#endif
#define __PROGRAMDATA_END   ((unsigned int *)&__ro_end__)
#define __DATA_START        ((unsigned int *)&__data_beg__)
#define __DATA_END          ((unsigned int *)&__data_end__)
#define __BSS_START         ((unsigned int *)&__data_end__)
#define __BSS_END           ((unsigned int *)&__bss_end__)
#define __STACK_START       ((unsigned int *)&__stack_end__)
/* Stack RAM usage starts at __stack_end__ ..?
   Yes; it's a top-down stack, so from a stack point of view, this is the end,
   but from the RAM-usage point of view, it's the start... */
#define __FREE_RAM          ((unsigned int *)&__stack_end__) - ((unsigned int *)&__bss_end__)

#endif /* MEM_H */
