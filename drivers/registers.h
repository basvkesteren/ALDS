/*
    ALDS (ARM LPC Driver Set)

    registers.h:
              LPC2000 register definitions

    copyright:
              dunno.. This is a compilation of several LPC2000 header-files, so
              there are several copyright-holders here, i guess.. No idea who..

              Either way; this file is written/compiled/layout-ed (sp?) by me:

              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -This file contains defines for the following MCU's:
                LPC2101
                LPC2102
                LPC2103
                LPC2104
                LPC2105
                LPC2106
                LPC2131
                LPC2132
                LPC2134
                LPC2136
                LPC2138
                LPC2141
                LPC2142
                LPC2144
                LPC2146
                LPC2148
             More LPC21xx MCU's might be added when I get one ;-)
            -Bugfixes by Tarmigan; thanks!
*/
/*!
\file
LPC2000 register definitions
*/
#ifndef REGISTERS_H
#define REGISTERS_H

#ifndef  __ASSEMBLER__

/*
  ARM local bus address space
*/

/* Fast General Purpose Input/Output (GPIO); about 3.5 times faster than GPIO */
#define FIODIR          (*(volatile unsigned long *) (0x3fffc000))
#define FIOMASK         (*(volatile unsigned long *) (0x3fffc010))
#define FIOPIN          (*(volatile unsigned long *) (0x3fffc014))
#define FIOSET          (*(volatile unsigned long *) (0x3fffc018))
#define FIOCLR          (*(volatile unsigned long *) (0x3fffc01c))

/* Fast GPIO registerset 0
   These are exactly the same as the above fast GPIO defines. They're here for
   compatibility with __MCU's with more than one fast GPIO registerset (e.g. more than 32 pins) */
#define FIODIR0         (*(volatile unsigned long *) (0x3fffc000))
#define FIOMASK0        (*(volatile unsigned long *) (0x3fffc010))
#define FIOPIN0         (*(volatile unsigned long *) (0x3fffc014))
#define FIOSET0         (*(volatile unsigned long *) (0x3fffc018))
#define FIOCLR0         (*(volatile unsigned long *) (0x3fffc01c))

/* Fast GPOI registerset 1 */
#define FIODIR1         (*(volatile unsigned long *) (0x3fffc020))
#define FIOMASK1        (*(volatile unsigned long *) (0x3fffc030))
#define FIOPIN1         (*(volatile unsigned long *) (0x3fffc034))
#define FIOSET1         (*(volatile unsigned long *) (0x3fffc038))
#define FIOCLR1         (*(volatile unsigned long *) (0x3fffc03c))

/*
  APB address space
*/

/* Watchdog */
#define WDMOD           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x00000))      //Mode register
#define WDTC            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x00004))      //timer constant register
#define WDFEED          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x00008))      //feed sequence register
#define WDTV            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0000c))      //timer value register

/* Timer 0 */
#define T0IR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04000))      //Interrupt Register
#define T0TCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04004))      //Timer Control
#define T0TC            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04008))      //Timer Counter
#define T0PR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0400c))      //Prescale Register
#define T0PC            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04010))      //Prescale Counter
#define T0MCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04014))      //Match Control
#define T0MR0           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04018))      //Match Register 0
#define T0MR1           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0401c))      //Match Register 1
#define T0MR2           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04020))      //Match Register 2
#define T0MR3           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04024))      //Match Register 3
#define T0CCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04028))      //Capture Control
#define T0CR0           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0402c))      //Capture Register 0
#define T0CR1           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04030))      //Capture Register 1
#define T0CR2           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04034))      //Capture Register 2
#define T0CR3           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04038))      //Capture Register 3
#define T0EMR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0403c))      //External Match
#define T0CTCR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x04070))      //Count Control Register
#define T0PWMCON        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x04074))      //PWM Control Register

/* Timer 1 */
#define T1IR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08000))      //Interrupt Register
#define T1TCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08004))      //Timer Control
#define T1TC            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08008))      //Timer Counter
#define T1PR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0800c))      //Prescale Register
#define T1PC            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08010))      //Prescale Counter
#define T1MCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08014))      //Match Control
#define T1MR0           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08018))      //Match Register 0
#define T1MR1           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0801c))      //Match Register 1
#define T1MR2           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08020))      //Match Register 2
#define T1MR3           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08024))      //Match Register 3
#define T1CCR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08028))      //Capture Control
#define T1CR0           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0802c))      //Capture Register 0
#define T1CR1           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08030))      //Capture Register 1
#define T1CR2           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08034))      //Capture Register 2
#define T1CR3           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08038))      //Capture Register 3
#define T1EMR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x0803c))     //External Match
#define T1CTCR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x08070))      //Count Control Register
#define T1PWMCON        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x08074))      //PWM Control Register

