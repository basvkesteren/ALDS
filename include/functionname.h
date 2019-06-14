/*
    ALDS (ARM LPC Driver Set)

    functionname.h:
                   Functionname lookup during runtime

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -To make the functionname displaying work, the code has to be compiled with '-mpoke-function-name'

*/
/*!
\file
Functionname lookup during runtime
*/
#ifndef FUNCTIONNAME_H
#define FUNCTIONNAME_H

/* Include global configuration */
#include <config.h>

/* Function prototypes */
unsigned int* __getfunctionname(const unsigned int functionaddress);
unsigned int __getfunctionstart(unsigned int address);

#endif /* FUNCTIONNAME_H */
