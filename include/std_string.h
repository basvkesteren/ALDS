/*
    ALDS (ARM LPC Driver Set)

    std_string.h:
                 Some basic string functions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Some basic string functions
*/
#ifndef STD_STRING_H
#define STD_STRING_H

/* Include global configuration */
#include <config.h>

unsigned int strlen(const char *s);
char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int n);
unsigned int strpos(char *haystack, char *needle);

#endif /* STD_STRING_H */