/* UART0 */
#define U0RBR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c000))      //Receiver Buffer
#define U0THR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c000))      //Transmit Holding
#define U0IER           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0c004))      //Interrupt Enable
#define U0IIR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0c008))      //Interrupt ID
#define U0FCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c008))      //FIFO Control
#define U0LCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c00c))      //Line Control
#define U0LSR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c014))      //Line Status
#define U0SCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c01c))      //Scratch Pad
// DLAB = 1
#define U0DLL           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c000))      //Divisor Latch LSB
#define U0DLM           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c004))      //Divisor Latch MSB
#define U0ACR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0c020))      //Auto baud control
#define U0FDR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x0c028))      //Fractional divider
#define U0TER           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x0c030))      //TX enable

/* UART1 */
#define U1RBR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10000))      //Receiver Buffer
#define U1THR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10000))      //Transmit Holding
#define U1IER           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x10004))      //Interrupt Enable
#define U1IIR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x10008))      //Interrupt ID
#define U1FCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10008))      //FIFO Control
#define U1LCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1000c))      //Line Control
#define U1MCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10010))      //Modem Control
#define U1LSR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10014))      //Line Status
#define U1MSR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10018))      //Modem Status
#define U1SCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1001c))      //Scratch Pad
// DLAB = 1
#define U1DLL           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10000))      //Divisor Latch LSB
#define U1DLM           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10004))      //Divisor Latch MSB
#define U1ACR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x10020))      //Auto baud control
#define U1FDR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x10028))      //Fractional divider
#define U1TER           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x10030))      //TX enable

/* PWM */
#define PWMIR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14000))      //Interrupt Register
#define PWMTCR          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14004))      //Timer Control
#define PWMTC           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14008))      //Timer Counter
#define PWMPR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1400c))      //Prescale Register
#define PWMPC           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14010))      //Prescale Counter
#define PWMMCR          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14014))      //Match Control
#define PWMMR0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14018))      //Match Register 0
#define PWMMR1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1401c))      //Match Register 1
#define PWMMR2          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14020))      //Match Register 2
#define PWMMR3          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14024))      //Match Register 3
#define PWMMR4          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14040))      //Match Register 4
#define PWMMR5          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14044))      //Match Register 5
#define PWMMR6          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14048))      //Match Register 6
#define PWMPCR          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1404c))      //PWM Control
#define PWMLER          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x14050))      //Latch Enable

/* Inter-Integrated Circuit interface (I2C) */
#define I2CONSET        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C000))      //I2C Control Set
#define I2STAT          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C004))      //I2C Status
#define I2DAT           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C008))      //I2C Data
#define I2ADR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C00c))      //I2C Slave Address
#define I2SCLH          (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1C010))     //SCL Duty Cycle Register High Half Word
#define I2SCLL          (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1C014))     //SCL Duty Cycle Register Low Half Word
#define I2CONCLR        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C018))      //I2C Control Clear

/* Inter-Integrated Circuit interface 0 (I2C0)
   These are exactly the same as the I2C defines. They're here for
   compatibility with __MCU's with more than one I2C */
#define I2C0CONSET      (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C000))      //I2C Control Set
#define I2C0STAT        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C004))      //I2C Status
#define I2C0DAT         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C008))      //I2C Data
#define I2C0ADR         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C00C))      //I2C Slave Address
#define I2C0SCLH        (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1C010))     //SCL Duty Cycle Register High Half Word
#define I2C0SCLL        (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1C014))     //SCL Duty Cycle Register Low Half Word
#define I2C0CONCLR      (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1C018))      //I2C Control Clear

/* SPI */
#define SPCR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x20000))      //SPI Control
#define SPSR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x20004))      //SPI Status
#define SPDR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x20008))      //SPI Data
#define SPCCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x2000c))      //SPI Clock Counter
#define SPINT           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x2001c))      //SPI Interrupt Flag

