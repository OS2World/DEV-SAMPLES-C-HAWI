/* $Header: D:\projekte\gib mir die daten.1\source\RCS\DB22XMPL.H 1.1 1995/05/25 15:53:46 HaWi Exp $ */

#ifndef _DB22XMPL_H
   #define _DB22XMPL_H

   /* Klassenname */
   #define WCPDB22XMPL "WCPDB22XMPL"
   
   /* Messages (msg) */
   #define WMP          (WM_USER + 1100)
   #define WMP_MAININIT (WMP     + 0)
   #define WMP_REREAD   (WMP     + 1)
   #define WMP_SIZE     (WMP     + 2)
   #define WMP_WAIT     (WMP     + 3)

   /* FÅr 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Strukturdeklaration fÅr alle Container Objekte */
   typedef struct
      {
      CHAR  szTitle [128] ;
      ULONG ulFlData ;
      ULONG ulFlTitle ;
      ULONG ulIdTitle ;
      ULONG ulOffStruct ;
      } CONTNRINIT, *PCONTNRINIT ;

   /* Strukturdeklaration fÅr Instanzdaten des "SÑtze Ñndern" Dialoges */
   typedef struct
      {
      ULONG ulKey ;
      } DLGSA, *PDLGSA ;

   /* Strukturdeklaration der Instanzdaten des Client Windows */
   typedef struct
      {
      BOOL     fMinimized ;
      CHAR     szTitel [128] ;
      HPOINTER hptrArrow ;
      HPOINTER hptrWait ;
      HWND     hwndContainer ;
      HWND     hwndMenu ;
      ULONG    ulThread ;
      } WND, *PWND ;

   /* Funktionsdeklaration der Container Records */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szName [65] ;
      CHAR           szTelefon [65] ;
      CHAR           szVorname [65] ;
      PSZ            pszName ;
      PSZ            pszTelefon ;
      PSZ            pszVorname ;
      ULONG          ulKey ;
      } USRREC, *PUSRREC ;

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY SADlgProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY SEDlgProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Resource IDs */
   #define IDM_DB22XMPL        1000
   #define IDM_DATENBANK       1001
   #define IDM_DANLEGEN        1002
   #define IDM_DLOESCHEN       1003
   #define IDM_SAETZE          1004
   #define IDM_SEINFUEGEN      1005
   #define IDM_SAENDERN        1006
   #define IDM_SLOESCHEN       1007

   #define IDCO_CONTAINER      1008

   #define IDD_SEINFUEGEN      1009
   #define IDE_SENAME          1010
   #define IDE_SETELEFON       1011
   #define IDE_SEVORNAME       1012

   #define IDD_SAENDERN        1013
   #define IDE_SANAME          1014
   #define IDE_SATELEFON       1015
   #define IDE_SAVORNAME       1016

   #define IDS_CREATEERROR     1017
   #define IDS_CREATEERRORT    1018
   #define IDS_DELETEERROR     1019
   #define IDS_DELETEERRORT    1020
   #define IDS_INSERTERROR     1021
   #define IDS_INSERTERRORT    1022
   #define IDS_NAME            1023
   #define IDS_READERROR       1024
   #define IDS_READERRORT      1025
   #define IDS_TELEFON         1026
   #define IDS_TITEL           1027
   #define IDS_UPDATEERROR     1028
   #define IDS_UPDATEERRORT    1029
   #define IDS_VORNAME         1030

#endif /* _DB22XMPL_H */
