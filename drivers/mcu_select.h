/*
    ALDS (ARM LPC Driver Set)

    mcu_select.h:
                 lpc2000 RAM/ROM and clock defines

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
LPC2000 RAM/ROM and clock defines
*/
#ifndef SELECT_H
#define SELECT_H

/* These are all the MCU's from the LPC21xx series at this moment.
   Note that ALDS doesn't support all of these completely, or at all even. */
#define LPC2101             101
#define LPC2102             102
#define LPC2103             103
#define LPC2104             104
#define LPC2105             105
#define LPC2106             106
#define LPC2114             114
#define LPC2119             119
#define LPC2124             124
#define LPC2129             129
#define LPC2131             131
#define LPC2132             132
#define LPC2134             134
#define LPC2136             136
#define LPC2138             138
#define LPC2141             141
#define LPC2142             142
#define LPC2144             144
#define LPC2146             146
#define LPC2148             148
#define LPC2194             194

/* Memory range defines. These are the same for all LPC2000 MCU's */
#define __MCU_ROM_BASE      0x00000000
#define __MCU_RAM_BASE      0x40000000
#define __MCU_EXT_BASE      0x80000000
#define __MCU_APB_BASE      0xE0000000
#define __MCU_AHB_BASE      0xF0000000

/* MCU specific ROM/RAM sizes in kbytes (1024 bytes).
   NOTE: Some datasheets give the sizes with one kb being 1000 bytes! */
#if (__MCU == LPC2101)
#define __MCU_RAM_SIZE      2
#define __MCU_ROM_SIZE      8
#elif (__MCU == LPC2102)
#define __MCU_RAM_SIZE      4
#define __MCU_ROM_SIZE      16
#elif (__MCU == LPC2103)
#define __MCU_RAM_SIZE      8
#define __MCU_ROM_SIZE      32
#elif (__MCU == LPC2104)
#define __MCU_RAM_SIZE      16
#define __MCU_ROM_SIZE      128
#elif (__MCU == LPC2105)
#define __MCU_RAM_SIZE      32
#define __MCU_ROM_SIZE      128
#elif (__MCU == LPC2106)
#define __MCU_RAM_SIZE      64
#define __MCU_ROM_SIZE      128
#elif (__MCU == LPC2131)
#define __MCU_RAM_SIZE      8
#define __MCU_ROM_SIZE      31
#elif (__MCU == LPC2132)
#define __MCU_RAM_SIZE      16
#define __MCU_ROM_SIZE      62
#elif (__MCU == LPC2134)
#define __MCU_RAM_SIZE      32
#define __MCU_ROM_SIZE      125
#elif (__MCU == LPC2136)
#define __MCU_RAM_SIZE      32
#define __MCU_ROM_SIZE      250
#elif (__MCU == LPC2138)
#define __MCU_RAM_SIZE      32
#define __MCU_ROM_SIZE      500
#elif (__MCU == LPC2141)
#define __MCU_RAM_SIZE      8
#define __MCU_ROM_SIZE      31
#elif (__MCU == LPC2142)
#define __MCU_RAM_SIZE      16
#define __MCU_ROM_SIZE      62
#elif (__MCU == LPC2144)
#define __MCU_RAM_SIZE      32
#define __MCU_ROM_SIZE      125
#elif (__MCU == LPC2146)
#define __MCU_RAM_SIZE      32      /* Can add 8 kB from the USB DMA */
#define __MCU_ROM_SIZE      250
#elif (__MCU == LPC2148)
#define __MCU_RAM_SIZE      32      /* Can add 8 kB from the USB DMA */
#define __MCU_ROM_SIZE      500
#else
    /* Dunno..
       Stop right here, since unknown RAM/ROM sizes is guaranteed to cause trouble further down the compilation process anyways */
    #error "Don't know RAM/ROM sizes of this MCU!"
#endif

