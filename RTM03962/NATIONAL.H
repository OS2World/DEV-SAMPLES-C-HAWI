/* $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/NATIONAL.H 1.1 1996/02/28 17:03:56 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _NATIONAL_H
   #define _NATIONAL_H

   /* Neue private Messages */
   #define WMP_MAININIT (WM_USER + 1100)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktions Prototypen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY CalendarWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Einige Ressource IDs */
   #define IDM_NATIONAL       1000
   #define IDM_DIALOG         1001
   #define IDM_DIALOGCALENDAR 1002

   #define IDD_DC             1003
   #define IDT_DC1            1004

   /* IDs fuer Texte */
   #define IDS_NATIONAL       1005

#endif /* _NATIONAL_H */

#ifdef __cplusplus
   }
#endif

