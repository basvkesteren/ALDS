/*
    ALDS (ARM LPC Driver Set)

    pll.h:
          PLL, MAM & VPB initialisation definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
PLL, MAM & VPB initialisation, the definitions
*/
#ifndef PLL_H
#define PLL_H

/* Include global configuration */
#include <config.h>

/* PLL register bits & values */
#define PLLCFG_MSEL  ((PLL_MUL - 1) << 0)   //PLL Multiplier
#define PLLCFG_PSEL  ((PLL_DIV - 1) << 5)   //PLL Divider
#define PLLCON_PLLE  (1<<0)                 //PLL Enable
#define PLLCON_PLLC  (1<<1)                 //PLL Connect
#define PLLSTAT_PLLE (1<<8)                 //PLL Enable Status Bit
#define PLLSTAT_PLLC (1<<9)                 //PLL Connect Status Bit
#define PLLSTAT_LOCK (1<<10)                //PLL Lock Status Bit

/* MAM register bits & values */
#define MAMTIM_CYCLES (((CCLK) + 19999999) / 20000000)
#define MAMCR_OFF     0
#define MAMCR_PART    1
#define MAMCR_FULL    2

/* Function prototypes */
void pll_init(void);

#define pll_locked()    (PLLSTAT & PLLSTAT_LOCK)


#endif /* PLL_H */