/* RTC */
#define ILR             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24000))      //Interrupt Location Register
#define CTC             (*(volatile unsigned short *)(__MCU_APB_BASE + 0x24004))     //Clock Tick Counter
#define CCR             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24008))      //Clock Control Register
#define CIIR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x2400c))      //Counter Increment Interrupt Register
#define AMR             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24010))      //Alarm Mask Register
#define CT1ME0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x24014))      //Consolidated Time Register 0
#define CT1ME1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x24018))      //Consolidated Time Register 1
#define CT1ME2          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2401c))      //Consolidated Time Register 2
#define SEC             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24020))      //Seconds Register
#define MIN             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24024))      //Minutes Register
#define HOUR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24028))      //Hours Register
#define DOM             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x2402c))      //Day of Month Register
#define DOW             (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24030))      //Day of Week Register
#define DOY             (*(volatile unsigned short *)(__MCU_APB_BASE + 0x24034))     //Day of Year Register
#define MONTH           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24038))      //Months Register
#define YEAR            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x2403c))     //Years Register
#define ALSEC           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24060))      //Alarm value for Seconds
#define ALMIN           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24064))      //Alarm value for Minutes
#define ALHOUR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24068))      //Alarm value for Hours
#define ALDOM           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x2406c))      //Alarm value for Day of Month
#define ALDOW           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24070))      //Alarm value for Day of Week
#define ALDOY           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x24074))     //Alarm value for Day of Year
#define ALMONTH         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x24078))      //Alarm value for Months
#define ALYEAR          (*(volatile unsigned short *)(__MCU_APB_BASE + 0x2407c))     //Alarm value for Year
#define PREINT          (*(volatile unsigned short *)(__MCU_APB_BASE + 0x24080))     //Prescale value, integer portion
#define PREFRAC         (*(volatile unsigned short *)(__MCU_APB_BASE + 0x24084))     //Prescale value, fractional portion

/* GPIO */
#define IOPIN           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28000))      //GPIO pin value
#define IOSET           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28004))      //Output value SET
#define IODIR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28008))      //Direction control
#define IOCLR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2800c))      //Output value CLEAR

/* GPIO registerset 0
   These are exactly the same as the above GPIO defines. They're here for
   compatibility with __MCU's with more than one GPIO register set (e.g more than 32 pins) */
#define IOPIN0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28000))      //GPIO pin value
#define IOSET0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28004))      //Output value SET
#define IODIR0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28008))      //Direction control
#define IOCLR0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2800c))      //Output value CLEAR

/* GPIO registerset 1 */
#define IOPIN1          (*(volatile unsigned long *)(__MCU_APB_BASE+ 0x28010))       //GPIO pin value
#define IOSET1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28014))      //Output value SET
#define IODIR1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x28018))      //Direction control
#define IOCLR1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2801c))      //Output value CLEAR

/* Pin Connect Block */
#define PINSEL0         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2c000))      //Pin function select 0
#define PINSEL1         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2c004))      //Pin function select 1
#define PINSEL2         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x2c014))      //Pin function select 2

/* Analog/Digital Converter (ADC) */
#define ADCR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34000))
#define ADGDR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34004))
#define ADINTEN         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3400C))
#define ADDRArray       ((volatile unsigned long *)(__MCU_APB_BASE + 0x34010))       //An array of the ADDRx registers
#define ADDR0           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34010))
#define ADDR1           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34014))
#define ADDR2           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34018))
#define ADDR3           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3401C))
#define ADDR4           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34020))
#define ADDR5           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34024))
#define ADDR6           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34028))
#define ADDR7           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3402C))
#define ADSTAT          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34030))


/* A/D Converter 0 (ADC0)
   These are exactly the same as the ADC defines. They're here for
   compatibility with __MCU's with more than one ADC */
#define AD0CR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34000))
#define AD0GDR          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34004))
#define ADGSR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34008))      //A/D Converter Global
#define AD0INTEN        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3400C))
#define AD0DRArray      ((volatile unsigned long *)(__MCU_APB_BASE + 0x34010))       //An array of the AD0DRx registers
#define AD0DR0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34010))
#define AD0DR1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34014))
#define AD0DR2          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34018))
#define AD0DR3          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3401C))
#define AD0DR4          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34020))
#define AD0DR5          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34024))
#define AD0DR6          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34028))
#define AD0DR7          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x3402C))
#define AD0STAT         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x34030))

/* Inter-Integrated Circuit interface 1 (I2C1) */
#define I2C1CONSET      (*(volatile unsigned char *)(__MCU_APB_BASE + 0x5C000))      //I2C Control Set
#define I2C1STAT        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x5C004))      //I2C Status
#define I2C1DAT         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x5C008))      //I2C Data
#define I2C1ADR         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x5C00C))      //I2C Slave Address
#define I2C1SCLH        (*(volatile unsigned short *)(__MCU_APB_BASE + 0x5C010))     //SCL Duty Cycle Register High Half Word
#define I2C1SCLL        (*(volatile unsigned short *)(__MCU_APB_BASE + 0x5C014))     //SCL Duty Cycle Register Low Half Word
#define I2C1CONCLR      (*(volatile unsigned char *)(__MCU_APB_BASE + 0x5C018))      //I2C Control Clear

