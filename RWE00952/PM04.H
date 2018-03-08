/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM04.H 1.1 1995/05/25 15:34:51 HaWi Exp $ */

#ifndef _PM04_H
   #define _PM04_H

   #ifndef _PM04_HLP
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
      MRESULT EXPENTRY DOeffnenWndProc (HWND, MSG, MPARAM, MPARAM) ;
      INT              main (VOID) ;

      extern HAB  hab_g ;
      extern HWND hwndClient_g ;
      extern HWND hwndFrame_g ;
      extern PSZ  pszClass_g ;

   #endif /* _PM04_HLP */

   #ifndef DID_HELP
      #define DID_HELP 3
   #endif /*DID_HELP */

   #define IDM_PM04            1000
   
   #define IDM_DIALOG          1001
   #define IDM_DOEFFNEN        1002

   #define IDD_DOEFFNEN        1003
   #define IDE_DO1             1004
   #define IDG_DO1             1005
   #define IDG_DO2             1006
   #define IDG_DO3             1007
   #define IDH_DO1             1008
   #define IDH_DO2             1009
   #define IDH_DO3             1010
   #define IDL_DO1             1011
   #define IDR_DO1             1012
   #define IDR_DO2             1013
   #define IDR_DO3             1014
   #define IDT_DO1             1015
   #define IDT_DO2             1016

#endif /* _PM04_H */
