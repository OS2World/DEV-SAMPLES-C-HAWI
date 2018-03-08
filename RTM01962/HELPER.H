/* $Header: D:/PROJEKTE/Kleine Helferlein/Source/rcs/HELPER.H 1.3 1995/11/03 13:25:20 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _HELPER_H
   #define _HELPER_H

   /* Private Message fuer verzîgerten Start */
   #define WMP_MAININIT (WM_USER + 1000)

   /* Fehler oder kein Fehler der T-Helper Funktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Konstanten fÅr TResizeControls */
   #define TPUSHBUTTONS 1
   #define TENTRYFIELDS 2

   /* Fuer 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY Dialog1WndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             TCheckForReadOnlyDrive (CHAR) ;
   BOOL             TCutDirectory (PSZ) ;
   BOOL             TDistributePresparamChange (HWND, MPARAM) ;
   HINI             TLocateIni (HAB, PSZ) ;
   BOOL             TQueryDirectory (HINI, PSZ, PSZ) ;
   BOOL             TResizeControls (HWND,HMODULE) ;
   BOOL             TRestoreDialog (HINI, HWND, PSZ) ;
   BOOL             TRestoreWindow (HINI, HWND, PSZ) ;
   BOOL             TSaveDialog (HINI, HWND, PSZ) ;
   BOOL             TSaveDirectory (HINI, PSZ, PSZ) ;
   BOOL             TSaveWindow (HINI, HWND, PSZ) ;
   BOOL             TWindowCenter (HWND, HWND) ;
   USHORT           TWriteMessage (HWND, HMODULE, ULONG, ULONG, ULONG) ;

   /* Einige Resource Konstanten */
   /* MenÅs */
   #define IDM_HELPER  100
   #define IDM_DIALOGE 101
   #define IDM_DIALOG1 102
   #define IDM_OEFFNEN 103

   /* Dialoge */
   #define IDD_DIALOG1  1000
   #define IDE_DIALOG11 1001
   #define IDP_DIALOG11 1002
   #define IDP_DIALOG12 1003
   #define IDP_DIALOG13 1004

   /* Anwendungstexte */
   #define IDS_BEENDEN                  10000
   #define IDS_WOLLENSIEWIRKLICHBEENDEN 10001

#endif /* _HELPER_H */

#ifdef __cplusplus
   }
#endif


