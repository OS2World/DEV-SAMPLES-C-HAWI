/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF03.H 1.1 1996/01/26 09:51:09 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PMEINF03_H
   #define _PMEINF03_H

   /* Private Messages */
   #define WMP_MAININIT (WM_USER + 1000)

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Instanzdaten des Clientfensters */
   typedef struct
      {
      HWND hwndMenu ;
      } WND, *PWND ;

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY HInformationWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #define IDM_PMEINF03      1000

   #define IDM_HILFE         1001
   #define IDM_HINFORMATION  1002

   #define IDD_HINFORMATION  1003
   #define IDT_HINFORMATION1 1004
   #define IDT_HINFORMATION2 1005
   #define IDT_HINFORMATION3 1006
   #define IDT_HINFORMATION4 1007

#endif /* _PMEINF03_H */

#ifdef __cplusplus
   }
#endif




