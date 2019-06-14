/*
    ALDS (ARM LPC Driver Set)

    std.h:
          some all-around functions, the definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Some all-around functions, the definitions
*/
#ifndef STD_H
#define STD_H

/* Include global configuration */
#include <config.h>

/* Function prototypes */
unsigned char inttostr(unsigned int num, char *c, const unsigned char base);
unsigned int strtoint(char *c, const unsigned char base);
unsigned char ctoi(const unsigned char c, const unsigned char base);
int isdigit(const unsigned char c, const unsigned char base);

int fpow(const int ground, const int n);

void toupper(char *c);
void tolower(char *c);

#endif /* STD_H */
