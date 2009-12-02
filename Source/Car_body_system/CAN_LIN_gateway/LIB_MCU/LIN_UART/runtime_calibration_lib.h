/*
**
****************************************************************************
**
**
**             Copyright (c) 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN SLAVE EXAMPLE
** Module     	: runtime_calibration_lib.h
** Description	: LIN v1.3 SLAVE CONTROLLER (library)
**
**
** Version :     Date:         Author:      Comment:
**    1.0        16.11.2004    E.G.          Creation
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

#ifndef _RUNTIME_CALIBRATION_LIB_H_
#define _RUNTIME_CALIBRATION_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "config.h"
#include "lib_mcu/timer/timer1_lib.h"
#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#endif

#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
extern volatile U8 osccal_start;
extern volatile U8 _lin_synchronized;    //lin is synchronized with master
#endif

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**/
#if LIN_BAUDRATE == 19200
#define EXPECTED_TBIT                  833              //equals to 2 nominal master Tbit
#define TBIT_DIFF_THRESHOLD_MAX        (signed int) 18
#define TBIT_DIFF_THRESHOLD_MIN        (signed int) -18
#elif  LIN_BAUDRATE == 9600
#define EXPECTED_TBIT                  1667             //equals to 2 nominal master Tbit
#define TBIT_DIFF_THRESHOLD_MAX        (signed int)  29
#define TBIT_DIFF_THRESHOLD_MIN        (signed int) -29
#elif  LIN_BAUDRATE == 2400
#define EXPECTED_TBIT                  6667             //equals to 2 nominal master Tbit
#define TBIT_DIFF_THRESHOLD_MAX        (signed int)  146
#define TBIT_DIFF_THRESHOLD_MIN        (signed int) -123
#else
#error "LIN BAUDRATE NOT SUPPORTED FOR RC OSC RT CALIBRATION"
#endif

/*_____ FUNCTIONS PROTOTYPES ____________________________________________*/
void Start_Capturing (void);
void Timer_stop_capture(void);
#endif
