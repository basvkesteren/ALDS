/*
    ALDS (ARM LPC Driver Set)

    config.h:
             Configuration and general defines

    copyright:
              Copyright (c) 2006-2008 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -

*/
/*!
\file
Configuration and general defines
*/
#ifndef CONFIG_H
#define CONFIG_H

/********************
  Clock settings
  Note that the actual crystal frequency is set in the makefile!
*/

/*! PLL multiplier. possible values from 1 to 32 */
#define PLL_MUL             1

/*! Peripheral Bus Speed Divider. Can be 1, 2, or 4 */
#define PBSD                1

/********************
  Runmode configuration
*/

/*! As you might know, the ARM7 CPU has several runmodes.
    These runmodes can be used to provide a certain level
    of protection against buggy software.

    Possible options are:
      USER_MODE
          In this case all code runs in usermode, which means this
          software cannot modify the lower bits of the CPSR register
          (which set the I and F bit, ARM or Thumb mode and the
          current CPU runmode).
          When code running in usermode has to change one of these
          bits, this has to be done through a so called supervisor
          call (aka a software interrupt, or SWI).
      SUPERVISOR_MODE
          Protected mode, useful for operating systems
      SYSTEM_MODE
          Privileged mode; the default

    See the file drivers/swi_ARM.S for the SWI handler. */
#define RUNMODE             SYSTEM_MODE

/*! When the above is enabled, it is sort of required to have a SWI
    handler to be able to handle the supervisor calls. (unless,
    of course, you've made a program which doesn't need those at all..)

    On the other hand, when the above is disabled, the default
    drivercode doesn't need the SWI handling at all, so this will only
    make the RAM and ROM usage higher.

    Either way, with the following setting set to '1', all SWI calls
    will end up in the SWI handler found in the file drivers/swi_ARM.S.
    If this setting is disabled ('0'), SWI's will end up in the SWI
    exception handler (see drivers/exceptions.c), which basically halts
    program execution. */
#define HANDLE_SWI          0

/********************
  Debug configuration
*/

/*! This setting defines the 'debug-level'. Possible values are:
     D_NONE      Disable all debug messages
     D_ERROR     Only enable error messages
     D_WARN      Enable both error- and warning-messages
     D_INFO      Besides the above, also enable more verbose messages
     D_XINFO     Be as verbose as possible
    When DEBUG == D_NONE, debug is not initialized. For all other settings; it's
    up to the programmer (you?) to make use of it..
    For debug output settings, see config-debug.h. */
#define DEBUG               D_ERROR

/*! When this is set to '1', the ASSERT macro is enabled. When set to
    '0' ASSERT is disabled (it'll be an empty macro) .
    Note that the default assert macro uses dprint() for the output,
    so DEBUG should be enabled to make this useful. */
#define ASSERT_ENABLED      1

/*! When ASSERT_ENABLED is set, and this is enabled, the callback_assert()
    (See callback.c) function is called when an ASSERT() is executed with a
    non-true expression. */
#define ASSERT_CALLBACK     1

/********************
  Exception handling
*/

/*! This setting defines what happens when the system encounters a 'crash'.
    It is a bitmask of one or more of the following values:
     C_REBOOT            Reboot. When any of the following bits are set, the reboot
                         happens after these are done.
     C_CALLBACK          Call the callback_crash() function (to be written by the
                         user; this function doesn't do anything by default. See callback.c)
     C_WARN              Issue a warning on the debug console
     C_CONTEXT           Print the CPU context as it was during the crash
     C_TRACE             Do a backtrace on the context as it was during the crash

    The C_WARN, C_CONTEXT and C_TRACE use the dprint() function, so DEBUG needs
    to be D_ERROR or higher.
    Note that C_TRACE requires C_CONTEXT to be enabled. Setting C_TRACE without
    setting C_CONTEXT won't give you a trace
    Also note that using the backtrace functionality successfully requires some
    special compilation settings. See the file drivers/backtrace.c for
    more information. */
#define CRASHACTION         (C_WARN | C_CONTEXT | C_TRACE | C_CALLBACK | C_REBOOT)

/********************
  Miscellaneous settings
*/

/*! This defines the watchdog counter value. This is calculated with
    tPCLK x interval x 4, which results in the timeout in seconds.
    Use a value of '0' to disable the watchdog. */
#define WATCHDOG_TIMEOUT    0

/*! Some drivers contain wait-loops, used to wait for an interrupt-based event to
    occur. It is possible to put the CPU to sleep in these waitloops to preserve
    power. When the following setting is enabled, the CPU will go in idle-mode
    in these wait-loops.
    Note that this isn't used very much yet, so only enable this if you like to
    test it. */
#define SLEEPWHENWAITING    1

