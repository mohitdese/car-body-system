/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE
** Module     	: timer1_lib.h
** Description	: timer 1 library header file
**
**
** Version :     Date:         Author:      Comment:
**    1.0        9.11.2004    E.G.          Creation 
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

#ifndef _TIMER0_LIB_H_
#define _TIMER0_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include "config.h"

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**/
  void Timer0_start(U8 timout);
  void Timer0_stop(void);
 
#endif 
