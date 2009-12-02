/*H**************************************************************************
* $RCSfile: uart_drv.h,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $
* REVISION:     $Revision: 1.1.2.2 $
* FILE_CVSID:   $Id: uart_drv.h,v 1.1.2.2 2004/04/26 06:57:58 jberthy Exp $
*----------------------------------------------------------------------------
* PURPOSE:
* Provide Uart driver
*****************************************************************************/

#ifndef _UART_DRV_H_
#define _UART_DRV_H_

#define MSK_UART_5BIT           0x00 /* UCSRnC register */
#define MSK_UART_6BIT           0x02
#define MSK_UART_7BIT           0x04
#define MSK_UART_8BIT           0x06
#define MSK_UART_9BIT           0x06

#define MSK_UART_RX_DONE        0x80 /* UCSRnA register */
#define MSK_UART_TX_COMPLET     0x40
#define MSK_UART_DRE            0x20

#define MSK_UART_ENABLE_IT_RX   0x80
#define MSK_UART_ENABLE_IT_TX   0x40
#define MSK_UART_ENABLE_RX      0x10 /* UCSRnB register */
#define MSK_UART_ENABLE_TX      0x08
#define MSK_UART_TX_BIT9        0x01
#define MSK_UART_RX_BIT9        0x02

#ifdef USE_UART
    #define UCSRC   (UCSRC)
    #define UCSRB   (UCSRB)
    #define UCSRA   (UCSRA)
    #define UDR     (UDR)
    #define UBRRL   (UBRRL)
    #define UBRRH   (UBRRH)
#endif

#ifdef USE_UART1
    #define UCSRC   (UCSR0C)
    #define UCSRB   (UCSR0B)
    #define UCSRA   (UCSR0A)
    #define UDR     (UDR0)
    #define UBRRL   (UBRR0L)
    #define UBRRH   (UBRR0H)
#endif

#ifdef USE_UART2
    #define UCSRC    (UCSR1C)
    #define UCSRB    (UCSR1B)
    #define UCSRA    (UCSR1A)
    #define UDR      (UDR1)
    #define UBRRL    (UBRR1L)
    #define UBRRH    (UBRR1H)
#endif


#define Uart_hw_init(config)    (UCSRC=config)
#define Uart_enable()           (UCSRB |= MSK_UART_ENABLE_RX|MSK_UART_ENABLE_TX)
#define Uart_tx_ready()         (UCSRA&MSK_UART_DRE)
#define Uart_set_tx_busy()
#define Uart_send_byte(ch)      (UDR=ch)
#define Uart_rx_ready()         (UCSRA&MSK_UART_RX_DONE)
#define Uart_get_byte()         (UDR)
#define Uart_get_error()        (UCSRA & 0x10)
#define Uart_ack_rx_byte()

#define Uart_enable_it_rx()    (UCSRB|= MSK_UART_ENABLE_IT_RX)
#define Uart_enable_it_tx()    (UCSRB|= MSK_UART_ENABLE_IT_TX)
#define Uart_disable_it_rx()   (UCSRB&=~MSK_UART_ENABLE_IT_RX)

#endif

