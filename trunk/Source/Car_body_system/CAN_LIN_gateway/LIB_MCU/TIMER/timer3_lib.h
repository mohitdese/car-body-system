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

#ifndef _TIMER3_LIB_H_
#define _TIMER3_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include "config.h"
//#include "lib_mcu/lin_uart/timer3_lib.h"


/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**/
  void Timer3_start(void);
  void Timer3_init_capture(void);
  void Timer3_stop_capture(void);
  void Timer3_stop(void);
 
#endif /*  */
