/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF05.H 1.1 1996/02/07 12:16:55 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PMEINF05_H
   #define _PMEINF05_H

   /* Private Messages */
   #define WMP_MAININIT (WM_USER + 1000)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Hilfsstruktur zur Initialisierung von Containern */
   typedef struct
      {
      CHAR  szTitle [128] ;
      ULONG ulFlData ;
      ULONG ulFlTitle ;
      ULONG ulIdTitle ;
      ULONG ulOffStruct ;
      } CONTAINERINIT, *PCONTAINERINIT ;

   /* Record Struktur */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szColumn1 [65] ;
      CHAR           szColumn2 [65] ;
      PSZ            pszColumn1 ;
      PSZ            pszColumn2 ;
      } USRREC, *PUSRREC ;

   /* Instanzdaten des Clientfensters */
   typedef struct
      {
      HWND hwndMenu ;
      } WND, *PWND ;

   /* Instanzdaten des Clientfensters */
   typedef struct
      {
      CHAR  szHeader [128] ;
      HWND  hwndContainer ;
      HWND  hwndNotebook ;
      ULONG aulPageId [26] ;
      } DLG, *PDLG ;

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY DOeffnenWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #ifndef DID_HELP
      #define DID_HELP 3
   #endif /*DID_HELP */

   #define IDM_PMEINF05                     1000

   #define IDM_DIALOG                       1001
   #define IDM_DIALOGOEFFNEN                1002
   #define IDM_STANDARDDIALOGE              1003
   #define IDM_STANDARDDIALOGEDATEIOEFFNEN  1004
   #define IDM_STANDARDDIALOGEFARBAUSWAHL   1005

   #define IDD_DIALOGOEFFNEN                1008
   #define IDCO_DO1                         1007
   #define IDNO_DO1                         1008
   #define IDP_DO1                          1009
   #define IDSL_DO1                         1010
   #define IDSP_DO1                         1011
   #define IDT_DO1                          1012
   #define IDVS_DO1                         1013

   #define IDS_COLUMN1                      1014
   #define IDS_COLUMN2                      1015
   #define IDS_HEADER                       1016

#endif /* _PMEINF05_H */

#ifdef __cplusplus
   }
#endif




