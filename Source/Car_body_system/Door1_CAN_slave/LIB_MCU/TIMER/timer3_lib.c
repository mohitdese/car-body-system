/**
 * @file $RCSfile: lin_bdr.h,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file contains LIN drivers for the LIN macro.
 *
 * @version $Revision: 1.2 $ $Name:  $
 *
 * @todo
 * @bug
 */


/*_____ I N C L U D E - F I L E S ____________________________________________*/


/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**/

#include "timer3_lib.h"

 void Timer3_start(void){
  ETIMSK  |= (1 << TOIE3);    // Int T3 Overflow Enabled 
  TCCR3A  = 0x00;           // Counter Mode - Normal   
  TCCR3B  = 0x01;           // Set clock source to internal 
  ETIFR   = 0xFF;           //clear flag
 
  // Set Timer Value for a Tbit overflow            	     
  TCNT3H = 0;
  TCNT3L = 0;   
 }

 void Timer3_init_capture(void){
  TCCR3B  |=  1<<ICNC3 ;    //enable noise canceller
  TCCR3B  &= ~(1<<ICES1) ;  //input capture on falling edge
  ETIMSK  |=  1<<TICIE3 ;   //enable input capture interrupt
 }

 void Timer3_stop_capture(void){
  ETIMSK  &=  ~(1<<TICIE3) ;   //disable input capture interrupt
  TCCR3B  = 0x0;             // Stop Timer 3 clock 
 }

  void Timer3_stop(void){
     TCCR3B  = 0x0;            // Stop timer clock  
     ETIMSK  &= ~( 1<<TICIE3 ); //disable input capture interrupt
     ETIFR = 0xFF;//clear flag
 }
  
 
