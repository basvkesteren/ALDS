/*
    ALDS (ARM LPC Driver Set)

    power.c:
            power management

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file only contains the brown-out detection stuff; everything else is located in
             power.h, as macro's
*/
/*!
\file
Power management
*/
#include <power.h>

#ifdef BOD_ENABLED

#include <err.h>
#include <vic.h>
#include "registers.h"
#include "callback.h"

void bod_init(bool enabled,bool resetenabled, bool activeduringpowerdown)
/*!
  Initialise brown-out detect.
  Note that BOD is active by default; the interrupt (which gives you the ability to react to a BOD), is only activated after this function is
  called to enable the BOD.
  Arguments:
   enabled:                 Enable or disable the BOD
   resetenabled:            Enable or disable BOD second stage reset (note that the first stage is always enabled, this will trigger
                            the BOD interrupt)
   activeduringpowerdown:   Keep BOD active during powerdown (and enable the BOD wakeup) or not.
*/
{
    /* Set BOD bits */
    PCON |= ( ((~enabled)&0x01)<<4 | ((~resetenabled)&0x01)<<5 | ((~activeduringpowerdown)&0x01)<<2 );

    if(activeduringpowerdown) {
        /* Enable BOD wakeup */
        cpu_addwakeupsources(BODWAKE);
    }
    else {
        /* Disable BOD wakeup */
        cpu_removewakeupsources(BODWAKE);
    }

    if(enabled) {
        /* Set brownout detect interrupt */
        vic_setup(VIC_CH_BOD, IRQ, PRIO_BOD, callback_brownoutdetect);
    }
    else if(vic_channelenabled(VIC_CH_BOD)) {
        /* Disable BOD interrupt */
        vic_disablechannel(VIC_CH_BOD);
    }
}

#else
#warning "Driver disabled"
#endif
