/*
    ALDS (ARM LPC Driver Set)

    uart0.c:
            UART0 driver, both poll and interrupt-based versions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -UART0 is the 'default' UART interface. UART1 is identical to thisone, with the addition
             of a modem interface. See 'uart1.c' for a UART1 driver.

*/
/*!
\file
UART0 driver, both poll and interrupt-based versions
*/
#include <uart.h>

#ifdef UART0_ENABLED

#include <err.h>
#include <vic.h>
#include "registers.h"
#include "uart_bits.h"

/*
  Default configuration bits. All of these except the UART0_INT and UART0_TXBUFFERSIZE
  settings can be changed at runtime with uart0_setparameters().
*/

/*! UART sending can be done poll-based and interrupt-based. The LPC2106 hardware
    uart has a FiFo, which is used as a buffer. When this buffer is full and the driver
    is configured to use the poll-based code, the driver will block until the FiFo is empty.
    But when using interrupt-based code, the characters will be stored in a local ringbuffer.
    Do note that when working in interrupt-mode, characters will get lost if the hardware
    UART can't keep up! In poll-mode, characters should never get lost.

    When UART0_INT is enabled, the send-mode is configurable at runtime with
    uartX_enableinterrupts(). If UART0_INT is disabled, this function will still be available,
    but won't do anything (interrupt-based transfers are not supported) */
#define UART0_INT               0

/*! Configure the size of the ringbuffer. A smaller buffer increases the change of lost
    characters. */
#if UART0_INT
#define UART0_TXBUFFERSIZE      2048
#endif

/*! Configure the wordlength. Possible values are 5, 6, 7 or 8. */
#define UART0_WORDLENGTH        8

/*! Configure the parity. Possible values are:
     UART_PARITY_ODD      Odd parity
     UART_PARITY_EVEN     Even parity
     UART_PARITY_1        Parity bit is always '1'
     UART_PARITY_0        Parity bit is always '0'
     UART_PARITY_NONE     Parity bit is disabled */
#define UART0_PARITY            UART_PARITY_EVEN

/*! Configure the number of stopbits. Possible values are 1 and 2. */
#define UART0_STOPBITS          1

/*! Break transmission or not? If enabled, TX will be forced low when no transmission
    is active. */
#define UART0_BREAKTRANSENABLE  0

/*! Configure the RX FIFO trigger level. This determines how many
    characters must be written to the RX FIFO before an interrupt
    is generated. Possible values are:
     UART_RX_TRIGGERLEVEL1    1 character
     UART_RX_TRIGGERLEVEL4    4 characters
     UART_RX_TRIGGERLEVEL8    8 characters
     UART_RX_TRIGGERLEVEL14   14 characters */
#define UART0_RXTRIGGERLEVEL    UART_RX_TRIGGERLEVEL1

static void (* RXintHandler0)(void);
static void (* RxLineintHandler0)(void);
static void (* AutoBaudHandler0)(void);

static void uart0_intHandler(void);

/* Stuff for the interrupt-FiFo buffer */
#if UART0_INT
static unsigned char FiFoCount0;
static unsigned short TXpointer0=0;
static unsigned short TXbufferpointer0;
static unsigned char TXbuffer0[UART0_TXBUFFERSIZE];
static unsigned char uart0_interruptbased;
#endif

