/* $Header: D:\Projekte\Redaktion Toolbox\An der langen Leine\Source\RCS\NSDDE.H 1.0 1997/08/29 16:06:01 HAWI Exp $ */

/* Wenn C++ */
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _NSDDE_H
#define _NSDDE_H

   /* Private Events */
   #define WMP_SIZE   (WM_USER + 1100)
   #define WMP_REREAD (WM_USER + 1101)

   /* Fuer 32 Bit Presentation Manager */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* DDE Arten */
   #define K_POKE    1
   #define K_REQUEST 2

   /* Instanzdaten Client Fenster */
   typedef struct
      {
      ULONG ulType ;
      } WND, *PWND ;

   /* Instanzdaten ListWindows Dialog */
   typedef struct
      {
      CHAR szWindows [CCHMAXPATH] ;
      PSZ  pszWindows ;
      ULONG ulType ;
      } DLGLISTWINDOWS, *PDLGLISTWINDOWS ;

   /* Instanzdaten OpenURL Dialog */
   typedef struct
      {
      CHAR  szURL [CCHMAXPATH] ;
      ULONG ulFlags ;
      ULONG ulWindow ;
      } DLGOPENURL, *PDLGOPENURL ;

   /* Funktionsprototypen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY LISTWINDOWSWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   MRESULT EXPENTRY OPENURLWndProc (HWND, MSG, MPARAM, MPARAM) ;
   PDDESTRUCT       TMakeDDESeg (HWND, PSZ, ULONG, ULONG, PVOID, ULONG) ;
   USHORT           TWriteMessage (HWND, HMODULE, ULONG, ULONG, ULONG) ;

   /* Resource IDs */
   #define IDM_NSDDE            1000
   #define IDM_NSDDEEXIT        1001
   #define IDM_NSDDELISTWINDOWS 1002
   #define IDM_NSDDENETSCAPE    1003
   #define IDM_NSDDEOPENURL     1004
   #define IDM_NSDDEVERSION     1005

   #define IDD_LISTWINDOWS 1006
   #define IDL_LISTWINDOWS 1007
   #define IDT_LISTWINDOWS 1008

   #define IDD_OPENURL 1009
   #define IDE_OPENURL 1010
   #define IDT_OPENURL 1011

   /* Resource IDs der Strings */
   #define IDS_DDEACKARRIVED                 1012
   #define IDS_DDEDATAARRIVED                1013
   #define IDS_DDEINITIATEACKARRIVED         1014
   #define IDS_DDENOTIFICATION               1015
   #define IDS_DDESERVERBUSY                 1016
   #define IDS_UNSUPPORTEDDDETRANSACTIONDATA 1017

#endif /* _NSDDE_H */

#ifdef __cplusplus
   }
#endif

