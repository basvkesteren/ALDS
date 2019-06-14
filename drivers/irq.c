/*
    ALDS (ARM LPC Driver Set)

    irq.c:
          Low-level interrupt functions

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
Low-level interrupt functions
*/

/*! To temporally store the interruptstatus, the following variable is used */
unsigned int __interrupt_status;
