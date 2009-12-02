/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN SLAVE CONTROLLER
** Module     	: SLAVE_LIN_USART.C
** Description	: LIN v1.3 SLAVE CONTROLLER
** Target       : ATMEGA48/88/168 and AT90CAN128
** Compiler    	: IAR Embedded Workbench 4.1
**
**
** Version :     Date:       Author:      Comment:
**    1.0        5.1.2005    E.G.          Creation

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
/* Define SLAVE_LIN_C if  not already defined */
#define SLAVE_LIN_C

#include "lib_mcu/lin_uart/slave_lin.h"

t_schedule my_schedule ; //current schedule
lin_status_t lin_status; //lin controller status
st_lin_t msg_lin;        //msg description objetc
st_cmd_lin_t cmd_lin;    //lin command object
U8 tab_data[8];          //lin data buffer
signed char  _current_frame_index ;

#ifdef _SLEEP_TIMOUT_DETECTION
U8    LINSleepFlag;
U8    MyCpuIsSleepingFlag;
U8    NoBusActivityFlag;
U16   SleepCtr;
#endif

#ifdef _COUNTING_ERRORS_ENABLE
U16 countCERR;  // checksum error counter
U16 countRERR;  // slave response error (master only)
U16 countBERR;  // bit error counter
U16 countSERR;  // synchro error counter
U16 countPERR;  // parity error counter
#endif

#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
volatile U8 Timer_IC_cnt;
volatile U8 osccal_start;
volatile U8 _lin_synchronized;    //lin is synchronized with master (1 - ok within 2%)
#endif

//------------------------------------------------------------------------------
//
// Description : integer to ascii conversion function (usefull for display !)
// Input Parameters :  character (8 bits) to convert
// Output  Parameters : corresponding ASCII byte
// 	
//
// ------------------------------------------------------------------------------
char int2hex(unsigned char integer) {
  if(integer < 10)  {
    return integer+ASCII_NUM_OFFSET;
  }else{
    return integer+55;
  }
}

//----------------------------------------------------------------------------//
// Description :	Start lin scheduler timer
// Input Parameters :  none
// Output  Parameters : none
//
//----------------------------------------------------------------------------//
void LIN_TIMER_init (void){
  TIMSK1  |= 1 << TOIE1;    // Int T Overflow Enabled
  TCCR1A  = 0x00;           // Counter Mode - Normal
  TCCR1B  = 1<<CS10;           // Set clock source to internal

  // Set Timer Value for a Tbit overflow
  TCNT1H = _8MHZ_TCNT1H; // Set Timer Value for a Tbit overflow
  TCNT1L = _8MHZ_TCNT1L;

  StartLinSlave();
}

