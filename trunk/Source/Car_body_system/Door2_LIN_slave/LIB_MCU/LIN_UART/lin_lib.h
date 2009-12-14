/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE
** Module     	: LIN_LIB.H
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
**
**
**
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

#ifndef _LIN_LIB_H_
#define _LIN_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "config.h"
#include "lib_mcu/lin_uart/lin_drv_usart.h"
#include "lib_mcu/lin_uart/runtime_calibration_lib.h"

#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#else
#include "lib_mcu/lin_uart/master_lin.h"
#endif

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
#define LIN_CMD_REFUSED  0
#define LIN_CMD_ACCEPTED 1

//bit definition for _lin_status
#define LIN_STATUS_RX   0x01
#define LIN_STATUS_TX   0x02
#define LIN_STATUS_ID   0x04
#define LIN_STATUS_ERR  0x08
#define LIN_STATUS_BUSY 0x10 //muutuu

/*_____ T Y P E D E F - D E C L A R A T I O N ________________________________*/
/**
* @brief This enum is used to select an action for a specific message declare
* on structure st_cmd_lin_t.
**/
typedef enum {
  LIN_CMD_M_NONE, 	// no command
  LIN_CMD_M_TX, 	// Master transmits (Frame Header + Response)
  LIN_CMD_M_RX,     // Master transmits (Frame Header) and switches to RX mode
  LIN_CMD_S_RX,     // Slave waits for (Header + Response)
  LIN_CMD_S_TX,     // Slave waits for (Header) and switches to Tx data
  LIN_CMD_SLP,		// Sleep mode command (broadcast)
  LIN_CMD_WUP		// Wakeup command 	
} lin_cmd_t;

/**
* @brief This enum is used to select the trigger of a transmission:
*  - Periodically
*  - on event
**/
typedef enum {
  TX_EVENT,
  TX_PERIODICALLY,
  Tx_DELAY
} lin_trigger_t;

/**
* @brief This enum is identified the return value of lin_getstatus:
*
**/
typedef enum {
  LIN_AVAILABLE,
  LIN_ACTION_COMPLETED,
  LIN_ID_RECEIVED,
  LIN_ACTION_ONGOING,
  LIN_ERROR_BIT = 0xC1,           //bit error
  LIN_ERROR_CSUM = 0xC2,          //checksum error
  LIN_ERROR_PARITY = 0xC4,        //parity error
  LIN_ERROR_SYNCHRO = 0xC8,       //synchro error
  LIN_ERROR_NO_ACTIVITY = 0xD0,  //no bus activity
  LIN_ERROR_NO_RESPONDING = 0xE0 , //slave not responding
  LIN_SLEEP_MODE = 0xF0
} lin_status_t;

/**
* @brief This structure allows to define Lin message.
**/
typedef  struct{
  U8 id; //lin identifier with dlc info
  U8* pt_data; //data pointer
} st_lin_t;

/**
* @brief This structure allows to define a specific action on LIN network.
**/
typedef  struct{
  U8 handle;  //command handler
  lin_cmd_t cmd; //command type
  st_lin_t* pt_lin_msg; //message descrition
} st_cmd_lin_t;


/*_____ P R O T O T Y P E S - D E C L A R A T I O N __________________________*/
/**
* @fn lin_init
* @brief This function initializes and
* enables software/hardware ressources
* needed to handle the LIN protocol.
**/
U8 lin_init (void);

/**
* @fn lin_cmd
* @brief This function is the LIN command interpreter .
* action to be executed are given by the cmd structure
* @param pt_cmd
**/
U8 lin_cmd (st_cmd_lin_t* pt_cmd);

/**
* @fn lin_getstatus
* @brief This function returns the LIN controller status.
* @return  lin_status
**/
lin_status_t lin_getstatus (st_cmd_lin_t* pt_cmd);

/**
* @fn dlc bits Conversion into number of bytes
* @brief
* @return number of bytes
**/
U8 nb_byte_to_lin_dlc(U8 dlc_trame);

/**
* @fn
* @brief
* @return
**/
U8 lin_dlc_to_nb_byte(U8 nb_byte);

/**
* @fn
* @brief
* @return
**/
U8 LIN_CalcChecksum (void);

/**
* @fn
* @brief
* @return
**/
U8 LIN_InitIdField(U8 lin_id ) ;

/**
* @fn
* @brief
* @return
**/
void Lin_hw_init(void);

#endif /* _LIN_LIB_H_ */

