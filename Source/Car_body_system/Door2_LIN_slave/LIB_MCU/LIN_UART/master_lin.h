/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN MASTER EXAMPLE
** Module     	: MASTER_LIN.H
** Description	: LIN v1.3 MASTER CONTROLLER
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

/*
** Define Whether to detect header/response time-out
*/
#define _TIMOUT_DETECTION

/*
** Define Whether to support sleep mode
*/
#define _SLEEP_TIMOUT_DETECTION

/*
** Define Whether to enable lin error counters
*/
#define _COUNTING_ERRORS_ENABLE

/*
** Define to enable lin/can gateway feature
*/
//#define CAN_LIN_GATEWAY_ENABLE

// Timer Overflow set to Tbit
#if LIN_BAUDRATE == 2400
#define _8MHZ_TCNT1H            0xf3
#define _8MHZ_TCNT1L            0x00
#define _8_BIT_TIMER_PRESCALER  64
#define _8_BIT_TIMER_SEED       204
#elif LIN_BAUDRATE == 4800
#define _8MHZ_TCNT1H            0xf9
#define _8MHZ_TCNT1L            0x80
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
#error " LIN BAUDRATE NOT SUPPORTED  "
#endif

#define ASCII_NUM_OFFSET         48
#define SLEEP_TIMOUT             25000
#define NB_FRAME                 5
#define FALSE                    0
#define TRUE                     1
#define REMOTE_LIN_FRAME_TYPE    1   //master transmits (Header) and waits for Slave Response)
#define STANDART_LIN_FRAME_TYPE  0   //master transmits (Header + Response)


/* Define MASTER_LIN_H if already not defined */
#ifndef MASTER_LIN_H
#define MASTER_LIN_H

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
  U16 frame_delay;
  U8 data_has_changed;
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

extern t_schedule my_schedule ; //current schedule
extern U8 tab_data[8];          //LIN data buffer
extern U8 Frame_index;

#ifdef _SLEEP_TIMOUT_DETECTION
extern   U8    LINSleepFlag;
extern U8     lin_sleep_message_request;
#endif

#ifdef _COUNTING_ERRORS_ENABLE
extern U16 countCERR;  // checksum error counter
extern U16 countRERR;  // slave response error (master only)
extern U16 countBERR;  // bit error counter
extern U16 countSERR;  // synchro error counter
extern U16 countPERR;  // parity error counter
#endif

#ifdef CAN_LIN_GATEWAY_ENABLE
extern volatile U8     SendCanErrorFrame;
extern volatile U8     LinErrorMessage;
#endif

/*
** *****************************************************************************
**                   GLOBAL FUNCTION PROTOTYPES
** *****************************************************************************
*/
void LIN_TIMER_init (void);
void DataRequest_Notification(U8 ReceivedId,U8 ReceivedSize);
void Start_Lin_Master (void);
U8 DataReceived_Notification(U8 ReceivedId,U8 ReceivedSize);
U8 l_sch_tick(void); //schedule manager
U8 id_to_dlc (U8 lin_id);
U8 ComputeIdField(U8 lin_id_wo_dlc,U8 dlc)  ;
U8 CompDataBuffer(U8 * Buffer1, U8 * Buffer2,U8 BufferSize);
char int2hex(unsigned char integer) ;

#ifdef _COUNTING_ERRORS_ENABLE
void update_error_counter(U8 status);
#endif

#endif /* end #ifndef MASTER_LIN_H */


