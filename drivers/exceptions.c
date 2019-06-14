/*
    ALDS (ARM LPC Driver Set)

    exceptions.c:
                 ARM exception handlers

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains debugging handlers for all the ARM exceptions:
                                    warning     backtrace   context
             -undefined instruction    X           X           X
             -software interrupt       X
             -prefetch abort           X           X           X
             -data abort               X           X           X
             -FIQ                      X
             -IRQ                      X
             Also, when the VIC is configured wrong, '__HaltVICerr' is called which will issue
             a warning, then hang.
             Note that all of the actions done here are configurable from the configuration file, 'config.h'

*/
/*!
\file
ARM exception handlers
*/
#include "exceptions.h"
#include <err.h>
#include <debug.h>
#include "registers.h"
#if (CRASHACTION) & C_CONTEXT
#include "savecontext.h"
#if (CRASHACTION) & C_TRACE
#include "backtrace.h"
#endif
#endif
#if (CRASHACTION) & C_CALLBACK
#include "callback.h"
#endif

#if (CRASHACTION) & C_CONTEXT
/*! This points to a struct containing the data gathered just after the crash */
#define CRASHDATA ( ((crashdata_t *)&crashdata) )
__noreturn static void __HaltAnalysis(error_t exitcode);
#else
/* Analysis is disabled, so stop immediately */
#define __HaltAnalysis(x)   __HaltExit(x)
#endif

void __HaltUndef(void)
/*!
  Occurs if neither the processor, or any coprocessor, recognizes the currently
  executed instruction.
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("Undefined instruction exception!\n\r");
    #endif

    __HaltAnalysis(EXCEPTION);
}

void __HaltSwi(void)
/*!
  This is a user-defined exception. It allows a program running in User-mode,
  for example, to request priveleged operations that run in Supervisor mode,
  such as an RTOS function.
*/
{
    #if (CRASHACTION) & C_WARN
    unsigned int SWIargs;
    #endif

    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    asm volatile ("mov %0, r14"
        : "=r"(SWIargs)
        );
    SWIargs&=0xFFFFFF;
    dprint("Unhandled SWI exception! argument=%h\n\r",SWIargs);
    #endif

    __HaltExit(UNHANDLED);
}

void __HaltPabort(void)
/*!
  Occurs when the processor attempts to execute an instruction that was not
  fetched, because the address was illegal.
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("Prefetch abort exception!\n\r");
    #endif

    __HaltAnalysis(EXCEPTION);
}

void __HaltDabort(void)
/*!
  Occurs when a data transfer instruction attempts to load or store data at an
  illegal address
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("Data abort exception!\n\r");
    #endif

    __HaltAnalysis(EXCEPTION);
}

void __HaltFiq(void)
/*!
  Occurs when the processor external fast interrupt request pin is asserted(LOW)
  and the F bit in the CPSR is clear.
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("Unhandled FIQ exception! VICFIQStatus=%b\n\r",VICFIQStatus);
    #endif

    __HaltExit(UNHANDLED);
}

void __HaltIrq(void)
/*!
  Occurs when the processor external interrupt request pin is asserted(LOW) and
  the I bit in the CPSR is clear
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("Unhandled IRQ exception! VICIRQStatus=%b\n\r",VICIRQStatus);
    #endif

    __HaltExit(UNHANDLED);
}

/*
  The following are not really ARM-exceptions; they are used to catch some other (software) errors
*/

void __HaltVICerr(void)
/*!
  Invalid address in VICVectAddr
  This is a serious software bug; we just hang, no need to do a context analysis or stack backtrace
*/
{
    debug_enterexception();

    #if (CRASHACTION) & C_WARN
    dprint("VICVectAddr not initialised! VICIRQStatus=%b\n\r", VICIRQStatus);
    #endif

    __HaltExit(UNHANDLED);
}

#if (CRASHACTION) & C_CONTEXT
static void __HaltAnalysis(error_t exitcode)
/*!
  Do some analysis on the gathered data before we stop the program
*/
{
    #if (CRASHACTION) & C_TRACE
    backtrace(CRASHDATA->r11);
    #endif

    /* Display the full context as it where when the exception occured */
    dprint("Crash occured at location %h\n\r",CRASHDATA->lr);
    dprint("Context during crash: R0: %h\t R1: %h\t R2: %h\t R3: %h\n\r",CRASHDATA->r0,CRASHDATA->r1,CRASHDATA->r2,CRASHDATA->r3);
    dprint("                      R4: %h\t R5: %h\t R6: %h\t R7: %h\n\r",CRASHDATA->r4,CRASHDATA->r5,CRASHDATA->r6,CRASHDATA->r7);
    dprint("                      R8: %h\t R9: %h\t R10:%h\t R11:%h\n\r",CRASHDATA->r8,CRASHDATA->r9,CRASHDATA->r10,CRASHDATA->r11);
    dprint("                      R12:%h\t LR: %h\t SP: %h\n\r",CRASHDATA->r12,CRASHDATA->lr_c,CRASHDATA->sp_c);
    dprint("CPU was running in ");
    switch(CRASHDATA->cpsr & 0x1F) {
        case 0x10:
            dprint("User");
            break;
        case 0x11:
            dprint("FIQ");
            break;
        case 0x12:
            dprint("IRQ");
            break;
        case 0x13:
            dprint("SVC");
            break;
        case 0x17:
            dprint("Abort");
            break;
        case 0x1B:
            dprint("Undef");
            break;
        case 0x1F:
            dprint("System");
            break;
        default:
            dprint("unknown(0x%h)",CRASHDATA->cpsr & 0x1F);
    }
    dprint(" operating mode\n\r");

    /* And stop */
    __HaltExit(exitcode);
}
#endif

void __HaltExit(error_t exitcode)
/*!
  Program stop
    'Houston, we've had a problem. We've had an unexpected program termination'
*/
{
    #if (CRASHACTION) & C_WARN
    dprint("program ended, exitcode %i\n\r",exitcode);
    #endif

    #if (CRASHACTION) & C_CALLBACK
    callback_crash(exitcode);
    #endif

    #if (CRASHACTION) & C_REBOOT
    /* Restart the system */
    asm volatile("ldr r2,=_start\n"
                 "bx r2\n");
    #endif

    /* And hang */
    while(1);   /* "He's dead, Jim" */
}
