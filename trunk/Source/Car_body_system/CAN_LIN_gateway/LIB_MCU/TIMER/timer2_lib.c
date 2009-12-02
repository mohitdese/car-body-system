/*
**
****************************************************************************
**
**
**             Copyright (c) 2004-2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER
** Module     	: timer2_lib.c
** Description	: timer 2 library
**
**
** Version :     Date:         Author:      Comment:
**    1.0        23.06.2005    EG
**
**
**
**
** LICENSE -
**
** ATMEL - 2004-2005
** All software programs are provided 'as is' without warranty of any kind:
** Atmel does not state the suitability of the provided materials for any
** purpose. Atmel hereby disclaim all warranties and conditions with regard
** to the provided software, including all implied warranties, fitness for
** a particular purpose, title and non-infringement.In no event will Atmel
** be liable for any indirect or consequential damages or any damages
** whatsoever resulting from the usage of the software program.
****************************************************************************
**
*/



/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include "lib_mcu/timer/timer2_lib.h"

extern U8 timout_ovf_cnt ;//for 2400 baud rate support @8MHz only

#ifdef _TIMOUT_DETECTION
/*------------------------------------------------*/
/*  Timer Start                                   */
/*------------------------------------------------*/
void Timer2_start(U16 timout,U8 dlc){
  TIMSK  |= (1 << TOIE2);    // Int T2 Overflow Enabled

#ifndef TIMER_2_PRESCALER
#error "timer 2 prescaler not defined"
#endif

#if LIN_BAUDRATE == 2400
  if (dlc == 8 ) {
    timout_ovf_cnt = FALSE;
  }else {
    timout_ovf_cnt = TRUE;
  }
#endif

  TCNT2 = timout ; // Set Timer Value for a Tbit overflow

#if TIMER_2_PRESCALER == 256
#ifdef AT90CAN128_USED
  TCCR2  = (1<<CS22)  | (1<<CS21);         // Set clock source to fosc / 256
#else
  TCCR2B  = (1<<CS22)  | (1<<CS21);         // Set clock source to fosc / 256
#endif

#elif TIMER_2_PRESCALER == 1024
#ifdef AT90CAN128_USED
  TCCR2A  = (1<<CS22)  | (1<<CS21) | (1<<CS20);    // Set clock source to fosc / 1024
#else
  TCCR2B  = (1<<CS22)  | (1<<CS21) | (1<<CS20);    // Set clock source to fosc / 1024
#endif

#else
#error "timer 2 prescaler value not supported (256 / 1024 only)"
#endif
}


/*------------------------------------------------*/
/*  Timer Stop                                    */
/*------------------------------------------------*/
void Timer2_stop(void){
  TIMSK  &= ~(1 << TOIE2);    // disable Int T2 Overflow Enabled
#ifdef AT90CAN128_USED
  TCCR2  = 0x00; //stop timer clock
#else
  TCCR2B  = 0x00; //stop timer clock
#endif
}
#endif