error_t uart0_init(const unsigned short baudrate, const FUNCTION RXhandler)
/*!
  Initialise UART0 with the given baudrate
*/
{
    /* Initialize Pin Select Block for Tx and Rx */
    PINSEL0 &= ~((3<<0) | (3<<2));
    PINSEL0 = ((1<<0) | (1<<2));

    /* Enable FIFO's, reset them and set the RX trigger level. */
    U0FCR = FCR_FIFOEN | FCR_RXFIFORESET | FCR_TXFIFORESET | UART0_RXTRIGGERLEVEL;
    /* Set DLAB (and clear all other bits in U0LCR while we're at it..) */
    U0LCR = LCR_DLAB;
    /* Set baudrate */
    U0DLL = (unsigned char)baudrate;
    U0DLM = baudrate>>8;
    /* Clear DLAB */
    U0LCR &= ~LCR_DLAB;

    /* Set wordlength, .. */
    U0LCR |= UART0_WORDLENGTH-5;
    /* .. number of stopbits, .. */
    #if UART0_STOPBITS == 2
    U0LCR |= LCR_STOPBIT;
    #endif
    /* .. break control .. */
    #if UART0_BREAKTRANSENABLE
    U0LCR |= LCR_BREAKCTRL;
    #endif
    /* .. and parity checking */
    #if UART0_PARITY != UART_PARITY_NONE
    U0LCR |= LCR_PARITYEN & UART0_PARITY;
    #endif

    /* Clear interrupt bits */
    U0IER = 0x00;

    /* Install a vector in the VIC */
    vic_setup(VIC_CH_UART0, IRQ, PRIO_UART0, uart0_intHandler);

    U0IER = 0;

    #if UART0_INT
    uart0_enableTXinterrupt();
    #endif

    /* Set RX interrupt */
    if(RXhandler!=NULL) {
        uart0_setRXinterruptHandler(RXhandler);
        uart0_enableRXinterrupt();
    }
    else {
        RXintHandler0 = NULL;
    }

    return GOOD;
}

void uart0_deinit()
/*
  Configure UART1 pins as GPIO
*/
{
    /* Make Tx and Rx pins GPIO */
    PINSEL0 &= ~((3<<0) | (3<<2));

    vic_disablechannel(VIC_CH_UART0);
}

void uart0_setparameters(const signed char stopbits, const signed char parity, const signed char wordlength, const signed char breakcontrol, const signed char rxtriggerlevel)
/*!
  Set several parameters. Before this function is called, all parameters settable with this function have the default values as set with
  the defines at the top of this file.
  Using a value < 0 for any of the parameters of this function leaves it unchanged.
  The parameters are:
   stopbits         No. of stopbits; can be 1 or 2
   parity           The parity in use. Can be UART_PARITY_ODD, UART_PARITY_EVEN, UART_PARITY_1 (always 1), UART_PARITY_0 (always 0) or UART_PARITY_NONE (disabled)
   wordlength       Wordlength; can be 5, 6, 7 or 8
   breakcontrol     When TRUE, breakcontrol is enabled, thus forcing Tx low when no transmission is active.
   rxtriggerlevel   Sets the triggerlevel for the Rx FiFo. Has no effect when the RXhandler argument of uart0_init() isn't set. Value's can be
                    UART_RX_TRIGGERLEVEL1, UART_RX_TRIGGERLEVEL4, UART_RX_TRIGGERLEVEL8 or UART_RX_TRIGGERLEVEL14.
*/
{
    /* Set no. of stopbits.. */
    if(stopbits == 2) {
        U0LCR |= LCR_STOPBIT;
    }
    else if(stopbits == 1) {
        U0LCR &= ~LCR_STOPBIT;
    }
    /* .. parity checking .. */
    if(parity >= 0) {
        if(parity != UART_PARITY_NONE) {
            U0LCR |= LCR_PARITYEN & parity;
        }
        else {
            U0LCR &= ~LCR_PARITYEN;
        }
    }
    /* .. wordlength, .. */
    if(wordlength >= 5 && wordlength <= 8) {
        U0LCR |= wordlength-5;
    }
    /* .. break control .. */
    if(breakcontrol) {
        U0LCR |= LCR_BREAKCTRL;
    }
    else if(breakcontrol==0) {
        U0LCR &= ~LCR_BREAKCTRL;
    }
    /* .. and the Rx trigger level */
    if(rxtriggerlevel >= UART_RX_TRIGGERLEVEL1 && rxtriggerlevel <= UART_RX_TRIGGERLEVEL14) {
        U0FCR |= rxtriggerlevel;
    }
}

