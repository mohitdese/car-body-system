/*
**
****************************************************************************
**
**
**
**             Copyright (c) 2003, 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN MASTER EXAMPLE (JSB version)
** Module     	:
** Description	:
**
**
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.01.2005    E.G.          Creation
**
**
**
**
**
** LICENSE -
**
** ATMEL - 2003,2004
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


/*__________________________________________________________________________*/
/*_____ I N C L U D E S ____________________________________________________*/
/*__________________________________________________________________________*/

#include "config.h"
#include "lib_mcu/lin_uart/master_lin.h"
#include "mcu_avr_atmega128_api.h"
#include <avr/interrupt.h>

extern   U8 Buf_SET_SLAVE [8];
extern   U8 Buf_GET_SLAVE_0[4];
extern   U8 Buf_GET_SLAVE_1[4];
extern   U8 Buf_GET_SLAVE_2[4];

U16    CmdCtr     ;

//**************************************************************************
// Timer overflow interrupt service routine
//
// Overflow Rate is Tbit -  2400 Tbit  = 416us
//                          4800 Tbit  = 208us
//                          9600 Tbit  = 104us
//                          19200 Tbit = 52us
//
//**************************************************************************

ISR(TIMER1_OVF_vect) {


  // Reload: Set Timer Value for a Tbit overflow
  TCNT1H = _8MHZ_TCNT1H;
  TCNT1L = _8MHZ_TCNT1L;

  if(19200 == CmdCtr) {
    CmdCtr = 0;
  }else{
    CmdCtr++;
  }
  l_sch_tick();//call lin slave handler
}

