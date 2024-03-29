/*
    ALDS (ARM LPC Driver Set)

    ringbuffer.c:
                 General purpose ringbuffer

    copyright:
              Copyright (c) 2008 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -Can read and write all data types, uses bytes (unsigned char) for all internal actions.

*/
/*!
\file
General purpose ringbuffer
*/
#include "ringbuffer.h"
#include "debug.h"

/* Uncomment the following to enable debug-output */
//#define DEBUG_RINGBUFFER

void ringbuffer_init(ringbufferctrl_t *ringbuffer, unsigned char *buffer, unsigned int bufferlength)
/**
  Initialise ringbuffer
*/
{
    ringbuffer->data=buffer;
    ringbuffer->length=bufferlength;
    ringbuffer->readpos=0;
    ringbuffer->writepos=0;
}

size_t ringbuffer_write(ringbufferctrl_t *ringbuffer, const void *pointer, const size_t size, const size_t length)
/**
  Store data in the ringbuffer; amount of entries (of size 'size') written is returned
*/
{
    unsigned int towrite;
    unsigned int written=0;
    const unsigned char* buffer = pointer;

    /* Figure out how much bytes we have to write */
    towrite = length * size;

    #ifdef DEBUG_RINGBUFFER
    dprint("ringbuffer_write(): towrite=%i, readpos = %i, writepos = %i, free = %i\n\r",towrite, ringbuffer->readpos, ringbuffer->writepos, ringbuffer_getfreebytes(ringbuffer));
    #endif

    if(towrite > ringbuffer_getfreebytes(ringbuffer)) {
        /* This won't do; too much data */
        #ifdef DEBUG_RINGBUFFER
        dprint("ringbuffer_write(): Too much data, abort! (%i > %i)\n\r",towrite,ringbuffer_getfreebytes(ringbuffer));
        #endif
        return 0;
    }

    while(written < towrite) {
        /* Store byte.. */
        ringbuffer->data[ringbuffer->writepos]=buffer[written];
        written++;
        /* ..and advance ringbuffer pointer */
        ringbuffer->writepos++;
        if(ringbuffer->writepos >= ringbuffer->length) {
            ringbuffer->writepos = 0;
        }
    }

    return written / size;
}

signed short ringbuffer_peek(ringbufferctrl_t *ringbuffer, const size_t offset)
/**
  Read a single byte from the ringbuffer, at 'offset' bytes from the current read-position,
  without modifying the readpointer.
  Returns -1 if the given offset results in an empty position.
*/
{
    unsigned int peekpos;

    #ifdef DEBUG_RINGBUFFER
    dprint("ringbuffer_peek(): offset = %i, readpos = %i, writepos = %i, free = %i\n\r", offset, ringbuffer->readpos, ringbuffer->writepos, ringbuffer_getfreebytes(ringbuffer));
    #endif

    if(offset < 0 || offset >= ringbuffer->length || ringbuffer_isempty(ringbuffer)) {
        /* Invalid offset, or no data at all */
        return -1;
    }

    /* Figure out what position to read from */
    peekpos = (ringbuffer->readpos + offset) % ringbuffer->length;

    if(ringbuffer->writepos > ringbuffer->readpos && peekpos >= ringbuffer->writepos) {
        /* Readpointer is before writepointer, thus peekpos must also be before writepointer */
        return -1;
    }
    else if (ringbuffer->readpos > ringbuffer->writepos && peekpos < ringbuffer->readpos && peekpos >= ringbuffer->writepos) {
        /* Readpointer is after writepointer, thus peekpos must be after readpointer or before writepointer */
        return -1;
    }

    #ifdef DEBUG_RINGBUFFER
    dprint("ringbuffer_peek(): peekpos = %i\n\r", peekpos);
    #endif

    return ringbuffer->data[peekpos];
}

