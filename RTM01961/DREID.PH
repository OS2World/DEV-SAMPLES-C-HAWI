/* $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREID.PH 1.2 1995/10/04 13:06:36 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DREID_PH
   #define _DREID_PH

   /* Private PM message */
   #define WMP_PAINT (WMP + 1)

   /* Einige Konstanten */
   #define DREID_AUTHOR      "Harald Wilhelm"
   #define DREID_DESCRIPTION "3D Static Field"
   #define DREID_MAJOR       1
   #define DREID_MINOR       0
   #define DREID_NAME        "DREID"

   /* FÅr 32Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Instanzdaten der Klasse */
   typedef struct
      {
      PFNWP pfnwp ;
      ULONG ulSide ;
      ULONG ulStyle ;
      } WNDDREID, *PWNDDREID ;

   /* Deklarationen der PMCX Funktionen */
   BOOL EXPENTRY    DREIDQuery (PUSERINFO) ;
   MRESULT EXPENTRY DREIDStyles (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY DREIDWndProc (HWND, MSG, MPARAM, MPARAM) ;

   /* Ressource IDs */
   #define IDD_DREID    1000
   #define IDC_ID       1001
   #define IDE_ID       1002
   #define IDF_TEXT     1003
   #define IDH_DISABLED 1004
   #define IDH_VISIBLE  1005
   #define IDP_BIDI     1006
   #define IDP_FONTS    1007
   #define IDR_BOTTOM   1008
   #define IDR_CENTER   1009
   #define IDR_IN       1010
   #define IDR_LEFT     1011
   #define IDR_OUT      1012
   #define IDR_RIGHT    1013
   #define IDR_TOP      1014
   #define IDT_TEXT     1015

#endif /* _DREID_PH */

#ifdef __cplusplus
   }
#endif


