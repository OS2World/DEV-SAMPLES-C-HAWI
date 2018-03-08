/* $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/DRIVES.H 1.1 1996/03/05 18:04:09 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DRIVES_H
   #define _DRIVES_H

   /* Neue private Messages */
   #define WMP_MAININIT (WM_USER + 1100)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktions Prototypen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY DDWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Several Resource IDs */
   #define IDM_DRIVES       1000
   #define IDM_DIALOG       1001
   #define IDM_DIALOGDRIVES 1002

   #define IDD_DD           1003
   #define IDL_DD1          1004
   #define IDL_DD2          1005

#endif /* _DRIVES_H */

#ifdef __cplusplus
   }
#endif

