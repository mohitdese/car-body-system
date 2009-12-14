/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE (USART)
** Module     	: LIN_DRV.H
** Description	: LIN driver for USART header file
** Target       : ATMEGA48/88/168 and AT90CAN128
** Compiler    	: IAR Embedded Workbench 4.1
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.1.2005      E.G.          created
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

#ifndef _LIN_DRV_H_
#define _LIN_DRV_H_


/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "config.h"
#include "lib_mcu/uart/uart_drv.h"
#include "lib_mcu/lin_uart/lin_bdr.h"
#include "lib_mcu/timer/timer2_lib.h"
#include "lib_mcu/lin_uart/runtime_calibration_lib.h"

#if LIN_CONFIG == 1 //1:slave 0:master
#include "lib_mcu/lin_uart/slave_lin.h"
#else
#include "lib_mcu/lin_uart/master_lin.h"
#endif

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/

//bit definition for "_lin_status" variable
#define MSK_STATUS_RXOK		0x01	
#define MSK_STATUS_TXOK		0x02
#define MSK_STATUS_IDOK		0x04	
#define MSK_STATUS_ERR		0x08	
#define MSK_STATUS_BSY          0x10

//bit definition for "_lin_err" variable
#define MSK_STATUS_ERRBIT       0x01
#define MSK_STATUS_ERRCSUM      0x02
#define MSK_STATUS_ERRPARITY    0x04
#define MSK_STATUS_ERRSYNCHRO   0x08
#define MSK_STATUS_TIME_OUT     0x20
#define MSK_STATUS_SLEEP_MODE   0x40

//bit definition for "_lin_state" variable
#define STATE_CHECK_BREAK   0x00
#define STATE_CHECK_SYNC    0x01
#define STATE_ID_TX         0x02
#define STATE_ID_RX         0x03
#define STATE_DATA_TX       0x04
#define STATE_DATA_RX       0x05
#define STATE_CRC_TX        0x06
#define STATE_CRC_RX        0x07
#define STATE_SLEEP         0x08

//bit definition for lin command variable
#define LIN_CF_M_TX   0x01
#define LIN_CF_M_RX   0x02
#define LIN_CF_S_TX   0x03
#define LIN_CF_S_RX   0x04
#define LIN_CF_S_WKUP 0x05

//#define Lin_set_baudrate()  (  Uart_enable_it_rx(), Uart_set_baudrate(UART_BAUDRATE_SYNC))

#define Lin_set_baudrate()  (Uart_set_baudrate(UART_BAUDRATE_SYNC))

#define Lin_enable()        (Uart_enable())

#define Lin_reset_index()  (_lin_index=0)
#define Lin_inc_index()    (_lin_index++)
#define Lin_set_data(data) (_lin_tab_data[_lin_index] = data, _lin_index++)
#define Lin_get_data()     (_lin_tab_data[_lin_index])
#define Lin_set_id(id)     (_lin_id = id)
#define Lin_get_id()       (_lin_id & 0x3F)


#define Lin_cmds_txdata()  (Lin_set_baudrate(), \
  Uart_set_baudrate(UART_BAUDRATE_NORM), \
    Uart_send_byte(_lin_tab_data[0x00]), \
      _lin_status = MSK_STATUS_BSY, \
        _lin_cmd =LIN_CF_S_TX, \
          _lin_state = STATE_DATA_TX, \
            _lin_cpt = 0x01 , \
              _lin_crc = LIN_CalcChecksum(), \
                _lin_index--)

#define Lin_cmds_rxdata()        (_lin_status = MSK_STATUS_BSY, _lin_cmd =LIN_CF_S_RX)
#define Lin_cmds_wkup()          ( Uart_set_baudrate(UART_BAUDRATE_NORM), Uart_send_byte(0x80),_lin_status = MSK_STATUS_TXOK)
#define Lin_cmd_gotosleep()      ( Uart_set_baudrate(UART_BAUDRATE_NORM), _lin_state = STATE_SLEEP)
#define Lin_get_status()         (_lin_status & 0x1F)
#define Lin_get_busy()           (_lin_status & MSK_STATUS_BSY)
#define Lin_get_error()          (_lin_err)
#define Lin_clear_status()       ( _lin_status = 0)
#define Lin_clear_error_status() (_lin_err = 0)

U8 Lin_dlctonbyte(U8 _lin_id);
void Uart_set_baudrate(U16 expected_baudrate) ;
void Lin_cmdm_txdata(void);
void Lin_cmdm_rxdata(void);

/*_____ T Y P E D E F - D E C L A R A T I O N ________________________________*/
extern volatile U8  _lin_id;
extern volatile U8  _lin_tab_data[8];
extern volatile U16 _lin_crc;
extern volatile U8  _lin_index;
extern volatile U8  _lin_status;
extern volatile U8  _lin_err ;
extern volatile U8  _lin_cmd;
extern volatile U8  _lin_state;
extern volatile U8  _lin_cpt;
#if LIN_CONFIG == 0 //1:slave 0:master
extern volatile  U8 lin_master_in_sleep_mode;
#endif


#endif /* _LIN_DRV_H_ */

