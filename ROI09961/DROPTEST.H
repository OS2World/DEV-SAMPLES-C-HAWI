/* $Header: D:/Projekte/Hin und Her/Source/rcs/DROPTEST.H 1.1 1996/06/23 21:42:35 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DROPTEST_H
#define _DROPTEST_H

   #ifndef _DROPTEST_HLP

      #define WMP_MAININIT (WM_USER + 1100)
      #define WMP_SIZE     (WM_USER + 1101)
      #define WMP_REREAD   (WM_USER + 1102)

      #ifndef _MSG_
         #define _MSG_
         typedef unsigned long MSG ;
      #endif

      typedef struct
         {
         HWND hwndMLE ;
         } WND, *PWND ;

      MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
      INT              main (VOID) ;

   #endif /* _DROPTEST_HLP */

   #define IDM_DROPTEST 1000
   #define IDF_DROP     1001

   #define IDS_CONTAINERB      1002
   #define IDS_DROPOBJECTSHERE 1003
   #define IDS_OBJECTB         1004
   #define IDS_PROGRAMM        1005
   #define IDS_RMFB            1006
   #define IDS_SOURCEB         1007
   #define IDS_STATUSBAR       1008
   #define IDS_TARGETB         1009
   #define IDS_TYPEB           1010

#endif /* _DROPTEST_H */

#ifdef __cplusplus
   }
#endif

