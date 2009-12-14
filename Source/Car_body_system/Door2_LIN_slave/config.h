/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN MASTER EXAMPLE (JSB)
** Module     	: config.h
** Description	: configuration file
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

#define AT90CAN128_USED
//#define ATMEGA168_USED
//#define ATMEGA88_USED
//#define ATMEGA48_USED

#ifdef  ATMEGA168_USED
#include <iom168.h>
#include "lib_board/board_example_mega168.h"
#define  USE_UART1
#endif

#ifdef  ATMEGA88_USED
#include <iom88.h>
#include "lib_board/board_example_mega88.h"
#define  USE_UART1
#endif

#ifdef AT90CAN128_USED
#include <avr/io.h>
#define  USE_UART2
#endif

#ifndef CONFIG_H
#define CONFIG_H


/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ LIN LIBRARY configuration ______*/
#define FOSC 8000 // in kHz
#define LIN_BAUDRATE 19200
#define BAUDRATE LIN_BAUDRATE
//#define LIN_BAUDRATE 2400
#define LIN_CONFIG    1 //1:slave 0:master

#ifndef U8_TYPE_DEF
#define U8_TYPE_DEF
typedef unsigned char U8;
#endif

#ifndef U16_TYPE_DEF
#define U16_TYPE_DEF
typedef unsigned int U16 ;
#endif


#endif
