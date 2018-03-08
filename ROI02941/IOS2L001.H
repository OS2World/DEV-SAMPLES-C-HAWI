/* $Header: D:\projekte\gib mir die daten\source\RCS\IOS2L001.H 1.1 1995/05/25 15:49:03 HaWi Exp $ */

/* Klassenname */
#define WCP_IOS2L              "WC.IOS2L"

/* Fehlercodes */
#define IOS2L_ERROR            0
#define IOS2L_NOERROR          1
#define IOS2L_NOTFOUND         2
#define IOS2L_END              3

/* Messages (msg) */
#define WMP_IOS2LOPENED        (WM_USER +  2)
#define WMP_IOS2LOPENERROR     (WM_USER +  3)
#define WMP_IOS2LDELETE        (WM_USER +  4)
#define WMP_IOS2LINSERT        (WM_USER +  5)
#define WMP_IOS2LREQUEST       (WM_USER +  6)
#define WMP_IOS2LUPDATE        (WM_USER +  7)
#define WMP_IOS2LSEND          (WM_USER +  8)

/* ZusÑtzliche Info zu Message (mp1) */
#define IOS2L_DELETEALL        1
#define IOS2L_DELETEPART       2
#define IOS2L_INSERTPART       3
#define IOS2L_REQUESTALL       4
#define IOS2L_REQUESTCOUNT     5
#define IOS2L_REQUESTPART      6
#define IOS2L_UPDATEALL        7
#define IOS2L_UPDATEPART       8

/* Tabellen (ulEbene) */
#define IOS2L_IGNORE           0
#define IOS2L_DATA             1

/* Informationsstruktur */
typedef struct
   {
   HWND                        hwndSender ;
   ULONG                       ulEbene ;
   ULONG                       ulRC ;
   ULONG                       ulEID ;
   ULONG                       ulSatzNr ;
   PVOID                       pData ;
   } IOS2LINFO, *PIOS2LINFO ;

/* Abbild der ersten Tabelle (IOS2L_DATA) */
typedef struct
   {
   ULONG                       ulKey ;
   CHAR                        szName [65] ;
   } IOS2LDATA, *PIOS2LDATA ;

/* FÅr 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsdeklarationen */
ULONG      EXPENTRY IOS2L_Register (const HWND) ;
VOID                IOS2L_Thread (VOID *) ;
MRESULT    EXPENTRY IOS2L_WndProc (const HWND,
                                   const MSG,
                                   const MPARAM,
                                   MPARAM) ;
PIOS2LINFO EXPENTRY IOS2L_AllocInfo (VOID) ;
PIOS2LINFO EXPENTRY IOS2L_AllocData (const ULONG) ;
ULONG      EXPENTRY IOS2L_FreeInfo (PIOS2LINFO) ;
ULONG      EXPENTRY IOS2L_FreeData (PIOS2LINFO) ;
ULONG      EXPENTRY IOS2L_OpenDatabase (VOID) ;
ULONG      EXPENTRY IOS2L_CloseDatabase (VOID) ;
ULONG      EXPENTRY IOS2L_CreateDB (const CHAR) ;
ULONG               IOS2L_TableIOS2L (const MSG,
                                      const MPARAM,
                                      MPARAM) ;
ULONG               IOS2L_CreateTable (VOID) ;
ULONG      EXPENTRY IOS2L_DropDB (VOID) ;

/* globale Variablen */
extern HWND          hwndIOS2P_g ;
