/*
    ALDS (ARM LPC Driver Set)

    irqstatus.c:
               Print IRQ/FIQ/VIC status on the debug console

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
IRQ/FIQ/VIC status debugging
*/
#include <irqstatus.h>
#include <debug.h>
#include <functionname.h>
#include "registers.h"

void irqstatus(void)
/*!
  This function prints a table of all interruptsources, which is quite useful
  when doing interrupt-related debugging
*/
{
    unsigned int statusregister;
    unsigned char channel;
    unsigned char vector;
    unsigned int* functionname;

    dprint("channel\ttype\tenabled\tpending\tvector\thandler\n\r");
    for(channel=0;channel<16;channel++) {
        dprint("%i\t",channel);
        /* IRQ or FIQ? */
        if(VICIntSelect&(1<<channel)) {
            dprint("FIQ\t");
        }
        else{
            dprint("IRQ\t");
        }
        /* enabled or not? */
        if(VICIntEnable&(1<<channel)) {
            dprint("yes\t");
        }
        else {
            dprint("no\t");
        }
        /* pending? */
        if(VICRawIntr&(1<<channel)) {
            dprint("yes\t");
        }
        else {
            dprint("no\t");
        }
        /* Now we figure out if this channel is vectored or not.
           If it's vectored, we figure out which function is to be called.
           Loop through the vectors searching for the right one */
        for(vector=0;vector<16;vector++) {
            if(VICVectCntlArray[vector] == (unsigned int)(0x20|channel)) {
                dprint("%i\t",vector);
                /* Get handler */
                dprint("[%h]:",VICVectAddrArray[vector]);
                functionname=__getfunctionname( VICVectAddrArray[vector] );
                if(functionname) {
                    dprint("%s\t",functionname);
                }
                else {
                    dprint("?unknown?\t");
                }
                break;
            }
        }
        /* This is a non-vectored interrupt */
        if(vector==16) {
            dprint("none\t");
            /* Function is found in VICDefVectAddr */
            dprint("[%h]:",VICDefVectAddr);
            functionname=__getfunctionname(VICDefVectAddr);
            if(functionname) {
                dprint("%s\t",functionname);
            }
            else {
                dprint("?unknown?\t");
            }
        }
        dprint("\n\r");
    }

    /* Figure out whether IRQ and FIQ are enabled or not.
       If the bit is set, the interrupt is disabled */
    dprint("\n\r");
    asm ("mrs %0, CPSR"
        : "=r"(statusregister)
        );
    dprint("IRQ ");
    if(statusregister&IRQ) {
        dprint("disabled,");
    }
    else {
        dprint("enabled,");
    }
    dprint("FIQ ");
    if(statusregister&FIQ) {
        dprint("disabled\n\r");
    }
    else {
        dprint("enabled\n\r");
    }
}