/* A/D Converter (ADC) */
#define AD1CR           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60000))
#define AD1GDR          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60004))
#define AD1STAT         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60030))
#define AD1INTEN        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x6000C))
#define AD1DRArray      ((volatile unsigned long *)(__MCU_APB_BASE + 0x60010))       //An array of the AD1DRx registers
#define AD1DR0          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60010))
#define AD1DR1          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60014))
#define AD1DR2          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60018))
#define AD1DR3          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x6001C))
#define AD1DR4          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60020))
#define AD1DR5          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60024))
#define AD1DR6          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x60028))
#define AD1DR7          (*(volatile unsigned long *)(__MCU_APB_BASE + 0x6002C))

/* Synchronous Serial Port interface (SSP) */
#define SSPCR0          (*(volatile unsigned short*)(__MCU_APB_BASE + 0x68000))
#define SSPCR1          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x68004))
#define SSPDR           (*(volatile unsigned short*)(__MCU_APB_BASE + 0x68008))
#define SSPSR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x6800C))
#define SSPCPSR         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x68010))
#define SSPIMSC         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x68014))
#define SSPRIS          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x68018))
#define SSPMIS          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x6801C))
#define SSPICR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x68020))

/* D/A converter (DAC) */
#define DACR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x6c000))

/* Timer 2 */
#define T2IR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x70000))
#define T2TCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x70004))
#define T2TC            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70008))
#define T2PR            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7000C))
#define T2PC            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70010))
#define T2MCR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70014))
#define T2MR0           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70018))
#define T2MR1           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7001C))
#define T2MR2           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70020))
#define T2MR3           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70024))
#define T2CCR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70028))
#define T2CR0           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7002C))
#define T2CR1           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70030))
#define T2CR2           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x70034))
#define T2EMR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7003C))
#define T2CTCR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x70070))
#define T2PWMCON        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x70074))

/* Timer 3 */
#define T3IR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x74000))
#define T3TCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x74004))
#define T3TC            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74008))
#define T3PR            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7400C))
#define T3PC            (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74010))
#define T3MCR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74014))
#define T3MR0           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74018))
#define T3MR4           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7401C))
#define T3MR2           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74020))
#define T3MR3           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74024))
#define T3CCR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74028))
#define T3CR0           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7402C))
#define T3CR1           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74030))
#define T3CR2           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x74034))
#define T3EMR           (*(volatile unsigned short *)(__MCU_APB_BASE + 0x7403C))
#define T3CTCR          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x74070))
#define T3PWMCON        (*(volatile unsigned long *)(__MCU_APB_BASE + 0x74074))

/* Memory Accelerator Module (MAM) */
#define MAMCR           (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc000))     //Control Register
#define MAMTIM          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc004))     //Timing control
#define MEMMAP          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc040))     //Memory mapping control

/* Phase Locked Loop (PLL) */
#define PLLCON          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc080))     //PLL control
#define PLLCFG          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc084))     //PLL configuration
#define PLLSTAT         (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1fc088))    //PLL status
#define PLLFEED         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc08c))     //PLL feed

/* Phase Locked Loop 0 (PLL0)
   These are exactly the same as the PLL defines. They're here for
   compatibility with __MCU's with more than one PLL */
#define PLL0CON         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc080))     //PLL control
#define PLL0CFG         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc084))     //PLL configuration
#define PLL0STAT        (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1fc088))    //PLL status
#define PLL0FEED        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc08c))     //PLL feed

/* Phase Locked Loop 1 (PLL1), used for the USB clock */
#define PLL1CON         (*((volatile unsigned char *)(__MCU_APB_BASE + 0x1fc0A0))    //PLL control
#define PLL1CFG         (*((volatile unsigned char *)(__MCU_APB_BASE + 0x1fc0A4))    //PLL configuration
#define PLL1STAT        (*((volatile unsigned short*)(__MCU_APB_BASE + 0x1fc0A8))    //PLL status
#define PLL1FEED        (*((volatile unsigned char *)(__MCU_APB_BASE + 0x1fc0Ac))    //PLL feed

/* Power Control */
#define PCON            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc0c0))     //Power control
#define PCONP           (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1fc0c4))     //Power control for peripherals
#define INTWAKE         (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1fc144))    //External interrupt wakeup

/* VPB Divider */
#define VPBDIV          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc100))     //VPB divider control

/* External Interrupts */
#define EXTINT          (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc140))     //External interrupt flag
#define EXTWAKE         (*(volatile unsigned short *)(__MCU_APB_BASE + 0x1fc144))    //External interrupt wakeup    (NOTE: same as INTWAKE; on older LPC's (LPC2106 for example,
                                                                                     //                                    EXTWAKE was used for eint pins only. On newer devices other
                                                                                     //                                    sources can be used aswell (RTC, USB, e.d.), so this register
                                                                                     //                                    was renamed to INTWAKE
