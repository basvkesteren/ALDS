/*
    ALDS (ARM LPC Driver Set)

    uart_bits.h:
                UART register bit definitions

    copyright:
              Copyright (c) 2006,2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains several bit-definitions, used by the uart drivers. They are meant for
             the uart drivers only, not for global inclusion.

*/
/*!
\file
UART register bit definitions
*/
#ifndef UART_BITS_H
#define UART_BITS_H

/*******************************************************************************
  General defines (both UART0 and UART1)
*/

/* The size of the hardware FiFo; this is 16 for the LPC210x devices */
#define UART_MAXFIFOSIZE    16

/*
  Interrupt enable register (UxIER) bit definitions
*/
#define IER_RBR             (1<<0)
#define IER_THRE            (1<<1)
#define IER_RXLINESTAT      (1<<2)
/* NOTE: For UART0, bit 3 is reserved. Do not write a one to it */
#define IER_MODEMSTAT       (1<<3)
/* Bits 4 to 6 are reserverd, do not write ones to it.
   For the LPC2104/5/6 bit 7 is reserved, too */
#define IER_CTS             (1<<7)
#define IER_ABEO            (1<<8)
#define IER_ABTO            (1<<9)

/*
  Interrupt identification register (UxIIR) bit definitions
*/
#define IIR_PENDING         (1<<0)
#define IIR_ID0             (1<<1)
#define IIR_ID1             (1<<2)
#define IIR_ID2             (1<<3)
/* Bits 4 and 5 are reserved, do not write ones to it. */
#define IIR_FIFOEN0         (1<<6)
#define IIR_FIFOEN1         (1<<7)
#define IIR_ABEO            (1<<8)
#define IIR_ABTO            (1<<9)

#define IIR_ID_MASK         14
/* IIR_ID values
   Note: these are sorted by priority: IIR_ID_RLS has the
         highest priority, */
#define IIR_ID_RLS          6
#define IIR_ID_RDA          4
#define IIR_ID_CTI          12
#define IIR_ID_THRE         2
/* NOTE: the following is only meaningfull for UART1! */
#define IIR_ID_MODEM        0

/*
  FiFo control register (UxFCR) bit definitions
*/
#define FCR_FIFOEN          (1<<0)
#define FCR_RXFIFORESET     (1<<1)
#define FCR_TXFIFORESET     (1<<2)
/* Bits 3 to 5 are reserverd, do not write ones to it */
#define FCR_RXTRIGGER0      (1<<6)
#define FCR_RXTRIGGER1      (1<<7)

/* FCR_RXTRIGGER values */
// Use UART_RX_TRIGGERLEVELx defines from uart.h

/*
  Line control register (UxLCR) bit definitions
*/
#define LCR_WORDLENGTH0     (1<<0)
#define LCR_WORDLENGTH1     (1<<1)
#define LCR_STOPBIT         (1<<2)
#define LCR_PARITYEN        (1<<3)
#define LCR_PARITYSEL0      (1<<4)
#define LCR_PARITYSEL1      (1<<5)
#define LCR_BREAKCTRL       (1<<6)
#define LCR_DLAB            (1<<7)

/* LCR_WORDLENGTH values */
#define WORDLENGTH5         0
#define WORDLENGTH6         1
#define WORDLENGTH7         2
#define WORDLENGTH8         3

/* LCR_PARITYSEL values */
// Use UART_PARITY_x defines from uart.h

/*
  Line status register (UxLSR) bit definitions
*/
#define LSR_RDR             (1<<0)
#define LSR_OE              (1<<1)
#define LSR_PE              (1<<2)
#define LSR_FE              (1<<3)
#define LSR_BI              (1<<4)
#define LSR_THRE            (1<<5)
#define LSR_TEMT            (1<<6)
#define LSR_RXFE            (1<<7)

/*
  Auto baudrate generation (UxACR) bit definitions
*/
#define ACR_START           (1<<0)
#define ACR_MODE            (1<<1)
#define ACR_AUTORESTART     (1<<2)
#define ACR_DONEINT         (1<<8)
#define ACR_TIMEOUTINT      (1<<9)

/*******************************************************************************
  UART1 specific definitions
*/

/*
  Modem control register (U1MCR) bit definitions
*/
#define MCR_DTR             (1<<0)
#define MCR_RTS             (1<<1)
/* Bits 2 and 3 are reserved, do not write ones to it */
#define MCR_LOOPBACKMODE    (1<<4)
/* Bit 5 is reserved, do not write a one to it.
   FOr the LPC2104/5/6 MCU's, bits 6 and 7 are also reserved */
#define MCR_RTSEN           (1<<6)
#define MCR_CTSEN           (1<<7)

/*
  Modem status register (U1MSR) bit definitions
*/
#define MSR_DELTACTS        (1<<0)
#define MSR_DELTADSR        (1<<1)
#define MSR_LOWHIGHRI       (1<<2)
#define MSR_DELTADCD        (1<<3)
#define MSR_CTS             (1<<4)
#define MSR_DSR             (1<<5)
#define MSR_RI              (1<<6)
#define MSR_DCD             (1<<7)

#endif /* UART_REGS_H */
