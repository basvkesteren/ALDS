/*
    ALDS (ARM LPC Driver Set)

    print.h:
            Simple printf like function, the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Simple printf like function, the definitions
*/
#ifndef PRINT_H
#define PRINT_H

/* Include global configuration */
#include <config.h>

void (* printputchar)(unsigned char c);

#define print(...)   fprint(printputchar,__VA_ARGS__)

/* Function prototypes */
void print_open(void * localputchar);
void fprint(void (* localputchar)(unsigned char c), char *fmt, ...);
unsigned int sprint(char c[], char *fmt, ...);

#endif /* PRINT_H */
