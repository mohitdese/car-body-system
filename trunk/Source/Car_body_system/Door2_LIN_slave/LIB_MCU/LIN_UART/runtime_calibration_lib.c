/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN SLAVE EXAMPLE
** Module     	: runtime_calibration_lib.c
** Description	: LIN v1.3 CONTROLLER (library)
**
**
** Version :     Date:         Author:      Comment:
**    1.0        16.11.2004    E.G.          Creation
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

#include "lib_mcu/lin_uart/runtime_calibration_lib.h"

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
void Start_Capturing (void){
  TIFR1 |= (1<<ICF1);//clear pending icapture interrupt
  Timer_IC_cnt = 0; //for rc auto calibration lin tbit measuring with timer 3 input capture module
  Timer1_start(0);
  Timer1_Start_Capture();
}

void Timer_stop_capture(void){
  Timer1_stop();
  Timer1_Stop_Capture();
}
#endif






