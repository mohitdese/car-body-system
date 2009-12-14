/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE
** Module     	: LIN_LIB.C
** Description	: LIN v1.3 CONTROLLER (library)
** Target       : AVRs
** Compiler    	: IAR Embedded Workbench 4.1
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.1.2005    E.G.          Creation
**
**
**
** LICENSE -
**
** ATMEL - 2004/2005
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

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "config.h"
#include "lin_lib.h"
#include "lin_drv_usart.h"

/*_____ G L O B A L S ________________________________________________________*/
#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
extern volatile U8  Timer_IC_cnt;
extern volatile U8 _lin_synchronized;    //lin is synchronized with master (1 - ok within 2%)
#endif

/*_____ P R I V A T E - F U N C T I O N S - D E C L A R A T I O N ____________*/

/*_____ L O C A L S __________________________________________________________*/

/*_____ P U B L I C - F U N C T I O N S ______________________________________*/

/*_____ P R I V A T E - F U N C T I O N S ____________________________________*/


/**
* @fn lin_init
* @brief This function initializes the lin controller.
**/
U8 lin_init (void)
{
  Lin_hw_init();
  Lin_set_baudrate();
  Lin_clear_status() ;

#ifdef _COUNTING_ERRORS_ENABLE
  countCERR=0;
  countRERR=0;
  countBERR=0;
  countSERR=0;
  countPERR=0;
#endif

#ifdef _SLEEP_TIMOUT_DETECTION
  LINSleepFlag = 0;
#if LIN_CONFIG == 1 //1:slave 0:master
  SleepCtr = 0;
  NoBusActivityFlag= 0;
  MyCpuIsSleepingFlag = 0;
#endif
#endif

#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
  _lin_synchronized = 0;
#endif
  LIN_TIMER_init(); //to start lin associated timer (tick function)
  return 1;
}


/**
* @fn lin_cmd
* @brief This is the lin command interpreter
**/
U8 lin_cmd (st_cmd_lin_t* pt_cmd) {
  U8 status;
  U8 cpt, temp;

  if ((pt_cmd->cmd ==LIN_CMD_S_TX) &&(pt_cmd->handle == 1)){
    temp = lin_dlc_to_nb_byte(pt_cmd->pt_lin_msg->id >> 4);
    Lin_reset_index();
    for (cpt=0; cpt < temp; cpt++){
      Lin_set_data(*(pt_cmd->pt_lin_msg->pt_data + cpt));
    }
    Lin_cmds_txdata();
    status = LIN_CMD_ACCEPTED;
  }else if ( Lin_get_busy() ){
    status = LIN_CMD_REFUSED;
  } else{
    status = LIN_CMD_ACCEPTED;
    Lin_clear_status();
    Lin_clear_error_status();
    switch(pt_cmd->cmd){
    case LIN_CMD_M_TX:
      pt_cmd->handle = 1;
      temp = lin_dlc_to_nb_byte(pt_cmd->pt_lin_msg->id >> 4);
      Lin_reset_index();
      for (cpt=0; cpt < temp; cpt++){
        Lin_set_data(*(pt_cmd->pt_lin_msg->pt_data + cpt));
      }
      Lin_set_id(pt_cmd->pt_lin_msg->id);
      Lin_cmdm_txdata();
      break;

    case LIN_CMD_M_RX:
      pt_cmd->handle = 1;
      Lin_set_id(pt_cmd->pt_lin_msg->id);
      Lin_cmdm_rxdata();
      break;

    case LIN_CMD_S_RX:
      pt_cmd->handle = 1;
      Lin_cmds_rxdata();
      break;

    case LIN_CMD_WUP:
      pt_cmd->handle = 1;
      Lin_cmds_wkup();
      break;

    case LIN_CMD_SLP:
      pt_cmd->handle = 1;
      Lin_cmds_wkup();
      break;

    default:
      break;
    }
  }
  return(status);
}

