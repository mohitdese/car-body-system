/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE (USART)
** Module     	: LIN_DRV.C
** Description	: LIN v1.3 CONTROLLER (drivers)
** Target    	: ATMEGA48/88/168 and AT90CAN128
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
#include "lin_drv_usart.h"
#include <avr/interrupt.h>
#include "mcu_avr_atmega128_api.h"

/*_____ G L O B A L S ________________________________________________________*/
volatile U8  _lin_id;          //without parity bits
volatile U8  _lin_id_w_parity; //lin v1.3 identifier with computed parity bits
volatile U8  _lin_tab_data[8]; //8 bytes data buffer
volatile U8  _lin_index;      //index for data buffer
volatile U8  _lin_status;
volatile U8  _lin_err;    //lin error status
volatile U8  _lin_state;  //lin state machine status
volatile U8  _lin_cmd;    //lin current command
volatile U8  _lin_cpt;    //counter
volatile U16 _lin_crc;    //lin checksum

#if LIN_CONFIG == 0 //1:slave 0:master
volatile  U8 lin_master_in_sleep_mode;
#endif

#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
extern volatile U8 _lin_synchronized;    //lin is synchronized with master
extern  volatile U8 osccal_start;
#endif

/*_____ P R I V A T E - F U N C T I O N S - D E C L A R A T I O N ____________*/
/*_____ L O C A L S __________________________________________________________*/
/*_____ P U B L I C - F U N C T I O N S ______________________________________*/
/*_____ P R I V A T E - F U N C T I O N S ____________________________________*/


#ifdef AT90CAN128_USED
#ifdef USE_UART2
#define LIN_RX_VECTOR USART1_RX_vect
#endif

#ifdef USE_UART1
#define LIN_RX_VECTOR USART0_RX_vect
#endif

#endif



#ifdef ATMEGA168_USED
#define LIN_RX_VECTOR USART_RX_vect
#endif

#ifdef ATMEGA88_USED
#define LIN_RX_VECTOR USART_RX_vect
#endif

#ifdef ATMEGA48_USED
#define LIN_RX_VECTOR USART_RX_vect
#endif


#ifdef AT90PWM3_USED
#define LIN_RX_VECTOR USART_RX_vect
#endif


