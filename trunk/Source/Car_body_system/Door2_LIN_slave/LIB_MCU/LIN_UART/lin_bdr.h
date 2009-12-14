/*
**
****************************************************************************
**
**
**             Copyright (c) 2004/2005 - Atmel Corporation
**             Proprietary Information
**
** Project    	: AVR LIN CONTROLLER EXAMPLE
** Module     	: lin_bdr.h
** Description	: lin baud rate header file
**
**
** Version :     Date:         Author:      Comment:
**    1.0        5.1.2005    E.G.          Creation
**    1.1        7.4.2005    E.G.
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

#ifndef _LIN_BDR_H_
#define _LIN_BDR_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/


/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**
* The TIMEOUT_HEADER is 35-bit + 40% width .
*
*/

#ifndef LIN_CONFIG
#error You must define LIN_CONFIG (0 for master, 1 for slave) in config.h
#endif

#ifndef LIN_BAUDRATE
#error You must define LIN_BAUDRATE (2400/480/9600/19200)  in config.h
#endif

#ifndef FOSC
#error You must define FOSC in config.h

#elif FOSC == 8000
#if LIN_BAUDRATE == 19200
#if LIN_CONFIG == 1 //1:slave 0:master
#define UART_BAUDRATE_SYNC (32) //synch break detection
#endif

#if LIN_CONFIG == 0 //1:slave 0:master
#define UART_BAUDRATE_SYNC (37) //synch break generation
#endif

#define UART_BAUDRATE_NORM          (25)
#define UART_BAUDRATE_SYNC_MASTER   (37)  //synch break generation (13 Tbit)
#define TIMEOUT_M_HEADER            (0xAF)
#define TIMEOUT_S_HEADER            (0xB9)
#define TIMEOUT_RESPONSE_2DATA      (0xBA)
#define TIMEOUT_RESPONSE_4DATA      (0x8D)
#define TIMEOUT_RESPONSE_8DATA      (0x31)
#define TIMER_2_PRESCALER           (256)

#elif LIN_BAUDRATE == 9600
#if LIN_CONFIG == 1 //1:slave 0:master
#define UART_BAUDRATE_SYNC (66) //synch break detection
#endif

#if LIN_CONFIG == 0 //1:slave 0:master
#define UART_BAUDRATE_SYNC          (75) //synch break generation
#endif

#define UART_BAUDRATE_NORM          (51)
#define UART_BAUDRATE_SYNC_MASTER   (75)  //synch break generation (13 Tbit)
#define TIMEOUT_M_HEADER            (213)
#define TIMEOUT_S_HEADER            (211)
#define TIMEOUT_RESPONSE_2DATA      (220)
#define TIMEOUT_RESPONSE_4DATA      (197)
#define TIMEOUT_RESPONSE_8DATA      (151)
#define TIMER_2_PRESCALER           (1024)
#elif LIN_BAUDRATE == 4800
#error " GIVEN LIN BAUDRATE IS NOT SUPPORTED (CONFIG.H) "
#elif LIN_BAUDRATE == 2400

#if LIN_CONFIG == 1 //1:slave 0:master
#define UART_BAUDRATE_SYNC (269) //synch break detection
#endif

#if LIN_CONFIG == 0 //1:slave 0:master
#define UART_BAUDRATE_SYNC (300) //synch break generation
#endif

#define UART_BAUDRATE_NORM          (207)
#define UART_BAUDRATE_SYNC_MASTER   (300)  //synch break generation (13 Tbit)
#define TIMEOUT_M_HEADER            (97)
#define TIMEOUT_S_HEADER            (125)
#define TIMEOUT_RESPONSE_2DATA      (117)
#define TIMEOUT_RESPONSE_4DATA      (26)
#define TIMEOUT_RESPONSE_8DATA      (99) //overflow
#define TIMER_2_PRESCALER           (1024)
#else
#error " GIVEN LIN BAUDRATE IS NOT SUPPORTED (CONFIG.H) "
#endif

#else
#error no values are entered for this frequency in the  $RCSfile: lin_bdr.h,v $
#endif

#endif /* _LIN_BDR_H_ */
