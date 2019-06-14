/*
    ALDS (ARM LPC Driver Set)

    config-debug.h:
                   Debug function macro's

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This code is meant to be as efficient as possible, so it's macro's all the way.

*/
/*!
\file
Debug function macro's
*/
#ifndef CONFIG_DEBUG_H
#define CONFIG_DEBUG_H

/*! Default debug console configuration uses UART0 as the output.
    The following define sets the baudrate to use. */
#define DEBUG_BAUDRATE          b9600

/*! Define how to show the debug information. First, a debug initialisation
    function. This function must return > 0 when the initialisation is
    succesfull, or 0 on error. */
#define debug_init()            uart0_init(DEBUG_BAUDRATE,NULL)

/*! Next up, the debug output function. This function must accept one argument,
    namely the character to print */
#define debug_putchar           uart0_putchar

/*! Finally, what to do when interrupts are not available (e.g. when an
    exception occurs) */
#define debug_enterexception()  uart0_disableTXinterrupt(FALSE)

/*! The actual debug function. There shouldn't be any reason to change this. */
#define dprint(...)             fprint(debug_putchar, __VA_ARGS__)

/* Include any headerfiles needed for the functions defined above */
#include <uart.h>
#include <print.h>

#endif /* CONFIG_DEBUG_H */
