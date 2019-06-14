/*
    ALDS (ARM LPC Driver Set)

    vic.h:
          VIC driver definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
VIC driver definitions
*/
#ifndef VIC_H
#define VIC_H

/*! Peripheral priorities. When two or more VIC interrupts occur at the same time, the first slot
    (thus the lowest number) will be handled first. There are 16 slots, numered from 0 (the first)
    to 15. Each slot can handle only one interrupt source.
    There are, indeed, not enough channels to handle all sources. Giving a priority > 15 (or PRIO_NONVECTOR)
    will result in a non-vectored IRQ (which you'll have to program yourself */

/* External interrupts */
#define PRIO_EINT0                          0
#define PRIO_EINT1                          1
#define PRIO_EINT2                          2
#define PRIO_EINT3                          PRIO_NONVECTOR
/* UART0 RX line status, Transmit
   holding register empty, RX data
   available and Character timeout
   interrupts */
#define PRIO_UART0                          3
/* UART1 RX line status, Transmit holding
   register empty, RX data available,
   Character timeout and modem status
   interrupts */
#define PRIO_UART1                          4
/* timer0 match 0-2 and compare 0-2
   interrupts */
#define PRIO_TIMER0                         5
/* timer1 match 0-3 and compare 0-3
   interrupts */
#define PRIO_TIMER1                         6
/* timer2 match 0-2 and compare 0-2
   interrupts */
#define PRIO_TIMER2                         7
/* timer3 match 0-3 and compare 0-3
   interrupts */
#define PRIO_TIMER3                         8
/* PWM match 0-6 and compare 0-3
   interrupts */
#define PRIO_PWM                            9
/* I2C state change interrupt */
#define PRIO_I2C                            10
#define PRIO_I2C0 PRIO_I2C
#define PRIO_I2C1                           11
/* SPI interrupt flag and mode fault
   interrupt */
#define PRIO_SPI                            12
/* SSP TX FiFo at least half empty,
   RX FiFo at least half full, receive
   timeout and receive overrun
   interrupts */
#define PRIO_SSP                            13
/* RTC counter increment and alarm
   interrupts */
#define PRIO_RTC                            14
/* ADC0 conversion complete interrupt */
#define PRIO_ADC                            15
#define PRIO_ADC0 PRIO_ADC
/* ADC1 conversion complete interrupt */
#define PRIO_ADC1                           PRIO_NONVECTOR
/* USB */
#define PRIO_USB                            1
/* Watchdog interrupt */
#define PRIO_WDT                            PRIO_NONVECTOR
/* PLL locked interrupt */
#define PRIO_PLL                            PRIO_NONVECTOR
/* Embedded ICE interrupts */
#define PRIO_DBGCOMMRX                      PRIO_NONVECTOR
#define PRIO_DBGCOMMTX                      PRIO_NONVECTOR
/* Brown out detect */
#define PRIO_BOD                            PRIO_NONVECTOR

/* Include global configuration */
#include <config.h>

#include <types.h>
#include "drivers/registers.h"

/* Some priority defines */
#define PRIO_MAX            0
#define PRIO_MIN            15
#define PRIO_NONVECTOR      16

/* VIC channels */
#define VIC_CH_WDT          0
#define VIC_CH_DBGCOMMRX    2
#define VIC_CH_DBGCOMMTX    3
#define VIC_CH_TIMER0       4
#define VIC_CH_TIMER1       5
#define VIC_CH_UART0        6
#define VIC_CH_UART1        7
#define VIC_CH_PWM          8
#define VIC_CH_I2C          9
#define VIC_CH_I2C0         9   // I2C0 equals I2C, for devices with more than one I2C interface
#define VIC_CH_SPI          10
#define VIC_CH_SPI0         10  // SPI0 equals SPI, for devices with more than one SPI
#define VIC_CH_SSP          11  // Also known as..
#define VIC_CH_SPI1         11  // ..the second SPI
#define VIC_CH_PLL          12
#define VIC_CH_RTC          13
#define VIC_CH_EINT0        14
#define VIC_CH_EINT1        15
#define VIC_CH_EINT2        16
#define VIC_CH_EINT3        17
#define VIC_CH_ADC          18
#define VIC_CH_ADC0         18  // ADC0 equals ADC, for devices with more than one ADC
#define VIC_CH_I2C1         19
#define VIC_CH_BOD          20
#define VIC_CH_ADC1         21
#define VIC_CH_USB          22
#define VIC_CH_TIMER2       26
#define VIC_CH_TIMER3       27

/*! To change the default IRQ handler without calling vic_init(), use the following */
#define vic_setdefaulthandler(handler)  do { \
                                            VICDefVectAddr = (unsigned int)handler; \
                                        } while(0)

/*! The following is true when the given channel is enabled, false otherwise */
#define vic_channelenabled(channel)     (VICIntEnable & (1<<channel))

void vic_init(const FUNCTION defaultIRQhandler);
void vic_setup(const unsigned char channel, const unsigned char IRQorFIQ, const unsigned char priority, const FUNCTION handler);
void vic_disablechannel(const unsigned int channel);
void vic_enablechannel(const unsigned int channel);
unsigned char vic_gethighestfreepriority(void);
unsigned char vic_getpriority(const unsigned char channel);

#endif /* VIC_H */
