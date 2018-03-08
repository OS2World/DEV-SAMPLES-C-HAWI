/* $Header: y:/projekte/redaktion os!2 inside/Ganz geheim/Source/rcs/SECRET.H 1.1 1997/03/07 16:00:02 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _SECRET_H
   #define _SECRET_H

   #define WMPTX          (WM_USER + 1080)
   #define WMPTX_MAININIT (WMPTX +  0)
   
   #define TXK_ERROR   FALSE
   #define TXK_NOERROR TRUE
   
   #define TXK_TIMER 90

   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   typedef struct
      {
      HAB    hab ;
      USHORT us ;
      } DLGSW, *PDLGSW ;

   typedef struct
      {
      CHAR szTitel [128] ;
      HWND hwndMenu ;
      } WND, *PWND ;

   MRESULT EXPENTRY AWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY HRWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             TCheckForReadOnlyDrive (CHAR) ;
   HINI             TLocateIni (HAB, PSZ) ;
   BOOL             TRegister (HINI, PSZ, PSZ, PULONG) ;
   HWND             TStickyWindowCreate (HWND, HWND) ;
   MRESULT EXPENTRY TStickyWindowProc (HWND, MSG, MPARAM, MPARAM) ;
   BOOL             TWindowCenter (HWND, HWND) ;
   
   #define IDM_SECRET    1000
   #define IDM_HELP      1001
   #define IDM_HABOUT    1002
   #define IDM_HREGISTER 1003
   
   #define IDD_HABOUT  1004
   #define IDT_HABOUT1 1005
   #define IDT_HABOUT2 1006
   #define IDT_HABOUT3 1007
   #define IDT_HABOUT4 1008
   #define IDT_HABOUT5 1009

   #define IDD_HR  1010
   #define IDE_HR1 1011
   #define IDE_HR2 1012
   #define IDT_HR1 1013
   #define IDT_HR2 1014

   #define IDD_SW   1015
   #define IDT_SW_1 1016
   #define IDT_SW_2 1017

   #define IDS_PROGRAMM      1018
   #define IDS_REGISTRIERT   1019
   #define IDS_UNREGISTRIER  1020
   #define IDS_UNREGISTRIERT 1021
   #define IDS_WICHTIG       1022

#endif /* _SECRET_H */

#ifdef __cplusplus
   }
#endif