/**
* @fn _lin_it_uart_rx
* @brief This interrupt function manages the State machine of the LIN.
* The receive interrrupt is used to be able to check at each byte
* if the byte monitored (received) is the same than  byte sent.
**/
ISR(LIN_RX_VECTOR) {

  
  switch (_lin_state)  {

    /** STATE_CHECK_BREAK: This state is used for both SLAVE and MASTER
    * configuration.
    * This step checks that a valid SYNCHBREAK character has been sent/received.
    * Note:
    *    in MASTER, the checksum is computed and the synchro character is
    * sent
    */
  case STATE_CHECK_BREAK:
    if (0x00 == Uart_get_byte()){  //NO STOP BIT CHECK HERE !!
      Uart_set_baudrate(UART_BAUDRATE_NORM);
#ifdef _RUN_TIME_RC_CALIBRATION_ENABLE
      Start_Capturing();
      OSCCAL = osccal_start;
#endif
      switch(_lin_cmd){
      case LIN_CF_S_TX: // no break, kiitos;
      case LIN_CF_S_RX:

#ifdef _TIMOUT_DETECTION
        Timer2_start(TIMEOUT_S_HEADER,0);
#endif
        _lin_state = STATE_CHECK_SYNC;
        break;

      case LIN_CF_M_TX:
      case LIN_CF_M_RX:
        Uart_send_byte(0x55);
        _lin_crc = LIN_CalcChecksum();
        _lin_state = STATE_CHECK_SYNC;
        break;

      default:
        _lin_state = STATE_CHECK_SYNC;
        break;
      }
    }else{
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_state = STATE_CHECK_BREAK;
    }
    break;


    /** STATE_CHECK_SYNC: This state is used for both MASTER and SLAVE
    * configurations. This step verifies that a valid synchro byte has
    * been (sent/received).
    * Note:
    *   in MASTER,the parity is computed and the identifier is sent
    */
  case STATE_CHECK_SYNC:
    /*  if (Uart_get_error()){
    Timer2_stop();
    _lin_err = MSK_STATUS_ERRBIT;
    _lin_status = MSK_STATUS_ERR;
    _lin_state = STATE_CHECK_BREAK;
    Uart_set_baudrate(UART_BAUDRATE_SYNC);
  }else  if ((0x55 == Uart_get_byte()) ){
    */
    if ((0x55 == Uart_get_byte()) ){
      switch(_lin_cmd){
      case LIN_CF_M_TX:
      case LIN_CF_M_RX:
        //compute whole ID here for example, INCLUNDING P0 / P1 parity  bits
        _lin_id_w_parity = LIN_InitIdField(_lin_id ); //add P0/P1 parity bits
        Uart_send_byte(_lin_id_w_parity);
        _lin_state = STATE_ID_TX;
        break;
      default: // in slave mode
        _lin_state = STATE_ID_RX;
        break;
      }
    }else{
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_state = STATE_CHECK_BREAK;
      _lin_status = MSK_STATUS_ERR;
      switch(_lin_cmd){
      case LIN_CF_M_TX:
      case LIN_CF_M_RX:
        _lin_err   = MSK_STATUS_ERRBIT;
        break;
      default: // in slave mode
        _lin_err = MSK_STATUS_ERRSYNCHRO;
        break;
      }
    }
    break;

    /** STATE_ID_TX: This state is used only in MASTER
    * configuration. This step verifies if the lin indentifier has
    * been correctly sent.
    * Note:
    *   in Tx frame, the master starts sending data.
    *   in Tx request, the master switches in data reception mode,
    * The number of data to received is computed.
    */
  case STATE_ID_TX:
    if (Uart_get_error()){
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }else if((_lin_id_w_parity == Uart_get_byte())){
      switch (_lin_cmd){
      case LIN_CF_M_TX:
        _lin_index = Lin_dlctonbyte(_lin_id);
        if (_lin_index){
          Uart_send_byte(_lin_tab_data[0x00]);
          _lin_state = STATE_DATA_TX;
          _lin_index--;
          _lin_cpt = 0x01;
        }else{
          /* to be defined */
        }
        break;

      case LIN_CF_M_RX:
        _lin_state = STATE_DATA_RX;
        _lin_cpt  = 0x00;
        _lin_index = Lin_dlctonbyte(_lin_id);
        break;

      default:
        break;
      }
    }else{
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_state = STATE_CHECK_BREAK;
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }
    break;

    /** STATE_ID_RX: This state is used only in SLAVE configuration.
    * This step verifies that a valid indentifier has beeen received.
    * Note: the number of data to be received is computed.
    */
  case STATE_ID_RX:
    _lin_id = Uart_get_byte();
#ifdef _TIMOUT_DETECTION
    Timer2_stop();
#endif
    //check for parity error or frame error
    if ((_lin_id != LIN_InitIdField(_lin_id)) || ( Uart_get_error() )){
      _lin_err = MSK_STATUS_ERRPARITY;
      _lin_status = MSK_STATUS_ERR;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }else{
      _lin_status = MSK_STATUS_IDOK;
      _lin_state = STATE_DATA_RX;
      _lin_cpt =0;
      _lin_index = Lin_dlctonbyte(_lin_id);
    }
    break;

    /** STATE_DATA_RX: This state is used for both SLAVE and MASTER
    * configurations (SLAVE TASK).This step stores the incoming data into the temp buffer.
    * Note:
    *    The number of data to store was computed and stored in _lin_index.
    */
  case STATE_DATA_RX:
    if (Uart_get_error()){
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    } else {
      _lin_tab_data[_lin_cpt]=Uart_get_byte();
      _lin_cpt++;
      if(_lin_cpt == _lin_index){
        _lin_crc  =  LIN_CalcChecksum();
        _lin_state = STATE_CRC_RX;
      }
    }
    break;

    /** STATE_DATA_TX: This state is used for both SLAVE and MASTER
    * configurations (SLAVE TASK). This step verifies that previous
    * sent data byte was not corrupted, and next byte is sent.
    * Note:
    *  The number of data to transmit was computed and stored in
    * _lin_index.
    */
  case STATE_DATA_TX:
    if (Uart_get_error()){//stop bit error
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }else if ((_lin_tab_data[ _lin_cpt-1 ] == Uart_get_byte())  ){
      if (_lin_index){
        Uart_send_byte(_lin_tab_data[_lin_cpt]);
        _lin_index--;
        _lin_cpt++;
      }else{
        Uart_send_byte( _lin_crc);
        _lin_state = STATE_CRC_TX;
      }
    }else{
#ifdef _TIMOUT_DETECTION
      Timer2_stop();
#endif
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }
    break;

    /** STATE_CRC_RX: This state is used for both SLAVE and MASTER
    * configurations (SLAVE TASK). This step verifies that a valid checksum has been received .
    * Note:
    *    If the CRC is correct then the status RXOK is returned.
    */
  case STATE_CRC_RX:
    if (Uart_get_error()) {
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
    } else if ( ((_lin_crc) & (0x00FF)) != Uart_get_byte() ) {
      _lin_err = MSK_STATUS_ERRCSUM;
      _lin_status = MSK_STATUS_ERR;
    }else{
      _lin_status = MSK_STATUS_RXOK;
    }
#ifdef _TIMOUT_DETECTION
    Timer2_stop();
#endif
    _lin_state = STATE_CHECK_BREAK;
    Uart_set_baudrate(UART_BAUDRATE_SYNC);
    break;

    /** STATE_CRC_TX: This state is used for both SLAVE and MASTER
    * configurations (SLAVE TASK). This state verifies if CRC byte has been
    * correctly sent (bit error detection).
    * Note:
    *    If the CRC is correct then the status TXOK is returned.
    */
  case STATE_CRC_TX:
    if (Uart_get_error()) { //check frame error (stop bit)
      _lin_err = MSK_STATUS_ERRBIT;
      _lin_status = MSK_STATUS_ERR;
    }else  if (((_lin_crc) & 0xFF) != Uart_get_byte()){
      _lin_err = MSK_STATUS_ERRCSUM;
      _lin_status = MSK_STATUS_ERR;
    }else{
      _lin_status = MSK_STATUS_TXOK;
    }
#ifdef _TIMOUT_DETECTION
    Timer2_stop();
#endif
    _lin_state = STATE_CHECK_BREAK;
    Uart_set_baudrate(UART_BAUDRATE_SYNC);
    break;

    /** STATE_SLEEP: This state is used for both MASTER and SLAVE
    * configurations. This step is used to memeorize that controller is in sleep mode.
    * Note:
    *
    */
#ifdef _SLEEP_TIMOUT_DETECTION
  case STATE_SLEEP:
    _lin_err = MSK_STATUS_SLEEP_MODE;
    _lin_status = MSK_STATUS_ERR;

    if( Uart_get_byte()==0x80) {
#if LIN_CONFIG == 0 //1:slave 0:master
      lin_master_in_sleep_mode = 0;
#elif LIN_CONFIG == 1 //1:slave 0:master
      NoBusActivityFlag = 0;
      SleepCtr = 0;
#else
#error "  "
#endif
      _lin_status = MSK_STATUS_RXOK;
      _lin_err = MSK_STATUS_SLEEP_MODE;
      LINSleepFlag = 0;
      _lin_state = STATE_CHECK_BREAK;
      Uart_set_baudrate(UART_BAUDRATE_SYNC);
    }
    break;
#endif

  default:
#ifdef _TIMOUT_DETECTION
    Timer2_stop();
#endif
    _lin_state = STATE_CHECK_BREAK;
    Uart_set_baudrate(UART_BAUDRATE_SYNC);
    break;

  }//end case
}//end usart rxc isr

