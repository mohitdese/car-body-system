/*
****************************************************************************
** car-body-system project/LIN-es ablakvezérlõk
**
** Verzió :     Dátum:         Szerzõ:      Comment:
**    1.0       11.11.2009    B.L.          
****************************************************************************
*/


/*__________________________________________________________________________*/
/*_____ I N C L U D E S ____________________________________________________*/
/*__________________________________________________________________________*/
#include "config.h"
#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#else
#include "lib_mcu/lin_uart/master_lin.h"
#endif

#include "lib_mcu/lin_uart/lin_lib.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "platform.h"
#include "mcu_avr_atmega128_api.h"
#include "dpy_trm_s01.h"

// Két Slave van, melyeknek különbözõ az elérési címe, így kétféleképpen fordítható a kód

//#define SLAVE1
#define SLAVE2


// Elsõ Slave

#ifdef SLAVE1

U8 Buf_SET_SLAVE [4];
t_frame MESS_SET_SLAVE;


int main (void) {
  
  U8 number_of_frame ;

  //Slave frame inicializálása
  MESS_SET_SLAVE.frame_id    = 0x00 ;
  MESS_SET_SLAVE.frame_size  = 4 ;
  MESS_SET_SLAVE.frame_type  = STANDART_LIN_FRAME_TYPE ;
  MESS_SET_SLAVE.frame_data  = Buf_SET_SLAVE;

  number_of_frame = 1;
  my_schedule.frame_message[0] = MESS_SET_SLAVE;
  my_schedule.number_of_frame = number_of_frame;

  //Kijelzõkártya inicializálása
  dpy_trm_s01__Init();
  
  //LIN Controller inicializálása
  lin_init();

  //Globális interrupt engedélyezés
  sei();
  
  Buf_SET_SLAVE[0]=0;

  while(1) {
  
  //A laborban használt motort a LED-ek vezérlésével tudjuk forgatni
  //(ugyanarra a port-ra van kötve a két jel) A LED3 engedélyezõ jel,
  //A LED1, illetve LED2-vel jobbra vagy balra tudjuk forgatni a motort.
  
  //Forgatómotor engedélyezése
	DPY_TRM_S01__LED_3_ON();
    
   //Ha az üzenet 4. bitje aktív, jobbra forgatunk
	 if (Buf_SET_SLAVE[0] == 16) DPY_TRM_S01__LED_1_ON(); 
	 	else DPY_TRM_S01__LED_1_OFF();
	 //Ha az üzenet 5. bitje aktív, balra forgatunk
	 if (Buf_SET_SLAVE[0] == 32) DPY_TRM_S01__LED_2_ON();
	 	else DPY_TRM_S01__LED_2_OFF();
	 	
	 //a az üzenet 6. bitje aktív, beállítjuk a központi zárat
	 if (Buf_SET_SLAVE[0] == 64) DPY_TRM_S01__LED_4_ON();
	 	else DPY_TRM_S01__LED_4_OFF();  
	 	
	 //A vett LIN-es üzenet kijelzése a hétszegmenses kijelzõn  
	dpy_trm_s01__7seq_write_number(Buf_SET_SLAVE[0],0);
			 
  }
  
  return 0;
}


#endif




#ifdef SLAVE2

U8 Buf_SET_SLAVE2 [4];
t_frame MESS_SET_SLAVE2;

int main (void) {
  
  U8 number_of_frame ;

  
  MESS_SET_SLAVE2.frame_id    = 0x01 ;
  MESS_SET_SLAVE2.frame_size  = 4 ;
  MESS_SET_SLAVE2.frame_type  = STANDART_LIN_FRAME_TYPE ;
  MESS_SET_SLAVE2.frame_data  = Buf_SET_SLAVE2;

  number_of_frame = 1;
  my_schedule.frame_message[0] = MESS_SET_SLAVE2;
  my_schedule.number_of_frame = number_of_frame;

  dpy_trm_s01__Init();
  
  lin_init();

  sei(); 	
  Buf_SET_SLAVE2[0]=0;

  while(1) {
     
	dpy_trm_s01__7seq_write_number(Buf_SET_SLAVE2[0],0);

	DPY_TRM_S01__LED_3_ON();

	 if (Buf_SET_SLAVE2[0] == 16) DPY_TRM_S01__LED_1_ON(); 
	 	else DPY_TRM_S01__LED_1_OFF();
	 if (Buf_SET_SLAVE2[0] == 32) DPY_TRM_S01__LED_2_ON();
	 	else DPY_TRM_S01__LED_2_OFF();
	 if (Buf_SET_SLAVE2[0] == 64) DPY_TRM_S01__LED_4_ON();
	 	else DPY_TRM_S01__LED_4_OFF();  
			 
  }
  
  return 0;
}


#endif




