/*
    ALDS (ARM LPC Driver Set)

    savecontext.S:
                  Low-level code to store the current context to memory

    copyright:
              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -When C_CONTEXT is enabled, we want the unmodified context as it was during
             the crash/exception. We can do that from C too, but when optimisation is
             enabled, there is no way to assure the context isn't modified before we
             can save it. So, we do it in assembly
            -The __save_context code is based on work from Roger Lynx, from
             http://www.embedded.com/showArticle.jhtml?articleID=192202641

*/
#include <config.h>

.arm
.align 2

/* This is where the context is saved to */
    .global crashdata
crashdata:
.word 0x10101010    @ r0
.word 0x20202020    @ r1
.word 0x30303030    @ r2
.word 0x40404040    @ r3
.word 0x50505050    @ r4
.word 0x60606060    @ r5
.word 0x70707070    @ r6
.word 0x80808080    @ r7
.word 0x90909090    @ r8
.word 0xa0a0a0a0    @ r9
.word 0xb0b0b0b0    @ r10
.word 0xc0c0c0c0    @ r11
.word 0xd0d0d0d0    @ r12
__crashdata_special:
.word 0x11111111    @ lr
.word 0x22222222    @ cpsr
.word 0x33333333    @ lr_c
.word 0x44444444    @ sp_c

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Save current context to the above defined memorylocation
    .macro __save_context
    /* OK, get the crashdata */
    ldr     sp, =crashdata                  @ set stackpointer to buffer
    stmia   sp, {r0-r12}                    @ save context to buffer
    mov     r0, lr                          @ save linkregister
    /* Next, save state LR and SP as they where when the exception occured.
       To do this, we have to switch state */
    mrs     r5, CPSR                        @ first save the current state
    mrs     r6, SPSR                        @ save state as it was when the exception occured
    mov     r1, r6
    /* If the exception occured in usermode, we have to switch to system mode instead.
       Otherwise we won't be able to switch back once we're done (and user and sys share
       their registers, so we still get what we want) */
    tst     r6, #0xF                        @ check in which mode the CPU was during the exception
    orreq   r6, r6, #0xF                    @ was user, change to sys
    bic     r6, r6, #0x20                   @ make sure we get there in ARM mode (thus clear the Thumb bit)
    msr     cpsr_c, r6                      @ OK, do the switch
    mov     r2, lr                          @ get linkregister
    mov     r3, sp                          @ and stackpointer
    msr     cpsr_c, r5                      @ and switch back
    ldr     sp, =__crashdata_special        @ set stackpointer to buffer
    stmia   sp, {r0-r3}                     @ save LR, SPSR and LR an SP from the exceptionstate
    .endm

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Undefined instruction
    .global __HaltUndef_SC
__HaltUndef_SC:
    ldr     sp, =__stack_start_undefined__
    stmfd   sp!, {r0-r3,r12,lr}     @ Save current context on stack
    __save_context                  @ Save context to RAM
    ldr     sp, =__stack_start_undefined__
    ldmfd   sp!, {r0-r3,r12,lr}     @ Restore context
    #ifdef __THUMB
    ldr     r2, =__HaltUndef
    bx      r2
    #else
    b       __HaltUndef
    #endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prefetch abort
    .global __HaltPabort_SC
__HaltPabort_SC:
    ldr     sp, =__stack_start_abort__
    stmfd   sp!, {r0-r3,r12,lr}     @ Save current context on stack
    __save_context                  @ Save context to RAM
    ldr     sp, =__stack_start_abort__
    ldmfd   sp!, {r0-r3,r12,lr}     @ Restore context
    #ifdef __THUMB
    ldr     r2, =__HaltPabort
    bx      r2
    #else
    b       __HaltPabort
    #endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Data abort
    .global __HaltDabort_SC
__HaltDabort_SC:
    ldr     sp, =__stack_start_abort__
    stmfd   sp!, {r0-r3,r12,lr}     @ Save current context on stack
    __save_context                  @ Save context to RAM
    ldr     sp, =__stack_start_abort__
    ldmfd   sp!, {r0-r3,r12,lr}     @ Restore context
    #ifdef __THUMB
    ldr     r2, =__HaltDabort
    bx      r2
    #else
    b       __HaltDabort
    #endif

.end
