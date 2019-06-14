/*
    ALDS (ARM LPC Driver Set)

    main.c:
           User code

    copyright:
              Copyright (c) 2006-2008 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This is where the real work is done..

*/
/*!
\file
User code
*/
#include <ALDS.h>
#include <debug.h>
#include <mem.h>
#include <iap.h>

int main(void)
/*!
  This function is the entrypoint of the C-code.
  It should never return!

  What you do here is up to you. The only thing you should do here, before doing anything else is
  call init(), which requires ALDS.h; this will initialize all the required basics.
*/
{
    int i;

    /* Initialise system */
    while(init(FALSE)!=GOOD);

    /* Dump some status-info. 'dprint' is a macro defined in drivers/debug.h, and by default points to the function print(). */
    dprint("%i MHz, PBSD %i\n\r", (PLL_MUL*FOSC)/1000000, PBSD);
    dprint("MCU flashtype %i, %i sectors\n\r", __MCU_FLASHTYPE, __MCU_FLASHSECTORS);
    i=iap_getbootloaderversion();
    dprint("Bootloader v%i.%i, ID %x\n\r", ((i>>8)&0xFF), (i&0xff), iap_getpartid());

    dprint("DATA 0x%x-0x%x\n\r", (unsigned int *)&__data_beg__, (unsigned int *)&__data_end__);
    dprint("BSS 0x%x-0x%x\n\r", (unsigned int *)&__bss_beg__, (unsigned int *)&__bss_end__);
    dprint("Stack 0x%x-0x%x\n\r", (unsigned int *)&__stack_end__, (unsigned int *)&__stack_start__);

    switch(getresetcause()) {
        case RESET_WATCHDOG:
            dprint("Watchdog");
            break;
        case RESET_POWERUP:
            dprint("Cold");
            break;
        case RESET_SOFT:
            dprint("Hot");
            break;
    }
    dprint(" reboot\n\r");

    while(1);

    return 0;
}
