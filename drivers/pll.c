/*
    ALDS (ARM LPC Driver Set)

    pll.h:
          PLL, MAM & VPB initialisation

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -If the PLL doesn't lock, the CPU will hang.

*/
/*!
\file
PLL, MAM & VPB initialisation
*/
#include <pll.h>
#include <irq.h>
#include "registers.h"

void pll_init(void)
/*!
  Initialise the PLL, MAM and VPB
*/
{
    __store_interrupts(&__interrupt_status);

    /* Start the PLL, but only if needed */
    #if PLL_MUL > 1
    PLLCFG  = PLLCFG_MSEL | PLLCFG_PSEL;
    /* PLL enable */
    PLLCON  = PLLCON_PLLE;
    /* PLL feed sequence */
    PLLFEED = 0x000000AA;
    PLLFEED = 0x00000055;
    /* Wait until the PLL has locked */
    while(!(PLLSTAT & PLLSTAT_LOCK));
    /* PLL enable and connect */
    PLLCON  = PLLCON_PLLE | PLLCON_PLLC;
    /* PLL feed sequence */
    PLLFEED = 0x000000AA;
    PLLFEED = 0x00000055;
    #else
    /* Disable PLL, if it's connected and/or enabled */
    if(PLLSTAT & PLLSTAT_PLLC || PLLSTAT & PLLSTAT_PLLE) {
        /* It is. Disconnect and disable it */
        PLLCFG = 0;
        /* PLL feed sequence */
        PLLFEED = 0x000000AA;
        PLLFEED = 0x00000055;
    }
    #endif

    /* Configure the Memory Accelerator Module */
    MAMCR = MAMCR_OFF;
    /* fetch cycle */
    MAMTIM = MAMTIM_CYCLES;
    /* Fully enable MAM */
    MAMCR = MAMCR_FULL;

    /* Set VPB clock */
    VPBDIV = PBSD & 0x03;

    __restore_interrupts(&__interrupt_status);
}
