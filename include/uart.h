/*
    ALDS (ARM LPC Driver Set)

    uart.h:
           UART driver, the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several definitions, used for drivers. They are meant for global inclusion.

*/
/*!
\file
UART driver, the definitions
*/
#ifndef UART_H
#define UART_H

/* Include global configuration */
#include <config.h>

#include <types.h>
#include "drivers/registers.h"

/* Parity modes */
#define UART_PARITY_ODD         0
#define UART_PARITY_EVEN        1
#define UART_PARITY_1           2
#define UART_PARITY_0           3
#define UART_PARITY_NONE        4

/* Flowcontrol modes */
#define UART_FLOW_NONE          0
#define UART_FLOW_AUTORTS       (1<<1)
#define UART_FLOW_AUTOCTS       (1<<2)
#define UART_FLOW_INTCTS        (1<<3)

/* Rx triggerlevel modes */
#define UART_RX_TRIGGERLEVEL1   0
#define UART_RX_TRIGGERLEVEL4   1
#define UART_RX_TRIGGERLEVEL8   2
#define UART_RX_TRIGGERLEVEL14  3

/* Some of the more used baudrates */
#define b1200 (unsigned short)(((CCLK/PBSD) / ((1200) * 16.0)) + 0.5)
#define b2400 (unsigned short)(((CCLK/PBSD) / ((2400) * 16.0)) + 0.5)
#define b4800 (unsigned short)(((CCLK/PBSD) / ((4800) * 16.0)) + 0.5)
#define b9600 (unsigned short)(((CCLK/PBSD) / ((9600) * 16.0)) + 0.5)
#define b14400 (unsigned short)(((CCLK/PBSD) / ((14400) * 16.0)) + 0.5)
#define b19200 (unsigned short)(((CCLK/PBSD) / ((19200) * 16.0)) + 0.5)
#define b38400 (unsigned short)(((CCLK/PBSD) / ((38400) * 16.0)) + 0.5)
#define b57600 (unsigned short)(((CCLK/PBSD) / ((57600) * 16.0)) + 0.5)
#define b115200 (unsigned short)(((CCLK/PBSD) / ((115200) * 16.0)) + 0.5)

/* UxLSR bit definitions */
#define ULSR_OVERRUN_ERR        (1<<1)
#define ULSR_PARITY_ERR         (1<<2)
#define ULSR_FRAMING_ERR        (1<<3)
#define ULSR_BREAK              (1<<4)

/*
  UART0 functions
*/
/* This driver is known to work (or is very likely to do so) on all the LPC2000 MCU's (fingers crossed :-) )*/
#define UART0_ENABLED
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
/* On these MCU's this UART has auto baudrate */
#define UART0_HASAUTOBAUDRATE
#endif

/*! This is TRUE if there's new data, FALSE otherwise */
#define uart0_hasdata()     (U0LSR & (1<<0))
/*! Read data from UART0 directly */
#define uart0_getchar()     U0RBR
/*! Clear the Rx FiFo */
#define uart0_flushRX()     (U0FCR |= (1<<1))

error_t uart0_init(const unsigned short baudrate, const FUNCTION RXhandler);
void uart0_deinit(void);
void uart0_setparameters(const signed char stopbits, const signed char parity, const signed char wordlength, const signed char breakcontrol, const signed char rxtriggerlevel);
#ifdef UART0_HASAUTOBAUDRATE
void uart0_startautobaud(const char mode, const bool autorestart, const FUNCTION handler);
#endif
void uart0_setbaudrate(const unsigned short baudrate);
void uart0_putchar(const unsigned char c);
void uart0_put(char* string);
unsigned char uart0_get(void);
void uart0_flush(void);
void uart0_setRXinterruptHandler(const FUNCTION handler);
void uart0_setRXLineinterruptHandler(FUNCTION handler);
void uart0_enableRXinterrupt(void);
void uart0_disableRXinterrupt(void);
#if UART0_INT
unsigned char uart0_interruptTXenabled(void);
void uart0_enableTXinterrupt(void);
void uart0_disableTXinterrupt(const unsigned char graceful);
#else
#define uart0_interruptTXenabled() FALSE
#define uart0_enableTXinterrupt()
#define uart0_disableTXinterrupt(graceful)
#endif

/*
  UART1 functions
*/
/* This driver is known to work (or is very likely to do so) on all the LPC2000 MCU's (fingers crossed :-) )*/
#define UART1_ENABLED
/* However, some small things differ */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148) || (defined __DOXYGEN__)
/* On these MCU's this UART has auto flowcontrol and baudrate */
#define UART1_HASAUTOFLOWCONTROL
#define UART1_HASAUTOBAUDRATE
#endif

/*! This is TRUE if there's new data, FALSE otherwise */
#define uart1_hasdata()         (U1LSR & (1<<0))
/*! Read data from UART1 directly */
#define uart1_getchar()         U1RBR
/*! Clear the Rx FiFo */
#define uart1_flushRX()         (U1FCR |= (1<<1))

error_t uart1_init(const unsigned short baudrate, const FUNCTION RXhandler);
void uart1_deinit(void);
void uart1_setparameters(const signed char stopbits, const signed char parity, const signed char wordlength, const signed char breakcontrol, const signed char rxtriggerlevel);
#ifdef UART1_HASAUTOBAUDRATE
void uart1_startautobaud(const char mode, const bool autorestart, const FUNCTION handler);
#endif
void uart1_setbaudrate(const unsigned short baudrate);
void uart1_putchar(const unsigned char c);
bool uart1_putchar_timeout(const unsigned char c);
unsigned char uart1_get(void);
void uart1_flush(void);
void uart1_setRXinterruptHandler(const FUNCTION handler);
void uart1_setRXLineinterruptHandler(const FUNCTION handler);
void uart1_enableRXinterrupt(void);
void uart1_disableRXinterrupt(void);

void uart1_flowcontrol(const char mode, const FUNCTION handler);
#define uart1_cts_asserted()    (iopin & (1<<11))

#define uart1_modemstatus()     U1MSR
/* Modem status defines */
#define UART1_MS_DELTACTS          (1<<0)
#define UART1_MS_DELTADSR          (1<<1)
#define UART1_MS_TRAILINGEDGERI    (1<<2)
#define UART1_MS_DELTADCD          (1<<3)
#define UART1_MS_CTS               (1<<4)
#define UART1_MS_DSR               (1<<5)
#define UART1_MS_RI                (1<<6)
#define UART1_MS_DCD               (1<<7)

#if UART1_INT
unsigned char uart1_interruptTXenabled(void);
void uart1_enableTXinterrupt(void);
void uart1_disableTXinterrupt(const unsigned char graceful);
#else
#define uart1_interruptTXenabled() FALSE
#define uart1_enableTXinterrupt()
#define uart1_disableTXinterrupt(graceful)
#endif

#endif /* UART_GLOBAL_H */
