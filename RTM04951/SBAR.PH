/* $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBAR.PH 1.1 1995/05/25 15:37:43 HaWi Exp $ */

#ifndef _SBAR_PH
   #define _SBAR_PH

   #define WCP_SBARSTATIC "WCP.SBARSTATIC"

   #define SK_CLASSNAME "SBAR"

   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   typedef struct
      {
      PFNWP pfnwp ;
      ULONG ulLook ;
      ULONG ulStyle ;
      } WNDSTATIC, *PWNDSTATIC ;

   typedef struct _SUBCLASS
      {
      struct _SUBCLASS *psubclassNext ;
      HWND             hwndStatusbar ;
      HWND             hwndSubclass ;
      PFNWP            pfnwp ;
      SBARITEM         asbi [99] ;
      ULONG            ulCount ;
      } SUBCLASS, *PSUBCLASS ;

   MRESULT EXPENTRY StaticWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY StatusbarWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY SubclassWndProc (HWND, MSG, MPARAM, MPARAM) ;

#endif /* _SBAR_PH */
