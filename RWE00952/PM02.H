/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM02.H 1.1 1995/05/25 15:32:58 HaWi Exp $ */

#ifndef _PM02_H
   #define _PM02_H

   #ifndef _PM02_HLP
      #define WMP_MAININIT (WM_USER + 1000)

      #ifndef _MSG_
         #define _MSG_
         typedef unsigned long MSG ;
      #endif

      typedef struct
         {
         HWND hwndMenu ;
         LONG lColor ;
         } WND, *PWND ;

      MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
      INT              main (VOID) ;

      extern HAB  hab_g ;
      extern HWND hwndClient_g ;
      extern HWND hwndFrame_g ;
      extern PSZ  pszClass_g ;

   #endif /* _PM02_HLP */

   #define IDM_PM02   1000
   #define IDM_FARBE  1001
   #define IDM_FBLAU  1002
   #define IDM_FGRUEN 1003
   #define IDM_FROT   1004

#endif /* _PM02_H */
