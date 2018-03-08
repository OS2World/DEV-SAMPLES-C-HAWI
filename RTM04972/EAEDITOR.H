/* $Header: D:/Projekte/Attraktive Attribute/Source/rcs/EAEDITOR.H 1.1 1996/06/01 10:41:28 HaWi Exp $ */

/* Fuer C++ */
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _EAEDITOR_H
#define _EAEDITOR_H

   /* Einige private Messages */
   #define WMP_ENABLE   (WM_USER + 1100)
   #define WMP_MAININIT (WM_USER + 1101)
   #define WMP_REREAD   (WM_USER + 1102)
   #define WMP_SIZE     (WM_USER + 1103)
   #define WMP_WAIT     (WM_USER + 1104)

   /* Fuer 64-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Konstanten fuer T-Helper Funktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Hilfsstruktur zum Einfuegen von Container Spalten */
   typedef struct
      {
      CHAR  szTitle [128] ;
      ULONG ulFlData ;
      ULONG ulFlTitle ;
      ULONG ulIdTitle ;
      ULONG ulOffStruct ;
      } CONTAINERINIT, *PCONTAINERINIT ;

   /* Einige EA Strukturen */
   typedef struct
      {
      PBYTE pbValue ;
      ULONG ulEAType ;
      ULONG ulValue ;
      } EATDATA, *PEATDATA ;

   #pragma pack (1)

      typedef struct
         {
         USHORT usEAType ;
         USHORT usValue ;
         BYTE   bValue [1] ;
         } EATSV, *PEATSV ;

      typedef struct
         {
         USHORT usValue ;
         BYTE   bValue [1] ;
         } MVST, *PMVST ;

      typedef struct
         {
         USHORT usEAType ;
         USHORT usValue ;
         BYTE   bValue [1] ;
         } MVMT, *PMVMT ;

      typedef struct
         {
         USHORT usEAType ;
         USHORT usCodepage ;
         USHORT usEA ;
         MVMT   mvmt [1] ;
         } EATMV, *PEATMV ;

   #pragma pack ()

   /* Weitere Konstanten */
   #define K_TIMERBUBBLE 1
   #define K_TIMERCLOCK  2

   #define K_REREADDIR  1
   #define K_REREADTREE 2

   /* Recordbeschreibung Dateicontainer */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CDATE          cdateTemp ;
      CHAR           szFilename [CCHMAXPATH] ;
      CTIME          ctimeTemp ;
      PSZ            pszFilename ;
      ULONG          ulEASize ;
      ULONG          ulSize ;
      } USRRECDIR, *PUSRRECDIR ;

   /* Typ eines Verzeichniscontainer Records */
   typedef enum
      {
      K_RECORDROOT,
      K_RECORDAREA,
      K_RECORDITEM
      } RECORDTYPE ;

   /* Recordbeschreibung Verzeichniscontainer */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szDir [CCHMAXPATH] ;
      CHAR           szFilename [CCHMAXPATH] ;
      RECORDTYPE     recordtype ;
      } USRRECTREE, *PUSRRECTREE ;

   /* Instanzdaten Dialog EA */
   typedef struct
      {
      CHAR szFilename [CCHMAXPATH] ;
      HWND hwndListbox ;
      } DLGEA, *PDLGEA ;

   /* Instanzdaten Dialog EA neu */
   typedef struct
      {
      CHAR szFilename [CCHMAXPATH] ;
      } DLGEANEW, *PDLGEANEW ;

   /* Instanzdaten Client */
   typedef struct
      {
      CHAR szHeaderDir [CCHMAXPATH] ;
      HWND hwndContainerDir ;
      HWND hwndContainerTree ;
      HWND hwndStaticDir ;
      HWND hwndStaticTree ;
      HWND hwndMenu ;
      } WND, *PWND ;

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY EADlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY EANewDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             ScanTree (PSZ, PWND, PUSRRECTREE) ;
   BOOL             TDeleteDir2 (PSZ, BOOL, BOOL) ;
   PEAOP2           TEACreateEAOP2Read (ULONG, PGEA2LIST) ;
   PEAOP2           TEACreateEAOP2Write (PFEA2LIST) ;
   PFEA2LIST        TEACreateFEA2List (PSZ, PBYTE, ULONG) ;
   PGEA2LIST        TEACreateGEA2List (PSZ) ;
   BOOL             TEARead (PSZ, PSZ, PEATDATA) ;
   BOOL             TEAReadASCII (PSZ, PSZ, PSZ, PULONG) ;
   BOOL             TEAReadMV (PSZ, PSZ, ULONG, EATDATA []) ;
   BOOL             TEAWrite (PSZ, PSZ, PEATDATA) ;
   BOOL             TEAWriteASCII (PSZ, PSZ, PSZ) ;
   BOOL             TEAWriteMV (PSZ, PSZ, ULONG, EATDATA []) ;
   VOID             ThreadDir (VOID*) ;
   VOID             ThreadTree (VOID*) ;
   BOOL             TInitContainer (HWND, HMODULE, PCONTAINERINIT, ULONG) ;

   /* Ressource IDs */
   #ifndef DID_OK
      #define DID_OK 1
   #endif /* DID_OK */

   #ifndef DID_CANCEL
      #define DID_CANCEL 2
   #endif /* DID_CANCEL */

   #define IDM_DIR          1000
   #define IDM_DIRDELETE    1001
   #define IDM_DIREAEDIT    1002
   #define IDM_EAEDITOR     1003
   #define IDM_EDIT         1004
   #define IDM_EDITCOLLAPSE 1005
   #define IDM_EDITEXPAND   1006
   #define IDM_TREE         1007
   #define IDM_TREEDELETE   1008
   #define IDM_TREEOPEN     1009
   #define IDCO_DIR         1010
   #define IDCO_TREE        1011
   #define IDT_DIR          1012
   #define IDT_TREE         1013

   #define IDD_EA           1014
   #define IDE_EA_1         1015
   #define IDL_EA_1         1016
   #define IDP_EA_1         1017
   #define IDP_EA_2         1018
   #define IDP_EA_3         1019
   #define IDT_EA_1         1020
   #define IDT_EA_2         1021
   #define IDT_EA_3         1022
   #define IDT_EA_4         1023
   #define IDT_EA_5         1024
   #define IDT_EA_6         1025

   #define IDD_EANEW        1026
   #define IDE_EANEW_1      1027
   #define IDG_EANEW_1      1028
   #define IDR_EANEW_1      1029
   #define IDR_EANEW_2      1030
   #define IDR_EANEW_3      1031
   #define IDT_EANEW_1      1032
   #define IDT_EANEW_2      1033
   #define IDT_EANEW_3      1034

   #define IDS_ASCII                1035
   #define IDS_BINARY               1036
   #define IDS_BITMAP               1037
   #define IDS_DATE                 1038
   #define IDS_EA                   1039
   #define IDS_EASIZE               1040
   #define IDS_FILENAME             1041
   #define IDS_ICON                 1042
   #define IDS_LOCALMAP             1043
   #define IDS_METAFILE             1044
   #define IDS_MULTIVALUEMULTITYPE  1045
   #define IDS_MULTIVALUESINGLETYPE 1046
   #define IDS_PROGRAMM             1047
   #define IDS_SIZE                 1048
   #define IDS_TIME                 1049
   #define IDS_UNKNOWN              1050

#endif /* _EAEDITOR_H */

#ifdef __cplusplus
   }
#endif

