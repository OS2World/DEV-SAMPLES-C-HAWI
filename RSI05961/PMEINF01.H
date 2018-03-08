/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF01.H 1.1 1996/01/26 09:50:29 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PMEINF01_H
   #define _PMEINF01_H

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #define IDM_PMEINF01 1000

#endif /* _PMEINF01_H */

#ifdef __cplusplus
   }
#endif

