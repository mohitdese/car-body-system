/*C**************************************************************************
* $RCSfile: uart_lib.c,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.20.4.2 $     
* FILE_CVSID:   $Id: uart_lib.c,v 1.20.4.2 2004/03/31 05:58:55 jberthy Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This file provides a minimal VT100 terminal access through UART
* and compatibility with Custom I/O support
*****************************************************************************/

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "uart_lib.h"
#include "lin_drv_usart.h"



/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/



unsigned char uart_init (void)
{
  Uart_hw_init(UART_CONFIG);
  Uart_set_baudrate(BAUDRATE);
  Uart_enable();
  return 1;
}


unsigned char uart_putchar (unsigned char ch)
{
  while(!Uart_tx_ready());
  Uart_set_tx_busy(); // Set Busy flag before sending (always)
  Uart_send_byte(ch);
  return ch;
}




char uart_getchar (void)
{
  register char c;

  while(!Uart_rx_ready());
  c = Uart_get_byte();
  Uart_ack_rx_byte();
  return c; 
}


