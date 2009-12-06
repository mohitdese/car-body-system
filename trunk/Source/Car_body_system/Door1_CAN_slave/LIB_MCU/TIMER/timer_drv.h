/*H**************************************************************************
* $RCSfile: timer_drv.h,v $         
*----------------------------------------------------------------------------
* Copyright (c) 2002 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: timer_drv.h,v 1.1 2003/01/14 09:07:57 ffosse Exp $       
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the timers 0, 1 and watchdog driver definition
*****************************************************************************/

#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/

#if T0_X2 == TRUE
  #define FT0_IN        FOSC/2
#else
  #define FT0_IN        FPER
#endif

#if T1_X2 == TRUE
  #define FT1_IN        FOSC/2
#else
  #define FT1_IN        FPER
#endif


#define   T0_MODE_0     0x00
#define   T0_MODE_1     0x01
#define   T0_MODE_2     0x02
#define   T0_MODE_3     0x03
#define   T0_COUNTER    0x04
#define   T0_TIMER      0x00
#define   T0_GATED      0x08
#define   T0_NOT_GATED  0x00

#define   T1_MODE_0     0x00
#define   T1_MODE_1     0x10
#define   T1_MODE_2     0x20
#define   T1_MODE_3     0x30
#define   T1_COUNTER    0x40
#define   T1_TIMER      0x00
#define   T1_GATED      0x80
#define   T1_NOT_GATED  0x00


/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ D E C L A R A T I O N ______________________________________________*/
/*
void    t0_set_prio (Byte);
void    t1_set_prio (Byte);
*/
#if T0_X2 == TRUE
  #define T0_init(g,c,m)          (CKCON |= MSK_T1X2); (TMOD &= 0xF0); (TMOD |= (g | c | m))
#else
  #define T0_init(g,c,m)          (CKCON &= ~MSK_T1X2); (TMOD &= 0xF0); (TMOD |= (g | c | m))
#endif
#define T0_start()                (TR0= 1)
#define T0_stop()                 (TR0= 0)
#define T0_enable_int()           (ET0= 1)
#define T0_disable_int()          (ET0= 0)
#define T0_set_low(l)             (TL0= l)
#define T0_set_high(h)            (TH0= h)

#if T1_X2 == TRUE
  #define T1_init(g,c,m)          (CKCON |= MSK_T1X2); (TMOD &= 0x0F); (TMOD |= (g | c | m))
#else
  #define T1_init(g,c,m)          (CKCON &= ~MSK_T1X2); (TMOD &= 0x0F); (TMOD |= (g | c | m))
#endif
#define T1_start()                (TR1= 1)
#define T1_stop()                 (TR1= 0)
#define T1_enable_int()           (ET1= 1)
#define T1_disable_int()          (ET1= 0)
#define T1_set_low(l)             (TL1= l)
#define T1_set_high(h)            (TH2= h)


#endif /* _TIMER_DRV_H_ */




