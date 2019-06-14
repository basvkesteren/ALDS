/*
    ALDS (ARM LPC Driver Set)

    adc_bits.h:
                ADC register bit definitions

    copyright:
              Copyright (c) 2006,2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several bit-definitions, used by the ADC drivers. They are meant for
             the ADC drivers only, not for global inclusion.

*/
/*!
\file
ADC register bit definitions
*/
#ifndef ADC_BITS_H
#define ADC_BITS_H

/* ADCR Register Bit Definitions */
#define ADC_BURST               (1<<16)
#define ADC_BURSTCLK_SHIFT      17  // No. of clocks used in burst mode
#define ADC_BURSTCLK_MIN        4
#define ADC_BURSTCLK_MAX        11
#define ADC_PDN                 (1<<21)

// If the ADC runs in any of the *EDGE_* modes,
// the following selects the falling (set) or rising
// (clear) edge */
#define ADC_EDGE                (1<<27)
// ADDRx defines
#define ADC_READRESULT(x)       ((x&0xFFC0)>>6)
#define ADC_OVERRUN             (1<<30)
#define ADC_DONE                (1<<31)

#endif /* ADC_BITS */
