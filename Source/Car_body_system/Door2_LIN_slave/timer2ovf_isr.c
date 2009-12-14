/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE (JSB version)
** Module     	: timer_2_isr.C
** Description	: timer 2 overflow interrupt subroutine
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.01.2005    E.G.          Creation
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

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "lib_mcu/timer/timer2_lib.h"
#include "lib_mcu/lin_uart/lin_drv_usart.h"
#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#else
#include "lib_mcu/lin_uart/master_lin.h"
#endif
#include <avr/interrupt.h>

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
U8 timout_ovf_cnt ;//for 2400 baud rate support @8MHz only

//**************************************************************************
// Timer overflow interrupt service routine for LIN time-out detection
//
// occurs if lin slave time-out detected (header and/or data)
//
//**************************************************************************

ISR(TIMER2_OVF_vect){

#ifdef _TIMOUT_DETECTION
#if LIN_BAUDRATE == 2400
  if(timout_ovf_cnt){
  Timer2_stop();
  Uart_set_baudrate(UART_BAUDRATE_SYNC);
  _lin_state = STATE_CHECK_BREAK;
  _lin_err = MSK_STATUS_TIME_OUT;
  _lin_status = MSK_STATUS_ERR;
  timout_ovf_cnt = FALSE;

  }else{
   timout_ovf_cnt = TRUE;
   TCNT2 = 0x0; //reload with max only for 2400 baud

  }
#else
  Timer2_stop();
  Uart_set_baudrate(UART_BAUDRATE_SYNC);
  _lin_state = STATE_CHECK_BREAK;
  _lin_err = MSK_STATUS_TIME_OUT;
  _lin_status = MSK_STATUS_ERR;
#endif
#endif

}