/**
* @fn lin_getstatus
* @brief This function returns the lin controller status.
**/
lin_status_t lin_getstatus (st_cmd_lin_t* pt_cmd){
  lin_status_t status;
  U8 temp;
  U8 cpt;

  if (0 == pt_cmd->handle){
    status = LIN_AVAILABLE;
  }else{
    temp = Lin_get_status();
    if (temp == LIN_STATUS_TX){
      pt_cmd->handle = 0;
      Lin_clear_status();
      status = LIN_ACTION_COMPLETED;
    }else if (temp == LIN_STATUS_RX){
      pt_cmd->handle = 0;
      Lin_clear_status();
      temp = lin_dlc_to_nb_byte(pt_cmd->pt_lin_msg->id >> 4);
      Lin_reset_index();
      for ( cpt=0 ; cpt<temp ; cpt++){
        *(pt_cmd->pt_lin_msg->pt_data + cpt ) = Lin_get_data();
        Lin_inc_index();
      }
      status = LIN_ACTION_COMPLETED;
    }else if (temp == LIN_STATUS_ID){
      pt_cmd->handle = 1;
      Lin_clear_status();
      pt_cmd->pt_lin_msg->id = Lin_get_id();
      status = LIN_ID_RECEIVED;
    }else if (temp == LIN_STATUS_ERR){
      pt_cmd->handle = 0;
      Lin_clear_status();
      status = (lin_status_t)(0xC0 + Lin_get_error());
      Lin_clear_error_status();
    }else if (temp == LIN_STATUS_BUSY){
      status =LIN_ACTION_ONGOING;
    }else{
      if (_lin_state == 0) {
        status =  LIN_AVAILABLE;
      }
    }
  }
  return (status);
}

/**
* @fn nb_byte_to_lin_dlc
* @brief This function converts the lin dlc into
* the associated number of bytes.
**/
U8 lin_dlc_to_nb_byte(U8 dlc_field){
  U8 nb_byte;

  if (dlc_field < 2){
    nb_byte = 2;
  }else if (2 == dlc_field){
    nb_byte = 4;
  }else{
    nb_byte = 8;
  }
  return nb_byte;
}

/**
* @fn lin_dlc_to_nb_byte
* @brief This function gives the number of byte .
**/
U8 nb_byte_to_lin_dlc(U8 nb_byte){
  U8 lin_dlc;

  if (nb_byte <= 2){
    lin_dlc = 0;
  }else if (nb_byte <= 4){
    lin_dlc = 2;
  }else{
    lin_dlc = 3;
  }
  return lin_dlc;
}

/**
* @fn _lin_calcchecksum
* @brief Calculates the checksum based on LinData buffer content.
*
**/
U8 LIN_CalcChecksum (void){
  U16   uiResult = 0x0000;
  U8    index;

  // Add with carry for all LIN Data Bytes
  for(index = 0; index < _lin_index; index++) {
    uiResult += _lin_tab_data[index];     // Sum
    uiResult += ((uiResult & 0x100) >> 8);// Add Carry
    uiResult &= 0x00FF;                   // Clear Carry Bit
  }
  return (unsigned char)(~(uiResult & 0x00ff));  // Checksum is inverted sum
}

/**
* @fn lin_initidfiel
* @brief add parity bits to the LIN Identifier.
**/
U8 LIN_InitIdField(U8 lin_id ){
  U8 	LINParity0;
  U8 	LINParity1;
  U8    LinIdField;

  LinIdField = lin_id & 0x3F; //clear parity bits if set

  // Calculate Parity
  LINParity0 = (
                (LinIdField & 0x01)
                  ^
                    ((LinIdField & 0x02) >> 1)
                      ^
                        ((LinIdField & 0x04) >> 2 )
                          ^
                            ((LinIdField & 0x10) >> 4)
                              );

  LINParity1 = ( !(
                   ((LinIdField & 0x02) >> 1)
                     ^
                       ((LinIdField & 0x08) >> 3)
                         ^
                           ((LinIdField & 0x10) >> 4)
                             ^
                               ((LinIdField & 0x20) >> 5)
                                 )
                );


  LinIdField |=  ((LINParity1 & 0x01) << 7)|((LINParity0 & 0x01) << 6);  // Add parity to LIN ID Field
  return LinIdField;
}
