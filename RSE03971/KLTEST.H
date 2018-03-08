/* $Header: Y:/Projekte/Redaktion SE/Knopfleiste/Source/rcs/KLTEST.H 1.1 1996/12/29 13:49:44 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _KLTEST_H
   #define _KLTEST_H

   #define WMP_CREATE (WM_USER + 1040)
   
   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Prozedurdeklarationen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;

   /* Konstantdeklarationen */
   #define IDM_KLTEST                           1000
   
   #define IDM_DATEI                            1001
   #define IDM_DATEIOEFFNEN                     1002
   #define IDM_DATEISCHLIESSEN                  1003
   #define IDM_BEARBEITEN                       1004
   #define IDM_BEARBEITENAUSSCHNEIDEN           1005
   #define IDM_BEARBEITENKOPIEREN               1006
   #define IDM_BEARBEITENEINFUEGEN              1007
   #define IDM_BEARBEITENLOESCHEN               1008
   #define IDM_SCHALTEN                         1009
   #define IDM_SCHALTENAUSSCHNEIDEN             1010
   #define IDM_SCHALTENAUSSCHNEIDENAKTIVIEREN   1011
   #define IDM_SCHALTENAUSSCHNEIDENANSCHALTEN   1012
   #define IDM_SCHALTENAUSSCHNEIDENAUSSCHALTEN  1013
   #define IDM_SCHALTENAUSSCHNEIDENDEAKTIVIEREN 1014
   #define IDM_SCHALTENEINFUEGEN                1015
   #define IDM_SCHALTENEINFUEGENAKTIVIEREN      1016
   #define IDM_SCHALTENEINFUEGENANSCHALTEN      1017
   #define IDM_SCHALTENEINFUEGENAUSSCHALTEN     1018
   #define IDM_SCHALTENEINFUEGENDEAKTIVIEREN    1019
   #define IDM_SCHALTENKOPIEREN                 1020
   #define IDM_SCHALTENKOPIERENAKTIVIEREN       1021
   #define IDM_SCHALTENKOPIERENANSCHALTEN       1022
   #define IDM_SCHALTENKOPIERENAUSSCHALTEN      1023
   #define IDM_SCHALTENKOPIERENDEAKTIVIEREN     1024
   #define IDM_SCHALTENLOESCHEN                 1025
   #define IDM_SCHALTENLOESCHENAKTIVIEREN       1026
   #define IDM_SCHALTENLOESCHENANSCHALTEN       1027
   #define IDM_SCHALTENLOESCHENAUSSCHALTEN      1028
   #define IDM_SCHALTENLOESCHENDEAKTIVIEREN     1029

   #define IDM_HILFE                            1030

#endif /* _KLTEST_H */

#ifdef __cplusplus
   }
#endif
