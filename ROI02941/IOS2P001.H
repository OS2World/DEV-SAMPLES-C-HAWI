/* $Header: D:\projekte\gib mir die daten\source\RCS\IOS2P001.H 1.1 1995/05/25 15:49:37 HaWi Exp $ */

#define WCP_IOS2P     "WC.IOS2P"

/* Neue Messages */
#define WMP_IOS2PDBOK      (WM_USER +  12)
#define WMP_IOS2PWAIT      (WM_USER +  13)
#define WMP_IOS2PDBCREATED (WM_USER +  14)

/* Resource IDs */
#define IDD_IOS2P     11
#define IDE_IOS2P_1   12
#define IDE_IOS2P_2   13
#define IDL_IOS2P_1   14
#define IDP_IOS2P_1   15
#define IDP_IOS2P_2   16
#define IDP_IOS2P_3   17
#define IDP_IOS2P_4   18
#define IDP_IOS2P_5   19
#define IDT_IOS2P_1   20

/* FÅr 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsdeklarationen */
INT              main (VOID) ;
MRESULT EXPENTRY IOS2P_DlgWndProc (const HWND,
                                   const MSG,
                                   const MPARAM,
                                   const MPARAM) ;
VOID             IOS2P_CreateDBThread (VOID *) ;

/* externe Variablen */
extern CHAR      chDrive_g ;
extern HAB       hab_g ;
extern HWND      hwndIOS2L_g ;
extern HWND      hwndIOS2P_g ;
extern ULONG     ulWait_g ;
