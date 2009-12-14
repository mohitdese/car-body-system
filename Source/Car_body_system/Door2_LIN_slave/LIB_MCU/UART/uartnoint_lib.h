/*H**************************************************************************
* $RCSfile: uartnoint_lib.h,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: uartnoint_lib.h,v 1.1 2003/07/04 13:17:57 jberthy Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This file contains all prototypes and macros exported for FROYA. 
* This template file can be parsed by langdoc for automatic documentation
* generation.                                         
*****************************************************************************/

#ifndef _UARTNOINT_LIB_H_
#define _UARTNOINT_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/

#define XTAL_CPU_1         1000000      //       1 Mhz
#define XTAL_CPU_2         2000000      //       2 Mhz
#define XTAL_CPU_4         4000000      //       4 Mhz
#define XTAL_CPU_8         8000000      //       8 Mhz
#define XTAL_CPU_10       10000000      //      10 Mhz
#define XTAL_CPU_12       12000000      //      12 Mhz
#define XTAL_CPU_16       16000000      //      16 Mhz

#define BAUD_RATE_2400        2400      //  2400  baud
#define BAUD_RATE_4800        4800      //  4800  baud
#define BAUD_RATE_9600        9600      //  9600  baud
#define BAUD_RATE_14K4       14400      //  14.4 Kbaud
#define BAUD_RATE_19K2       19200      //  19.2 Kbaud
#define BAUD_RATE_28K8       28800      //  28.8 Kbaud - Max for 1 Mhz !
#define BAUD_RATE_38K4       38400      //  38.4 Kbaud 
#define BAUD_RATE_57K6       57600      //  57.6 Kbaud - Max for 2 Mhz !
#define BAUD_RATE_76K8       76800      //  76.8 Kbaud 
#define BAUD_RATE_115K2     115200      // 115.2 Kbaud

/*_____ M A C R O S - DE C L A R A T I O N ___________________________________*/

typedef	unsigned short int Uint16;
typedef	unsigned char Uchar;


#define UART_BAUD(baudrate,xtalcpu) (Uint16)((xtalcpu/(((Uint32)(baudrate))*16))-1)

/*_____ P R O T O T Y P E S - D E C L A R A T I O N __________________________*/

extern void InitUART_0 (Uint16 baud);
extern void InitUART_1 (Uint16 baud);

extern Uchar GetCharUART_0 (void);
extern Uchar GetCharUART_1 (void);

extern void PutCharUART_0 (Uchar);
extern void PutCharUART_1 (Uchar);

extern void CPrintfUART_0 (const Uchar *cstring);
extern void CPrintfUART_1 (const Uchar *cstring);

extern void DPrintfUART_0 (Uchar *cstring);
extern void DPrintfUART_1 (Uchar *cstring);

#endif  /* UARTNOINT_LIB_H */
