/*C**************************************************************************
* $RCSfile: uartnoint_lib.c,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: uartnoint_lib.c,v 1.1 2003/07/04 13:17:56 jberthy Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This template file can be parsed by langdoc for automatic documentation
* generation.
* FILE_PURPOSE: provide low level functions for UART running without interrupts   
******************************************************************************/



//-----------------------------------------------------------------------
// Exemple of using
// ===================
//
// #define XTAL_CPU_8         8000000      //       8 Mhz
// #define BAUD_RATE_9600        9600      //  9600  baud
// #define BAUD_RATE_115K2     115200      // 115.2 Kbaud
//
// InitUART_0(UART_BAUD(BAUD_RATE_115K2,XTAL_CPU_8)); // Setup the UART 0
// InitUART_1(UART_BAUD(BAUD_RATE_9600, XTAL_CPU_8)); // Setup the UART 1
//
// CPrintfUART_0("\r\nBonjour, Maitre\r\n");
//
// DPrintfUART_1(UART_Temp);
//------------------------------------------------------------------------



/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include "config.h"
#include "uartnoint_lib.h"

/*_____ G L O B A L S ________________________________________________________*/


/*_____ P R I V A T E - F U N C T I O N S - D E C L A R A T I O N ____________*/


/*_____ L O C A L S __________________________________________________________*/


/*_____ P U B L I C - F U N C T I O N S ______________________________________*/


/*_____ P R I V A T E - F U N C T I O N S ____________________________________*/




/*F****************************************************************************
* FUNCTION_NAME: InitUART_0                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: - initialize UART0: 8-bit, no parity, 2 stop-bit
*                   - set baudrate
*                   - no INT     
* FUNCTION_INPUTS : Uint16 = "baud" using the macro UART_BAUD()                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void InitUART_0 (Uint16 baud)
    {
        UCSR0B = 0 ;  // disable UART_0

        UBRR0H = (Uchar) (baud>>8);
        UBRR0L = (Uchar)  baud    ;

        UCSR0A = (1<<UDRE0);
        UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);

        UCSR0B = (1<<RXEN0) | (1<<TXEN0);  // then, (re)enable UART_0
    }


/*F****************************************************************************
* FUNCTION_NAME: InitUART_1                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: - initialize UART1: 8-bit, no parity, 2 stop-bit
*                   - set baudrate
*                   - no INT     
* FUNCTION_INPUTS : Uint16 =  "baud" using the macro UART_BAUD()                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void InitUART_1 (Uint16 baud)
    {
         UCSR1B = 0 ;  // disable UART_1

         UBRR1H = (Uchar) (baud>>8);
         UBRR1L = (Uchar) baud;

         UCSR1A = (1<<UDRE1);
         UCSR1C = (1<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10);

         UCSR1B = (1<<RXEN1) | (1<<TXEN1);  // then, (re)enable UART_1
    }



/*F****************************************************************************
* FUNCTION_NAME: GetCharUART_0                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: get byte from RX UART_0     
* FUNCTION_INPUTS : void                                                          
* FUNCTION_OUTPUTS: Uchar = character from RX UART_0 or 0x00 if nothing                                                      
******************************************************************************/
Uchar GetCharUART_0 (void)
    {
    Uchar  temp;
        
        if ( !(UCSR0A & (1<<RXC0)) ) temp = 0;
        else temp = UDR0;
        return temp;
    }


/*F****************************************************************************
* FUNCTION_NAME: GetCharUART_1                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: get character from RX UART_1     
* FUNCTION_INPUTS : void                                                          
* FUNCTION_OUTPUTS: Uchar = character from RX UART_1 or 0x00 if nothing                                                       
******************************************************************************/
Uchar GetCharUART_1 (void)
    {
    Uchar  temp;
        
        if ( !(UCSR1A & (1<<RXC1)) ) temp = 0;
        else temp = UDR1;
        return temp;
    }


/*F****************************************************************************
* FUNCTION_NAME: PutCharUART_0                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put character on TX UART_0     
* FUNCTION_INPUTS : Uchar = character to send on TX UART_0                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void PutCharUART_0 (Uchar char2send)
    {
        while ( !(UCSR0A & (1<<UDRE0)) ); // wait for empty transmit buffer 

        UDR0 = char2send;
    }



/*F****************************************************************************
* FUNCTION_NAME: PutCharUART_1                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put character on TX UART_1     
* FUNCTION_INPUTS : Uchar = character to send on TX UART_1                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void PutCharUART_1 (Uchar char2send)
    {
        while ( !(UCSR1A & (1<<UDRE1)) ); // wait for empty transmit buffer
        UDR1 = char2send;
    }



/*F****************************************************************************
* FUNCTION_NAME: CPrintfUART_0                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put a code-string on TX UART_0     
* FUNCTION_INPUTS : pointer on Uchar code-string                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void CPrintfUART_0 (const Uchar *cstring)
    {
        while(*cstring) PutCharUART_0 (*cstring++);
    }


/*F****************************************************************************
* FUNCTION_NAME: CPrintfUART_1                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put a code-string on TX UART_1     
* FUNCTION_INPUTS : pointer on Uchar code-string                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void CPrintfUART_1 (const Uchar *cstring)
    {
        while(*cstring) PutCharUART_1 (*cstring++);
    }



/*F****************************************************************************
* FUNCTION_NAME: DPrintfUART_0                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put a code-string on TX UART_0     
* FUNCTION_INPUTS : pointer on Uchar data-string                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void DPrintfUART_0 (Uchar *dstring)
    {
        while(*dstring) PutCharUART_0 (*dstring++);
    }


/*F****************************************************************************
* FUNCTION_NAME: DPrintfUART_1                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: J TELLIER                                              
* FUNCTION_DATE  : 13/06/903                                                 
*   AVR RELEASE  : date - name                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: put a code-string on TX UART_1     
* FUNCTION_INPUTS : pointer on Uchar data-string                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void DPrintfUART_1 (Uchar *dstring)
    {
        while(*dstring) PutCharUART_1 (*dstring++);
    }

