/*
    ALDS (ARM LPC Driver Set)

    uart1.c:
            UART1 driver, both poll and interrupt-based versions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -The UART1 is identical to UART0, with the addition of a modem interface.

*/
/*!
\file
UART1 driver, both poll and interrupt-based versions
*/
#include <uart.h>

#ifdef UART1_ENABLED

#include <err.h>
#include <vic.h>
#include <delay.h>
#include "registers.h"
#include "uart_bits.h"

/*
  Default configuration bits. All of these except the UART1_INT and UART1_TXBUFFERSIZE
  settings can be changed at runtime with uart1_setparameters().
*/

/*! UART sending can be done poll-based and interrupt-based. The LPC2106 hardware
    uart has a FiFo, which is used as a buffer. When this buffer is full and the driver
    is configured to use the poll-based code, the driver will block until the FiFo is empty.
    But when using interrupt-based code, the characters will be stored in a local ringbuffer.
    Do note that when working in interrupt-mode, characters will get lost if the hardware
    UART can't keep up! In poll-mode, characters should never get lost.

    When UART1_INT is enabled, the send-mode is configurable at runtime with
    uartX_enableinterrupts(). If UART1_INT is disabled, this function will still be available,
    but won't do anything (interrupt-based transfers are not supported) */
#define UART1_INT               0

/*! uart1_putchar_timeout() timeout value (in us)
*/
#define UART1_PUTCHAR_TIMEOUT   1000000

/*! Configure the size of the ringbuffer. A smaller buffer increases the change of lost
    characters. */
#if UART1_INT
#define UART1_TXBUFFERSIZE      2048
#endif

/*! Configure the wordlength. Possible values are 5, 6, 7 or 8. */
#define UART1_WORDLENGTH        8

/*! Configure the parity. Possible values are:
     UART_PARITY_ODD      Odd parity
     UART_PARITY_EVEN     Even parity
     UART_PARITY_1        Parity bit is always '1'
     UART_PARITY_0        Parity bit is always '0'
     UART_PARITY_NONE     Parity bit is disabled */
#define UART1_PARITY            UART_PARITY_EVEN

/*! Configure the number of stopbits. Possible values are 1 and 2. */
#define UART1_STOPBITS          1

/*! Break transmission or not? If enabled, TX will be forced low when no transmission
    is active. */
#define UART1_BREAKTRANSENABLE  0

/*! Configure the RX FIFO trigger level. This determines how many
    characters must be written to the RX FIFO before the Rx Line interrupt
    is generated (we don't use thatone), and it determines when auto-RTS activates.
    Possible values are:
     UART_RX_TRIGGERLEVEL1    1 character
     UART_RX_TRIGGERLEVEL4    4 characters
     UART_RX_TRIGGERLEVEL8    8 characters
     UART_RX_TRIGGERLEVEL14   14 characters */
#define UART1_RXTRIGGERLEVEL    UART_RX_TRIGGERLEVEL14

/* PINSEL values for flowcontrol*/
#define VAL_PINSEL0_U1FLOW  (0x5<<20)
#define MSK_PINSEL0_U1FLOW  (0xf<<20)
#define VAL_PINSEL0_U1FLOWRTS  (0x1<<20)
#define MSK_PINSEL0_U1FLOWRTS  (0x3<<20)
#define VAL_PINSEL0_U1FLOWCTS  (0x1<<22)
#define MSK_PINSEL0_U1FLOWCTS  (0x3<<22)

static void (* RXintHandler1)(void);
static void (* RxLineintHandler1)(void);
static void (* FlowControlintHandler1)(void);
static void (* AutoBaudHandler1)(void);

static void uart1_intHandler(void);

/* Stuff for the interrupt-FiFo buffer */
#if UART1_INT
static unsigned char FiFoCount1;
static unsigned short TXpointer1=0;
static unsigned short TXbufferpointer1;
static unsigned char TXbuffer1[UART1_TXBUFFERSIZE];
static unsigned char uart1_interruptbased;
#endif

