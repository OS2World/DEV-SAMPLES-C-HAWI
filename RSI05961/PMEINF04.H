/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF04.H 1.2 1996/01/26 11:47:55 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PMEINF04_H
   #define _PMEINF04_H

   /* Private Messages */
   #define WMP_MAININIT (WM_USER + 1000)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Instanzdaten des Clientfensters */
   typedef struct
      {
      HWND hwndMenu ;
      } WND, *PWND ;

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY DOeffnenWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #ifndef DID_HELP
      #define DID_HELP 3
   #endif /*DID_HELP */

   #define IDM_PMEINF04  1000

   #define IDM_DIALOG    1001
   #define IDM_DOEFFNEN  1002

   #define IDD_DOEFFNEN  1003
   #define IDC_DO1       1004
   #define IDE_DO1       1005
   #define IDG_DO1       1006
   #define IDG_DO2       1007
   #define IDG_DO3       1008
   #define IDG_DO4       1009
   #define IDH_DROSA     1010
   #define IDH_DHELLBLAU 1011
   #define IDH_DGELB     1012
   #define IDL_DO1       1013
   #define IDR_DROSA     1014
   #define IDR_DHELLBLAU 1015
   #define IDR_DGELB     1016
   #define IDT_DO1       1017
   #define IDT_DO2       1018
   #define IDT_DO3       1019

#endif /* _PMEINF04_H */

#ifdef __cplusplus
   }
#endif




