/*
    ALDS (ARM LPC Driver Set)

    adc.h:
          Analog to digital converter function definitions

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Analog to digital converter function definitions
*/
#ifndef ADC_H
#define ADC_H

/* Include global configuration */
#include <config.h>
#include "drivers/registers.h"

/* This driver is known to work (or is very likely to do so) on the following MCU's */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)

#include <types.h>

/*
  General ADC stuff
*/

/*! ADC maximum speed */
#define ADC_MAXSPEED    ((((CCLK/PBSD) / 4500000)) & 0xFF)

/*! Max. ADC value */
#define ADC_MAXVALUE    0x3FF

/* ADC trigger modes */
#define ADC_STARTNOW            (0x01<<24)
#define ADC_STARTEDGE_MAT02     (0x02<<24)
#define ADC_STARTEDGE_MAT00     (0x03<<24)
#define ADC_STARTEDGE_MAT01     (0x04<<24)
#define ADC_STARTEDGE_MAT03     (0x05<<24)
#define ADC_STARTEDGE_MAT10     (0x06<<24)
#define ADC_STARTEDGE_MAT11     (0x07<<24)

/* ADSTAT (adc*_status()) defines */
#define ADC_DONE0               (1<<0)
#define ADC_DONE1               (1<<1)
#define ADC_DONE2               (1<<2)
#define ADC_DONE3               (1<<3)
#define ADC_DONE4               (1<<4)
#define ADC_DONE5               (1<<5)
#define ADC_DONE6               (1<<6)
#define ADC_DONE7               (1<<7)
#define ADC_DONE8               (1<<8)
#define ADC_OVERRUN0            (1<<9)
#define ADC_OVERRUN1            (1<<10)
#define ADC_OVERRUN2            (1<<11)
#define ADC_OVERRUN3            (1<<12)
#define ADC_OVERRUN4            (1<<13)
#define ADC_OVERRUN5            (1<<14)
#define ADC_OVERRUN6            (1<<15)
#define ADC_OVERRUN7            (1<<16)
#define ADC_OVERRUN8            (1<<17)

/*
  ADC0 functions
*/
/* The following MCU's have atleast one ADC */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
#define ADC0_ENABLED
error_t adc0_init(const unsigned char channel);
unsigned short adc0_get(const unsigned char channel, const unsigned char speed);
void adc0_startburst(const unsigned char channel, const unsigned int trigger, const unsigned char edge, const unsigned char speed, const unsigned char clocks, const FUNCTION handler);
void adc0_stopburst(void);
/* Following macro's are here for ADC channels used in burst mode */
/*! Return status of ADC0; bits are defined in adc.h (ADC_DONE* and ADC_OVERRUN*) */
#define adc0_status()           AD0STAT
/*! Get data from given channel of ADC0 */
#define adc0_data(channel)      ((AD0DRArray[channel&0x07]&0xFFC0)>>6)
/*! When one, one or more conversions where lost due to an overrun
    NOTE: the overun bit (which is read by this macro) is cleared when the ADxDR register is read. */
#define adc0_overrun(channel)   (AD0DRArray[channel&0x07]>>30)&0x01
#endif

/*
  ADC1 functions
*/
/* The following MCU's have a second ADC */
#if (__MCU >= LPC2134 && __MCU <= LPC2138) || (__MCU >= LPC2144 && __MCU <= LPC2148) || (defined __DOXYGEN__)
#define ADC1_ENABLED
error_t adc1_init(const unsigned char channel);
unsigned short adc1_get(const unsigned char channel, const unsigned char speed);
void adc1_startburst(const unsigned char channel, const unsigned int trigger, const unsigned char edge, const unsigned char speed, const unsigned char clocks, const FUNCTION handler);
void adc1_stopburst(void);
/* Following macro's are here for ADC channels used in burst mode */
/*! Return status of ADC1; bits are defined in adc.h (ADC_DONE* and ADC_OVERRUN*) */
#define adc1_status()           AD1STAT
/*! Get data from given channel of ADC1 */
#define adc1_data(channel)      ((AD1DRArray[channel&0x07]&0xFFC0)>>6)
/*! When one, one or more conversions where lost due to an overrun
    NOTE: the overun bit (which is read by this macro) is cleared when the ADxDR register is read. */
#define adc1_overrun(channel)   (AD1DRArray[channel&0x07]>>30)&0x01
#endif

#endif
#endif /* ADC_H */
