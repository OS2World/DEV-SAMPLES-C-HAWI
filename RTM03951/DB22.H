/* $Header: D:\projekte\gib mir die daten.1\source\RCS\DB22.H 1.1 1995/05/25 15:53:00 HaWi Exp $ */

#ifndef _DB22_H
   #define _DB22_H

   /* Klassenname */
   #define WCPDB22 "WCPDB22"

   #define DBK_CREATEBIND "DB222.BND"
   #define DBK_DATENBANK  "ADRESSE"
   #define DBK_LOGFILE    "ERROR.LOG"
   #define DBK_BIND       "DB221.BND"

   /* Fehlercodes */
   #define DBC_ERROR    0
   #define DBC_NOERROR  1
   #define DBC_NOTFOUND 2
   #define DBC_END      3

   /* Messages (msg) */
   #define WMPDB             (WM_USER + 1000)
   #define WMPDB_CREATE      (WMPDB +  0)
   #define WMPDB_DELETE      (WMPDB +  1)
   #define WMPDB_INSERT      (WMPDB +  2)
   #define WMPDB_OPEN        (WMPDB +  3)
   #define WMPDB_REQUEST     (WMPDB +  4)
   #define WMPDB_SEND        (WMPDB +  5)
   #define WMPDB_UPDATE      (WMPDB +  6)

   /* ZusÑtzliche Info zu Message (mp1) */
   #define DBK_DELETEALL    1
   #define DBK_DELETEPART   2
   #define DBK_INSERTPART   3
   #define DBK_REQUESTALL   4
   #define DBK_REQUESTCOUNT 5
   #define DBK_REQUESTPART  6
   #define DBK_UPDATEALL    7
   #define DBK_UPDATEPART   8

   /* Tabellen (ulEbene) */
   #define DBK_IGNORE 0
   #define DBK_ADR    1
   #define DBK_ANS    2

   /* Abbild der ersten Tabelle (DBK_ADR) */
   typedef struct /* dbadr */
      {
      CHAR  szName [65] ;
      CHAR  szTelefon [65] ;
      CHAR  szVorname [65] ;
      ULONG ulKey ;
      } DBADR, *PDBADR ;

   /* Abbild zu einer mîglichen zweiten Tabelle (DBK_ANS) */
   typedef struct /* dbans */
      {
      CHAR  szName [65] ;
      CHAR  szTelefon [65] ;
      CHAR  szVorname [65] ;
      ULONG ulKey ;
      } DBANS, *PDBANS ;

   /* Informationsstruktur */
   typedef struct
      {
      HWND  hwnd ;
      ULONG ulEbene ;
      ULONG ulEID ;
      ULONG ulRC ;
      ULONG ulSatzNr ;
      union
         {
         DBADR dbadr ;
         DBANS dbans ;
         } ;
      } DBINFO, *PDBINFO ;

   /* FÅr 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktionsdeklarationen */
   PDBINFO EXPENTRY AllocInfo (VOID) ;
   ULONG   EXPENTRY CloseDatabase (VOID) ;
   ULONG   EXPENTRY CreateDatabase (CHAR) ;
   ULONG            CreateTables (VOID) ;
   ULONG   EXPENTRY DropDatabase (VOID) ;
   ULONG   EXPENTRY FreeInfo (PDBINFO) ;
   ULONG   EXPENTRY OpenDatabase (VOID) ;
   ULONG   EXPENTRY Register (HWND) ;
   ULONG            TableAdr (MSG, MPARAM, MPARAM) ;
   ULONG            TableAns (MSG, MPARAM, MPARAM) ;
   VOID             ThreadCreateDatabase (VOID*) ;
   VOID             ThreadWndProc (VOID*) ;
   MRESULT EXPENTRY WndProc (HWND, MSG, MPARAM, MPARAM) ;

#endif /* _DB22_H */
