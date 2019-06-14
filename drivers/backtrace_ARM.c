/*
    ALDS (ARM LPC Driver Set)

    backtrace.c:
                Backtrace generation during runtime

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -The frame_tail idea is copied from the ARM-port of Linux-2.6
            -To make the functionname displaying work, the code has to be compiled with '-mpoke-function-name'
            -The use of optimalization can have strange effects on the backtrace. -O0 and -O1 should not cause
             any problems. With -O2, -O3 and -Os things should still work, but the trace can appear to be 'incomplete';
             this is usually caused by the compiler which has optimized some stuff out.

*/
/*!
\file
Backtrace generation during runtime
*/
#include "backtrace.h"
#include <types.h>
#include <debug.h>
#include <functionname.h>

/*! Print the full stackframe or not? If disabled, one will only see the functions on
    stack during the trace. In case of an exception, the full stackframe can come in
    handy.. */
#define BACKTRACE_PRINTSTACKFRAME     1

/*! When the following is enabled, the name of the function on the bottom of the stack
    (The one prior to the function found in the first frame) is printed using
    __getfunctionaddress() */
#define BACKTRACE_PRINTSTACKORIGIN    1


/* The stackdump is dumped with dprint(), so that will by default end up on the UART.
   If another output-device is to be used for the stackdump, or if you want to get
   a stackdump even if debug is disabled, locally redefine dprint() as follows:

   #undef dprint(char *fmt, ...)
   #define dprint(char *fmt, ...) myownstackdumpfunction(char *fmt, ...)
   #endif

*/

/*! This is used to detect a crash occuring during a backtrace; it prevents
    us from a crash->trace->crash->trace->... loop */
static bool doingtrace;

/*! End of stack, as defined in the linkerscript */
extern unsigned int __stack_end__;

/*! The registers we're interested in are at the end of the variable
    length saved register structure. The fp points at the end of this
    structure so the address of this struct is:
    (struct frame_tail *)(xxx->fp)-1 */
struct frame_tail {
    struct frame_tail *fp;
    unsigned int sp;
    unsigned int lr;
} __packed;

void backtrace(unsigned int framepointer)
/*!
  Do a stack backtrace (or atleast, an attempt to do so..)
*/
{
    struct frame_tail *tail;
    unsigned int* functionname = NULL;
    unsigned int pc;

    if(doingtrace) {
        dprint("Seems the previous trace went sideways; abort\n\r");
        return;
    }
    /* Yes, we're tracing */
    doingtrace=TRUE;

    /* get framepointer */
    if(framepointer == 0) {
        /* No framepointer passed, so use the current one */
        asm volatile ("mov %0, fp"
            : "=r"(framepointer)
            );
    }
    tail = ((struct frame_tail *)framepointer)-1;

    dprint("Starting backtrace...\n\r");

    /* get stacklimit.. */
    dprint("Stacklimit is at 0x%h\n\r",&__stack_end__);

    /* Print out the stacktrace */
    while(tail->fp != NULL) {
        /* Get program counter */
        pc=*((&tail->lr) + 1);

        /* Get functionname.
           PC points to the beginning of the actual function, and thereby ignores the function entry code
           (stack stuff). So, to get to the beginning of the function INCLUDING the entry code, we need to move
           back a bit. Here i assume this entry code is always 4 instructions... Works OK for me, but it might
           very well fail in other situations. */
        dprint("[%h]:",pc-16);
        functionname=__getfunctionname( pc-16 );
        if(functionname) {
            dprint("%s\n\r",functionname);
        }
        else {
            dprint("?unknown?\n\r");
        }

        #if BACKTRACE_PRINTSTACKFRAME
        /* Print stackframe */
        dprint("\tFrame is at address 0x%h\n\r",&(tail->fp));
        dprint("\tPC:0x%h\n\r",pc);
        dprint("\tLR:0x%h\n\r",tail->lr);
        dprint("\tSP:0x%h\n\r",tail->sp);
        #endif

        dprint("\n\r");

        /* Locate next frame */
        if(tail >= tail->fp) {
            /* Stackframes always move up in the stack. So, if the previous frame was higher in
               memory than thisone, something is wrong.
               Or we just found the end of the tail.. */
            break;
        }
        tail=((struct frame_tail *)tail->fp)-1;
    }
    #if BACKTRACE_PRINTSTACKORIGIN
    pc = __getfunctionstart(tail->lr);
    if(pc) {
        functionname = __getfunctionname(pc);
        if(functionname) {
            dprint("Origin: %s()",functionname);
        }
    }
    if( pc == 0 || functionname == NULL ) {
        dprint("Cannot find origin!\n\r");
    }
    #endif

    dprint("-> End of stack..\n\r");

    /* Trace is done */
    doingtrace=FALSE;
}
