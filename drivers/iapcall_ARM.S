/*
    ALDS (ARM LPC Driver Set)

    iapcall.S:
              In Application Programming - ROM IAP function entry point

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:

*/
#include <config.h>

@ IAP location
.set IAP_LOCATION,   0x7ffffff1

    .arm

    .global iap_call
iap_call:
    mov     r2, #0x7ffffff1
    bx      r2

    .end
