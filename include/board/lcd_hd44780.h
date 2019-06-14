/*
    LPC21xx core set - Board specific functions

    lcd_hd44780.h:
                  HD44780 compatible LCD-driver HvU testboard 2.3 and 3.2

    copyright:
              Copyright (c) 2006 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
HD44780 compatible LCD-driver HvU testboard 2.3 and 3.2
*/
#ifndef LCD_HD44780_H
#define LCD_HD44780_H

/* Include global configuration */
#include <config.h>
#include <board.h>

#ifdef BOARD_ENABLED
#define BOARD_HAS_LCD

#define lcd_clear()          lcd_erase(0)
#define lcd_cursor_home()    lcd_erase(1)

#define lcd_display_on()     lcd_writecontrol(0x0E)
#define lcd_display_off()    lcd_writecontrol(0x08)

#define lcd_cursor_blink()   lcd_writecontrol(0x0F)
#define lcd_cursor_on()      lcd_writecontrol(0x0E)
#define lcd_cursor_off()     lcd_writecontrol(0x0C)

#define lcd_cursor_left()    lcd_writecontrol(0x10)
#define lcd_cursor_right()   lcd_writecontrol(0x14)
#define lcd_display_sleft()  lcd_writecontrol(0x18)
#define lcd_display_sright() lcd_writecontrol(0x1C)

void lcd_init(void);
void lcd_writecontrol(const unsigned char data);
void lcd_put(const char *c);
void lcd_putchar(const unsigned char c);
void lcd_erase(const unsigned char home);

#endif

#endif /* LCD_HD44780_H */
