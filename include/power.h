/*
    ALDS (ARM LPC Driver Set)

    power.h:
            power management

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -A CPU in idle or sleep mode is rather hard to debug with JTAG; the CPU will not respond
            -The watchdog stops when the CPU enters idle or sleepmode.
            -When powermode is enabled, devices which are not enabled with power_init() will not work;
             registers from these devices cannot be written to or read from.
             Note that this only applies to code running from flash; when running from ram, all
             devices work, wheter they are enabled or not.

*/
/*!
\file
Power management
*/
#ifndef POWER_H
#define POWER_H

/* Include global configuration */
#include <config.h>
#include <types.h>

#include "drivers/registers.h"

/* definition of power-control bits */
#define PCTIM0      (1<<1)
#define PCTIM1      (1<<2)
#define PCURT0      (1<<3)
#define PCURT1      (1<<4)
#define PCPWM0      (1<<5)
#define PCI2C       (1<<7)
/* Thisone is exactly the same as the above PCI2C define. It's here for
   compatibility with __MCU's with more than one I2C interface */
#define PCI2C0      (1<<7)
#define PCSPI       (1<<8)
/* As with the I2C interface, PCSPI equals PCSPI0 */
#define PCSPI0      (1<<8)
#define PCRTC       (1<<9)
#define PCSSP       (1<<10)
/* SSP interface is sometimes reverred to as SPI1.. */
#define PCSPI1      (1<<10)
#define PCAD        (1<<12)
#define PCI2C1      (1<<19)
#define PCTIM2      (1<<22) // WARNING: according to the datasheet this should be (1<<28), but that doesn't work(for me..)
#define PCTIM3      (1<<23) //          thisone should be (1<<29)

/* Perhiperals capable of waking the cpu from power-down mode; use these with cpu_wakeupsources() */
#define EXTWAKE0    (1<<0)
#define EXTWAKE1    (1<<1)
#define EXTWAKE2    (1<<2)
#define EXTWAKE3    (1<<3)
#define USBWAKE     (1<<5)
#define BODWAKE     (1<<14)
#define RTCWAKE     (1<<15) // NOTE: this only works when the RTC runs from an external crystal
                            //       (so MCU's which don't have this option won't be able to use this at all)

/*! Configures which peripherals are active during idle-mode.
    'enabled' should consist of one or more of the above PC* defines, OR-ed together. */
#define power_init(enabled)         PCONP = enabled

/*! Enter idle-mode.
    The CPU stops, but any peripheral configured to do so (with power_init() will stay active.
    Normal operation will continue after an interrupt has occured. */
#define cpu_poweridle()             PCON |= 0x01

/*! Enter power-down mode.
    Both the CPU and all peripherals will stop, normal operation will
    continue after an external interrupt.
    Use cpu_wakeupsources() to set the perhiperals capable of waking the CPU from
    powerdown mode. */
#define cpu_powerdown()             PCON |= 0x02

/*! Set perhiperals capable of waking the CPU from power-down mode.
    'enabled' should consist of one or more of the above *WAKE defines, OR-ed together.
    \note With this macro it's quite easy to accidently disable a wakeup source. Better
          use cpu_addwakeupsources() and cpu_removewakeupsources() then. */
#define cpu_wakeupsources(enabled)  INTWAKE = enabled

/*! Add wakeup source(s), thereby not affecting the enablestate of the other sources */
#define cpu_addwakeupsources(sources)       INTWAKE |= sources

/*! Remove wakeup source(s), thereby not affecting the enablestate of the other sources */
#define cpu_removewakeupsources(sources)    INTWAKE &= ~(sources)

/* The following MCU's have a brown out detect (BOD) */
#if (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148)
#define BOD_ENABLED
#endif

#ifdef BOD_ENABLED
void bod_init(bool enabled,bool resetenabled, bool activeduringpowerdown);

#endif

#endif /* POWER_H */
