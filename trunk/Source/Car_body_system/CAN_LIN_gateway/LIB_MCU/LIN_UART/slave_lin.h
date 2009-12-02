/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN SLAVE EXAMPLE
** Module     	: SLAVE_LIN_USART.H
** Description	: LIN v1.3 SLAVE CONTROLLER
** Target       : ATMEGA48/88/168 and AT90CAN128
** Compiler    	: IAR Embedded Workbench 4.1
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.1.2005      E.G.          Creation
**
**
** LICENSE -
**
** ATMEL - 2004-2005
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
#include "config.h"
#include "lib_mcu/lin_uart/lin_lib.h"
#include "lib_mcu/lin_uart/lin_drv_usart.h"
#include "lib_mcu/timer/timer0_lib.h"
#include "lib_mcu/timer/timer1_lib.h"
#include "lib_mcu/timer/timer2_lib.h"
#include "lib_mcu/lin_uart/runtime_calibration_lib.h"

/*
** Define Whether to detect header/response time-out
*/
//#define _TIMOUT_DETECTION

/*
** Define Whether to support sleep detection (sleep message & NoBusActivity)
*/
//#define _SLEEP_TIMOUT_DETECTION

/*
** Define Whether to enable lin error counters
*/
//#define _COUNTING_ERRORS_ENABLE


/*
** Define Whether to enable run time rc oscillator auto calibration (only for lin slaves)
*/
//#define _RUN_TIME_RC_CALIBRATION_ENABLE


/*
** Define action while in sleep mode
*/
//#define _TURN_OFF_POWER_WHILE_SLEEPING //will turn off power supply
//#define _CLOCK_STOPPED_WHILE_SLEEPING //will simply stop system  clock


/* Define SLAVE_LIN_H if already not defined */
#ifndef SLAVE_LIN_H
#define SLAVE_LIN_H

#ifndef LIN_BAUDRATE
#error You must define LIN_BAUDRATE in config.H
#endif

#if LIN_BAUDRATE == 2400
#define _8MHZ_TCNT1H            0xf3// Timer Overflow set to Tbit
#define _8MHZ_TCNT1L            0x00
#define _8_BIT_TIMER_PRESCALER  64
#define _8_BIT_TIMER_SEED       204
#elif LIN_BAUDRATE == 9600
#define _8MHZ_TCNT1H            0xfc
#define _8MHZ_TCNT1L            0xc0
#define _8_BIT_TIMER_PRESCALER  8
#define _8_BIT_TIMER_SEED       152
#elif LIN_BAUDRATE == 19200
#define _8MHZ_TCNT1H            0xfe
#define _8MHZ_TCNT1L            0x60
#define _8_BIT_TIMER_PRESCALER  8
#define _8_BIT_TIMER_SEED       204
#else
#error " GIVEN LIN BAUDRATE IS NOT SUPPORTED (CONFIG.H) "
#endif

#define ASCII_NUM_OFFSET         48
#define SLEEP_TIMOUT             25000
#define NB_FRAME                 2
#define FALSE                    0
#define UNKWNON_LIN_FRAME_TYPE   2   //unknow identifier
#define REMOTE_LIN_FRAME_TYPE    1   //master transmits (Header) and waits for Slave Response)
#define STANDART_LIN_FRAME_TYPE  0   //master transmits (Header + Response)

/*
** *****************************************************************************
**                          STRUCTURES
** *****************************************************************************
*/
typedef struct  {
  U8 frame_id;
  U8 frame_size;
  U8 * frame_data;
  U8 frame_type;   //in frame response frame type if set
} t_frame;

typedef struct  {
  U8 number_of_frame;
  t_frame frame_message[NB_FRAME];
} t_schedule;

/*
** *****************************************************************************
**                     GLOBAL VARIABLES
** *****************************************************************************
*/

extern t_schedule my_schedule ;  //current schedule
extern U8 tab_data[8];           //LIN data buffer

#ifdef _SLEEP_TIMOUT_DETECTION
extern U8    LINSleepFlag;                 //LIN Slave Controller in Sleep mode if set
extern U8    MyCpuIsSleepingFlag;            //MCU in Sleep mode if set
extern U8    NoBusActivityFlag;              //No activity on the bus for 25000 Tbit if set
extern U16   SleepCtr;                       //No activity on the bus Counter (25000 Tbit)
#endif

#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
extern volatile U8 Timer_IC_cnt;
extern volatile U8 osccal_start;
extern volatile U8 _lin_synchronized;    //lin is synchronized with master (1 - ok within 2%)
#endif

#ifdef _COUNTING_ERRORS_ENABLE
extern U16 countCERR;  // checksum error counter
extern U16 countRERR;  // slave response error (master only)
extern U16 countBERR;  // bit error counter
extern U16 countSERR;  // synchro error counter
extern U16 countPERR;  // parity error counter
#endif

/*
** *****************************************************************************
**                   GLOBAL FUNCTION PROTOTYPES
** *****************************************************************************
*/
void LIN_TIMER_init (void);
void DataRequest_Notification(void);
void DataReceived_Notification(U8 ReceivedId,U8 ReceivedSize);
void StartLinSlave (void);
int IdReceived_Notification(U8 ReceivedId,U8 ExpectedSize);
U8 l_sch_tick(void); //schedule manager
U8 id_to_dlc (U8 lin_id);
char int2hex(unsigned char integer) ;

#ifdef _COUNTING_ERRORS_ENABLE
void update_error_counter(U8 status);
#endif

#endif /* end #ifndef SLAVE_LIN_H */


