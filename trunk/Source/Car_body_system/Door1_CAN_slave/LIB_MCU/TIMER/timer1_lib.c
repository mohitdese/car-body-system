/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE (JSB Version)
** Module     	: timer 1_lib.c
** Description	: timer 1 library
**
**
** Version :     Date:         Author:      Comment:
**    1.0        9.11.2004    E.G.           creation
**
**
**
** LICENSE -
**
** ATMEL - 2004
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
#include "lib_mcu/timer/timer1_lib.h"

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/


/*------------------------------------------------*/
/*  Timer Start                                   */
/*------------------------------------------------*/
void Timer1_start(U16 timout){

  // TIMSK1  |= (1 << TOIE1);    // Int T1 Overflow Interrupt Enabled

#ifdef AT90CAN128_USED
  TCCR1A  = 0x00;              // Counter Mode - Normal
  TCCR1B  = (1<<CS10);         // Set clock source to fosc / 256
#endif

#ifdef ATMEGA168_USED
  TCCR1A  = 0x00;               // Counter Mode - Normal
  TCCR1B  = (1<<CS10) ;         // Set clock source to fosc
#endif

#ifdef ATMEGA88_USED
  TCCR1A  = 0x00;               // Counter Mode - Normal
  TCCR1B  = (1<<CS10) ;         // Set clock source to fosc
#endif

#ifdef ATMEGA48_USED
  TCCR1A  = 0x00;               // Counter Mode - Normal
  TCCR1B  = (1<<CS10) ;         // Set clock source to fosc
#endif

  TCNT1 = timout ; // Set Timer Value for a Tbit overflow
}

/*------------------------------------------------*/
/*  Timer Stop                                    */
/*------------------------------------------------*/
void Timer1_stop(void){
  TIMSK  &= ~(1 << TOIE1);    // disable Int T2 Overflow Enabled

#ifdef AT90CAN128_USED
  TCCR1A  = 0x00;             //stop timer clock
  TCCR1B  = 0x00;  //stop timer 1 clock
#endif

#ifdef ATMEGA168_USED
  TCCR1B  = 0x00;  //stop timer 1 clock
#endif

#ifdef ATMEGA88_USED
  TCCR1B  = 0x00;  //stop timer 1 clock
#endif

#ifdef ATMEGA48_USED
  TCCR1B  = 0x00;  //stop timer 1 clock
#endif
}


/*------------------------------------------------*/
/*  Start Capturing                               */
/*------------------------------------------------*/
void Timer1_Start_Capture(void){
#ifdef ATMEGA168_USED
  TCCR1B  |=  1<<ICNC1 ;     //enable noise canceller
  TCCR1B  &= ~(1<<ICES1);    //input capture on falling edge
  TIMSK1  |=  (1<<ICIE1) ;   //enable input capture interrupt
#endif

#ifdef ATMEGA88_USED
  TCCR1B  |=  1<<ICNC1 ;     //enable noise canceller
  TCCR1B  &= ~(1<<ICES1);    //input capture on falling edge
  TIMSK1  |=  (1<<ICIE1) ;   //enable input capture interrupt
#endif

#ifdef ATMEGA48_USED
  TCCR1B  |=  1<<ICNC1 ;     //enable noise canceller
  TCCR1B  &= ~(1<<ICES1);    //input capture on falling edge
  TIMSK1  |=  (1<<ICIE1) ;   //enable input capture interrupt
#endif

#ifdef AT90CAN128_USED
  TCCR1B  |=  1<<ICNC1 ;     //enable noise canceller
  TCCR1B  &= ~(1<<ICES1);    //input capture on falling edge
  TIMSK  |=  (1<<TICIE1) ;   //enable input capture interrupt
#endif
}

/*------------------------------------------------*/
/*  Stop Capturing                               */
/*------------------------------------------------*/
void Timer1_Stop_Capture(void){
#ifdef ATMEGA168_USED
  TIMSK1  &=  ~(1<<ICIE1) ;   //disable input capture interrupt
#endif

#ifdef ATMEGA88_USED
  TIMSK1  &=  ~(1<<ICIE1) ;   //disable input capture interrupt
#endif

#ifdef ATMEGA48_USED
  TIMSK1  &=  ~(1<<ICIE1) ;   //disable input capture interrupt
#endif

#ifdef AT90CAN128_USED
  TIMSK  &=  ~(1<<TICIE1) ;   //disable input capture interrupt
#endif
}


