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

typedef struct ringbufferctrl {
    unsigned char *data;            /* Points to the data */
    unsigned int length;            /* Length of buffer (data) */
    unsigned int readpos;           /* Next unread byte */
    unsigned int writepos;          /* Next writeposition */
} ringbufferctrl_t;

typedef unsigned char ringbufferdata_t;

void ringbuffer_init(ringbufferctrl_t *ringbuffer, unsigned char *buffer, unsigned int bufferlength);
size_t ringbuffer_write(ringbufferctrl_t *ringbuffer, const void *pointer, const size_t size, const size_t length);
signed short ringbuffer_peek(ringbufferctrl_t *ringbuffer, const size_t offset);
size_t ringbuffer_read(ringbufferctrl_t *ringbuffer, void *pointer, const size_t size, const size_t length);
size_t ringbuffer_skip(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length);
size_t ringbuffer_revert(ringbufferctrl_t *ringbuffer, const size_t size, const size_t length);
unsigned int ringbuffer_getfreebytes(const ringbufferctrl_t *ringbuffer);
#define ringbuffer_getusedbytes(ringbuffer)     ((ringbuffer->length-1) - ringbuffer_getfreebytes(ringbuffer))
bool ringbuffer_isfull(const ringbufferctrl_t *ringbuffer);
bool ringbuffer_isempty(const ringbufferctrl_t *ringbuffer);

#endif /* _RINGBUFFER_H_ */
