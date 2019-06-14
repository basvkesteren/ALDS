/*
    ALDS (ARM LPC Driver Set)

    std_string.c:
                 Some basic string functions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
Some basic string functions
*/
#include <std_string.h>

unsigned int strlen(const char *s)
/*!
  Determine length of given string, max length is sizeof(unsigned int)
*/
{
    unsigned int i=0;

    while(*s) {
        s++;
        i++;
    }

    return i;
}

char *strcat(char *dest, const char *src)
/*!
  Concatenate two strings, or in other words add 'src' to 'dest'
*/
{
    char *begin=dest;

    /* Find end of src */
    while(*dest) {
        dest++;
    }

    /* Add src to the end of dest */
    while(*src) {
        *dest=*src;
        dest++;
        src++;
    }
    /* Terminate string */
    *dest='\0';

    return begin;
}

char *strcpy(char *dest, const char *src)
/*!
  Copy a string
*/
{
    char *begin=dest;

    while(*src) {
        *dest=*src;
        dest++;
        src++;
    }
    /* Terminate string */
    *dest='\0';

    return begin;
}

char *strncpy(char *dest, const char *src, int n)
/*!
  Copy a string, but no more than 'n' characters
*/
{
    char *begin=dest;

    while(*src && n) {
        *dest=*src;
        dest++;
        src++;
        n--;
    }

    while(n) {
        *dest='\0';
        n--;
    }

    return begin;
}

unsigned int strpos(char *haystack, char *needle)
/*!
  return numerical location of first occurance of 'needle' in 'haystack',
  return 0 if not found
*/
{
    unsigned int i=0;
    unsigned int start=0;
    char *needlestart=needle;

    while(haystack[i]) {
        if(haystack[i] == *needle) {
            needle++;
        }
        else {
            /* Restart */
            needle = needlestart;
            start = i;
        }
        /* Did we found him already? */
        if(*needle == '\0') {
            return start;
        }

        i++;
    }
    return 0;
}
