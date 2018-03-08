/* $Header: y:/projekte/redaktion os!2 inside/Gute Verbindung/Source/rcs/FTPEXMPL.H 1.2 1997/01/03 17:29:33 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _FTPEXMPL_H
   #define _FTPEXMPL_H

   #define WMP_CREATE        (WM_USER + 1040)
   #define WMP_SIZE          (WM_USER + 1041)
   #define WMP_WRITEFTPERROR (WM_USER + 1042)
   
   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   void _System ftpwindow (HWND) ;

   #define TXK_ERROR   FALSE
   #define TXK_NOERROR TRUE

   typedef enum
      {
      RECORDTYPEDIRECTORY,
      RECORDTYPEDRIVE,
      RECORDTYPEFILE
      } RECORDTYPE ;

   typedef enum
      {
      TRANSFERTYPEASCII,
      TRANSFERTYPEBINARY
      } TRANSFERTYPE ;

   typedef struct
      {
      CHAR  szTitle [128] ;
      ULONG ulFlData ;
      ULONG ulFlTitle ;
      ULONG ulIdTitle ;
      ULONG ulOffStruct ;
      } CONTAINERINIT, *PCONTAINERINIT ;

   typedef struct
      {
      MINIRECORDCORE minirec ;
      CDATE          cdateTemp ;
      CHAR           szFilename [CCHMAXPATH] ;
      CTIME          ctimeTemp ;
      RECORDTYPE     recordtype ;
      ULONG          ulSize ;
      } USRRECLOKAL, *PUSRRECLOKAL ;

   typedef struct
      {
      MINIRECORDCORE minirec ;
      CDATE          cdateTemp ;
      CHAR           szFilename [CCHMAXPATH] ;
      CTIME          ctimeTemp ;
      RECORDTYPE     recordtype ;
      ULONG          ulSize ;
      } USRRECENTFERNT, *PUSRRECENTFERNT ;

   typedef struct
      {
      BOOL         fClosePending ;
      CHAR         szAccount [CCHMAXPATH] ;
      CHAR         szEntfernt [CCHMAXPATH] ;
      CHAR         szHeaderEntfernt [CCHMAXPATH] ;
      CHAR         szHeaderLokal [CCHMAXPATH] ;
      CHAR         szKennung [CCHMAXPATH] ;
      CHAR         szLokal [CCHMAXPATH] ;
      CHAR         szLokation [CCHMAXPATH] ;
      CHAR         szPasswort [CCHMAXPATH] ;
      HWND         hwndEntfernt ;
      HWND         hwndLokal ;
      TRANSFERTYPE transfertype ;
      } WND, *PWND ;
   
   typedef struct
      {
      PWND pwnd ;
      } DLGENTFERNTERHOST, *PDLGENTFERNTERHOST ;
   
   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY EntfernterHostDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (INT, CHAR * []) ;
   BOOL             TCutString (PSZ, CHAR) ;
   VOID             ThreadEntfernt (VOID*) ;
   VOID             ThreadLoeschenEntfernt (VOID*) ;
   VOID             ThreadLoeschenLokal (VOID*) ;
   VOID             ThreadLogoff (VOID*) ;
   VOID             ThreadLokal (VOID*) ;
   VOID             ThreadTransferEntfernt (VOID*) ;
   VOID             ThreadTransferLokal (VOID*) ;
   BOOL             TInitContainer (HWND, HMODULE, PCONTAINERINIT, ULONG) ;
   USHORT           TWriteMessage (HWND, HMODULE, ULONG, ULONG, ULONG) ;
 
   /* Konstantdeklarationen */
   #define IDM_FTPEXMPL                         1000
   
   #define IDCO_ENTFERNT                        1001
   #define IDCO_LOKAL                           1002
   
   #define IDM_DATEIENTFERNTEDATEIENLOESCHEN    1003
   #define IDM_DATEIENTFERNTEDATEIENUEBERTRAGEN 1004
   #define IDM_DATEILOKALEDATEIENLOESCHEN       1005
   #define IDM_DATEILOKALEDATEIENUEBERTRAGEN    1006
   #define IDM_DATEI                            1007

   #define IDD_ENTFERNTERHOST                   1008
   #define IDE_ENTFERNTERHOST_1                 1009
   #define IDE_ENTFERNTERHOST_2                 1010
   #define IDE_ENTFERNTERHOST_3                 1011
   #define IDE_ENTFERNTERHOST_4                 1012
   #define IDR_ENTFERNTERHOST_1                 1013
   #define IDR_ENTFERNTERHOST_2                 1014
   
   #define IDS_DATE                             1015
   #define IDS_ERROR                            1016
   #define IDS_FILENAME                         1017
   #define IDS_FTPEXAMPLE                       1018
   #define IDS_FTPEXAMPLED                      1019
   #define IDS_SIZE                             1020
   #define IDS_TIME                             1021

   #define IDS_FTPERRORSTART                    1022
   #define IDS_FTPABORT                         (IDS_FTPERRORSTART + 0)
   #define IDS_FTPCOMMAND                       (IDS_FTPERRORSTART + 1)
   #define IDS_FTPCONNECT                       (IDS_FTPERRORSTART + 2)
   #define IDS_FTPDATACONN                      (IDS_FTPERRORSTART + 3)
   #define IDS_FTPDIRECTORYCHANGE               (IDS_FTPERRORSTART + 4)
   #define IDS_FTPDIRECTORYNEW                  (IDS_FTPERRORSTART + 5)
   #define IDS_FTPGROUPCHANGE                   (IDS_FTPERRORSTART + 6)
   #define IDS_FTPGROUPNEW                      (IDS_FTPERRORSTART + 7)
   #define IDS_FTPHOST                          (IDS_FTPERRORSTART + 8)
   #define IDS_FTPLOCALFILE                     (IDS_FTPERRORSTART + 9)
   #define IDS_FTPLOGIN                         (IDS_FTPERRORSTART + 10)
   #define IDS_FTPNOERROR                       (IDS_FTPERRORSTART + 11)
   #define IDS_FTPNOPRIMARY                     (IDS_FTPERRORSTART + 12)
   #define IDS_FTPNOXLATETBL                    (IDS_FTPERRORSTART + 13)
   #define IDS_FTPPROXYTHIRD                    (IDS_FTPERRORSTART + 14)
   #define IDS_FTPSERVICE                       (IDS_FTPERRORSTART + 15)
   #define IDS_FTPSOCKET                        (IDS_FTPERRORSTART + 16)
   #define IDS_FTPERROREND                      (IDS_FTPERRORSTART + 16)

#endif /* _FTPEXMPL_H */

#ifdef __cplusplus
   }
#endif