/**
* @fn _lin_dlctonbyte
* @brief extract dlc info from a full 6 bits LIN Identifier
*
**/
U8 Lin_dlctonbyte(U8 _lin_id){
  _lin_index = (_lin_id&0x30) >>4;
#ifdef _TIMOUT_DETECTION
  Timer2_stop();
#endif
  if (_lin_index < 2){
#ifdef _TIMOUT_DETECTION
    Timer2_start(TIMEOUT_RESPONSE_2DATA,2);
#endif
    _lin_index = 2;
  }else if (2 == _lin_index){
#ifdef _TIMOUT_DETECTION
    Timer2_start(TIMEOUT_RESPONSE_4DATA,4);
#endif
    _lin_index = 4;
  }else{
#ifdef _TIMOUT_DETECTION
    Timer2_start(TIMEOUT_RESPONSE_8DATA,8);
#endif
    _lin_index = 8;
  }
  return _lin_index;
}

/**
* @fn uart_set_baudrate
* @brief initiatilisation of USART baud rate register
**/
void Uart_set_baudrate(U16 expected_baudrate ) {

#ifdef  USE_UART1
  UBRR0L =  (U8)(expected_baudrate&0xFF);
  UBRR0H =  (U8)((expected_baudrate&0xFF00)>>8);
#endif

#ifdef  USE_UART2
  UBRR1L =  (U8)(expected_baudrate&0xFF);
  UBRR1H =  (U8)((expected_baudrate&0xFF00)>>8);
#endif
}

