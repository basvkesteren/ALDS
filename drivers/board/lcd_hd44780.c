/*
    LPC21xx core set - Board specific functions

    lcd_hd44780.c:
                  HD44780 compatible LCD-driver HvU testboard 2.3 and 3.2

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

    remarks:
            -All instructions for the display are reversed; this simplifies the code.
            -This code is based on the following code:
                "LCD routines for OLIMEX LPC-MT-2106,16x2 Character LCD 1602K,
                  4-bit mode operation",
             which is Copyright (c) 2004 senz at arm.dreamislife.com

    used I/O:
            P9 - D1
            P10 - D2
            P11 - D3
            P12 - D4
            P13 - RS
            P14 - Enable

*/
/*!
\file
HD44780 compatible LCD-driver HvU testboard 2.3 and 3.2
*/
#include <board/lcd_hd44780.h>

#ifdef BOARD_ENABLED

#include <delay.h>
#include <std.h>
#include <io.h>

/*! The standard LCD on the HvU testboard has a somewhat weird setup; after
    character 7 follows character 32. Setting the following to '1' takes this
    into account when writing to the LCD */
#define LCD_HD44780_SKIP8TO32   1

/*! The LCD driver can handle one and two line displays (Yes, there are 4 lined
    displays aswell, but internally they work as 2 lined displays with wrapped
    lines..). Chose 0 for one line displays, 1 for two (or more) lined displays.

    \note: Even tho the LCD on the HvU board has only one line, this setting has
           to be '2' (this is coused be the above mentioned 'missing
           characters') */
#define LCD_HD44780_LINES       1

/*! The LCD has two font-sets. The following define is used to select which
    fontset is used. Chose '0' for the 5x10 font or '1' for the 5x8 font.
    Note: not all LCD's support the 5x8 font; you'll just get the 5x10 font then.. */
#define LCD_HD44780_FONT        0

/* I/O-pins */
#define STARTBIT       9
#define LCD_D4         (1<<9)
#define LCD_D5         (1<<10)
#define LCD_D6         (1<<11)
#define LCD_D7         (1<<12)
#define LCD_EN         (1<<13)
#define LCD_RS         (1<<14)
#define LCD_GPIOMASK   (~(3<<(9*2)))& (~(3<<(10*2))) & (~(3<<(11*2))) \
                         & (~(3<<(12*2))) & (~(3<<(13*2))) & (~(3<<(14*2)))
#define LCD_DATA       (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define LCD_IOALL      (LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_EN|LCD_RS)

static unsigned char LCDpos;

static void lcd_writenibble(unsigned char data);
static void lcd_writebyte(unsigned char data);

void lcd_init(void)
/*!
  Initialise the LCD
*/
{
    /* configure I/O */
    PINSEL0 &= LCD_GPIOMASK;

    /* all output */
    iodir0_out(LCD_IOALL);

    ioclr0 = LCD_RS;
    ioclr0 = LCD_EN;

    delay_ms(50);

    /* function-set start */
    lcd_writebyte(0x30);
    lcd_writebyte(0x30);
    lcd_writebyte(0x30);

    /* interface is 4 bits */
    ioset0 = LCD_EN;
    lcd_writenibble(0x02);
    ioclr0 = LCD_EN;
    delay_ms(10);

    /* Function Set */
    lcd_writebyte(0x20 | (LCD_HD44780_LINES<<3) | (LCD_HD44780_FONT<<2));
    delay_ms(5);
    /* LCD on */
    lcd_writebyte(0x0E);
    delay_ms(5);
    /* clear screen */
    lcd_writebyte(0x01);
    delay_ms(5);
    /* Entry mode set */
    lcd_writebyte(0x06);
    delay_ms(5);

    /* clear positioncounter */
    LCDpos=0;
}

void lcd_put(const char *c)
/*!
  Put given string on the LCD
*/
{
    while(*c) {
        lcd_putchar(*c);
        c++;
    }
}

void lcd_putchar(const unsigned char c)
/*!
  Put given character on the screen
*/
{
    #if LCD_HD44780_SKIP8TO32
    unsigned char i;
    #endif

    ioset0 |= LCD_RS;
    lcd_writebyte(c);
    LCDpos++;
    #if LCD_HD44780_SKIP8TO32
    if(LCDpos==8) {
        /* This LCD is a bit weird; character 8 to 31 (row 2 and 3) are not
           available on the screen, but one can write to them.. */
        for (i=0; i<32; i++) {
            lcd_cursor_right();
        }
    }
    #endif
}

void lcd_erase(const unsigned char home)
/*!
  Clear the LCD. If home==TRUE, the cursor is set at the beginning of the LCD
*/
{
    if(home) {
        /* Cursor home */
        lcd_writecontrol(0x02);
        delay_ms(5);
    }
    /* Clear */
    lcd_writecontrol(0x01);
    delay_ms(5);
    LCDpos=0;
}

void lcd_writecontrol(const unsigned char data)
/*!
  Send a command to the LCD
*/
{
    ioclr0 = LCD_RS;
    lcd_writebyte(data);
}

static void lcd_writenibble(const unsigned char data)
/*!
  Send a nibble to the LCD
*/
{
    ioclr0 = LCD_DATA;
    ioset0 = (data<<STARTBIT);
}

static void lcd_writebyte(const unsigned char data)
/*!
  Send a byte to the LCD
*/
{
    /* upper 4 bits */
    ioset0 = LCD_EN;
    lcd_writenibble((data>>4)&0x0F);
    ioclr = LCD_EN;

    delay_us(50);

    /* lower 4 bits */
    ioset0 = LCD_EN;
    lcd_writenibble(data&0x0F);
    ioclr0 = LCD_EN;

    delay_us(50);
}

/* Check for invalid settings */
#if LCD_HD44780_FONT > 1
    #error "Invalid fontset selected. Chose '0' or '1'!"
#endif

#if LCD_HD44780_LINES > 1
    #error "Invalid number of rows selected. Chose '0' or '1'!"
#endif

#endif /* BOARD_ENABLED */
