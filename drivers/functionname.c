/*
    ALDS (ARM LPC Driver Set)

    functionname.c:
                   Functionname lookup during runtime

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -To make the functionname displaying work, the code has to be compiled with
             '-mpoke-function-name' (which is enabled by default); check the makefile, setting DBG.

*/
/*!
\file
Functionname lookup during runtime
*/
#include <functionname.h>
#include <types.h>
#include <debug.h>

/*! The function __getfunctionaddress() can be used to find the startingaddress of
    a function by passing a memorylocation to it. This is done by searching back
    in memory for a particular pattern. How much words should we look back? */
#define FUNCTIONNAME_MAXSEARCHTRACE      250

unsigned int* __getfunctionname(const unsigned int functionaddress)
/*!
  (try to) get the functionname; input is the address of the beginning of a function.
  The code must be compiled with -mpoke-function-name; in that case the name of the
  function will be embedded in the code, before the actual function.
*/
{
    /* 'functionaddress' points to the first instruction of the function. The word
        before that tells us where the functionname is stored, and how long it is.
        There's one 'gotcha' here; the first bit of 'functionaddress' cannot be
        one (if it is, we get in trouble with the alignment). Problem is, it is
        one when the function we're looking at is a thumb one.. So we clear that
        bit here aswell */
    unsigned int* functionname=(unsigned int *)(functionaddress&~0x01)-1;

    /* If the top 8 bits of the current location of 'functionname'
       are '1', the functionname should be available. We want bits 8 to 24 to
       be zero, to limit false positives in this check. This does limit the
       max. functionname to 256 chars. Should be more than enough, I think.. */
    if(((*functionname) & 0xFFFFFF00) == 0xFF000000) {
        /* Found the name; return it's location */
        return functionname - (((*functionname) & 0x00FFFFFF)>>2);
    }
    else {
        /* Not there..? */
        return NULL;
    }
}

unsigned int __getfunctionstart(unsigned int address)
/*!
  (try to) get the starting address of the function where the given address
  is located.
  This is done by looking for a value of 0xFF0000-- in memory (where - is don't care)
*/
{
    int maxsearch=FUNCTIONNAME_MAXSEARCHTRACE;

    /* Clear first bit to avoid alignment issues */
    address&=~0x01;

    /* Try to find the functionname location&length word */
    while((((*(unsigned int*)address) & 0xFFFFFF00) != 0xFF000000) && maxsearch) {
        address-=4;
        maxsearch--;
    }

    if(maxsearch) {
        return address+4;
    }
    return 0;
}
