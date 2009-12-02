/*
**
****************************************************************************
**
**
**
**             Copyright (c) 2003, 2004 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN MASTER EXAMPLE
** Module     	: main.C
** Description	: main application
**                Compatible with LIN Specifications 1.3
**
**
**
** Version :     Date:         Author:      Comment:
**    1.0        19.01.2004    E.G.          Creation
**
**
**
**              Assumes 8MHz external oscillator
**
**
**
** can_lin_gateway kettõs funkciójú:
**  1. CAN buszon slave modul
**  2. LIN buszon master modul
****************************************************************************
**
*/


/*__________________________________________________________________________*/
/*_____ I N C L U D E S ____________________________________________________*/
/*__________________________________________________________________________*/
#include "config.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "platform.h"
#include "mcu_avr_atmega128_api.h"
#include "dpy_trm_s01.h"
#include "mcp2515.h"

U8 active;

/* ************************************************************************** */
/*
** Description :	The main C function.  Program execution starts here.
**
** ------------------------------------------------------------------------------
*/
int main (void) {
  
  CAN_message p_message;

  dpy_trm_s01__Init();

  //CONFIG_IO_PORTS();

  sei(); 		/* Interrupts globally enabled */

  mcp2515_init();
  dpy_trm_s01__Init();

  while(1) {
    // CAN message receive
	can_receive_message(&p_message);

	if(p_message.id == 0x100){	
		switch (p_message.data[4]) {
			case (1): {active = 0; DPY_TRM_S01__LED_2_OFF(); DPY_TRM_S01__LED_1_OFF();break;}
			case (2): {active = 0; DPY_TRM_S01__LED_2_OFF(); DPY_TRM_S01__LED_1_OFF();break;}
			case (4): {active = 1; DPY_TRM_S01__LED_1_ON(); DPY_TRM_S01__LED_2_OFF(); break;}
			case (8): {active = 2; DPY_TRM_S01__LED_2_ON(); DPY_TRM_S01__LED_1_OFF(); break;}	
		}

	} //if p_message.id end
	_delay_ms(10);

  }//while(1) end
  
  return 0;

}//main end




