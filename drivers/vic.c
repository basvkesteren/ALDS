/*
    ALDS (ARM LPC Driver Set)

    vic.c:
          VIC driver

    copyright:
              Copyright (c) 2006,2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
VIC driver
*/
#include <vic.h>
#include <types.h>
#include <irq.h>
#include <functionname.h>
#include "exceptions.h"
#include "registers.h"

#define SLOT_ENABLED    0x20

void vic_init(const FUNCTION defaultIRQhandler)
/*!
  Initialize the VIC controller. All vectors will get an initial address, and all interrupt sources
  are disabled.
  The argument 'defaultirqhandler' specifies the handler to use for non-vectored IRQ's. Set to NULL
  if you don't care (in this case, when a non-vectored IRQ fires, program execution will stop in
  __HaltIRQ(), see drivers/exceptions.c)
*/
{
    unsigned char i;

    /* Set non-vectored interrupt-handler. Note that this is already set in the bootcode (crt0.S) to __HaltIRQ() */
    if(defaultIRQhandler!=NULL) {
        VICDefVectAddr = (unsigned int)defaultIRQhandler;
    }
    /* Fill vectors */
    for(i=0;i<16;i++) {
        VICVectAddrArray[i] = (unsigned int)__HaltVICerr;   /* Set vector address to a sensible default */
        VICVectCntlArray[i] = 0;                            /* And de-assign the channel */
    }
    /* disable all interrupt sources */
    VICIntEnClr = 0xFFFFFFFF;
    /* and write changes */
    VICVectAddr = 0xFF;
}

void vic_setup(const unsigned char channel, const unsigned char IRQorFIQ, const unsigned char priority, const FUNCTION handler)
/*!
  Install a new VIC handler.
  When installing a FIQ (thus 'IRQorFIQ' == FIQ), the 'priority' and 'handler' arguments are ignored since FIQ's cannot be vectored.
  If using a priority >= PRIO_NONVECTOR, the 'handler' argument is ignored; this IRQ will end up in the default, unvectored handler.
  Note: this is a 'dangerous' function. Using it the wrong way will most likely cause your program to crash.
*/
{
    __store_interrupts(&__interrupt_status);

    if (IRQorFIQ == IRQ) {
        VICIntSelect &=~ (1<<channel);                  /* IRQ */

        /* If this is a vectored interrupt, set it's vector.
           (Jup, only IRQ's can be vectored) */
        if(priority < PRIO_NONVECTOR) {
            VICVectAddrArray[priority] = (unsigned int)handler;  /* interrupt-handler */
            VICVectCntlArray[priority] = SLOT_ENABLED | channel; /* slot is enabled, and using the given source */
        }
    }
    else {
        VICIntSelect |=  (1<<channel);                  /* FIQ */
    }
    VICIntEnable = (1<<channel);                        /* interrupt source enabled */

    VICVectAddr = 0xFF;                                 /* write changes */

    __restore_interrupts(&__interrupt_status);
}

void vic_disablechannel(const unsigned int channel)
/*!
  Disable one single VIC-channel
*/
{
    __store_interrupts(&__interrupt_status);
    VICIntEnClr |= (1<<channel);
    VICVectAddr = 0xFF;
    __restore_interrupts(&__interrupt_status);
}

void vic_enablechannel(const unsigned int channel)
/*!
  Enable one single VIC-channel
*/
{
    __store_interrupts(&__interrupt_status);
    VICIntEnable = (1<<channel);
    VICVectAddr = 0xFF;
    __restore_interrupts(&__interrupt_status);
}

unsigned char vic_gethighestfreepriority(void)
/*!
  Return the highest free slot in the VIC, or PRIO_NONVECTOR if
  all slots are in use
*/
{
    unsigned char vector;

    /* Loop through the vector table until we find a free one */
    for(vector=PRIO_MAX;vector<=PRIO_MIN;vector++) {
        if(!(VICVectCntlArray[vector] & SLOT_ENABLED)) {
            return vector;
        }
    }
    return PRIO_NONVECTOR;
}

unsigned char vic_getpriority(const unsigned char channel)
/*!
  Return the priority of the given channel. Return PRIO_NONVECTOR when no vector is assigned to the given channel.
*/
{
    unsigned char vector;

    /* Loop through the vectors until we find the right one */
    for(vector=PRIO_MAX;vector<=PRIO_MIN;vector++) {
        if(VICVectCntlArray[vector] == (unsigned int)(0x20|channel)) {
            /* Got him! */
            return vector;
        }
    }
    return PRIO_NONVECTOR;
}
