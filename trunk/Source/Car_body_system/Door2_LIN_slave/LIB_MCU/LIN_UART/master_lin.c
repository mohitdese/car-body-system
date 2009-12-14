/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN MASTER EXAMPLE
** Module     	: MASTER_LIN.C
** Description	: LIN v1.3 MASTER SCHEDULER
** Target   	: ATMEGA48/88/168 and AT90CAN128
** Compiler    	: IAR Embedded Workbench 4.1
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.1.2005      E.G.         Creation
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

/* Define MASTER_LIN_C if not already  defined */
#define MASTER_LIN_C

#include "lib_mcu/lin_uart/master_lin.h"

t_schedule my_schedule ; //current schedule
lin_status_t lin_status; //lin controller status
st_lin_t msg_lin;        //msg description objetc
st_cmd_lin_t cmd_lin;    //lin command object
U8 tab_data[8];          //lin data buffer
static U16 LinTxCtr   ;
U8 Frame_index;

#ifdef _SLEEP_TIMOUT_DETECTION
U8 LINSleepFlag;
U8 lin_sleep_message_request;
#endif

#ifdef _COUNTING_ERRORS_ENABLE
U16 countCERR;  // checksum error counter
U16 countRERR;  // slave response error (master only)
U16 countBERR;  // bit error counter
U16 countSERR;  // synchro error counter
U16 countPERR;  // parity error counter
#endif

//------------------------------------------------------------------------------
//
// Description: integer to ascii conversion function (usefull for display !)
// Input Parameters:  character (8 bits) to convert
// Output  Parameters: corresponding ASCII byte
// ------------------------------------------------------------------------------
char int2hex(unsigned char integer) {
  if(integer < 10)  {
    return integer+ASCII_NUM_OFFSET;
  }else{
    return integer+55;
  }
}

// ------------------------------------------------------------------------------
// Description:        Start LIN timer (for tick function)
// Input Parameters:   none
// Output  Parameters: none
//------------------------------------------------------------------------------
void LIN_TIMER_init (void){
  TIMSK  |= 1 << TOIE1;    // Int T Overflow Enabled
  TCCR1A  = 0x00;           // Counter Mode - Normal
  TCCR1B  = 0x01;           // Set clock source to internal

  // Set Timer Value for a Tbit overflow
  TCNT1H = _8MHZ_TCNT1H;
  TCNT1L = _8MHZ_TCNT1L;
}

//----------------------------------------------------------------------------//
// Description:	LIN ID to  DLC conversion
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

// ------------------------------------------------------------------------------
// Description:  LIN ID Field initialisation.this function add dlc bytes into id field.
// Input Parameters:   id filed to be modified
// Output  Parameters:  modified id filed
// ------------------------------------------------------------------------------
U8 ComputeIdField(U8 lin_id_wo_dlc,U8 dlc) {
  U8    tmp_id;

  tmp_id= lin_id_wo_dlc & 0xCF; //clear dlc bits 5 and 4
  if(dlc == 2) {
    tmp_id |=   0x10 ; // 2 bytes to be transferred
  }else if(dlc== 4) {
    tmp_id |=   0x20 ; // 4 bytes to be transferred
  }else if(dlc== 8) {
    tmp_id |=   0x30 ; // 8 bytes to be transferred
  }
  return tmp_id;
}

//------------------------------------------------------------------------------
//
// Description:  data are required before starting transmitting a new lin frame
// Input Parameters:   frame id , frame size (my_schedule stucture used )
// Output Parameters:  none
// 	
//----------------------------------------------------------------------------//
void DataRequest_Notification(U8 TransmitId,U8 TransmitSize){
  U8 data_cnt;
  t_schedule *   pt_schedule;

  pt_schedule = &my_schedule;
  if (TransmitId != 0x3C) {
    for (data_cnt = 0 ; data_cnt < TransmitSize; data_cnt++ )
      tab_data[data_cnt] = pt_schedule->frame_message[Frame_index].frame_data[data_cnt];
  }else{
    for (data_cnt = 0 ; data_cnt < TransmitSize; data_cnt++ )
      tab_data[data_cnt] = 0; //init data for a sleep message frame
  }
}

//----------------------------------------------------------------------------//
// Description:  DataReceived_Notification Once valid LIN data has been received,
//                they are copied into the assocaited application buffer (if any)
// Input Parameters:   received Id (4 LSB), Size (number of bytes)  (use also my_schedule stucture)
// Output  Parameters:  '1' if data buffer has changed , '0' otherwise  (my_schedule stucture used)
//
//----------------------------------------------------------------------------//
U8 DataReceived_Notification(U8 ReceivedId,U8 ReceivedSize) {
  U8 i,u;
  t_schedule * pt_schedule;
  U8 data_has_changed_flag;
  U8 CompResult;

  data_has_changed_flag = FALSE;
  pt_schedule = &my_schedule;
  if(ReceivedId != 0xc) {
    for (i=0;i<pt_schedule->number_of_frame;i++) {
      if ((pt_schedule->frame_message[i].frame_type == 1) && ((ReceivedId & 0x0F) == pt_schedule->frame_message[i].frame_id)
          && (ReceivedSize == pt_schedule->frame_message[i].frame_size) )  {
            CompResult = CompDataBuffer(cmd_lin.pt_lin_msg->pt_data,pt_schedule->frame_message[i].frame_data,ReceivedSize);
            if(CompResult) {
              pt_schedule->frame_message[i].data_has_changed = CompResult;
              for (u=0;u<ReceivedSize;u++) {
                pt_schedule->frame_message[i].frame_data[u] = cmd_lin.pt_lin_msg->pt_data[u];
              }
            } //if data has changed
          } //if match
    } //for frame
  } else if (ReceivedId == 0x0C) { //check if sleep command received ?
#ifdef _SLEEP_TIMOUT_DETECTION
    if (cmd_lin.pt_lin_msg->pt_data[0] == 0) {
      LINSleepFlag =  1 ;
    }
#endif
  }//received id
  return data_has_changed_flag;
}