/* CPU clock values */
#define FOSC                (__FOSC * 1000)
#define CCLK                (FOSC * PLL_MUL)
#define FCCO_MAX            (320000000)
#define PLL_DIV             (FCCO_MAX / (2 * CCLK))
#define FCCO                (CCLK * 2 * PLL_DIV)

/* MCU specific clock parameters
   - __MCU_MAX_CCLK     Max. CPU clock frequency
   - __MCU_MIN_FOSC     Min. cristal frequency without PLL (and thus also min CCLK value)
   - __MCU_MAX_FOSC     Max. cristal frequency without PLL
   - __MCU_MIN_PLL_FOSC Min. cristal frequency with PLL
   - __MCU_MAX_PLL_FOSC Max. cristal frequency with PLL */
#if (__MCU >= LPC2101 && __MCU <= LPC2103)
#define __MCU_MAX_CCLK      70000000
#define __MCU_MIN_FOSC      1000000
#define __MCU_MAX_FOSC      25000000
#define __MCU_MIN_PLL_FOSC  10000000
#define __MCU_MAX_PLL_FOSC  25000000
#elif (__MCU >= LPC2104 && __MCU <= LPC2106)
#define __MCU_MAX_CCLK      60000000
#define __MCU_MIN_FOSC      10000000
#define __MCU_MAX_FOSC      25000000
#define __MCU_MIN_PLL_FOSC  10000000
#define __MCU_MAX_PLL_FOSC  25000000
#elif (__MCU >= LPC2131 && __MCU <= LPC2148)
#define __MCU_MAX_CCLK      60000000
#define __MCU_MIN_FOSC      1000000
#define __MCU_MAX_FOSC      30000000
#define __MCU_MIN_PLL_FOSC  10000000
#define __MCU_MAX_PLL_FOSC  25000000
#else
    /* Dunno..
       Stop right here, since unknown clock parameters is guaranteed to cause trouble further down the compilation process anyways */
    #error "Don't know clock parameters of this MCU!"
#endif

/* MCU flashlayout. There are currently two sector layout scheme's used by the LPC2000 MCU's.
   Following list shows the devices with the layout they use (source: OpenOCD sourcecode)
    variant 1:
        - 2104|5|6
        - 2114|9
        - 2124|9
        - 2194
        - 2212|4
        - 2292|4
    variant 2:
        - 2101|2|3
        - 213x
        - 214x
        - 2364|6|8
        - 2378
  Here we set the following two parameters:
   - __MCU_FLASHTYPE    The flash variant
   - __MCU_FLASHSECTORS The amount of sectors of the whole flash
  Both will be 0 if the flashvariant is unknown. Since this is not a critical error, we silently ignore this.
  Note that an unknown flashvariant will disable the IAP driver, and 'make load' and 'make debug' won't work
  when running from flash. */
#if (__MCU >= LPC2104 && __MCU <= LPC2106) || __MCU == LPC2114 || __MCU == LPC2119 || __MCU == LPC2124 || __MCU == LPC2129 || __MCU == LPC2194
#define __MCU_FLASHTYPE         1
#define __MCU_FLASHSECTORS      ((__MCU_ROM_SIZE*1024)>>13)
#elif (__MCU >= LPC2101 && __MCU <= LPC2103) || (__MCU >= LPC2131 && __MCU <= LPC2138) || (__MCU >= LPC2141 && __MCU <= LPC2148)
#define __MCU_FLASHTYPE         2
#if (__MCU_ROM_SIZE <= 32)
#define __MCU_FLASHSECTORS      ((__MCU_ROM_SIZE*1024)>>12)
#elif (__MCU_ROM_SIZE <= 480)
#define __MCU_FLASHSECTORS      (8 + (((__MCU_ROM_SIZE*1024) - 32768)>>15))
#else
#define __MCU_FLASHSECTORS      (22 + (((__MCU_ROM_SIZE*1024) - 491520)>>12))
#endif
#else
/* Don't know the variant? */
#define __MCU_FLASHTYPE         0
#define __MCU_FLASHSECTORS      0
#endif

#endif /* SELECT_H */
