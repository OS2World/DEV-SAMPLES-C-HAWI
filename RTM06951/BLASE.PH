/* $Header: D:/projekte/blasen\040im\040os!2/source/RCS/BLASE.PH 1.1 1995/07/02 13:37:50 HaWi Exp $ */

#ifndef _BLASE_PH
   #define _BLASE_PH

   /* Neue private Messages */
   #define WMPUX             (WM_USER + 1030)
   #define WMPUX_BUBBLEOFF   (WMPUX + 1)
   #define WMPUX_BUBBLEON    (WMPUX + 2)
   #define WMPUX_BUBBLESTART (WMPUX + 3)

   /* FÅr 32-Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Struktur fÅr jede Instanz eines Fensters fÅr das Blasenhilfe angefordert wurde */
   typedef struct
      {
      BOOL    fActive ;       /* Hat Fenster den Fokus? */
      BOOL    fBubble ;       /* Bubble ist sichtbar */
      BOOL    fParent ;       /* FÅr Positionierung wichtig ob Frame oder Dialog Element */
      BOOL    fTimer ;        /* Timer aktiv */
      HAB     hab ;           /* HAB fÅr aktuellen Thread */
      HBITMAP hbmBackground ; /* Bitmap Handle des Blasenhintergrundes */
      HMODULE hmod ;          /* Modulhandle der Textressourcen */
      HWND    hwnd ;          /* Aktuelle Fensterinstanz */
      HWND    hwndPrevious ;  /* Letztes aktives Fenster */
      PFNWP   pfnwp ;         /* Original Fensterprozedur */
      POINTL  aptl [4] ;      /* Blasenposition */
      ULONG   ulTimer ;       /* Timer ID */
      } INSTANCE, *PINSTANCE ;

   /* Verkettete Liste aller Blasenhilfe Instanzen */
   typedef struct _SUBCLASS
      {
      struct _SUBCLASS *psubclassNext ;
      INSTANCE         instance ;
      } SUBCLASS, *PSUBCLASS ;

   /* Neue private Funktionsprototypen */
   MRESULT EXPENTRY UWndProc (HWND, MSG, MPARAM, MPARAM) ;

   /* IDs fÅr die beiden Bitmaps */
   #define IDB_AND 1
   #define IDB_XOR 2

#endif /* _BLASE_PH */