// ------------------------------------------------------------------------------
// Description:         compare two lin data buffer
// Input Parameters:    data buffer pointer
// Output  Parameters:  '1' if data buffers are different , '0' otherwise
// ------------------------------------------------------------------------------
U8 CompDataBuffer (U8 * Buffer1, U8 * Buffer2,U8 BufferSize) {
  U8 i;

  for (i= 0;i<BufferSize;i++){
    if  (Buffer1[i] !=  Buffer2[i]) {
      return 1;//buffer are different
    }
  }
  return 0; //buffer are identical
}

// ------------------------------------------------------------------------------
// Description:  Start transmitting a new lin frame
// Input Parameters:    none  (use my_schedule stucture and frame_index)
// Output  Parameters:  none (use my_schedule stucture)
// ------------------------------------------------------------------------------
void Start_Lin_Master (void){
  t_schedule *   pt_schedule;
  U8 i;

  pt_schedule = &my_schedule;
  Lin_clear_status();
  if(Frame_index >=  (pt_schedule->number_of_frame-1)) {
    Frame_index = 0;
  }else {
    Frame_index ++ ;
  }

 // if(Frame_index <  pt_schedule->number_of_frame) {
#ifdef _SLEEP_TIMOUT_DETECTION
    if(lin_sleep_message_request) { //check if sleep message tx request is pending
      for(i=0;i<8;i++) {
        tab_data[i] = 0;
      }

      msg_lin.id = ComputeIdField(0x0C,8);
      cmd_lin.cmd = LIN_CMD_M_TX;    // Sent a sleep Frame
      cmd_lin.pt_lin_msg = &msg_lin;
      msg_lin.pt_data = &tab_data[0];
      LinTxCtr = 2000;
      lin_cmd(&cmd_lin);
    }else if(lin_master_in_sleep_mode){
      LinTxCtr = 100;
#else
      if(0) {
#endif

      }else if ( pt_schedule->frame_message[Frame_index].frame_type == STANDART_LIN_FRAME_TYPE ) {
        DataRequest_Notification(pt_schedule->frame_message[Frame_index].frame_id,pt_schedule->frame_message[Frame_index].frame_size);
        msg_lin.id = ComputeIdField(pt_schedule->frame_message[Frame_index].frame_id,pt_schedule->frame_message[Frame_index].frame_size);
        cmd_lin.cmd = LIN_CMD_M_TX;    // Sent a new LIN Frame
        cmd_lin.pt_lin_msg = &msg_lin;
        msg_lin.pt_data = &tab_data[0];
        LinTxCtr = pt_schedule->frame_message[Frame_index].frame_delay;
        lin_cmd(&cmd_lin);
      } else if ( pt_schedule->frame_message[Frame_index].frame_type == REMOTE_LIN_FRAME_TYPE ) { //response in frame
        msg_lin.id = ComputeIdField(pt_schedule->frame_message[Frame_index].frame_id,pt_schedule->frame_message[Frame_index].frame_size);
        cmd_lin.cmd = LIN_CMD_M_RX;    // Send a new LIN Frame
        cmd_lin.pt_lin_msg = &msg_lin;
        msg_lin.pt_data = &tab_data[0];
        LinTxCtr = pt_schedule->frame_message[Frame_index].frame_delay;
        lin_cmd(&cmd_lin);
      }
    //}
  }

  // ------------------------------------------------------------------------------
  // Description:    LIN Scheduler function
  // Input Parameters:  none (use my_schedule stucture)
  // Output  Parameters:  '1' if a frame has been transmitted, '0' otherwise
  // ------------------------------------------------------------------------------
  U8 l_sch_tick (void) {
    if(LinTxCtr > 0) {//decrease here the schedule transmission counter
      LinTxCtr--;
    }
    lin_status = lin_getstatus(&cmd_lin);
    if (lin_status == LIN_ACTION_ONGOING){
      //do nothing
    }else if ((LIN_ACTION_COMPLETED == lin_status) ) {
      if (LIN_CMD_M_RX == cmd_lin.cmd) {
        DataReceived_Notification((msg_lin.id & 0x0F) ,id_to_dlc (msg_lin.id));
        Lin_clear_status();
      } else {
#ifdef _SLEEP_TIMOUT_DETECTION
        //check if sleep command has been sent
        if(lin_sleep_message_request) {
          lin_sleep_message_request = 0;
          lin_master_in_sleep_mode = 1;
          //lin sleep message has been successfully sent
          //go to sleep and wait for a wake up (from slave or master command)
          Lin_cmd_gotosleep();
        }
#endif
      }
    } else if ((LIN_AVAILABLE == lin_status) && (LinTxCtr == 0) )  {
      Start_Lin_Master() ;
    } else {//error occured ?
#ifdef CAN_LIN_GATEWAY_ENABLE
      if(lin_status & 0x80)
        SendCanErrorFrame = 1; //send a can error frame
      LinErrorMessage = lin_status;
#endif
#ifdef _COUNTING_ERRORS_ENABLE
      update_error_counter(lin_status);
#endif

      if(LinTxCtr == 0) {
        Start_Lin_Master() ;
      }
    }
    return 0;
  }


#ifdef _COUNTING_ERRORS_ENABLE
  //----------------------------------------------------------------------------//
  // Description: updates error counters
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