error_t uart1_init(const unsigned short baudrate, const FUNCTION RXhandler)
/*!
  Initialise UART1 with the given baudrate
*/
{
    /* Initialize Pin Select Block for Tx and Rx */
    PINSEL0 &= ~((3<<16) | (3<<18));
    PINSEL0 |= ((1<<16) | (1<<18));

    /* Enable FIFO's, reset them and set the RX trigger level. */
    U1FCR = FCR_FIFOEN | FCR_RXFIFORESET | FCR_TXFIFORESET | UART1_RXTRIGGERLEVEL;
    /* Set DLAB (and clear all other bits in U1LCR while we're at it..) */
    U1LCR = LCR_DLAB;
    /* Set baudrate */
    U1DLL = (unsigned char)baudrate;
    U1DLM = baudrate>>8;
    /* Clear DLAB */
    U1LCR &= ~LCR_DLAB;

    /* Set wordlength, .. */
    U1LCR |= UART1_WORDLENGTH-5;
    /* .. number of stopbits, .. */
    #if UART1_STOPBITS == 2
    U1LCR |= LCR_STOPBIT;
    #endif
    /* .. break control .. */
    #if UART1_BREAKTRANSENABLE
    U1LCR |= LCR_BREAKCTRL;
    #endif
    /* .. and parity checking */
    #if UART1_PARITY != UART_PARITY_NONE
    U1LCR |= LCR_PARITYEN & UART1_PARITY;
    #endif

    /* Install a vector in the VIC */
    vic_setup(VIC_CH_UART1, IRQ, PRIO_UART1, uart1_intHandler);

    U1IER = 0;

    #if UART1_INT
    uart1_enableTXinterrupt();
    #endif

    /* Set RX interrupt */
    if(RXhandler!=NULL) {
        uart1_setRXinterruptHandler(RXhandler);
        uart1_enableRXinterrupt();
    }
    else {
        RXintHandler1 = NULL;
    }

    FlowControlintHandler1 = NULL;

    return GOOD;
}

void uart1_deinit()
/*
  Configure UART1 pins as GPIO
*/
{
    /* Make Tx and Rx pins GPIO */
    PINSEL0 &= ~((3<<16) | (3<<18));
}

void uart1_setparameters(const signed char stopbits, const signed char parity, const signed char wordlength, const signed char breakcontrol, const signed char rxtriggerlevel)
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
        U1LCR |= LCR_STOPBIT;
    }
    else if(stopbits == 1) {
        U1LCR &= ~LCR_STOPBIT;
    }
    /* .. parity checking .. */
    if(parity >= 0) {
        if(parity != UART_PARITY_NONE) {
            U1LCR |= LCR_PARITYEN & parity;
        }
        else {
            U1LCR &= ~LCR_PARITYEN;
        }
    }
    /* .. wordlength, .. */
    if(wordlength >= 5 && wordlength <= 8) {
        U1LCR |= wordlength-5;
    }
    /* .. break control .. */
    if(breakcontrol) {
        U1LCR |= LCR_BREAKCTRL;
    }
    else if(breakcontrol==0) {
        U1LCR &= ~LCR_BREAKCTRL;
    }
    /* .. and the Rx trigger level */
    if(rxtriggerlevel >= UART_RX_TRIGGERLEVEL1 && rxtriggerlevel <= UART_RX_TRIGGERLEVEL14) {
        U1FCR |= rxtriggerlevel;
    }
}

#ifdef UART1_HASAUTOBAUDRATE
void uart1_startautobaud(const char mode, const bool autorestart, const FUNCTION handler)
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
            U1IER |= IER_ABTO | IER_ABEO;
        }
        else {
            /* No point in having a timeout interrupt when auto restart is enabled */
            U1IER |= IER_ABEO;
        }
        AutoBaudHandler1 = handler;
    }
    /* Set parameters and start autobaud */
    U1ACR = ((autorestart&0x01) << 2) | ((mode&0x01) << 1) | ACR_START;
}
#endif

void uart1_setbaudrate(const unsigned short baudrate)
/*!
  Set baudrate
*/
{
    U1DLL = (unsigned char)baudrate;
    U1DLM = baudrate>>8;
}

void uart1_putchar(const unsigned char c)
/*!
  Write one character to UART1
*/
{
    #if UART1_INT
    if(uart1_interruptbased) {
        if(FiFoCount1>=UART_MAXFIFOSIZE) {
            /* FiFo is full!
               Store the character in the software-buffer */
            TXbuffer1[TXbufferpointer1]=c;
            TXbufferpointer1++;
            if(TXbufferpointer1==UART1_TXBUFFERSIZE) {
                TXbufferpointer1=0;
            }
            /* OK, all done here */
            return;
        }
        FiFoCount1++;
    }
    else {
        /* Just wait until the FiFo is empty */
        while(!(U1LSR & LSR_TEMT));
    }
    #else
    /* Wait until there's room for a new character */
    while(!(U1LSR & LSR_THRE));
    #endif

    /* Send character */
    U1THR = c;
}

