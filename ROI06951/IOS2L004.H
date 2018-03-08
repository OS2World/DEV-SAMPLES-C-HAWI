/* $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2L004.H 1.1 1995/05/25 18:33:52 HaWi Exp $ */

#define WCP_IOS2L "WC.IOS2L"

/* FÅr 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

MRESULT EXPENTRY IOS2L_WndProc (const HWND,
                                const MSG,
                                const MPARAM,
                                const MPARAM) ;
BOOL    EXPENTRY IOS2L_RegisterClass (const HAB) ;
