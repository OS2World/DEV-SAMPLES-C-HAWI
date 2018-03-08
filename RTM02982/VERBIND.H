/* $Header: D:\Projekte\Redaktion Toolbox\Verbindungen\Source\RCS\VERBIND.H 1.1 1997/11/26 21:39:35 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _VERBIND_H
   #define _VERBIND_H

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Private Messages */
   #define WMP_REFRESHRIGHT (WM_USER + 0)

   /* Styles */
   typedef enum
      {
      VSTYLE_FILTER,
      VSTYLE_TYPE
      } VSTYLE ;

   /* Instanzdaten des Dialoges */
   typedef struct
      {
      HWND   hwndCombobox ;
      HWND   hwndListbox1 ;
      HWND   hwndListbox2 ;
      VSTYLE vstyle ;
      } DLG, *PDLG ;

   /* Prozedurdeklarationen */
   INT              main (VOID) ;
   MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;

   /* Konstantdeklarationen */
   #define IDD_VERBIND 1000
   #define IDC_V1      1001
   #define IDE_V1      1002
   #define IDL_V1      1003
   #define IDL_V2      1004
   #define IDP_V1      1005
   #define IDP_V2      1006
   #define IDP_V3      1007
   #define IDP_V4      1008
   #define IDP_V5      1009
   #define IDR_V1      1010
   #define IDR_V2      1011
   #define IDR_V3      1012
   #define IDR_V4      1013

#endif /* _VERBIND_H */

#ifdef __cplusplus
   }
#endif

