/**
 * @file $RCSfile: uart_lib.h,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file contains Uart lib header file.
 *
 * @version $Revision: 1.14.4.3 $ $Name:  $
 *
 * @todo
 * @bug
 */

#ifndef _UART_LIB_H_
#define _UART_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "uart_drv.h"


/*_____ D E F I N I T I O N S ______________________________________________*/
#ifndef UART_CONFIG
#define UART_CONFIG     MSK_UART_8BIT
#endif

#ifndef BAUDRATE
#error You must define BAUDRATE in config.h
#define BAUDRATE        999
#endif

#define AUTOBAUD        0

#if BAUDRATE == AUTOBAUD
#define Uart_set_baudrate(bdr)  autobaud()
#include "modules/autobaud/autobaud.h"
#else
#include "lib_mcu/uart/uart_bdr.h"
#endif

/*_____ D E C L A R A T I O N ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/

/**
 * @brief This function configures the UART configuration and timming following 
 * the constant definition of BAUDRATE and enables the UART controller.
 *
 * @pre before calling this function some declaration must be define in config.h:\n
 * - FOSC Frequency of crystal in kHz (mandatory)Standard crystals available
 * - BAUDRATE Baudrate in bit per second used for UART configuration (mandatory)
 * - UART_CONFIG : (by default 8 bits| 1 bit stop |without parity)
 *
 */
unsigned char uart_init (void);

/**
 * @brief This function allows to send a character on the UART
 *
 * @param uc_wr_byte character to print on UART.
 *
 * @return character sent.
 *
 * @par Note:
*  the type p_uart_ptchar and r_uart_ptchar can be define to macth with a printf
 * need.
 *
 */
unsigned char uart_putchar (unsigned char uc_wr_byte);

/**
 * @brief This function allows to get a character from the UART
 *
 * @return character read.
 *
 */
char uart_getchar (void);

#endif /* _UART_LIB_H_ */
