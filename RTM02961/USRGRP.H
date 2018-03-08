/* $Header: D:/PROJEKTE/Kleine Helferlein!1/Source/rcs/USRGRP.H 1.1 1995/12/01 11:18:28 HaWi Exp $ */

/* Fuer C++ */
#ifdef __cplusplus
   extern "C" {
#endif

/* Anwendungsheader */
#ifndef _USRGRP_H
#define _USRGRP_H

   /* Einige private Messages */
   #define WMP_MAININIT       (WM_USER + 1100)
   #define WMP_SIZE           (WM_USER + 1101)

   /* Fuer 32Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Fehlerkonstanten der T-Helper Funktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Fehlerkonstanten der T-Helper Funktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Instanzdaten des Client Windows */
   typedef struct
      {
      HWND hwndListbox1 ;
      HWND hwndListbox2 ;
      } WND, *PWND ;

   /* Funktionsdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             TCutString (PSZ, CHAR) ;

   /* Ressource IDs */
   #define IDM_USRGRP 1000
   #define IDM_SCAN   1001
   #define IDM_SUSER  1002
   #define IDM_SGROUP 1003
   #define IDL_GROUP  1004
   #define IDL_USER   1005

   #define IDS_PROGRAMM 1006

#endif /* _USRGRP_H */

#ifdef __cplusplus
   }
#endif