/*! Some newer LPC2000 devices support so called 'fast-I/O', where the I/O is
    addressed though the local bus instead of the (slower) APB.
    When the following is set, 'fast-I/O' will be used if available.
    Note that this only works on code using the macro's as found in io.h */
#define IO_USEFASTIO        1

/*! Enable code protection? When enabled, it's not possible to read the flash contents.
    Ignored when running from RAM.
    Note that LPC2K_PGM seems to have trouble erasing flashsectors when this is enabled.
    FlashMagic works as supposed. */
#define CODEPROTECTION      0

/*! When the following is enabled, it's possible to detect a soft reset. The function
    getresetcause() can return RESET_POWERUP (hard-reset), RESET_WATCHDOG (watchdog timeout)
    or RESET_SOFT (software reset). To be able to detect a software reset, a location in RAM
    is reserved, and written with a specific value. At startup this location is checked; if
    it's value doesn't match the specific value, a hard reset has occured.
    With this setting disabled, getresetcause() will never return RESET_SOFT, obviously since
    that case cannot be detected. */
#define POWERLOSSDETECTION  1

/********************
  The stack
  NOTE: some runmodes require a minimal stacksize, depending on some configuration-parameters
  (read on below for the details). If these are set too small, they'll automatically be enlarged at link-time.
  Therefore, do not use these defines for assumptions/calculations in your own code; instead use the
  defines from the linkerscript, since these always have the actually used value.
  These defines are:
    __STACK_SIZE_FIQ__
    __STACK_SIZE_IRQ__
    __STACK_SIZE_SUPERVISOR__
    __STACK_SIZE_ABORT__
    __STACK_SIZE_UNDEFINED__
    __STACK_SIZE_SYS__
*/

/*! Stacksize for FIQ mode. Not used by default, so minimal size */
#define STACKSIZE_FIQ       0x4
/*! Stacksize for IRQ mode. Used quite often, so set to a reasonably big size */
#define STACKSIZE_IRQ       0x200
/*! Stacksize for SVC (supervisor) mode. When HANDLE_SWI is enabled (see above),
    the minimal size is 0xC. Otherwise a minimal stacksize is enough */
#define STACKSIZE_SVC       0x4
/*! Stacksize for ABT (abort) mode. When C_CONTEXT is enabled (see CRASHACTION, above),
    the minimal size is 0x18. Otherwise a minimal stacksize is enough */
#define STACKSIZE_ABT       0x18
/*! Stacksize for UND (undefined) mode. When C_CONTEXT is enabled (see CRASHACTION, above),
    the minimal size is 0x18. Otherwise a minimal stacksize is enough */
#define STACKSIZE_UND       0x18
/*! Stacksize for SYS (system) mode. This is the 'normal' stack and thus used
    quite often; it's set to a reasonably big size. */
#define STACKSIZE_SYS       0x200

/*! When set to a value > 0, this value is written to the last word of each used stack.
    The function stack_canary_check() checks if this value is still present;
    this can be used as a stack overflow detection. */
#define STACKCANARY         0xA5A5A5A5

/********************
  Memory layout
  Use with care, only change when you know what it does!
  NOTE: changing the layout in such a way that the BSS is located right below
        the stack causes the 'sbrk()' syscall to stop working (it won't have any
        more memory to use). This call is important for several newlib functions.
        Most noticeably the malloc() functions, but others (like sprintf()) seem
        to need this call too.
*/

/*! The stack is located at the top of RAM; by changing this setting the stack
    will move down, away from the top. For example, setting STACK_OFFSET to 50
    will place the stack 50 bytes under the top, thus freeing 50 bytes at the
    top of the RAM address range).
    Setting this to 0 leaves the stack at the top of RAM. */
#define STACK_OFFSET        0

/*! By default the data section is located right after the program data. The
    following setting can be used to change this location.
    For example, setting this to 0x40005100 will place the start of the data
    at this address.
    Setting this to 0 leaves the data section at it's default location.
    NOTE: a useful symbol here is '__stack_end__', which is the location of
          the end of the stack. */
#define DATA_LOCATION       0

/*! By default the BSS is located right after the data section (also
    when DATA_LOCATION is set) data. The following setting can be used to change
    this location. For example, setting this to 0x40005000 will place the start
    of the BSS at this address.
    Setting this to 0 leaves the BSS at it's default location.
    NOTE: a useful symbol here is '__stack_end__', which is the location of
          the end of the stack. */
#define BSS_LOCATION        0

/********************
  Board settings
*/

/*! ALDS provides some board-specific code for special devices, such as keys
    or an LCD. The following boards are supported:
    - BOARD_HVU_2_3
    - BOARD_HVU_3_2
    When this is set to 0, all board specific code is disabled */
#define BOARD               0

/* Finally, include some stuff (no need to change this, really) */
#include <gcc.h>
#include "drivers/mcu_select.h"

#endif /* CONFIG_H */