bool uart1_putchar_timeout(const unsigned char c)
/*!
  Write one character to UART1, with a timeout on the tx-ready wait
*/
{
    unsigned int timeout = 0;

    /* Wait until there's room for a new character */
    while(!(U1LSR & LSR_THRE)) {
        delay_us(1);
        timeout++;
        if(timeout == UART1_PUTCHAR_TIMEOUT) {
            return FALSE;
        }
    }

    /* Send character */
    U1THR = c;
    return TRUE;
}

unsigned char uart1_get(void)
/*!
  Read one character from UART1. This will return 0 if there is no data.
  For a more direct and fast function (a macro actually), use uart1_getchar().
*/
{
    /* Do we have any new data? */
    if (U1LSR & LSR_RDR) {
        /* jup, return it */
        return U1RBR;
    }
    return 0;
}

void uart1_flush(void)
/*!
  Flush FiFo's
*/
{
    #if UART1_INT
    TXpointer1 = 0;
    TXbufferpointer1 = 0;
    #endif
    U1FCR |= FCR_RXFIFORESET;
    U1FCR |= FCR_TXFIFORESET;
}

void uart1_flowcontrol(const char mode, const FUNCTION handler)
/*!
  Configure the flowcontrol settings.
  Possible values for 'mode' are:
    UART_FLOW_NONE       No flowcontrol
    UART_FLOW_AUTORTS    Automatic RTS flowcontrol (LPC2101/2/3 only!)
    UART_FLOW_AUTOCTS    Automatic CTS flowcontrol (LPC2101/2/3 only!)
    UART_FLOW_INTCTS     A CTS change will trigger an interrupt. On the LPC2104/5/6
                         this is the only reasonable way to create CTS flowcontrol.
  'handler' is the function which will be called upon a modem status change interrupt.
*/
{
    FlowControlintHandler1 = handler;
    U1IER &= ~(IER_CTS | IER_MODEMSTAT);

    if(mode == UART_FLOW_NONE) {
        /* Configure RTS and CTS lines as GPIO */
        PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOW);
        #ifdef UART1_HASAUTOFLOWCONTROL
        U1MCR &= ~(MCR_RTSEN | MCR_CTSEN);
        #endif
    }
    else {
        /* Set flowcontrol mode */
        #ifdef UART1_HASAUTOFLOWCONTROL
        if(mode & UART_FLOW_AUTORTS) {
            U1MCR |= MCR_RTSEN;
            PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOWRTS) | VAL_PINSEL0_U1FLOWRTS;
        }
        else {
            U1MCR &= ~MCR_RTSEN;
            PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOWRTS);
        }
        if(mode & UART_FLOW_AUTOCTS) {
            U1MCR |= MCR_CTSEN;
            PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOWCTS) | VAL_PINSEL0_U1FLOWCTS;
        }
        else {
            U1MCR &= ~MCR_CTSEN;
            PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOWCTS);
        }
        #endif
        if(mode & UART_FLOW_INTCTS) {
            U1IER |= IER_MODEMSTAT;
            #ifdef UART1_HASAUTOFLOWCONTROL
            if(mode & UART_FLOW_AUTOCTS) {
                U1IER |= IER_CTS;
            }
            #endif
            PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U1FLOWCTS) | VAL_PINSEL0_U1FLOWCTS;
        }
    }
}

void uart1_setRXinterruptHandler(FUNCTION handler)
/*!
  Set the RX interrupt handler
*/
{
    RXintHandler1 = handler;
}

void uart1_setRXLineinterruptHandler(FUNCTION handler)
/*!
  Set the RX line status interrupt handler
*/
{
    U1IER |= IER_RXLINESTAT;
    RxLineintHandler1 = handler;
}

void uart1_enableRXinterrupt(void)
/*!
  Enable the RX interrupt
*/
{
    /* Enable 'Receive data enable' interrupt */
    U1IER |= IER_RBR;
}

void uart1_disableRXinterrupt(void)
/*!
  Disable the RX interrupt
*/
{
    /* Disable 'Receive data enable' interrupt */
    U1IER &= ~IER_RBR;
}

