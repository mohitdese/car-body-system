/** \mainpage Car Body System
 *
 * \section intro_sec Introduction
 *
 * The Car Body System project is a homework, created by
 *
 *	- Bartakovics Tamas
 *	- Bekessy Laszlo
 *	- Horvath Zsolt
 *	- Kelemen Tibor
 *	- Kovari Balazs
 *	- Patonai Balazs
 *	- Sass Peter
 *
 * for Embedded System Design (VIMIM238) subject.
 * The project has four parts:
 *
 *
 * \section part1 Door CAN slave
 *
 *
 * This part contains the door controller connected to CAN bus
 * \sa Door1_CAN_slave.c
 *
 *
 * \section part2 Door LIN slave
 *
 *
 * This section contains the door controller connected to LIN bus
 * \sa Door2_LIN_slave.c
 *
 *
 * \section part3 CAN-LIN gateway
 *
 *
 * This part contains the CAN-LIN gateway which is a slave at CAN bus, and a master at LIN bus
 * \sa CAN_LIN_gateway_main.c
 *
 *
 * \section part4 Hardware Test Application
 *
 *
 * This section contains a Hardware Test Application
 * \sa test.c
 *
 *
 */


/** \file test.c
* \brief Test application
* \details This file contains the hardware test application
*		- Initialization
*			- CAN message initialization (0x100)
*			- CAN interface initialization
*			- Display initialization
*		- Process loop
*			- Receiving CAN message
*			- Handling CAN message
*			- 50 ms delay
*
* \author Software Developer Team
* \version 1.0
* \date 2009-12-16
* \todo Rewriting the code using functions. The main function should contain only the main logics
*/


#include "platform.h"
#include "mcp2515.h"
#include "dpy_trm_s01.h"
//#include <avr/interrupt.h>


/**\brief Received CAN message
 */
CAN_message received;

/**\brief CAN message to send
 */
CAN_message sent;


/**
* \brief Main function
* \details This function is the only user defined function in this file. Program execution starts here.
* \param The function doesn't have any parameter
* \return The function doesn't have a return value
*/

void main()
{


mcp2515_init();            //CAN Init
dpy_trm_s01__Init() ;    //LED Init


sent.id = 0x0100;        //Message ID = 0x100;
sent.rtr = 0;
sent.length = 8;

while(1) {


    can_receive_message(&recieved);

//    if(recieved.data[4] == 1 || recieved.data[3] == 1||recieved.data[2] == 1||recieved.data[5] == 1)  DPY_TRM_S01__LED_1_ON();
//    dpy_trm_s01__7seq_write_3digit('4','3','3');
    
    if(recieved.data[4] == 1 )  DPY_TRM_S01__LED_1_ON();
//    else DPY_TRM_S01__LED_1_OFF();
_delay_ms(50);
    }
}
