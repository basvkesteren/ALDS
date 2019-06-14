/*
    ALDS (ARM LPC Driver Set)

    eint.c:
           External interrupt initialisation

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Each MCU has different extpin settings... If you use an MCU which isn't defined
             here, you have to add the settings to the extpin_t array's

*/
/*!
\file
External interrupt initialisation
*/
#include <eint.h>

#ifdef EINT_ENABLED

#include <types.h>
#include <err.h>
#include <vic.h>
#include "registers.h"

typedef struct extpin_vic {
    /* VIC priority */
    unsigned char priority;
    /* VIC channel */
    unsigned char channel;
} extpin_vic_t;

#if (__MCU >= LPC2101 && __MCU <= LPC2103)
const extpin_t extpin0[] = {
    {0, &PINSEL1, 1, 0}
};

const extpin_t extpin1[] = {
    {1, &PINSEL0, 1, 28}
};

const extpin_t extpin2[] = {
    {2, &PINSEL0, 1, 30}
};
#elif (__MCU >= LPC2104 && __MCU <= LPC2106)
const extpin_t extpin0[] = {
    {0, &PINSEL1, 1, 0}
};

const extpin_t extpin1[] = {
    {1, &PINSEL0, 2, 28}
};

const extpin_t extpin2[] = {
    {2, &PINSEL0, 2, 30}
};
#elif (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148)
const extpin_t extpin0[] = {
    {0, &PINSEL0, 3, 2},
    {0, &PINSEL1, 1, 0}
};

const extpin_t extpin1[] = {
    {1, &PINSEL0, 3, 6},
    {1, &PINSEL0, 2, 28}
};

const extpin_t extpin2[] = {
    {2, &PINSEL0, 3, 14},
    {2, &PINSEL0, 2, 30}
};

const extpin_t extpin3[] = {
    {3, &PINSEL0, 3, 18},
    {3, &PINSEL1, 3, 8},
    {3, &PINSEL1, 2, 28}
};
#endif

/* Map the VIC info of the eint pins to an array here */
static const extpin_vic_t extpin_vic[] = { {PRIO_EINT0,VIC_CH_EINT0},
                                           {PRIO_EINT1,VIC_CH_EINT1},
                                           {PRIO_EINT2,VIC_CH_EINT2},
                                           {PRIO_EINT3,VIC_CH_EINT3}   };

error_t eint_init(const extpin_t pin, const char mode, const char edge, const FUNCTION handler)
/*!
  Configure external interrupt pin and install a new interrupt vector for it.
  For the extpin argument, you use one of the extpinX constants, with the pin selected. Some (not all)
  MCU's can assign the extint function to different pins. So, for example, to get eint1 enabled on
  I/O pin 28 on an LPC2148, you use extpin1[1].

  On MCU's withoug mode and edge settings, the 'mode' and 'edge' arguments can be '0' (they are ignored).
*/
{
    /* If this external interrupt is already active, we need to disable it before changing it's values */
    if(vic_channelenabled(extpin_vic[(int)pin.no].channel)) {
        vic_disablechannel(extpin_vic[(int)pin.no].channel);
    }

    /* Select the EINT function for the pin */
    *pin.reg &= ~(0x3<<pin.shift);
    *pin.reg |= (pin.mask<<pin.shift);

    #ifdef EXINT_HASMODEANDEDGE
    /* Set pin mode (level or edge) */
    if(mode==EINT_LEVEL) {
        /* Clear bit; level sensitivity */
        EXTMODE &= ~(1<<pin.no);
    }
    else {
        /* Set bit; edge sensitivity */
        EXTMODE |= (1<<pin.no);
    }

    /* Set pin polarity */
    if(edge==EINT_LOW || edge==EINT_FALLING) {
        /* Clear bit; low active or falling edge */
        EXTPOLAR &= ~(1<<pin.no);
    }
    else {
        /* Set bit; high active or rising edge */
        EXTPOLAR |= (1<<pin.no);
    }
    #endif

    /* Clear interrupt flag by setting the bit; required after an initialization change prior to enabling the interrupt */
    EXTINT |= (1<<pin.no);

    /* Install a new vector in the VIC (If this vector was already active, we'll just overwrite it here) */
    vic_setup(extpin_vic[(int)pin.no].channel, IRQ, extpin_vic[(int)pin.no].priority, handler);

    /* All is good */
    return GOOD;
}

error_t eint_disable(const extpin_t pin)
/*!
  Disable interrupt functionality of given pin
*/
{
    /* If this external interrupt is active, disable it */
    if(vic_channelenabled(extpin_vic[(int)pin.no].channel)) {
        vic_disablechannel(extpin_vic[(int)pin.no].channel);
    }

    /* Select default (non eint) functionality of pin */
    *pin.reg &= ~(0x3<<pin.shift);

    return GOOD;
}

#else
#warning "Driver disabled"
#endif /* EINT_ENABLED */
