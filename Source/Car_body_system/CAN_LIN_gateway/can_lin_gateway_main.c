 /** \file CAN_LIN_gateway_main.c
* \brief CAN-LIN gateway
* \author Software Developer Team
* \version 2.0
* \date 2009-12-16
* \details This file contains the CAN-LIN gateway which is a slave at CAN bus, and a master at LIN bus
*		- Initialization
*			- Set Slave Frame 0x00
*			- Set Slave Frame 0x01
*			- Set Frame parameters
*			- IO ports configuration
*			- LIN controller initialization
*			- CAN interface initialization
*			- Display initialization
*		- Process loop
*			- Receiving CAN message
*			- Handling CAN message
*			- Forwarding message to corresponding LIN slave
*			- 10 ms delay
*
* \todo Rewriting the code using functions. The main function should contain only the main logics
*/

/*
**
****************************************************************************
**
**
**
**             Copyright (c) 2003, 2004 - Atmel Corporation
**             Proprietary Information
**
** Project      : AVR CAN SLAVE - LIN MASTER
** Module       : main.C
** Description  : main application
**                Compatible with LIN Specifications 1.3
**
**
**
** Version :     Date:         Author:      Comment:
**    2.0        25.11.2009    Team
**
**
**
**              Assumes 8MHz external oscillator
**
**
**
** can_lin_gateway kettos funkciójú:
**  1. CAN buszon slave modul
**  2. LIN buszon master modul
****************************************************************************
**
*/


/*__________________________________________________________________________*/
/*_____ I N C L U D E S ____________________________________________________*/
/*__________________________________________________________________________*/
#include "config.h"
#include "lib_mcu/lin_uart/master_lin.h"
#include "lib_mcu/lin_uart/lin_lib.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "platform.h"
#include "mcu_avr_atmega128_api.h"
#include "dpy_trm_s01.h"
#include "mcp2515.h"

U8 Buf_SET_SLAVE [4];
U8 Buf_SET_SLAVE2 [4];
U8 active;

/* ************************************************************************** */
/*
** Description :        The main C function.  Program execution starts here.
**
** ------------------------------------------------------------------------------
*/

/**
* \brief Main function
* \details This function is the only user defined function in this file. Program execution starts here.
* \param main function doesn't have any parameter
* \return The function returns 0 if executed
* \warning Variables are not included in documentation!
*/

int main (void) {
  t_frame MESS_SET_SLAVE;
  t_frame MESS_SET_SLAVE2;
  CAN_message p_message;
  U8 number_of_frame ;
  U8 lin_message;

  //SET SLAVE FRAME
  MESS_SET_SLAVE.frame_id    = 0x00 ;
  MESS_SET_SLAVE.frame_size  = 4 ;
  MESS_SET_SLAVE.frame_type  = STANDART_LIN_FRAME_TYPE;
  MESS_SET_SLAVE.frame_delay  = 1920 ;
  MESS_SET_SLAVE.frame_data  = Buf_SET_SLAVE;

  //SET SLAVE FRAME
  MESS_SET_SLAVE2.frame_id    = 0x01;
  MESS_SET_SLAVE2.frame_size  = 4 ;
  MESS_SET_SLAVE2.frame_type  = STANDART_LIN_FRAME_TYPE;
  MESS_SET_SLAVE2.frame_delay  = 1920 ;
  MESS_SET_SLAVE2.frame_data  = Buf_SET_SLAVE2;

  // FRAME parameter
  number_of_frame = 2;
  my_schedule.frame_message[0] = MESS_SET_SLAVE;
  my_schedule.frame_message[1] = MESS_SET_SLAVE2;
  my_schedule.number_of_frame = number_of_frame;

  //CONFIG_IO_PORTS();

  // Initialise LIN Controller
  lin_init();// Performs Initialisation of LIN Software Driver

  sei();                /* Interrupts globally enabled */

  // CAN interface Init
  mcp2515_init();

  // Display panel Init
  dpy_trm_s01__Init();

  while(1) {
    // CAN message receive
        can_receive_message(&p_message);
        
        // A kormányról érkezett üzenet a CAN buszon?
        if(p_message.id == 0x100){      
                switch (p_message.data[4]) {
                        // bal elso ajtó -> nem ez a modul kezeli
                        case (1): {active = 0; DPY_TRM_S01__LED_2_OFF(); DPY_TRM_S01__LED_1_OFF();break;}
                        // jobb elso ajtó -> nem ez a modul kezeli
                        case (2): {active = 0; DPY_TRM_S01__LED_2_OFF(); DPY_TRM_S01__LED_1_OFF();break;}
                        // bal hátsó ajtó aktív -> üzenetet majd tovább kell küldeni LIN buszon
                        case (4): {active = 1; DPY_TRM_S01__LED_1_ON(); DPY_TRM_S01__LED_2_OFF(); break;}
                        // jobb hátsó ajtó aktív -> üzenetet majd tovább kell küldeni LIN buszon
                        case (8): {active = 2; DPY_TRM_S01__LED_2_ON(); DPY_TRM_S01__LED_1_OFF(); break;}       
                } // end case

                // LIN_SLAVE-nek továbbküldjük az adatot, ha ot szólítottuk meg
                if(active == 1) {
                        Buf_SET_SLAVE[0] = p_message.data[4];
                        dpy_trm_s01__7seq_write_number(Buf_SET_SLAVE[0],0);
                }

                // LIN_SLAVE2-nek továbbküldjük az adatot, ha ot szólítottuk meg
                else if(active == 2) {
                        Buf_SET_SLAVE2[0] = p_message.data[4];
                        dpy_trm_s01__7seq_write_number(Buf_SET_SLAVE2[0],0);
                } //if active end


        } //if p_message.id end
        _delay_ms(10);

  }//while(1) end
  
  return 0;

}//main end
