/*
    ALDS (ARM LPC Driver Set)

    timer.h:
            Timer function definitions

    copyright:
              Copyright (c) 2006-2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>

              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
              This program is free software; you can redistribute it and/or modify it under the terms
              of the GNU General Public License as published by the Free Software Foundation; either
              version 2 of the License, or (at your option) any later version.

*/
/*!
\file
Timer function definitions
*/
#ifndef TIMER_H
#define TIMER_H

/* Include global configuration */
#include <config.h>

#include <types.h>
#include "drivers/registers.h"
#include "drivers/timer_bits.h"

/*
  Timer0 functions
*/
/* All MCU's have a first timer, and this driver should work on all of them(fingers crossed :-) ) */
#define TIMER0_ENABLED
void timer0_init(const unsigned int interval, const unsigned int prescaler, const FUNCTION handler);

/*! Clear all interrupt flags */
#define timer0_clearint()   T0IR = 0xFF

/*! Hold timer, can be resumed with timer0_resume() */
#define timer0_hold()       T0TCR &= ~BIT_TCR_ENABLE
/*! Resume timer (from a timer0_hold()) */
#define timer0_resume()     T0TCR |= BIT_TCR_ENABLE
/*! Reset timer (timer will restart counting from 0) */
#define timer0_reset()      do { \
                                T0TCR |= BIT_TCR_RESET; \
                                T0TCR &= ~BIT_TCR_RESET; \
                            } while(0)

/*! Return current value of the timer */
#define timer0_value()      T0TC
/*! Set value of timer */
#define timer0_setvalue(x)  do { \
                                T0TC = x; \
                            } while(0)

/*
  Timer1 functions
*/
/* All MCU's have a second timer, and this driver should work on all of them(fingers crossed :-) ) */
#define TIMER1_ENABLED
void timer1_init(const unsigned int interval, const unsigned int prescaler, const FUNCTION handler);

/*! Clear all interrupt flags */
#define timer1_clearint()   T1IR = 0xFF

/*! Hold timer, can be resumed with timer1_resume() */
#define timer1_hold()       T1TCR &= ~BIT_TCR_ENABLE
/*! Resume timer (from a timer1_hold()) */
#define timer1_resume()     T1TCR |= BIT_TCR_ENABLE
/*! Reset timer (timer will restart counting from 0) */
#define timer1_reset()      do { \
                                T1TCR |= BIT_TCR_RESET; \
                                T1TCR &= ~BIT_TCR_RESET; \
                            } while(0)

/*! Return current value of the timer */
#define timer1_value()      T1TC
/*! Set value of timer */
#define timer1_setvalue(x)  do { \
                                T1TC = x; \
                            } while(0)
/*
  Timer2 functions
*/
/* The following MCU's have a third timer */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (defined __DOXYGEN__)
#define TIMER2_ENABLED

void timer2_init(const unsigned short interval, const unsigned short prescaler, const FUNCTION handler);

/*! Clear all interrupt flags */
#define timer2_clearint()   T2IR = 0xFF

/*! Hold timer, can be resumed with timer2_resume() */
#define timer2_hold()       T2TCR &= ~BIT_TCR_ENABLE
/*! Resume timer (from a timer2_hold()) */
#define timer2_resume()     T2TCR |= BIT_TCR_ENABLE
/*! Reset timer (timer will restart counting from 0) */
#define timer2_reset()      do { \
                                T2TCR |= BIT_TCR_RESET; \
                                T2TCR &= ~BIT_TCR_RESET; \
                            } while(0)

/*! Return current value of the timer */
#define timer2_value()      T2TC
/*! Set value of timer */
#define timer2_setvalue(x)  do { \
                                T2TC = x; \
                            } while(0)

#endif

/*
  Timer3 functions
*/
/* The following MCU's have a fourth timer */
#if (__MCU >= LPC2101 && __MCU <= LPC2103) || (defined __DOXYGEN__)
#define TIMER3_ENABLED

void timer3_init(const unsigned short interval, const unsigned short prescaler, const FUNCTION handler);

/*! Clear all interrupt flags */
#define timer3_clearint()   T3IR = 0xFF

/*! Hold timer, can be resumed with timer3_resume() */
#define timer3_hold()       T3TCR &= ~BIT_TCR_ENABLE
/*! Resume timer (from a timer3_hold()) */
#define timer3_resume()     T3TCR |= BIT_TCR_ENABLE
/*! Reset timer (timer will restart counting from 0) */
#define timer3_reset()      do { \
                                T3TCR |= BIT_TCR_RESET; \
                                T3TCR &= ~BIT_TCR_RESET; \
                            } while(0)

/*! Return current value of the timer */
#define timer3_value()      T3TC
/*! Set value of timer */
#define timer3_setvalue(x)  do { \
                                T3TC = x; \
                            } while(0)

#endif

#endif /* TIMER_H */
