/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM03.H 1.1 1995/05/25 15:33:56 HaWi Exp $ */

#ifndef _PM03_H
   #define _PM03_H

   #ifndef _PM03_HLP
      #define WMP_MAININIT (WM_USER + 1000)

      #ifndef _MSG_
         #define _MSG_
         typedef unsigned long MSG ;
      #endif

      typedef struct
         {
         HWND hwndMenu ;
         } WND, *PWND ;

      MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
      MRESULT EXPENTRY HAboutWndProc (HWND, MSG, MPARAM, MPARAM) ;
      INT              main (VOID) ;

      extern HAB  hab_g ;
      extern HWND hwndClient_g ;
      extern HWND hwndFrame_g ;
      extern PSZ  pszClass_g ;

   #endif /* _PM03_HLP */

   #define IDM_PM03            1000
   
   #define IDM_HILFE           1001
   #define IDM_HABOUT          1002

   #define IDD_HABOUT          1003
   #define IDT_HABOUT1         1004
   #define IDT_HABOUT2         1005
   #define IDT_HABOUT3         1006
   #define IDT_HABOUT4         1007

#endif /* _PM03_H */