//----------------------------------------------------------------------------//
// Description :	lin slave scheduler (tick = 1 LIN Tbtit)
//
//----------------------------------------------------------------------------//
U8 l_sch_tick (void) {
  U8     frame_type;

#ifdef _SLEEP_TIMOUT_DETECTION
  if (SleepCtr < SLEEP_TIMOUT) {
    SleepCtr ++ ;
  } else {
    if(!LINSleepFlag) {
      NoBusActivityFlag = 1;
      Lin_cmd_gotosleep();
    }
  }
#endif

  lin_status = lin_getstatus(&cmd_lin);

  if (lin_status == LIN_ACTION_ONGOING){ //controller is busy , quit
    return 0;
  } else if (lin_status == LIN_ID_RECEIVED) { //valid identifier has been recieved
    frame_type = IdReceived_Notification((msg_lin.id) & 0x0F, id_to_dlc (msg_lin.id));
    if (frame_type == REMOTE_LIN_FRAME_TYPE) {
#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
      if( _lin_synchronized) {    //lin is synchronized with master (1 - ok within 2%)
#else
        if(1) {
#endif
#ifdef _SLEEP_TIMOUT_DETECTION
          SleepCtr = 0 ;
#endif
          DataRequest_Notification(); //get data bytes
          cmd_lin.cmd = LIN_CMD_S_TX; //transmit frame response
          lin_cmd(&cmd_lin);
        }  //lin synchro ok
      } else if (frame_type == STANDART_LIN_FRAME_TYPE)  {
#ifdef _SLEEP_TIMOUT_DETECTION
        SleepCtr = 0 ;
#endif
        cmd_lin.cmd = LIN_CMD_S_RX; //wait for response, sent by master node
        lin_cmd(&cmd_lin);
      }else if (frame_type ==UNKWNON_LIN_FRAME_TYPE ){
        _lin_state = STATE_CHECK_BREAK;
        Uart_set_baudrate(UART_BAUDRATE_SYNC);
        StartLinSlave() ;
      }
    } else if ((LIN_ACTION_COMPLETED == lin_status) &&  (LIN_CMD_S_TX == cmd_lin.cmd)) { //end of transmission of the whole frame
      StartLinSlave() ;
    } else if ((LIN_ACTION_COMPLETED == lin_status) &&  (LIN_CMD_S_RX == cmd_lin.cmd)) { //end of reception of the whole frame
      DataReceived_Notification((msg_lin.id) & 0x0F ,id_to_dlc (msg_lin.id));
    } else  if (LIN_AVAILABLE == lin_status )  { //lin controller is available
      StartLinSlave() ;
    } else { //error occured
#ifdef _COUNTING_ERRORS_ENABLE
      update_error_counter(lin_status);//should we stop the wake mode in case of lin frame error ?
#endif
      StartLinSlave() ;
    }
    return 0;
  }

  //----------------------------------------------------------------------------//
  // Description :	LIN ID to  DLC conversion
  //
  //----------------------------------------------------------------------------//
  U8 id_to_dlc (U8 lin_id) {
    U8 lin_dlc = (lin_id & 0x30) >>4; // extract dlc bits only from identifier
    if (lin_dlc < 2){
      lin_dlc = 2; //2 data bytes to be transferred
    }else if (lin_dlc == 2){
      lin_dlc = 4;//4 data bytes to be transferred
    }else{
      lin_dlc = 8;//8 data bytes to be transferred
    }
    return lin_dlc;
  }

  //----------------------------------------------------------------------------//
  //
  // Description : data are required before sending a new LIN TX frame (response)	
  //               retrieve data from application bufefr and then copy them
  //               into LIN contoller private data buffer
  //----------------------------------------------------------------------------//
  void DataRequest_Notification(void) {
    t_schedule * pt_schedule;
    pt_schedule = &my_schedule;
    if (_current_frame_index>=0) {
      cmd_lin.pt_lin_msg->pt_data = pt_schedule->frame_message[_current_frame_index].frame_data;
    }
  }

  //----------------------------------------------------------------------------//
  //
  // Description :   Data received notification
  //                 Received data are copied in the frame associated data buffer
  //
  //----------------------------------------------------------------------------//
  void DataReceived_Notification(U8 ReceivedId,U8 ReceivedSize) {
    U8 u;
    t_schedule * pt_schedule;
    pt_schedule = &my_schedule;
    if(ReceivedId != 0xc) {
      if (_current_frame_index>=0) {
        for (u=0;u<ReceivedSize;u++) {
          pt_schedule->frame_message[_current_frame_index].frame_data[u] = cmd_lin.pt_lin_msg->pt_data[u];
        }
      }
    } else if (ReceivedId == 0x0C) { //check if sleep command received ?
#ifdef _SLEEP_TIMOUT_DETECTION
      if (cmd_lin.pt_lin_msg->pt_data[0] == 0) {
        LINSleepFlag =  1 ;
        Lin_cmd_gotosleep();
      }
#endif
    }
  }

  //----------------------------------------------------------------------------//
  //
  // Description :     a valid LIN Identifier has been received
  //                     this function checks which lin frame is associated with
  //                    this  particular identifier,if any
  //
  //----------------------------------------------------------------------------//
  int IdReceived_Notification(U8 ReceivedId,U8 ExpectedSize) {
    U8 i;
    t_schedule * pt_schedule;
    pt_schedule = &my_schedule;
    if (ReceivedId != 0x0C) {
      for (i=0;i<pt_schedule->number_of_frame;i++) {
        if ((ReceivedId == pt_schedule->frame_message[i].frame_id) && (ExpectedSize == pt_schedule->frame_message[i].frame_size)) {
          _current_frame_index = i;
          return (pt_schedule->frame_message[i].frame_type);
        }
      }
    } else {
      if (ExpectedSize == 8)
        _current_frame_index = -1;
      return  STANDART_LIN_FRAME_TYPE; //sleep message ? //go on with analysing data bytes
    }
    _current_frame_index = -1;
    return UNKWNON_LIN_FRAME_TYPE ; //no match, frame is ignored
  }

  //----------------------------------------------------------------------------//
  // Description : Start lin controller to be ready for receiving a new lin frame
  //
  //----------------------------------------------------------------------------//
  void StartLinSlave (void){
    cmd_lin.cmd = LIN_CMD_S_RX;    // Slave Reception configuration
    cmd_lin.pt_lin_msg = &msg_lin; // Slave Reception configuration
    msg_lin.pt_data = &tab_data[0]; //init data buffer
    msg_lin.id = 0; //clear previous identifier
    lin_cmd(&cmd_lin); //execute command
  }

#ifdef _COUNTING_ERRORS_ENABLE
  //----------------------------------------------------------------------------//
  // Description : updates error counters
  //----------------------------------------------------------------------------//
  void update_error_counter(U8 status){
    if ((status & LIN_ERROR_BIT)==LIN_ERROR_BIT){
      countBERR++; //bit error
    }else if ((status & LIN_ERROR_CSUM)==LIN_ERROR_CSUM){
      countCERR++; //checksum error
    }else if ((status & LIN_ERROR_PARITY)==LIN_ERROR_PARITY){
      countPERR++; //lin indentifier parity error
    }else if ((status & LIN_ERROR_SYNCHRO)==LIN_ERROR_SYNCHRO){
      countSERR++; //synchro byte error
    }else if ((status & LIN_ERROR_NO_RESPONDING)==LIN_ERROR_NO_RESPONDING){
      countRERR++; //slave not responding error
    }
  }
#endif
