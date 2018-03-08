/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF02.H 1.2 1996/01/26 11:16:14 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PMEINF02_H
   #define _PMEINF02_H

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
      LONG lColor ;
      } WND, *PWND ;

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #define IDM_PMEINF02             1000
   #define IDM_FARBEDESHINTERGRUNDS 1001
   #define IDM_FROSA                1002
   #define IDM_FHELLBLAU            1003
   #define IDM_FGELB                1004

#endif /* _PMEINF02_H */

#ifdef __cplusplus
   }
#endif




