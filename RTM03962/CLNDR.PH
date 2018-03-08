/* $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/CLNDR.PH 1.1 1996/02/28 17:03:56 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _CLNDR_PH
   #define _CLNDR_PH

   /* Neue Konstanten fuer 3D Rahmen */
   #ifndef DB_RAISED
      #define DB_RAISED 0x0400
   #endif

   #ifndef DB_DEPRESSED
      #define DB_DEPRESSED 0x0800
   #endif

   /* Private Control Messages */
   #define WMPAX_CONTROL (WMPAX + 3)
   #define WMPAX_PAINT   (WMPAX + 4)
   #define WMPAX_REDRAW  (WMPAX + 5)
   #define WMPAX_SETLOOK (WMPAX + 6)

   /* Klassenname der integrierten Static Klasse */
   #define WCP_CLNDRSTATIC "WCP.CLNDRSTATIC"

   /* Konstenten fuer die Listenverwaltung */
   #define TXK_HEADERPREV 0
   #define TXK_HEADERNEXT 4
   #define TXK_HEADERDATA 8
   #define TXK_PTR2DATAINCURR pb + TXK_HEADERDATA
   #define TXK_PTR2PREVINCURR *((PBYTE *) (pb + TXK_HEADERPREV))
   #define TXK_PTR2NEXTINCURR *((PBYTE *) (pb + TXK_HEADERNEXT))
   #define TXK_PTR2NEXTINPREV *((PBYTE *) ((*((PBYTE *) (pb + TXK_HEADERPREV))) + TXK_HEADERNEXT))
   #define TXK_PTR2PREVINNEXT *((PBYTE *) ((*((PBYTE *) (pb + TXK_HEADERNEXT))) + TXK_HEADERPREV))

   /* Root Struktur der Listenverwaltung */
   typedef struct _LISTROOT
      {
      PVOID pvFirst ;
      PVOID pvLast ;
      ULONG ulCount ;
      ULONG ulDataSize ;
      ULONG ulKeyOffset ;
      ULONG ulKeyStyle ;
      } LISTROOT, *PLISTROOT ;

   /* IDs der integrierten Kontrollelemente */
   #define FID_OUTPUT  1
   #define FID_HEADER1 2
   #define FID_HEADER2 3
   #define FID_HEADER3 4
   #define FID_HEADER4 5
   #define FID_HEADER5 6
   #define FID_HEADER6 7
   #define FID_HEADER7 8
   #define FID_DAY1    9
   #define FID_DAY2    10
   #define FID_DAY3    11
   #define FID_DAY4    12
   #define FID_DAY5    13
   #define FID_DAY6    14
   #define FID_DAY7    15
   #define FID_DAY8    16
   #define FID_DAY9    17
   #define FID_DAY10   18
   #define FID_DAY11   19
   #define FID_DAY12   20
   #define FID_DAY13   21
   #define FID_DAY14   22
   #define FID_DAY15   23
   #define FID_DAY16   24
   #define FID_DAY17   25
   #define FID_DAY18   26
   #define FID_DAY19   27
   #define FID_DAY20   28
   #define FID_DAY21   29
   #define FID_DAY22   30
   #define FID_DAY23   31
   #define FID_DAY24   32
   #define FID_DAY25   33
   #define FID_DAY26   34
   #define FID_DAY27   35
   #define FID_DAY28   36
   #define FID_DAY29   37
   #define FID_DAY30   38
   #define FID_DAY31   39
   #define FID_DAY32   40
   #define FID_DAY33   41
   #define FID_DAY34   42
   #define FID_DAY35   43
   #define FID_DAY36   44
   #define FID_DAY37   45
   #define FID_DAY38   46
   #define FID_DAY39   47
   #define FID_DAY40   48
   #define FID_DAY41   49
   #define FID_DAY42   50

   #define AK_LOOKIGNORE 0
   #define AK_LOOKIN     1
   #define AK_LOOKOUT    2

   /* Struktur fuer Instanzdaten der integrierten Static Klasse */
   typedef struct
      {
      PFNWP pfnwp ;
      ULONG ulLook ;
      } WNDSTATIC, *PWNDSTATIC ;

   /* Struktur fuer die Instanzdaten des Kalender Kontrollelementes */
   typedef struct
      {
      CHAR      aszDay [7] [16] ;
      CHAR      aszMonth [12] [16] ;
      HWND      ahwndDay [42] ;
      HWND      ahwndHeader [7] ;
      HWND      hwndOutput ;
      PFNWP     pfnwp ;
      PVOID     plistroot ;
      struct tm tm ;
      ULONG     ulDay ;
      ULONG     ulMonth ;
      ULONG     ulStyle ;
      ULONG     ulYear ;
      } WND, *PWND ;

   /* Fuer 32Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Private Funktionsdeklarationen */
   MRESULT EXPENTRY AWndProc (HWND, MSG, MPARAM, MPARAM) ;
   MRESULT EXPENTRY StaticWndProc (HWND, MSG, MPARAM, MPARAM) ;
   ULONG            TListCount (PVOID) ;
   BOOL             TListDelete (PVOID, ULONG, PVOID) ;
   BOOL             TListDestroy (PVOID) ;
   PVOID            TListInit (ULONG, ULONG, ULONG) ;
   BOOL             TListInsert (PVOID, ULONG, PVOID) ;
   BOOL             TListQuery (PVOID, ULONG, PVOID, PVOID) ;
   BOOL             TListUpdate (PVOID, ULONG, PVOID, PVOID) ;

#endif /* _CLNDR_PH */

#ifdef __cplusplus
   }
#endif

