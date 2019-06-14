/*
    ALDS (ARM LPC Driver Set)

    print.c:
            Simple printf like function

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This print(f) function provides s(string), c(char), i/d(integer), b (binair) and h/x(hexdecimal).

*/
/*!
\file
Simple printf like function
*/
#include <print.h>
#include <std.h>
#include <stdarg.h> // For va_arg and friends

void print_open(void * localputchar)
/*!
  Set the output function
*/
{
    printputchar=localputchar;
}

void fprint(void (* localputchar)(unsigned char c), char *fmt, ...)
/*!
  Simple fprintf like function, based on code from the 'C-handboek', page 212
*/
{
    char c[32];
    char* string;
    unsigned char base=0;

    /* Create an object containing all given arguments */
    va_list argpointer;

    /* Initialise argpointer, so it points to the first argument given */
    va_start(argpointer, fmt);

    /* Walk through al the given arguments */
    for(;*fmt;fmt++) {
        /* Is it a variable? */
        if(*fmt != '%') {
            /* Nope, so just print it */
            localputchar(*fmt);
        }
        else {
            /* It is.. What kind? */
            switch(*++fmt) {
                case 's':
                    /* string */
                    for(string=va_arg(argpointer, char *); *string; string++) {
                        /* Walk through the entire string, printing each character */
                        localputchar(*string);
                    }
                    break;
                case 'c':
                    /* char */
                    localputchar(va_arg(argpointer, int));
                    break;
                case 'b':
                    /* binair */
                    base=2;
                    break;
                case 'd':
                    /* Fall through */
                case 'i':
                    /* int */
                    base=10;
                    break;
                case 'h':
                    /* Fall through */
                case 'x':
                    /* hex */
                    base=16;
                    break;
                default:
                    localputchar(*fmt);
                    break;
            }
            if(base!=0) {
                /* Convert the number to a string.. */
                inttostr(va_arg(argpointer, int),c,base);
                /* ..and print it */
                for(string=c; *string; string++) {
                    /* Walk through the entire string, printing each character */
                    localputchar(*string);
                }
                base=0;
            }
        }
    }

    /* free memory n' stuff.. */
    va_end(argpointer);
}

unsigned int sprint(char c[], char *fmt, ...)
/*!
  Simple sprintf like function. Very similair to print(), only the output is done in 'c[]'
  instead of the output-device
*/
{
    char *string;
    int outputcounter=0;

    /* Create an object containing all given arguments */
    va_list argpointer;

    /* Initialise argpointer, so it points to the first argument given */
    va_start(argpointer, fmt);

    /* Walk through al the given arguments */
    for(;*fmt;fmt++) {
        /* Is it a variable? */
        if(*fmt != '%') {
            /* Nope, so just print it */
            c[outputcounter]=*fmt;
            outputcounter++;
        }
        else {
            /* It is.. What kind? */
            switch(*++fmt) {
                case 's':
                    /* string */
                    for(string=va_arg(argpointer, char *); *string; string++) {
                        /* Walk through the entire screen, printing each character */
                        c[outputcounter]=*string;
                        outputcounter++;
                    }
                    break;
                case 'c':
                    /* char */
                    c[outputcounter]=(char)va_arg(argpointer, int);
                    outputcounter++;
                    break;
                case 'b':
                    /* binair */
                    outputcounter+=inttostr((char)va_arg(argpointer, int),&c[outputcounter],2);
                    break;
                case 'd':
                    /* Fall through */
                case 'i':
                    /* int */
                    outputcounter+=inttostr(va_arg(argpointer, int),&c[outputcounter],10);
                    break;
                case 'h':
                    /* Fall through */
                case 'x':
                    /* hex */
                    outputcounter+=inttostr(va_arg(argpointer, int),&c[outputcounter],16);
                    break;
                default:
                    c[outputcounter]=*fmt;
                    outputcounter++;
                    break;
            }
        }
    }
    c[outputcounter]='\0';

    /* free memory n' stuff.. */
    va_end(argpointer);

    /* Return length of created string */
    return outputcounter;
}
