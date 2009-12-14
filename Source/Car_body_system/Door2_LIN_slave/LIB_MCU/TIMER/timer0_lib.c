/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE (JSB Version)
** Module     	: timer0_lib.c
** Description	: timer 0 library
**
**
** Version :     Date:         Author:      Comment:
**    1.0        9.11.2004     E.G.           creation
**    1.1        19.11.2004    E.G.           creation
**
**
**
**
**
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
#include "lib_mcu/timer/timer0_lib.h"

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/


 /*------------------------------------------------*/
 /*  Timer Start                                   */
 /*------------------------------------------------*/
 void Timer0_start(U8 timout){

   #ifdef AT90CAN128_USED
    TCNT0   =  timout ;      // Set Timer Value for a Tbit overflow
    TCCR0  = (1<<CS01);     // Set clock source to fosc / 8
    TIMSK  |= (1 << TOIE0);  // Int T0 Overflow Enabled
   #endif

   #ifdef ATMEGA168_USED
    TCCR0A  = 0x00;          // Counter Mode - Normal
    TCCR0B  = (1<<CS01);     // Set clock source to fosc / 8
    TCNT0   = timout ;       // Set Timer Value for a Tbit overflow
    TIMSK0  = (1 << TOIE0);  // Int T0 Overflow Enabled
   #endif

    #ifdef ATMEGA88_USED
    TCCR0A  = 0x00;          // Counter Mode - Normal
    TCCR0B  = (1<<CS01);     // Set clock source to fosc / 8
    TCNT0   = timout ;       // Set Timer Value for a Tbit overflow
    TIMSK0  = (1 << TOIE0);  // Int T0 Overflow Enabled
   #endif
  }

 /*------------------------------------------------*/
 /*  Timer Stop                                    */
 /*------------------------------------------------*/
  void Timer0_stop(void){

   TIMSK  &= ~(1 << TOIE0);    // disable Int T0 Overflow Interrupt
   TCCR0  = 0x00; //stot timer clock
  #ifdef AT90CAN128_USED
 #endif

 #ifdef ATMEGA168_USED
   TCCR0B  = 0x00;  //set timer to normal mode
 #endif

  #ifdef ATMEGA88_USED
   TCCR0B  = 0x00;  //set timer to normal mode
 #endif

   }


