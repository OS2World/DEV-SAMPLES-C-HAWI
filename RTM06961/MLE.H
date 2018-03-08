/* $Header: D:/Projekte/Tief im MLE!1/Source/rcs/MLE.H 1.3 1996/04/19 18:21:00 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _MLE_H
#define _MLE_H

   /* FÅr 64-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Einige private Messages */
   #define WMP_DESTROY  (WM_USER + 1000)
   #define WMP_MAININIT (WM_USER + 1001)
   #define WMP_REPLACE  (WM_USER + 1002)
   #define WMP_SEARCH   (WM_USER + 1003)
   #define WMP_SIZE     (WM_USER + 1004)
   #define WMP_STOP     (WM_USER + 1005)

   /* Einige Konstanten fÅr die T Helperfunktionen */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Kommunikationsstruktur zum Suchen/Ersetzen */
   typedef struct
      {
      BOOL  fAlle ;
      BOOL  fExakt ;
      BOOL  fVonOben ;
      CHAR  szReplace [CCHMAXPATH] ;
      CHAR  szSearch [CCHMAXPATH] ;
      ULONG ulDialog ;
      } MLECOMM, *PMLECOMM ;

   /* Struktur fuer Optionen die in einer INI abgelegt werden */
   typedef struct
      {
      BOOL   fWordWrap ;
      CHAR   szFile [CCHMAXPATH] ;
      FATTRS fat ;
      LONG   lBackground ;
      LONG   lForeground ;
      } OPTIONEN, *POPTIONEN ;

   /* Instanzdaten des BearbeitenErsetzen Dialoges */
   typedef struct
      {
      HWND hwndMLE ;
      } DLGBE, *PDLGBE ;

   /* Instanzdaten des OptionenFarbe Dialoges */
   typedef struct
      {
      HWND hwndValueset ;
      } DLGOF, *PDLGOF ;

   /* Instanzdaten des Clients */
   typedef struct
      {
      BOOL fChanged ;
      BOOL fStop ;
      CHAR szFile [CCHMAXPATH] ;
      HINI hini ;
      HWND hwndDialog ;
      HWND hwndMenu ;
      HWND hwndMLE ;
      HWND hwndTitlebar ;
      } WND, *PWND ;

   /* Funktionsdeklarationen */
   INT              main (INT, CHAR * []) ;
   MRESULT EXPENTRY BearbeitenErsetzenDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY BearbeitenSuchenDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY OptionenFarbeDlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   BOOL             TCheckForReadOnlyDrive (CHAR) ;
   BOOL             TDeleteMLEContents (HWND) ;
   HINI             TLocateIni (HAB, PSZ) ;
   BOOL             TReadFileInMLE (PSZ, HWND) ;
   BOOL             TWriteFileFromMLE (PSZ, HWND) ;
   USHORT           TWriteMessage (HWND, HMODULE, ULONG, ULONG, ULONG) ;

   /* Resource IDs */
   #define IDM                          1000
   #define IDM_BEARBEITEN               1001
   #define IDM_BEARBEITENAUSSCHNEIDEN   1002
   #define IDM_BEARBEITENEINFUEGEN      1003
   #define IDM_BEARBEITENERSETZEN       1004
   #define IDM_BEARBEITENKOPIEREN       1005
   #define IDM_BEARBEITENLOESCHEN       1006
   #define IDM_BEARBEITENRUECKGAENGIG   1007
   #define IDM_BEARBEITENSUCHEN         1008
   #define IDM_DATEI                    1009
   #define IDM_DATEINEU                 1010
   #define IDM_DATEIOEFFNEN             1011
   #define IDM_DATEISICHERN             1012
   #define IDM_DATEISICHERNALS          1013
   #define IDM_OPTIONEN                 1014
   #define IDM_OPTIONENHINTERGRUNDFARBE 1015
   #define IDM_OPTIONENSCHRIFTART       1016
   #define IDM_OPTIONENVORDERGRUNDFARBE 1017
   #define IDM_OPTIONENZEILENUMBRUCH    1018
   #define IDM_OPTIONENZEILENUMBRUCHAN  1019
   #define IDM_OPTIONENZEILENUMBRUCHAUS 1020
   #define IDF_MLE                      1021

   #define IDD_BEARBEITENERSETZEN       1022
   #define IDE_BE_1                     1023
   #define IDE_BE_2                     1024
   #define IDH_BE_1                     1025
   #define IDH_BE_2                     1026
   #define IDH_BE_3                     1027
   #define IDP_BE_1                     1028
   #define IDP_BE_2                     1029
   #define IDT_BE_1                     1030
   #define IDT_BE_2                     1031

   #define IDD_BEARBEITENSUCHEN         1032
   #define IDE_BS_1                     1033
   #define IDH_BS_1                     1034
   #define IDH_BS_2                     1035
   #define IDP_BS_1                     1036
   #define IDT_BS_1                     1037

   #define IDD_OPTIONENFARBE            1038
   #define IDVS_OF_1                    1039

   #define IDS_DIEDATEIWURDEVERAENDERT  1040
   #define IDS_HINTERGRUNDFARBE         1041
   #define IDS_MLE                      1042
   #define IDS_VORDERGRUNDFARBE         1043

#endif /* _MLE_H */

#ifdef __cplusplus
   }
#endif

