/*
    ALDS (ARM LPC Driver Set)

    std.c:
          Some all-around functions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

              the pow() function is (C) by Mark Zealey <mark@itsolve.co.uk>
    remarks:
            -

*/
/*!
\file
Some all-around functions
*/
#include <std.h>
#include <types.h>

/*! The maximum length of a number. For a 32 bit CPU, 32 is a good value */
#define STD_MAXNUMLENGTH    32

/*! When the following is enabled, the function inttostr() will handle negative
    input correctly. This will obviously limit the max. input value. */
#define STD_SUPPORTNEGATIVE 1

/*******************************************************************************
  general functions
*/

#if SUPPORTNEGATIVE
unsigned char inttostr(int num, char *c, const unsigned char base)
#else
unsigned char inttostr(unsigned int num, char *c, const unsigned char base)
#endif
/*!
  Coverts the given number 'num' with base 'base' to a string. The result is stored in 'c',
  the length of the result is returned.
  note: 'c' must be long enough! this function does not check this. Maximum length of the result
  is STD_MAXNUMLENGTH chars.
*/
{
    const unsigned char chars[16] = "0123456789ABCDEF";
    unsigned char i=0;
    unsigned char j=0;
    unsigned int rest;
    #if STD_SUPPORTNEGATIVE
    bool negative=FALSE;
    #endif

    /* Convert it to a string */
    if(num==0) {
        c[0]='0';
        c[1]='\0';
        return 1;
    }
    else {
        #if STD_SUPPORTNEGATIVE
        if(num < 0) {
            negative=TRUE;
            num=0-num;
        }
        #endif
        while(num && i<STD_MAXNUMLENGTH) {
            rest=num%base;
            num/=base;
            c[i]=chars[rest];
            i++;
        }
        #if STD_SUPPORTNEGATIVE
        if(negative) {
            c[i] = '-';
            i++;
        }
        #endif

        /* Terminate string */
        c[i]='\0';
        i--; /* Don't include the '\0' in the string length */
    }

    /* The string is now reversed. Let's fix that */
    while(j<i) {
        rest=c[j];
        c[j]=c[i];
        c[i]=rest;
        i--;
        j++;
    }

    return j+i+1; /* But do include the '\0' in the returned length */
}

unsigned int strtoint(char *c, const unsigned char base)
/*!
  Convert the given string to a number, ignoring any invalid charachters in the string
*/
{
    unsigned int result = 0;
    #if STD_SUPPORTNEGATIVE
    bool negative=FALSE;
    #endif

    while(*c) {
        if(isdigit(*c,base)) {
            result *= base;
            result += ctoi(*c,base);
        }
        #if STD_SUPPORTNEGATIVE
        else if(result==0 && *c == '-') {
            negative = TRUE;
        }
        #endif
        c++;
    }

    #if STD_SUPPORTNEGATIVE
    if(negative) {
        return 0-result;
    }
    #endif
    return result;
}

unsigned char ctoi(const unsigned char c, const unsigned char base)
/*!
  Convert the given ASCII character to the corresponding number, using the given base.
  Note that this function does not check wheter 'c' actually is a digit!
  Use isdigit() for that.
*/
{
    if (c >= 'a' && base > 10) {
        return c-'a'+10;
    }

    if (c >= 'A' && base > 10) {
        return c-'A'+10;
    }

    return c-'0';
}

int isdigit(const unsigned char c, const unsigned char base)
/*!
  Determine wheter the given char is a digit according to the given base.
  We assume ASCII encoding here, so '0' < 'A' < 'a'.
*/
{
    if (c < '0') {
        /* This is not a diget, no matter what base.. */
        return FALSE;
    }
    if (base > 10) {
        if ((c >= 'a' && c <= 'a' + base-11) || (c >= 'A' && c <= 'A' + base-11)) {
            return TRUE;
        }
    }

    if (c >= '0' && c <= '0' + base-1) {
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
  math functions
*/

int fpow(const int ground, const int n)
/*!
  Returns ground^n (fixed point)
*/
{
    int i;
    int p=1;

    for(i=1; i<=n; ++i) {
        p=p*ground;
    }

    return p;
}

/*******************************************************************************
  string functions
*/

void toupper(char *c)
/*!
  Convert all characters (a-z) to uppercase
*/
{
    while( *c ) {
        if((*c >= 'a') && (*c <= 'z')) {
            *c-=('a'-'A');
        }
        c++;
    }
}

void tolower(char *c)
/*!
  Convert all characters (A-Z) to lowercase
*/
{
    while( *c ) {
        if((*c >= 'A') && (*c <= 'Z')) {
            *c+=('a'-'A');
        }
        c++;
    }
}
