/* $Header: D:/Projekte/Exploring WebExplorer/Source/rcs/WEBEX.H 1.2 1996/04/15 13:19:25 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _WEBEX_H
#define _WEBEX_H

   /* Einige private Messages */
   #define WMP_ENABLE   (WM_USER + 1100)
   #define WMP_MAININIT (WM_USER + 1101)
   #define WMP_REREAD   (WM_USER + 1102)
   #define WMP_SIZE     (WM_USER + 1103)

   /* Fuer 64 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Einige Konstanten fÅr die T Helperfunktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* EA Strukturen */
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

   typedef enum
      {
      K_RECORDROOT,
      K_RECORDAREA,
      K_RECORDITEM
      } RECORDTYPE ;

   /* Container Recordbeschreibung */
   typedef struct
      {
      MINIRECORDCORE minirec ;
      CHAR           szName [CCHMAXPATH] ;
      CHAR           szURL [CCHMAXPATH] ;
      RECORDTYPE     recordtype ;
      ULONG          ulKey ;
      ULONG          ulKeyParent ;
      } USRREC, *PUSRREC ;

   /* Instanzdaten des Clients */
   typedef struct
      {
      BOOL        fViewActive ;
      HMODULE     hmodWEB ;
      HWND        hwndContainer ;
      HWND        hwndMenu ;
      HWND        hwndWEB ;
      WXPRINTINFO wxprintinfo ;
      } WND, *PWND ;

   /* Instanzdaten des Area Einfuegen Dialoges */
   typedef struct
      {
      PWND pwnd ;
      } DLGEG, *PDLGEG ;

   /* Instanzdaten des Cache Optionen Dialoges */
   typedef struct
      {
      PWND        pwnd ;
      WXCACHEOPTS wxcacheopts ;
      } DLGOC, *PDLGOC ;

   /* Instanzdaten des Display Optionen Dialoges */
   typedef struct
      {
      PWND          pwnd ;
      WXDISPLAYOPTS wxdisplayopts ;
      } DLGOD, *PDLGOD ;

   /* Instanzdaten des Farbauswahl Dialoges */
   typedef struct
      {
      HWND hwndValueset ;
      } DLGOF, *PDLGOF ;

   /* Prozeduren Deklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY EinfuegenAreaDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   MRESULT EXPENTRY OptionenCacheDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY OptionenDisplayDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY OptionenFarbeDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   PUSRREC          SearchRecord (HWND, PUSRREC, RECORDTYPE, ULONG) ;
   PEAOP2           TEACreateEAOP2Read (ULONG, PGEA2LIST) ;
   PGEA2LIST        TEACreateGEA2List (PSZ) ;
   BOOL             TEAReadMV (PSZ, PSZ, ULONG, EATDATA []) ;
   BOOL             TQueryDefaultQueueName (PSZ) ;
   ULONG            TQueryJobProperties (HAB, PVOID, PSZ, PSZ, PSZ, BOOL) ;
   BOOL             TQueryQueueDriver (PSZ, PSZ) ;
   USHORT           TWriteMessage (HWND, HMODULE, ULONG, ULONG, ULONG) ;
   VOID             WriteRecord (HWND, PUSRREC, FILE*, FILE*, PULONG, PULONG) ;

   /* Ressource IDs */
   #define IDM_WEBEX                1000
   #define IDM_AREA                 1001
   #define IDM_AREAEINFUEGEN        1002
   #define IDM_AREALOESCHEN         1003
   #define IDM_DOKUMENT             1004
   #define IDM_DOKUMENTDRUCKEN      1005
   #define IDM_DOKUMENTINDATEILADEN 1006
   #define IDM_DOKUMENTLADEN        1007
   #define IDM_DOKUMENTLADENSTOPPEN 1008
   #define IDM_EDITIEREN            1009
   #define IDM_EDITIERENCOLLAPSE    1010
   #define IDM_EDITIERENEXPAND      1011
   #define IDM_ITEM                 1012
   #define IDM_ITEMINDATEILADEN     1013
   #define IDM_ITEMLADEN            1014
   #define IDM_ITEMLOESCHEN         1015
   #define IDM_OPTIONEN             1016
   #define IDM_OPTIONENCACHE        1017
   #define IDM_OPTIONENDISPLAY      1018
   #define IDM_ROOT                 1019
   #define IDM_ROOTEINFUEGEN        1020
   #define IDCO_WEBSEITEN           1021
   #define IDWEB_SEITE              1022

   #define IDD_EINFUEGENAREA        1023
   #define IDE_EG_1                 1024
   #define IDT_EG_1                 1025

   #define IDD_OPTIONENCACHE        1026
   #define IDE_OC_1                 1027
   #define IDE_OC_2                 1028
   #define IDE_OC_3                 1029
   #define IDH_OC_1                 1030
   #define IDH_OC_2                 1031
   #define IDT_OC_1                 1032
   #define IDT_OC_2                 1033
   #define IDT_OC_3                 1034

   #define IDD_OPTIONENDISPLAY      1035
   #define IDE_OD_1                 1036
   #define IDP_OD_1                 1037
   #define IDP_OD_2                 1038
   #define IDP_OD_3                 1039
   #define IDP_OD_4                 1040
   #define IDP_OD_5                 1041
   #define IDT_OD_1                 1042
   #define IDT_OD_2                 1043
   #define IDT_OD_3                 1044
   #define IDT_OD_4                 1045
   #define IDT_OD_5                 1046
   #define IDT_OD_6                 1047
   #define IDT_OD_7                 1048
   #define IDT_OD_8                 1049
   #define IDT_OD_9                 1050

   #define IDD_OPTIONENFARBE        1051
   #define IDVS_OF_1                1052

   #define IDS_PROGRAMM                                1053
   #define IDS_SICHERHEITSABFRAGE                      1054
   #define IDS_WOLLENSIEDIESEWEBGRUPPEWIRKLICHLOESCHEN 1055
   #define IDS_WOLLENSIEDIESEWEBSEITEWIRKLICHLOESCHEN  1056

#endif /* _WEBEX_H */

#ifdef __cplusplus
   }
#endif

