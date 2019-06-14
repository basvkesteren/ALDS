/*
    ALDS (ARM LPC Driver Set)

    err.h:
          Error codes and stuff..

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
Error codes and stuff..
*/
#ifndef ERR_H
#define ERR_H

/* Include global configuration */
#include <config.h>

#include <types.h>

/*! Thisone *should* be used to store the last encountered error; this provides a way for functions
    without an error_t return to tell the caller someting went wrong.
    \note: This is for the driverpackage-functions only! Using this for other purposes can cause problems
           related to functions overwriting this, reantrancy, that kind'o stuff */
extern error_t err;

/* All-round error-codes */
#define GOOD                1   /* All is good */
#define ERROR               11  /* Something has gone wrong */
#define EXCEPTION           12  /* An exception occured, either UND, PAB or DAB */
#define TERM                13  /* Program terminated unexpectedly */
#define TIMEOUT             14  /* A timeout occured (hardware mostly) */
#define IOERROR             15  /* An I/O error occured (the hardware did not respond as expected) */
#define OVERFLOW            16  /* This will not fit in */
#define UNKNOWN             18  /* Don't know what to do with this */
#define NOTIMPLEMENTED      19  /* Sorry, that function is not available at this time */
#define INVALID             20  /* Invalid input received */
#define UNHANDLED           22  /* Something happend which the programmer did not forsee (thus, unhandled) */

#define BUSY                80  /* We're busy, come back later */
#define READY               81  /* Device is ready */

#endif /* ERR_H */
