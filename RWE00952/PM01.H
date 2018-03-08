/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM01.H 1.1 1995/05/25 15:32:06 HaWi Exp $ */

#ifndef _PM01_H
   #define _PM01_H

   #ifndef _PM01_HLP
      #ifndef _MSG_
         #define _MSG_
         typedef unsigned long MSG ;
      #endif

      MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
      INT              main (VOID) ;

      extern HAB  hab_g ;
      extern HWND hwndClient_g ;
      extern HWND hwndFrame_g ;
      extern PSZ  pszClass_g ;

   #endif /* _PM01_HLP */

   #define IDM_PM01 1000

#endif /* _PM01_H */
