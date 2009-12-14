/** \file Door1_CAN_slave.c
* \brief Controller of doors connected to CAN bus
* \author Software Developer Team
* \version 1.0
* \date 2009-12-06
*/


//$WCDATE$
//$WCREV$
// ver: 1.0 (nem találtam a megfelelõ kulcsszót, hogy automatikusan töltse ki a verziószámot)

#include "config.h"
#include "platform.h"
#include "dpy_trm_s01.h"
#include "mcp2515.h"
#include "mcu_avr_atmega128_api.h"

CAN_message received,sent;
U8 active, openclose;
/* ************************************************************************** */
/*
** Description :	The main C function.  Program execution starts here.
**
** ------------------------------------------------------------------------------
*/

/** \fn int main (void)
*   \brief main function
*   \param nothing
*	\return nothing
*/
int main (void) {
  // Initialize
  sei(); 		/* Interrupts globally enabled */
  mcp2515_init(); // CAN initialization
  dpy_trm_s01__Init(); // Display card initialization

  sent.id = 0x0100;        //Message ID = 0x100;
  sent.rtr = 0;
  sent.length = 8;

  // nyitva van az ajtó
  openclose = 0;

  while(1) {

  	// Ablakemelés az ajtón lévõ gombbal
	if(DPY_TRM_S01__BUTTON_1_GET_STATE()){
		DPY_TRM_S01__LED_3_ON();
		DPY_TRM_S01__LED_1_ON();
	}
	else{
		DPY_TRM_S01__LED_3_OFF();
		DPY_TRM_S01__LED_1_OFF();
	}
	// Ablak leengedés az ajtón lévõ gombbal
	if(DPY_TRM_S01__BUTTON_2_GET_STATE()){
		DPY_TRM_S01__LED_3_ON();
		DPY_TRM_S01__LED_2_ON();
	}
	else{
		
		DPY_TRM_S01__LED_3_OFF();
		DPY_TRM_S01__LED_2_OFF();
	}
	// Központi zár CAN üzenetet kell küldeni a többi ajtó vezérlõ moduljának is
	if(DPY_TRM_S01__BUTTON_3_GET_STATE()){
		if(openclose = 1) openclose = 0;
		else openclose = 1;
		DPY_TRM_S01__LED_4_ON();
		sent.data[4] = 64;
		can_send_standard_message(&sent);
	}
	else{
		DPY_TRM_S01__LED_4_OFF();
	}


    // CAN üzenet vétele
	can_receive_message(&received);
	// A CAN üzenet a kormánytól érkezett-e
	if( received.id == 0x100){
		// központi zár
		if (received.data[4] == 64){
			if(openclose = 1) openclose = 0;
			else openclose = 1;
			DPY_TRM_S01__LED_4_ON();
		}
		else
			DPY_TRM_S01__LED_4_OFF();
		// Az 1-es ajtóvezérlõnek szól-e az üzenet
		if(received.data[4] == 1) {
			// motor engedélyezése
			active = 1;
			DPY_TRM_S01__LED_3_ON();
			dpy_trm_s01__7seq_write_number(received.data[4],0);
		}
		else if(received.data[4] != 16 || received.data[4] != 32){
			// motor tiltása
			active = 0;
			DPY_TRM_S01__LED_3_OFF();
		}
		if(active == 1){
			// Ablakemelés
			if (received.data[4] == 16)
				DPY_TRM_S01__LED_1_ON(); 
		 	else 
				DPY_TRM_S01__LED_1_OFF();
			// Ablak leengedése
		 	if (received.data[4] == 32)
				DPY_TRM_S01__LED_2_ON();
		 	else
				DPY_TRM_S01__LED_2_OFF();
		}
		_delay_ms(10);
	}
  }//while(1)
  return 0;
}//main
