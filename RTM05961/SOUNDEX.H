/* $Header: D:/Projekte/Kleine Helferlein!3/Source/rcs/SOUNDEX.H 1.1 1996/04/27 11:12:58 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _SOUNDEX_H
   #define _SOUNDEX_H

   /* Neue private Messages */
   #define WMP_MAININIT (WM_USER + 1100)

   /* Einige Konstanten */
   #define TERROR   FALSE
   #define TNOERROR TRUE

   /* Hilfsstruktur fuer TReplaceCharacter */
   typedef struct
      {
      CHAR chSearch ;
      PSZ  pszReplace ;
      } TREPLACE, *PTREPLACE ;

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Funktions Prototypen */
   MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY SoundexWndProc (HWND, MSG, MPARAM, MPARAM) ;
   INT              main (VOID) ;
   BOOL             TFillControls (HWND, ULONG, ULONG, ULONG) ;
   BOOL             TReplaceCharacter (PSZ, PSZ) ;
   BOOL             TSoundex (PSZ, PSZ) ;

   /* Several Resource IDs */
   #define IDM_SOUNDEX       1000
   #define IDM_DIALOG        1001
   #define IDM_DIALOGSOUNDEX 1002

   #define IDD_SOUNDEX       1003
   #define IDE_SX1           1004
   #define IDL_SX1           1005
   #define IDP_SX1           1006
   #define IDT_SX1           1007

   #define IDS_BTREFFER                       1008
   #define IDS_INITLISTBOXLOW                 1009 /* 12 */
   #define IDS_INITLISTBOXHIGH                1020
   #define IDS_KEINENPASSENDENEINTRAGGEFUNDEN 1021

#endif /* _SOUNDEX_H */

#ifdef __cplusplus
   }
#endif

