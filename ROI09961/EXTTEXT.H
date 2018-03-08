/* $Header: D:/Projekte/Hin und Her/Source/rcs/EXTTEXT.H 1.1 1996/06/23 21:42:36 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _EXTTEXT_H
   #define _EXTTEXT_H

   #define WCP_MAIN    "WCP.MAIN"
   #define WCP_EXTTEXT "WCP.EXTTEXT"

   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   typedef struct
      {
      PFNWP pfnwp ;
      } WND, *PWND ;

   INT              main (VOID) ;
   MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   BOOL             RegisterClass (HAB) ;
   MRESULT EXPENTRY WndProc (HWND, MSG, MPARAM, MPARAM) ;

   #define IDD   11
   #define IDT_1 12
   #define IDT_2 13

#endif /* _EXTTEXT_H */

#ifdef __cplusplus
   }
#endif


