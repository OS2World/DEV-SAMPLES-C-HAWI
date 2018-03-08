/* $Header: D:/Projekte/Viel System!1/Source/rcs/SYSLVL.H 1.2 1995/11/27 16:48:35 HaWi Exp $ */

/* Fuer C++ */
#ifdef __cplusplus
   extern "C" {
#endif

/* Anwendungsheader */
#ifndef _SYSLVL_H
#define _SYSLVL_H

   /* Einige private Messages */
   #define WMP_MAININIT       (WM_USER + 1100)
   #define WMP_SIZE           (WM_USER + 1101)
   #define WMP_REREAD         (WM_USER + 1102)
   #define WMP_THREADFINISHED (WM_USER + 1103)
   #define WMP_WAIT           (WM_USER + 1104)

   /* Fuer 32Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Fehlerkonstanten der T-Helper Funktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Initialisierungsstruktur des Containers */
   typedef struct
      {
      CHAR  szTitle [128] ; /* Ueberschrift */
      ULONG ulFlData ;      /* Spaltenflags */
      ULONG ulFlTitle ;     /* Ueberschriftflags */
      ULONG ulIdTitle ;     /* Ressource ID der Ueberschrift */
      ULONG ulOffStruct ;   /* Offset zum Datum */
      } CONTAINERINIT, *PCONTAINERINIT ;

   /* Einige Konstanten fuer die SYSLEVEL Dateien */
   #define SYSED_SE      0x00
   #define SYSED_EE      0x01
   #define SYSED_ES      0x02
   #define SYSED_GENERAL 0x0F
   #define SYSLEVEL      "SYSLEVEL"

   /* Struktur fuer die COUNTRY Informationen in der SYSLEVEL Datei */
   typedef struct
      {
      CHAR ch ;
      PSZ  psz ;
      } COUNTRY, *PCOUNTRY ;

   /* Struktur fuer die EDITION Informationen in der SYSLEVEL Datei */
   typedef struct
      {
      ULONG ul ;
      PSZ  psz ;
      } EDITION, *PEDITION ;

   /* Struktur fuer die RELEASE Informationen in der SYSLEVEL Datei */
   typedef struct
      {
      UINT minor : 4 ;
      UINT major : 4 ;
      } RELEASE, *PRELEASE ;

   /* Struktur fuer die SYSID Informationen in der SYSLEVEL Datei */
   typedef struct
      {
      PSZ pszSysId ;
      PSZ pszPart ;
      } SYSID, *PSYSID ;

   /* 1-Byte Packing anschalten */
   #pragma pack(1)

   /* Header der SYSLEVEL Dateien */
   typedef struct
      {
      USHORT usSignature ;
      CHAR   achSignature [8] ;
      CHAR   achJulian [5] ;
      USHORT usSlfVersion ;
      USHORT ausReserved [8] ;
      ULONG  ulTableOffset ;
      } SYSHEADER, *PSYSHEADER ;

   /* Inhalte SYSLEVEL Dateien */
   typedef struct
      {
      USHORT usSysId ;
      BYTE   bSysEdition ;
      BYTE   bSysVersion ;
      BYTE   bSysModify ;
      USHORT usSysDate ;
      CHAR   achCsdLevel [8] ;
      CHAR   achCsdPrev [8] ;
      CHAR   szSysName [80] ;
      CHAR   szCompId [9] ;
      BYTE   bRefreshLevel ;
      CHAR   szType [9] ;
      BYTE   abReserved [6] ;
      } SYSTABLE, *PSYSTABLE ;

   /* 1-Byte Packing wieder ausschalten */
   #pragma pack()

   /* Struktur zur Aufnahme einer kompletten SYSLEVEL Datei */
   typedef struct
      {
      CHAR      szLocation [CCHMAXPATH] ;
      FDATE     fdate ;
      FTIME     ftime ;
      ULONG     ulSize ;
      SYSHEADER sysheader ;
      SYSTABLE  systable ;
      } SYSFILE, *PSYSFILE ;

   /* Kommunikationsstruktur fuer den Thread. Uebergabe der Startdaten */
   typedef struct
      {
      USHORT    usLength ;
      HWND      hwnd ;
      PSYSFILE* ppsysfile ;
      PULONG    pulCount ;
      } THREAD, *PTHREAD ;

   /* Instanzdaten des Client Windows */
   typedef struct
      {
      CHAR     szTitel [128] ;
      HWND     hwndContainer ;
      PSYSFILE psysfile ;
      THREAD   thread ;
      ULONG    ulCount ;
      } WND, *PWND ;

   /* Recordstruktur fuer den Container */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CDATE          cdate ;
      CTIME          ctime ;
      PSZ            pszModul ;
      SYSFILE        sysfile ;
      ULONG          ulGroesse ;
      } USRREC, *PUSRREC ;

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             ScanDirectory (PSZ, PSYSFILE*, PULONG) ;
   MRESULT EXPENTRY SWndProc (HWND, MSG, MPARAM, MPARAM) ;
   VOID             Thread (VOID*) ;
   BOOL             TInitContainer (HWND, HMODULE, PCONTAINERINIT, ULONG) ;

   /* Ressource IDs */
   #define IDM_SYSLVL     1000
   #define IDM_SYSLEVEL   1001
   #define IDM_SVIEW      1002
   #define IDCO_SYSLEVELS 1003

   #define IDD_S                1004
   #define IDC_SSYSEDITION      1005
   #define IDC_SCOUNTRY         1006
   #define IDE_SDATE            1007
   #define IDE_SMAJOR           1008
   #define IDE_SMINOR           1009
   #define IDE_SMODIFY          1010
   #define IDE_SMODUL           1011
   #define IDE_SREFRESH         1012
   #define IDE_SSYSID           1013
   #define IDE_SSYSTEM          1014
   #define IDE_STYPE            1015
   #define IDE_SCOMPONENT       1016
   #define IDE_SCSDPREFIX       1017
   #define IDE_SCSDCURRENT      1018
   #define IDE_SCSDPREVIOUS     1019
   #define IDT_SDATUMDELIMITER1 1020
   #define IDT_SDATUMDELIMITER2 1021
   #define IDT_SDATUMDELIMITER3 1022
   #define IDT_SSYSID           1023
   #define IDT_S1               1024
   #define IDT_S2               1025
   #define IDT_S3               1026
   #define IDT_S4               1027
   #define IDT_S5               1028
   #define IDT_S6               1029
   #define IDT_S7               1030
   #define IDT_S8               1031
   #define IDT_S9               1032
   #define IDT_S10              1033
   #define IDT_S11              1034
   #define IDT_S12              1035

   #define IDS_CREATE           1036
   #define IDS_DATUM            1037
   #define IDS_GROESSE          1038
   #define IDS_MODUL            1039
   #define IDS_PROGRAMM         1040
   #define IDS_SYSLEVEL         1041
   #define IDS_TITEL            1042
   #define IDS_UHRZEIT          1043
   #define IDS_VIEW             1034
   #define IDS_WAIT             1045

#endif /* _SYSLVL_H */

#ifdef __cplusplus
   }
#endif

