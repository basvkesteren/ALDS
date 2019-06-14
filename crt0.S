/*
    ALDS (ARM LPC Driver Set)

    crt0.s:
           the actual bootcode; the beginning of all

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -The IRQ and Reset exceptions are the only ones that are handled properly. The others
             point some sort of dummy function, or just reset the CPU(depending on the configuration,
             see config.h)
            -If you want to change the stacksizes (which are initialised here) see the Linkerscript.
            -"A re-entrant interrupt handler (at least its 'top-level') *must* be written in assembler."
             Since we atleast *attempt* to be re-entrant, we do that..

*/
#include <config.h>
#include "drivers/registers.h"
.include "config_crt0.S"

    .global main

    @ Some defines for the CPSR initialisation values for the different operating
    @ modes. (also see the ARM system on chip book, page 108, table 5.1)
    .set USER_MODE,        0x10
    .set FIQ_MODE,         0x11
    .set IRQ_MODE,         0x12
    .set SUPERVISOR_MODE,  0x13
    .set ABORT_MODE,       0x17
    .set UNDEFINED_MODE,   0x1B
    .set SYSTEM_MODE,      0x1F

    @ Defines for the interrupt bits in the CPSR register. '1' means the
    @ interrupt is disabled
    .set CPSR_I_BIT,       0x80
    .set CPSR_F_BIT,       0x40

    @ These are set in the linkerscript
    .extern __stack_start_fiq__
    .extern __stack_start_irq__
    .extern __stack_start_supervisor__
    .extern __stack_start_abort__
    .extern __stack_start_undefined__
    .extern __stack_start_sys__
    .extern __stack_end__
    .extern __bss_beg__
    .extern __bss_end__
    .extern __data_beg__
    .extern __data_beg_src__
    .extern __data_end__

    .text
    .arm
    .align 2   	@ Make sure the code is aligned properly. The '2' tells the assembler
                @ how much of the lower bits of the PC have to be zero.
                @ In ARM mode, this has to be 1 or more. In thumb mode, this may be 0

    .global _start
    .func _start