/**
* @fn _lin_cmdm_txdata
* @brief .
**/
void Lin_cmdm_txdata(void){
#ifdef _TIMOUT_DETECTION
  Timer2_start(TIMEOUT_M_HEADER,0);
#endif
  Uart_set_baudrate(UART_BAUDRATE_SYNC_MASTER);
  Uart_send_byte(0);//send SynchBreak
  _lin_status = MSK_STATUS_BSY;
  _lin_cmd =LIN_CF_M_TX;
}

/**
* @fn _lin_cmdm_rxdata
* @brief In master mode, Master starts by sending Lin Header
**/
void Lin_cmdm_rxdata(void){
#ifdef _TIMOUT_DETECTION
  Timer2_start(TIMEOUT_M_HEADER,0);
#endif
  Uart_set_baudrate(UART_BAUDRATE_SYNC_MASTER);
  Uart_send_byte(0); //send SynchBreak
  _lin_status = MSK_STATUS_BSY;
  _lin_cmd =LIN_CF_M_RX;
}

/**
* @fn performs hardware initialisation
* @brief
**/
void Lin_hw_init(void) {
  Uart_hw_init(MSK_UART_8BIT);
//  ENABLE_LIN();

#ifdef ATMEGA168_USED
  UCSR0B = (1<<RXEN0 ) | (1<<TXEN0) | (1<<RXCIE0) ;
#endif

#ifdef ATMEGA88_USED
  UCSR0B = (1<<RXEN0 ) | (1<<TXEN0) | (1<<RXCIE0) ;
#endif

#ifdef ATMEGA48_USED
  UCSR0B = (1<<RXEN0 ) | (1<<TXEN0) | (1<<RXCIE0) ;
#endif

#ifdef AT90PWM3_USED
  UCSR0B = (1<<RXEN0 ) | (1<<TXEN0) | (1<<RXCIE0) ;
#endif

#ifdef AT90CAN128_USED
#ifdef USE_UART2
  UCSR1B = (1<< RXEN1 ) | (1<<TXEN1) | (1<<RXCIE1);
#endif

#ifdef USE_UART1
  UCSR0B = (1<<RXEN0 ) | (1<<TXEN0) | (1<<RXCIE0);
#endif
#endif
}

