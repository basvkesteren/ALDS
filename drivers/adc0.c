/*
    ALDS (ARM LPC Driver Set)

    adc0.c:
           Analog to digital converter 0 driver

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Some testing and bugfixing done by Gaspar Pollano; thanks!
            -Channel initialisation bugfix by Tarmigan; thanks!

*/
/*!
\file
Analog to digital converter 0 driver
*/
#include <adc.h>

#ifdef ADC0_ENABLED

#include <types.h>
#include <debug.h>
#include <vic.h>
#include <types.h>
#include <err.h>
#include "registers.h"
#include "adc_bits.h"

error_t adc0_init(const unsigned char channel)
/*!
  Initialise ADC
  The 'channel' argument is a bitmask wich specifies which inputs to enable
*/
{
#if (__MCU >= LPC2101 && __MCU <= LPC2103)
    if(channel & (1<<0)) {
        /* p0.22, bits 13 and 12 to 11 */
        PINSEL1 |= (3<<12);
    }
    if(channel & (1<<1)) {
        /* p0.23, bits 15 and 14 to 11 */
        PINSEL1 |= (3<<14);
    }
    if(channel & (1<<2)) {
        /* p0.24, bits 13 and 12 to 11 */
        PINSEL1 |= (3<<12);
    }
    if(channel & (1<<3)) {
        /* p0.10, bits 21 and 20 to 11 */
        PINSEL0 |= (3<<20);
    }
    if(channel & (1<<4)) {
        /* p0.11, bits 23 and 22 to 11 */
        PINSEL0 |= (3<<22);
    }
    if(channel & (1<<5)) {
        /* p0.12, bits 25 and 24 to 11 */
        PINSEL0 |= (3<<24);
    }
    if(channel & (1<<6)) {
        /* p0.25, bits 19 and 18 to 11 */
        PINSEL0 |= (3<<18);
    }
    if(channel & (1<<7)) {
        /* p0.26, bits 21 and 20 to 11 */
        PINSEL0 |= (3<<20);
    }
#elif (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148)
    if(channel & (1<<0)) {
        #if (__MCU >= LPC2131 && __MCU <= LPC2138)
        /* p0.27, bits 22 and 23 to 01 */
        PINSEL1 &= ~(3<<22);
        PINSEL1 |= (1<<22);
        #else
        /* No such channel */
        return INVALID;
        #endif
    }
    if(channel & (1<<1)) {
        /* p0.28, bits 25 and 24 to 01 */
        PINSEL1 &= ~(3<<24);
        PINSEL1 |= (1<<24);
    }
    if(channel & (1<<2)) {
        /* p0.29, bits 27 and 26 to 01 */
        PINSEL1 &= ~(3<<26);
        PINSEL1 |= (1<<26);
    }
    if(channel & (1<<3)) {
        /* p0.30, bits 29 and 28 to 01 */
        PINSEL1 &= ~(3<<28);
        PINSEL1 |= (1<<28);
    }
    if(channel & (1<<4)) {
        /* p0.25, bits 19 and 18 to 01 */
        PINSEL1 &= ~(3<<18);
        PINSEL1 |= (1<<18);
    }
    if(channel & (1<<5)) {
        #if (__MCU >= LPC2131 && __MCU <= LPC2138)
        /* p0.26, bits 20 and 21 to 01 */
        PINSEL1 &= ~(3<<20);
        PINSEL1 |= (1<<20);
        #else
        /* No such channel */
        return INVALID;
        #endif
    }
    if(channel & (1<<6)) {
        /* p0.4, bits 9 and 8 to 11 */
        PINSEL0 |= (3<<8);
    }
    if(channel & (1<<7)) {
        /* p0.5, bits 11 and 10 to 11 */
        PINSEL0 |= (3<<10);
    }
#endif

    return GOOD;
}

unsigned short adc0_get(const unsigned char channel, const unsigned char speed)
/*!
  Start AD-converter in softwaremode, wait until the conversion is
  complete and return the result.
  Arguments:
   channel: The channel to use; use a value of 0 to 7
   speed:   The ADC clockspeed.
*/
{
    unsigned int result;

    /* First set channel, clock and enable AD converter. Clock results
       from dividing PCLK by (the clockvalue set here + 1) */
    AD0CR = ADC_PDN | (speed<<8) | ((1<<channel)&0xFF);
    /* OK, start conversion */
    AD0CR |= ADC_STARTNOW;
    /* Wait until the conversion is complete */
    do {
        result = AD0DRArray[channel];
        if (result & ADC_OVERRUN) {
            dprint("ADC0 overrun!\n\r");
        }
    } while(!(result&ADC_DONE));
    /* Conversion is complete, return result */
    return ADC_READRESULT(result);
}

void adc0_startburst(const unsigned char channel, const unsigned int trigger, const unsigned char edge, const unsigned char speed, const unsigned char clocks, const FUNCTION handler)
/*!
  Start AD-converter in burstmode. Each time a conversion is complete an
  interrupt occurs, which in turn calls the function 'handler'.
  Arguments:
   channel: The channel to scan; this is a bitmask, thus more than one channel can 'burst'.
   trigger: The trigger; use one of the ADC_STARTEDGE_ defines from adc.h
   edge:    When set, triggers on a rising edge. Only usefull when trigger is one of the ADC_STARTEDGE_MATxx values
   speed:   ADC clock
   clocks:  No. of clocks used for each burst, which sets the accuracy and no. of bits of the result. Can be anything from 11 clocks (10 bits) to 4 clocks (3 bits)
   handler: The function to call when a burst is complete

  The handler function should be able to handle all enabled channels. So, do simething like this:
  \code
    if(adc0_status() & ADC_DONE0) {
        dprint("ADC0 channel 0 done\n\r");
        if(adc0_overrun(0)) {
            dprint("Overruns did occure\n\r");
        }
        dprint("result: %i\n\r",adc0_data(0));

    }
    if(adc0_status() & ADC_DONE1) {
        dprint("ADC0 channel 1 done\n\r");
        if(adc0_overrun(1)) {
            dprint("Overruns did occure\n\r");
        }
        dprint("result: %i\n\r",adc0_data(1));
    }
  \endcode
  And so on; each enabled channel should be checked.
*/
{
    /* First set channel, clock and enable AD converter. Clock results
       from dividing PCLK by (the clockvalue set here + 1) */
    AD0CR = ((edge&0x01)<<27) | ADC_PDN | ((clocks&0x7)<<17) | ADC_BURST | (speed<<8) | (channel&0xFF);
    AD0CR |= trigger;

    /* configure VIC */
    vic_setup(VIC_CH_ADC0, IRQ, PRIO_ADC0, handler);

    /* Set interrupt enable bits */
    AD0INTEN = channel&0xFF;

    /* OK, start conversion */
    AD0CR |= ADC_BURST;
}

void adc0_stopburst(void)
/*!
  Stop AD-converter burst.
*/
{
    /* Stop burstmode and put AD converter in power-down mode */
    AD0CR &= ~(ADC_BURST | ADC_PDN);
}

#else
#warning "Driver disabled"
#endif /* ADC0_ENABLED */
