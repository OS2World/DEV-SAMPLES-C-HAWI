/* $Header: D:\projekte\einfach sql\source\RCS\EINFSQL.H 1.1 1995/05/25 15:29:54 HaWi Exp $ */

#ifndef _EINFSQL_H
   #define _EINFSQL_H

   #define DBK_DATENBANK "ADRESSE"

   /* Messages (msg) */
   #define WMP          (WM_USER + 1000)
   #define WMP_ENABLE   (WMP + 0)
   #define WMP_MAININIT (WMP + 1)
   #define WMP_READ     (WMP + 2)
   #define WMP_REREAD   (WMP + 3)

   #define K_AENDERN  0
   #define K_ANFANG   1
   #define K_ENDE     2
   #define K_LOESCHEN 3
   #define K_NEU      4
   #define K_VOR      5
   #define K_ZURUECK  6

   /* Abbild der Tabelle */
   typedef struct /* adresse */
      {
      CHAR  szName [65] ;
      CHAR  szOrt [65] ;
      CHAR  szPlz [9] ;
      CHAR  szStrasse [65] ;
      CHAR  szTelefon [65] ;
      CHAR  szVorname [65] ;
      ULONG ulSchluessel ;
      } ADRESSE, *PADRESSE ;

   /* Struktur fÅr verkettete Liste */
   typedef struct _LIST /* list */
      {
      struct _LIST *plistNext ;
      struct _LIST *plistPrev ;
      ULONG        ulSchluessel ;
      } LIST, *PLIST ;

   /* Instanzdaten des Dialoges */
   typedef struct
      {
      PLIST plistCurrent ;
      PLIST plistRoot ;
      ULONG ulCount ;
      } DLG, *PDLG ;

   /* FÅr 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktionsdeklarationen */
   INT              main (VOID) ;
   MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;

   #define IDD            1000
   #define IDE_NAME       1001
   #define IDE_ORT        1002
   #define IDE_PLZ        1003
   #define IDE_STRASSE    1004
   #define IDE_TELEFON    1005
   #define IDE_VORNAME    1006
   #define IDP_AENDERN    1007
   #define IDP_ANFANG     1008
   #define IDP_ENDE       1009
   #define IDP_LOESCHEN   1010
   #define IDP_NEU        1011
   #define IDP_VOR        1012
   #define IDP_ZURUECK    1013
   #define IDT_SCHLUESSEL 1014

#endif /* _EINFSQL_H */