size_t ringbuffer_read(ringbufferctrl_t *ringbuffer, void *pointer, const size_t size, const size_t length)
/**
  Read data from the ringbuffer; amount of entries (of size 'size') read is returned
*/
{
    unsigned int toread;
    unsigned int read=0;
    unsigned char* buffer = pointer;

    /* Figure out how much bytes we have to read */
    toread = length * size;

    #ifdef DEBUG_RINGBUFFER
    dprint("ringbuffer_read(): toread = %i, readpos = %i, writepos = %i, free = %i\n\r", toread, ringbuffer->readpos, ringbuffer->writepos, ringbuffer_getfreebytes(ringbuffer));
    #endif

    if(toread > ringbuffer_getusedbytes(ringbuffer)) {
        /* Do nothing */
        #ifdef ringbufferdebug
        DEBUG("ringbuffer_read(): too much data, leave\n\r");
        #endif
        return 0;
    }

    while(read < toread) {
        /* Read byte.. */
        buffer[read] = ringbuffer->data[ringbuffer->readpos];
        read++;
        /* And advance ringbuffer pointer */
        ringbuffer->readpos++;
        if(ringbuffer->readpos >= ringbuffer->length) {
            ringbuffer->readpos = 0;
        }
    }

    return read / size;
}

size_t ringbuffer_skip(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length)
/**
  Skip 'length' entries of 'size' size (iow, advance the read-pointer).
  Return amount of entries actually skipped.
*/
{
    unsigned int toskip;

    /* Figure out how much bytes we have to skip */
    toskip = length * size;

    if(toskip > ringbuffer_getusedbytes(ringbuffer)) {
        /* Oops.. trying to skip more than available. We skip as much as possible, or iow empty the ringbuffer */
        toskip = ringbuffer_getusedbytes(ringbuffer);
        ringbuffer->readpos = ringbuffer->writepos;
    }
    else if(ringbuffer->readpos + toskip >= ringbuffer->length) {
        ringbuffer->readpos = toskip - (ringbuffer->length - ringbuffer->readpos);
    }
    else {
        ringbuffer->readpos += toskip;
    }

    return toskip / size;
}

size_t ringbuffer_revert(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length)
/**
  Revert 'length' entries of 'size' size (iow, move the write-pointer back).
  Return amount of entries actually reverted.
*/
{
    unsigned int toskip;

    /* Figure out how much bytes we have to skip */
    toskip = length * size;

    if(toskip > ringbuffer_getusedbytes(ringbuffer)) {
        /* Oops.. trying to skip more than available. We skip as much as possible, or iow empty the ringbuffer */
        toskip = ringbuffer_getusedbytes(ringbuffer);
        ringbuffer->writepos = ringbuffer->readpos;
    }
    else if(toskip > ringbuffer->writepos) {
        ringbuffer->writepos = ringbuffer->length - (toskip - ringbuffer->writepos);
    }
    else {
        ringbuffer->writepos -= toskip;
    }

    return toskip / size;
}

unsigned int ringbuffer_getfreebytes(const ringbufferctrl_t *ringbuffer)
/**
  Return amount of free bytes in ringbuffer
*/
{
    if(ringbuffer->readpos == ringbuffer->writepos) {
        /* Ringbuffer is empty */
        return ringbuffer->length-1;
    }
    else if((ringbuffer->writepos+1)%ringbuffer->length == ringbuffer->readpos) {
        /* One byte remaining between readpos and writepos: ringbuffer is full */
        return 0;
    }
    else if(ringbuffer->writepos > ringbuffer->readpos) {
        /*  [   start       ]
            [               ]
            [   readpos     ]   -\
            [               ]    | -> used, unread data
            [   writepos    ]   -/
            [               ]
            [   length      ]
        */
        return (ringbuffer->readpos + (ringbuffer->length - ringbuffer->writepos)) - 1;
    }
    else {
        /*  [   start       ]    |
            [               ]    | -> used, unread data
            [   writepos    ]   -/
            [               ]
            [   readpos     ]   -\
            [               ]    | -> used, unread data
            [   length      ]    |
        */
        return (ringbuffer->readpos-ringbuffer->writepos) - 1;
    }
}

bool ringbuffer_isfull(const ringbufferctrl_t *ringbuffer)
/**
  Return TRUE is ringbuffer is full, FALSE otherwise
*/
{
    if((ringbuffer->writepos+1)%ringbuffer->length == ringbuffer->readpos) {
        /* One byte remaining between readpos and writepos: ringbuffer is full */
        return TRUE;
    }
    return FALSE;
}

bool ringbuffer_isempty(const ringbufferctrl_t *ringbuffer)
/**
  Return TRUE is ringbuffer is empty, FALSE otherwise
*/
{
    if(ringbuffer->readpos == ringbuffer->writepos) {
        /* Ringbuffer is empty */
        return TRUE;
    }
    return FALSE;
}