#define EXTMODE         (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc148))     //External interrupt mode
#define EXTPOLAR        (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc14c))     //External interrupt polarity

/* Reset Source Identification */
#define RSIR            (*(volatile unsigned char *)(__MCU_APB_BASE + 0x1fc180))

/* Code Security Protection */
#define CPSR            (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1fc184))

/* AHB configuration */
#define AHBCFG1         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1fc188))
#define AHBCFG2         (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1fc18c))

/* Syscon Miscellaneous */
#define SCS             (*(volatile unsigned long *)(__MCU_APB_BASE + 0x1fc1a0))

/*
  AHB address space
*/

/* VIC */
#define VICIRQStatus    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff000))    //IRQ Status
#define VICFIQStatus    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff004))    //FIQ Status
#define VICRawIntr      (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff008))    //Raw Interrupt Status
#define VICIntSelect    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff00c))    //Interrupt Select
#define VICIntEnable    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff010))    //Interrupt Enable
#define VICIntEnClr     (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff014))    //Interrupt Enable Clear
#define VICSoftInt      (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff018))    //Software Interrupt
#define VICSoftIntClear (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff01c))    //Software Interrupt Clear
#define VICProtection   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff020))    //Protection enable
#define VICVectAddr     (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff030))    //Vector Address
#define VICDefVectAddr  (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff034))    //Default Vector Address
#define VICVectAddrArray   ((volatile unsigned long *)(__MCU_AHB_BASE + 0xffff100))  //An array of the vector address registers
#define VICVectAddr0    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff100))    //Vector address 0 register
#define VICVectAddr1    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff104))    //Vector address 1 register
#define VICVectAddr2    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff108))    //Vector address 2 register
#define VICVectAddr3    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff10c))    //Vector address 3 register
#define VICVectAddr4    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff110))    //Vector address 4 register
#define VICVectAddr5    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff114))    //Vector address 5 register
#define VICVectAddr6    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff118))    //Vector address 6 register
#define VICVectAddr7    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff11c))    //Vector address 7 register
#define VICVectAddr8    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff120))    //Vector address 8 register
#define VICVectAddr9    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff124))    //Vector address 9 register
#define VICVectAddr10   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff128))    //Vector address 10 register
#define VICVectAddr11   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff12c))    //Vector address 11 register
#define VICVectAddr12   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff130))    //Vector address 12 register
#define VICVectAddr13   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff134))    //Vector address 13 register
#define VICVectAddr14   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff138))    //Vector address 14 register
#define VICVectAddr15   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff13c))    //Vector address 15 register
#define VICVectCntlArray   ((volatile unsigned long *)(__MCU_AHB_BASE + 0xffff200))  //An array of the vector control registers
#define VICVectCntl0    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff200))    //Vector control 0 register
#define VICVectCntl1    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff204))    //Vector control 1 register
#define VICVectCntl2    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff208))    //Vector control 2 register
#define VICVectCntl3    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff20c))    //Vector control 3 register
#define VICVectCntl4    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff210))    //Vector control 4 register
#define VICVectCntl5    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff214))    //Vector control 5 register
#define VICVectCntl6    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff218))    //Vector control 6 register
#define VICVectCntl7    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff21c))    //Vector control 7 register
#define VICVectCntl8    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff220))    //Vector control 8 register
#define VICVectCntl9    (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff224))    //Vector control 9 register
#define VICVectCntl10   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff228))    //Vector control 10 register
#define VICVectCntl11   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff22c))    //Vector control 11 register
#define VICVectCntl12   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff230))    //Vector control 12 register
#define VICVectCntl13   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff234))    //Vector control 13 register
#define VICVectCntl14   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff238))    //Vector control 14 register
#define VICVectCntl15   (*(volatile unsigned long *)(__MCU_AHB_BASE + 0xffff23c))    //Vector control 15 register

#else

/* As you can see, this is terribly incomplete. These are currently the only registers used from asm, so
   it suffices for now... */

/*
  ARM local bus address space
*/

/*
  APB address space
*/

MEMMAP              = __MCU_APB_BASE + 0x1fc040
VICDefVectAddr      = __MCU_AHB_BASE + 0xffff034
VICVectAddr         = __MCU_AHB_BASE + 0xffff030

/*
  AHB address space
*/

#endif /* __ASSEMBLER__ */

#endif /* REGISTERS_H */
