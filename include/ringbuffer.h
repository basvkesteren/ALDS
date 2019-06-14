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

*/
/*!
\file
General purpose ringbuffer, the definitions
*/
#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

/* Include global configuration */
#include <config.h>
#include <types.h>
#include <stddef.h>

/* What to do when ringbuffer_read() gets to read more than available?
   When this is set, we read as much as available (but less than requested)
   When not set, nothing is read and thus 0 is returned. */
#define RINGBUFFER_WHENREADOVERFLOW_SHRINK  0

typedef struct ringbufferctrl {
    unsigned char *data;            /* Points to the data */
    unsigned int length;            /* Length of buffer (data) */
    unsigned int readpos;           /* Where we stopped reading */
    unsigned int writepos;          /* Where we start writing */
    unsigned char hasdata;          /* Flag used to determine wheter the buffer is empty or full when readpos equals writepos */
} ringbufferctrl_t;

typedef unsigned char ringbufferdata_t;

void ringbuffer_init(ringbufferctrl_t *ringbuffer, unsigned char *buffer, unsigned int bufferlength);
size_t ringbuffer_write(ringbufferctrl_t *ringbuffer, const void *pointer, const size_t size, const size_t length);
signed short ringbuffer_peek(ringbufferctrl_t *ringbuffer, const size_t offset);
size_t ringbuffer_read(ringbufferctrl_t *ringbuffer, void *pointer, const size_t size, const size_t length);
size_t ringbuffer_skip(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length);
size_t ringbuffer_revert(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length);
unsigned int ringbuffer_getfreebytes(const ringbufferctrl_t *ringbuffer);
bool ringbuffer_isfull(const ringbufferctrl_t *ringbuffer);

#endif /* _RINGBUFFER_H_ */
