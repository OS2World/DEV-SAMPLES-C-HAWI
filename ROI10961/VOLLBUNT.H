/* $Header: D:/Projekte/Voll bunt/Source/rcs/VOLLBUNT.H 1.1 1996/07/31 19:25:18 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _VOLLBUNT_H
   #define _VOLLBUNT_H

   /* Private Message fuer verzîgerten Start */
   #define WMP_MAININIT (WM_USER + 1000)

   /* Fehler oder kein Fehler der Hilfsfunktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Fuer 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY VollBuntDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             CheckForReadOnlyDrive (CHAR) ;
   BOOL             DistributePresparamChange (HWND, MPARAM) ;
   HINI             LocateIni (HAB, PSZ) ;
   BOOL             RestoreDialog (HINI, HWND, PSZ) ;
   BOOL             RestoreWindow (HINI, HWND, PSZ) ;
   BOOL             SaveDialog (HINI, HWND, PSZ) ;
   BOOL             SaveWindow (HINI, HWND, PSZ) ;

   /* Einige Resource Konstanten */
   /* MenÅs */
   #define IDM_VOLLBUNT  100
   #define IDM_VOLLBUNT1 101
   #define IDM_VOLLBUNT2 102

   /* Dialoge */
   #define IDD_VOLLBUNT  1000
   #define IDP_VOLLBUNT1 1001
   #define IDP_VOLLBUNT2 1002
   #define IDT_VOLLBUNT  1003

#endif /* _VOLLBUNT_H */

#ifdef __cplusplus
   }
#endif