#ifdef UART0_HASAUTOBAUDRATE
void uart0_startautobaud(const char mode, const bool autorestart, const FUNCTION handler)
/*!
  Enable auto baudrate detection.
  The parameters are:
   mode             Can be 0 ('mode 0', measure on two falling edges) or 1 ('mode 1', measure on a falling and the next rising edge)
   autorestart      When set (TRUE), autobaud will automatically restart after a timeout
   callback         Function to call when an autobaud interrupt triggers. Set to NULL to disable it
*/
{
    if(handler != NULL) {
        if(autorestart != TRUE) {
            /* Enable both timeout and done interrupt */
            U0IER |= IER_ABTO | IER_ABEO;
        }
        else {
            /* No point in having a timeout interrupt when auto restart is enabled */
            U0IER |= IER_ABEO;
        }
        AutoBaudHandler0 = handler;
    }
    /* Set parameters and start autobaud */
    U0ACR = ((autorestart&0x01) << 2) | ((mode&0x01) << 1) | ACR_START;
}
#endif

void uart0_setbaudrate(const unsigned short baudrate)
/*!
  Set baudrate
*/
{    /* Set DLAB */
    U0LCR |= LCR_DLAB;

    U0DLL = (unsigned char)baudrate;
    U0DLM = baudrate>>8;

    /* Clear DLAB */
    U0LCR &= ~LCR_DLAB;
}

void uart0_putchar(const unsigned char c)
/*!
  Write one character to UART0
*/
{
    #if UART0_INT
    if(uart0_interruptbased) {
        if(FiFoCount0>=UART_MAXFIFOSIZE) {
            /* FiFo is full!
               Store the character in the software-buffer */
            TXbuffer0[TXbufferpointer0]=c;
            TXbufferpointer0++;
            if(TXbufferpointer0==UART0_TXBUFFERSIZE) {
                TXbufferpointer0=0;
            }
            /* OK, all done here */
            return TRUE;
        }
        FiFoCount0++;
    }
    else {
        /* Just wait until the FiFo is empty */
        while(!(U0LSR & LSR_TEMT));
    }
    #else
    /* Wait until there's room for a new character */
    while(!(U0LSR & LSR_THRE));
    #endif

    /* Send character */
    U0THR = c;
}

void uart0_put(char* string)
/*!
  Send a NULL terminated string
*/
{
    while(*string) {
        uart0_putchar(*string);
        string++;
    }
}

unsigned char uart0_get(void)
/*!
  Read one character from UART0. This will return 0 if there is no data.
  For a more direct and fast function (a macro actually), use uart0_getchar().
*/
{
    /* Do we have any new data? */
    if (U0LSR & LSR_RDR) {
        /* jup, return it */
        return U0RBR;
    }
    return 0;
}

void uart0_flush(void)
/*!
  Flush FiFo's
*/
{
    #if UART0_INT
    TXpointer0 = 0;
    TXbufferpointer0 = 0;
    #endif
    U0FCR |= FCR_RXFIFORESET;
    U0FCR |= FCR_TXFIFORESET;
}

void uart0_setRXinterruptHandler(FUNCTION handler)
/*!
  Set the RX interrupt handler
*/
{\
    RXintHandler0 = handler;
}

void uart0_setRXLineinterruptHandler(FUNCTION handler)
/*!
  Set the RX line status interrupt handler
*/
{
    U0IER |= IER_RXLINESTAT;
    RxLineintHandler0 = handler;
}

void uart0_enableRXinterrupt(void)
/*!
  Enable the RX interrupt
*/
{
    /* Enable 'Receive data enable' interrupt */
    U0IER |= IER_RBR;
}

void uart0_disableRXinterrupt(void)
/*!
  Disable the RX interrupt
*/
{
    /* Disable 'Receive data enable' interrupt */
    U0IER &= ~IER_RBR;
}

