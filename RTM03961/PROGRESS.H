/* $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/PROGRESS.H 1.1 1996/03/06 13:54:17 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PROGRESS_H
   #define _PROGRESS_H

   /* Neue private Messages */
   #define WMP_MAININIT     (WM_USER + 1100)
   #define WMP_PGNOCANCEL   (WM_USER + 1101)
   #define WMP_PGQUERYVALUE (WM_USER + 1102)
   #define WMP_PGSETTEXT    (WM_USER + 1103)
   #define WMP_PGSETVALUE   (WM_USER + 1104)
   #define WMP_PGSTOP       (WM_USER + 1105)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   #define K_TIMER 1

   /* Instanzdaten des Progressindiktor Dialoges */
   typedef struct
      {
      CTIME ctimeTemp ;
      } DLGPG, *PDLGPG ;

   /* Instanzdaten des Hauptfensters */
   typedef struct
      {
      HWND  hwndPGS ;
      ULONG ulCounter ;
      } WND, *PWND ;

   /* Funktions Prototypen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   VOID             TDateTimeToString (PSZ, PCDATE, PCTIME) ;
   VOID             TFillDateTime (PCDATE, PCTIME, PDATETIME) ;
   HWND EXPENTRY    TProgressCreate (HWND, HWND) ;
   MRESULT EXPENTRY TProgressProc (HWND, MSG, MPARAM, MPARAM) ;
   VOID             TTimeDiff (PCTIME, PCTIME) ;
   VOID             TTimeToString (PSZ, PCTIME) ;
   BOOL             TWindowCenter (HWND, HWND) ;

   /* Einige Resource IDs */
   #define IDM_PROGRESS       1000
   #define IDM_DIALOG         1001
   #define IDM_DIALOGPROGRESS 1002

   #define IDD_PG             1003
   #define IDSL_PG1           1004
   #define IDT_PG1            1005
   #define IDT_PG2            1006

   #define IDS_SIMPLERZAEHLER 1007
   #define IDS_WIRDGESTOPPT   1008

#endif /* _PROGRESS_H */

#ifdef __cplusplus
   }
#endif

