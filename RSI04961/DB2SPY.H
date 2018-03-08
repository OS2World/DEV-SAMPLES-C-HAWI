/* $Header: D:/PROJEKTE/Die Programmierung des DB2!2/Source/rcs/DB2SPY.H 1.2 1995/12/11 15:43:01 HaWi Exp $ */

/* FÅr C++ */
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DB2SPY_H
#define _DB2SPY_H

   /* Private Messages */
   #define WMP_MAININIT  (WM_USER + 1100)
   #define WMP_RESETMENU (WM_USER + 1101)
   #define WMP_SIZE      (WM_USER + 1102)

   /* FÅr 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Einige Funktionskonstanten */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Initialisierungsstruktur fÅr Container */
   typedef struct
      {
      CHAR  szTitle [128] ;
      ULONG ulFlData ;
      ULONG ulFlTitle ;
      ULONG ulIdTitle ;
      ULONG ulOffStruct ;
      } CONTAINERINIT, *PCONTAINERINIT ;

   /* Record Struktur fÅr Containerlayout ConnectionServices */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szComment [SQL_CMT_SZ + 1] ;
      CHAR           szHost [SQL_LONG_NAME_SZ + 1] ;
      CHAR           szName [SQL_DBNAME_SZ + 1] ;
      PSZ            pszComment ;
      PSZ            pszHost ;
      PSZ            pszName ;
      } USRRECCONNECTIONSERVICES, *PUSRRECCONNECTIONSERVICES ;

   /* Record Struktur fÅr Containerlayout Databases */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szAlias [SQL_ALIAS_SZ + 1] ;
      CHAR           szAuthentication [40] ;
      CHAR           szComment [SQL_CMT_SZ + 1] ;
      CHAR           szDBType [SQL_DBTYP_SZ + 1] ;
      CHAR           szDirectory [SQL_INAME_SZ + 1] ;
      CHAR           szDrive [SQL_DRIVE_SZ + 1] ;
      CHAR           szName [SQL_DBNAME_SZ + 1] ;
      CHAR           szNode [SQL_NNAME_SZ + 1] ;
      CHAR           szType [40] ;
      PSZ            pszAlias ;
      PSZ            pszAuthentication ;
      PSZ            pszComment ;
      PSZ            pszDBType ;
      PSZ            pszDirectory ;
      PSZ            pszDrive ;
      PSZ            pszName ;
      PSZ            pszNode ;
      PSZ            pszType ;
      } USRRECDATABASES, *PUSRRECDATABASES ;

   /* Record Struktur fÅr Containerlayout Packages */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szCreator [9] ;
      CHAR           szDBName [SQL_DBNAME_SZ + 1] ;
      CHAR           szName [9] ;
      PSZ            pszCreator ;
      PSZ            pszDBName ;
      PSZ            pszName ;
      } USRRECPACKAGES, *PUSRRECPACKAGES ;

   /* Record Struktur fÅr Containerlayout Tables */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szCreator [9] ;
      CHAR           szDBName [SQL_DBNAME_SZ + 1] ;
      CHAR           szName [19] ;
      PSZ            pszCreator ;
      PSZ            pszDBName ;
      PSZ            pszName ;
      } USRRECTABLES, *PUSRRECTABLES ;

   /* Record Struktur fÅr Containerlayout TableSpaces */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szDBName [SQL_DBNAME_SZ + 1] ;
      CHAR           szName [19] ;
      PSZ            pszDBName ;
      PSZ            pszName ;
      } USRRECTABLESPACES, *PUSRRECTABLESPACES ;

   /* Record Struktur fÅr Containerlayout Views */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szCreator [9] ;
      CHAR           szDBName [SQL_DBNAME_SZ + 1] ;
      CHAR           szName [19] ;
      PSZ            pszCreator ;
      PSZ            pszDBName ;
      PSZ            pszName ;
      } USRRECVIEWS, *PUSRRECVIEWS ;

   /* Struktur fÅr Instanzdaten des Client Windows */
   typedef struct
      {
      CHAR  szTitle [128] ;
      HWND  hwndContainer ;
      HWND  hwndMenu ;
      ULONG ulAction ;
      } WND, *PWND ;

   /* inige Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             TCutString (PSZ, CHAR) ;
   VOID             Thread (PVOID) ;
   BOOL             TInitContainer (HWND, HMODULE, PCONTAINERINIT, ULONG) ;

   /* Ressource IDs */
   #define IDM_DB2SPY              1000
   #define IDM_SCAN                1001
   #define IDM_SCONNECTIONSERVICES 1002
   #define IDM_SDATABASES          1003
   #define IDM_SNODES              1004
   #define IDM_SPACKAGES           1005
   #define IDM_STABLES             1006
   #define IDM_STABLESPACES        1007
   #define IDM_SVIEWS              1008
   #define IDCO_DB2                1009

   #define IDS_ALIAS                       1010
   #define IDS_AUTHENTICATION              1011
   #define IDS_COMMENT                     1012
   #define IDS_CONNECTIONSERVICES          1013
   #define IDS_CREATOR                     1014
   #define IDS_DATABASES                   1015
   #define IDS_DBNAME                      1016
   #define IDS_DBTYPE                      1017
   #define IDS_DIRECTORY                   1018
   #define IDS_DRIVE                       1019
   #define IDS_HOST                        1020
   #define IDS_LOCALLU                     1021
   #define IDS_MODE                        1022
   #define IDS_NAME                        1023
   #define IDS_NODE                        1024
   #define IDS_NODES                       1025
   #define IDS_PACKAGES                    1026
   #define IDS_PARTNERLU                   1027
   #define IDS_SQLEDINFOAUTHENTICATIONLOW  1028 /* 4 */
   #define IDS_SQLEDINFOAUTHENTICATIONHIGH 1032
   #define IDS_SQLEDINFOTYPELOW            1033 /* 4 */
   #define IDS_SQLEDINFOTYPEHIGH           1037
   #define IDS_TABLES                      1038
   #define IDS_TABLESPACES                 1039
   #define IDS_TYPE                        1040
   #define IDS_VIEWS                       1041

#endif /* _DB2SPY_H */

#ifdef __cplusplus
   }
#endif


