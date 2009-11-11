#include "platform.h"
#include "mcp2515.h"
#include "dpy_trm_s01.h"
//#include <avr/interrupt.h>



CAN_message sent,recieved;


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
