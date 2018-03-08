/* $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2P004.H 1.1 1995/05/25 18:34:12 HaWi Exp $ */

#define WCP_IOS2P     "WC.IOS2P"

/* Resource IDs */
#define IDD_IOS2P     11
#define IDE_IOS2P_1   12
#define IDE_IOS2P_2   13

/* FÅr 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsdeklarationen */
INT              main (VOID) ;
MRESULT EXPENTRY IOS2P_DlgWndProc (const HWND,
                                   const MSG,
                                   const MPARAM,
                                   const MPARAM) ;