_start:                             @ This is where the actual code execution starts
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ startup
ResetHandler:
    @ We start with setting up the stacks, one for each CPU-mode
    @ This basicly means we set the SP for each mode to the beginning of the
    @ stack. See the Linkerscript for the stacksizes
    msr     cpsr_c, #FIQ_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_fiq__
    msr     cpsr_c, #IRQ_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_irq__
    msr     cpsr_c, #SUPERVISOR_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_supervisor__
    msr     cpsr_c, #ABORT_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_abort__
    msr     cpsr_c, #UNDEFINED_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_undefined__
    msr     cpsr_c, #SYSTEM_MODE | CPSR_I_BIT | CPSR_F_BIT
    ldr     sp, =__stack_start_sys__

    #ifdef RUNMODE
    @ Switch to given runmode
    msr     cpsr_c, #RUNMODE | CPSR_I_BIT | CPSR_F_BIT
    #endif

    ldr     sl, =__stack_end__      @ And finally load the StackLimit.
                                    @ We don't use it (yet?) but it can't hurt setting it..

    mov     r1, #0                  @ FramePointer..
    mov     fp, r1                  @ ..cleared

    @ Next up, clear unused memory (also known as the BSS, see
    @ http://en.wikipedia.org/wiki/BSS)
    @
    @ According to the C-standard all new variables must be zero. The
    @ compiler doesn't do this as it assumes the system itsself does this
    @ during the startup sequence. That is, you guessed it, right here
    ldr     r1, =__bss_beg__
    ldr     r3, =__bss_end__
    subs    r3, r3, r1              @ Figure out length of BSS (__bss_end__ - __bss_beg__)
    beq     .end_clear_loop         @ Make sure there actually is a BSS (if not,
                                    @ skip the clear loop)

    mov     r2, #0                  @ We fill the BSS with..
.clear_loop:
    strb    r2, [r1], #1            @ Fill byte at location in r1, and shift to next word
    subs    r3, r3, #1              @ Length of BSS minus one
    bgt     .clear_loop             @ Are we done yet?
.end_clear_loop:

    @ Now load initialisation values from ROM to RAM
    ldr     r1, =__data_beg__
    ldr     r2, =__data_beg_src__
    ldr     r3, =__data_end__
    subs    r3, r3, r1              @ Figure out length of data block (__data_end__ - __data_beg__)
    beq     .end_set_loop           @ Make sure there is a data block (if not,
                                    @ skip the copy loop)
.set_loop:
    ldrb    r4, [r2], #1            @ Read word from ROM (and __data_beg_src__++)..
    strb    r4, [r1], #1            @ ..and store it in RAM (and __data_beg__++)
    subs    r3, r3, #1              @ Length of data block minus one
    bgt     .set_loop               @ Are we done yet?
.end_set_loop:

    #ifdef __THUMB
    add     r0, pc, #1              @ Get the addres of the thumb-code
    bx      r0                      @ And jump, thereby switching to thumb-mode

    .thumb
    .thumb_func
thumb_switch:
    #endif

    @ Now for some basic interrupt stuff.
    @ First we set MEMMAP, which tells the CPU where to map the vector table
    ldr     r0, =MEMMAP
    #ifdef __RUN_FROM_RAM
    mov     r1, #2                  @ vectors are in RAM
    #elif defined __RUN_FROM_ROM
    mov     r1, #1                  @ vectors are in ROM
    #else
    #error Invalid run-modus selected. Chose 'RUN_FROM_ROM' or 'RUN_FROM_RAM'
    #endif
    str     r1, [r0]

    @ And set the IRQ handler for non-vectored IRQ's
    ldr     r0, =VICDefVectAddr
    ldr     r1, =__HaltIrq
    str     r1, [r0]

    @ Finally, call the main() function, after setting some stuff
    mov     r7, #0                  @ Framepointer is empty for thumb, too
    bl      main                    @ Call main()

ExitMain:
    #if (CRASHACTION) == C_REBOOT
    @ Reboot immediatly, no need to call the C-code for that...
    #ifdef __THUMB
    ldr     r0, =ResetHandler       @ Need to switch from thumb to arm, first..
    bx      r0
    #else
    b       ResetHandler
    #endif
    #else
    @ We call the __HaltExit() function which will do crash analysis,
    @ notification and a reboot (depending on the configuration, that is..)
    bl      __HaltExit              @ Call __HaltExit. Note that a1 (== r0) is
                                    @ the first argument of this function,
                                    @ and also contains the returncode of main()
endless_loop:
    b       endless_loop            @ All done, hang forever
    #endif
    .endfunc @ end of _start

    #ifdef __THUMB
    .arm
    #endif

    .func IRQHandler
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ IRQ (VIC) handling
IRQHandler:
    IRQ_enter                       @ Call macro in config_crt0.S
    ldr     lr, =ExitISR            @ Set return address
    ldr     r0, =VICVectAddr
    ldr     r1, [r0]                @ Get address from interrupt handler
    bx      r1                      @ Call the IRQ handler function. Note that
                                    @ the current VIC channel is masked until
                                    @ the priority hardware is updated; this is
                                    @ important for nested interrupt handling,
                                    @ where one doesn't want an endless loop
                                    @ servicing the current and
                                    @ yet-to-be-cleared interrupt.
ExitISR:
    ldr     r0, =VICVectAddr
    mov     r1, #0xFF               @ Update priority hardware..
    str     r1, [r0]                @ ..by loading 0xFF in VICVectAddr
    IRQ_leave                       @ Call macro in config_crt0.S
    .endfunc

    .func SWIHandler
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Software interrupt
SWIHandler:
    SWI_unhandled                   @ Call macro in config_crt0.S
    .endfunc

    .func FIQHandler
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ FIQ exception
FIQHandler:
    FIQ_unhandled                   @ Call macro in config_crt0.S
    .endfunc

#if CODEPROTECTION
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Code protection
@  By writing a special value to a special location (...) all ISP code
@  reading functions AND the complete JTAG interface are disabled.
.section .codeprotection,"ax"
    .arm
    .align 2
    .word 0x87654321
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Exception-table.
@  The ARM CPU always starts in the exception (or vector) table. Therefore
@  these vectors are located at a fixed address, which is defined in the
@  Linkerscript
.section .startup,"ax"
    .arm
    .align 2

#if (CRASHACTION) == C_REBOOT
@ Reboot immediatly
    ldr     pc, =ResetHandler       @ reset vector
    ldr     pc, =ResetHandler       @ undefined instruction
    #if HANDLE_SWI
    ldr     pc, =__software_interrupt @ software interrupt (handled)
    #else
    ldr     pc, =SWIHandler         @ software interrupt (unhandled)
    #endif
    ldr     pc, =ResetHandler       @ prefetch abort
    ldr     pc, =ResetHandler       @ data abort
    nop                             @ signature, used by flashloaders
    ldr     pc, =IRQHandler         @ IRQ (VIC)
    ldr     pc, =FIQHandler         @ FIQ
#elif (CRASHACTION) & C_CONTEXT
@ Gather context, then call C-functions
    ldr     pc, =ResetHandler       @ reset vector
    ldr     pc, =__HaltUndef_SC     @ undefined instruction
    #if HANDLE_SWI
    ldr     pc, =__software_interrupt @ software interrupt (handled)
    #else
    ldr     pc, =SWIHandler         @ software interrupt (unhandled)
    #endif
    ldr     pc, =__HaltPabort_SC    @ prefetch abort
    ldr     pc, =__HaltDabort_SC    @ data abort
    nop                             @ signature, used by flashloaders
    ldr     pc, =IRQHandler         @ IRQ (VIC)
    ldr     pc, =FIQHandler         @ FIQ
#else
@ Call C-functions
    ldr     pc, =ResetHandler       @ reset vector
    ldr     pc, =__HaltUndef        @ undefined instruction
    #if HANDLE_SWI
    ldr     pc, =__software_interrupt @ software interrupt (handled)
    #else
    ldr     pc, =SWIHandler         @ software interrupt (unhandled)
    #endif
    ldr     pc, =__HaltPabort       @ prefetch abort
    ldr     pc, =__HaltDabort       @ data abort
    nop                             @ signature, used by flashloaders
    ldr     pc, =IRQHandler         @ IRQ (VIC)
    ldr     pc, =FIQHandler         @ FIQ
#endif

    .end