static void uart0_intHandler(void)
/*
  UART0 interrupt handling
*/
{
    switch(U0IIR & IIR_ID_MASK) {
        case IIR_ID_RDA:
        case IIR_ID_CTI:
            /* Data has been received */
            if(RXintHandler0 != NULL) {
                RXintHandler0();
            }
            return;
            //break;
        #if UART0_INT
        case IIR_ID_THRE:
            /* Ok, we'll fill the FiFo again */
            FiFoCount0=0;
            while(TXpointer0 != TXbufferpointer0 && FiFoCount0<UART_MAXFIFOSIZE) {
                U0THR=TXbuffer0[TXpointer0];
                FiFoCount0++;
                TXpointer0++;
                /* Buffer wrap */
                if(TXpointer0==UART0_TXBUFFERSIZE) {
                    TXpointer0=0;
                }
            }
            return;
            //break;
        #endif
        case IIR_ID_RLS:
            /* Receive Line Status */
            if(RxLineintHandler0 != NULL) {
                RxLineintHandler0();
            }
            /* Clear interrupt flag */
            U0SCR = U0LSR;
            return;
            //break;
    }
    #ifdef UART0_HASAUTOBAUD
    if(U0IIR & IIR_ABEO) {
        AutoBaudHandler0();
        /* Auto baudrate done */
        U0ACR &= ~ACR_DONEINT;
    }
    else if (U0ACR & IIR_ABTO) {
        AutoBaudHandler0();
        /* Auto baudrate timeout */
        U0ACR &= ~ACR_TIMEOUTINT;
    }
    #endif
}

#if UART0_INT
unsigned char uart0_interruptTXenabled(void)
/*!
  returns TRUE if interruptbased transfers are enabled, FALSE otherwise
*/
{
    return uart0_interruptbased;
}

void uart0_enableTXinterrupt(void)
/*!
  This function enabled interrupt-based transfers when supported.
  Note: calling this when interrupt-based transfers are already enabled will
        cause everything already in the interrupt-buffer to be lost.
*/
{
    /* Enable 'FIFO buffer empty' interrupt */
    U0IER = IER_THRE;
    uart0_interruptbased=TRUE;
    /* Clear buffer */
    TXbufferpointer0 = 0;
    TXbuffer0[0]='\0';
    FiFoCount0=0;
}

void uart0_disableTXinterrupt(const unsigned char graceful)
/*!
  This function disables interrupt-based transfers when supported.
  When graceful is TRUE, this function will first wait until the interrupt-buffer
  is empty, so no characters will be lost.
*/
{
    if(graceful) {
        /* Wait until the buffer is empty */
        while(TXpointer0 != TXbufferpointer0) {
            #if SLEEPWHENWAITING
            cpu_poweridle();
            #endif
        }
    }
    /* Disable the UART TX interrupt */
    U0IER &= ~IER_THRE;
    uart0_interruptbased=FALSE;
}
#endif /* UART0_INT */

/*
  Check for invalid default settings
*/
#if UART0_WORDLENGTH < 5 || UART0_WORDLENGTH > 8
    #error "Invalid wordlength selected! Chose a value between 5 and 8."
#endif

#if UART0_PARITY != UART_PARITY_NONE && \
    UART0_PARITY != UART_PARITY_ODD && \
    UART0_PARITY != UART_PARITY_EVEN && \
    UART0_PARITY != UART_PARITY_1 && \
    UART0_PARITY != UART_PARITY_0
    #error "Invalid parity-type selected! Chose one of UART_PARITY_NONE, UART_PARITY_ODD, UART_PARITY_EVEN, UART_PARITY_1 or UART_PARITY_0."
#endif

#if UART0_STOPBITS < 1 || UART0_STOPBITS > 2
    #error "Invalid number of stopbits selected! Chose '1' or '2'."
#endif

#if UART0_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL1 && \
    UART0_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL4 && \
    UART0_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL8 && \
    UART0_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL14
    #error "Invalid RX triggerlevel selected! Chose one of UART_RX_TRIGGERLEVEL1, UART_RX_TRIGGERLEVEL4, UART_RX_TRIGGERLEVEL8 or UART_RX_TRIGGERLEVEL14."
#endif

#else
#warning "Driver disabled"
#endif /* UART0_ENABLED */