static void uart1_intHandler(void)
/*
  UART1 interrupt handling
*/
{
     switch(U1IIR & IIR_ID_MASK) {
        case IIR_ID_RDA:
        case IIR_ID_CTI:
            /* Data has been received */
            if(RXintHandler1 != NULL) {
                RXintHandler1();
            }
            return;
            //break;
        #if UART1_INT
        case IIR_ID_THRE:
            /* Ok, we'll fill the FiFo again */
            FiFoCount1=0;
            while(TXpointer1 != TXbufferpointer1 && FiFoCount1<UART_MAXFIFOSIZE) {
                U1THR=TXbuffer1[TXpointer1];
                FiFoCount1++;
                TXpointer1++;
                /* Buffer wrap */
                if(TXpointer1==UART1_TXBUFFERSIZE) {
                    TXpointer1=1;
                }
            }
            return;
            //break;
        #endif
        case IIR_ID_MODEM:
            /* A modem input line has changed state */
            if(FlowControlintHandler1 != NULL) {
                FlowControlintHandler1();
            }
            /* Clear interrupt flag */
            U1SCR = U1MSR;
            return;
            //break;
        case IIR_ID_RLS:
            /* Receive Line Status */
            if(RxLineintHandler1 != NULL) {
                RxLineintHandler1();
            }
            /* Clear interrupt flag */
            U1SCR = U1LSR;
            return;
            //break;
    }
    #ifdef UART1_HASAUTOBAUD
    if(U1IIR & IIR_ABEO) {
        AutoBaudHandler1();
        /* Auto baudrate done */
        U1ACR &= ~ACR_DONEINT;
    }
    else if (U1ACR & IIR_ABTO) {
        AutoBaudHandler1();
        /* Auto baudrate timeout */
        U1ACR &= ~ACR_TIMEOUTINT;
    }
    #endif
}

#if UART1_INT
unsigned char uart1_interruptTXenabled(void)
/*!
  returns TRUE if interruptbased transfers are enabled, FALSE otherwise
*/
{
    return uart1_interruptbased;
}

void uart1_enableTXinterrupt(void)
/*!
  This function enabled interrupt-based transfers when supported.
  Note: calling this when interrupt-based transfers are already enabled will
        cause everything already in the interrupt-buffer to be lost.
*/
{
    /* Enable 'FIFO buffer empty' interrupt */
    U1IER |= IER_THRE;
    uart1_interruptbased=TRUE;
    /* Clear buffer */
    TXbufferpointer1 = 0;
    TXbuffer1[0]='\0';
    FiFoCount1=0;
}

void uart1_disableTXinterrupt(const unsigned char graceful)
/*!
  This function disables interrupt-based transfers when supported.
  When graceful is TRUE, this function will first wait until the interrupt-buffer
  is empty, so no characters will be lost.
*/
{
    if(graceful) {
        /* Wait until the buffer is empty */
        while(TXpointer1 != TXbufferpointer1) {
            #if SLEEPWHENWAITING
            cpu_poweridle();
            #endif
        }
    }
    /* Disable the UART TX interrupt */
    U1IER &= ~IER_THRE;
    uart1_interruptbased=FALSE;
}
#endif /* UART1_INT */

/*
  Check for invalid default settings
*/
#if UART1_WORDLENGTH < 5 || UART1_WORDLENGTH > 8
    #error "Invalid wordlength selected! Chose a value between 5 and 8."
#endif

#if UART1_PARITY != UART_PARITY_NONE && \
    UART1_PARITY != UART_PARITY_ODD && \
    UART1_PARITY != UART_PARITY_EVEN && \
    UART1_PARITY != UART_PARITY_1 && \
    UART1_PARITY != UART_PARITY_0
    #error "Invalid parity-type selected! Chose one of UART_PARITY_NONE, UART_PARITY_ODD, UART_PARITY_EVEN, UART_PARITY_1 or UART_PARITY_0."
#endif

#if UART1_STOPBITS < 1 || UART1_STOPBITS > 2
    #error "Invalid number of stopbits selected! Chose '1' or '2'."
#endif

#if UART1_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL1 && \
    UART1_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL4 && \
    UART1_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL8 && \
    UART1_RXTRIGGERLEVEL != UART_RX_TRIGGERLEVEL14
    #error "Invalid RX triggerlevel selected! Chose one of UART_RX_TRIGGERLEVEL1, UART_RX_TRIGGERLEVEL4, UART_RX_TRIGGERLEVEL8 or UART_RX_TRIGGERLEVEL14."
#endif

#else
#warning "Driver disabled"
#endif /* UART1_ENABLED */
