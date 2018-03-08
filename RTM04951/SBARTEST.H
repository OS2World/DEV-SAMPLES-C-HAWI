/* $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBARTEST.H 1.1 1995/05/25 15:38:02 HaWi Exp $ */


#define WMP_MAININIT (WM_USER + 1100)

#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

typedef struct
   {
   BOOL fMinimized ;
   } WND, *PWND ;

MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
INT              main (VOID) ;

#define IDM_SBARTEST 1000
