/* $Header: D:/projekte/blasen\040im\040os!2/source/RCS/BLASE.C 1.1 1995/07/03 12:59:05 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BLASE.H"
#include "BLASE.PH"

/* Einige globale Variablen */
HBITMAP   hbmAnd_g = NULLHANDLE ;
HBITMAP   hbmXor_g = NULLHANDLE ;
HDC       hdc_g = NULLHANDLE ;
HMODULE   hmodSelf_g = NULLHANDLE ;
HPS       hps_g = NULLHANDLE ;
PSUBCLASS psubclass_g = NULL ;
ULONG     ulCount_g = 0 ;
ULONG     ulCX_g = 0  ;
ULONG     ulCY_g = 0 ;

/* Funktion zum Deregistrieren der Bubble Help eines Fensters */
BOOL EXPENTRY UDeregister (HWND hwnd)
   {
   PSUBCLASS psubclass ;
   PSUBCLASS psubclassPrev ;
   ULONG     ulI ;

   /* Sind Åberhaupt Instanzen der BubbleHelp aktiv? */
   if   (ulCount_g)
        {
        /* Ja, in einer Schleife durch alle registrierten Instanzen suchen */
        for  (ulI = 0,
               psubclassPrev = NULL,
               psubclass = psubclass_g;
              ulI < ulCount_g;
              ulI++,
               psubclassPrev = psubclass,
               psubclass = psubclass->psubclassNext)
             {
             /* An Hand des Fensterhandles nach der gewÅnschten Instanz suchen */
             if   (psubclass->instance.hwnd == hwnd)
                  {
                  /* Wenn gefunden, Bitmap des Desktop Hintergrundes lîschen */
                  GpiDeleteBitmap (psubclass->instance.hbmBackground) ;
                  /* Eventuell aktive BubbleHelp vom Bildschirm nehmen */
                  WinSendMsg (psubclass->instance.hwnd,
                              WMPUX_BUBBLEOFF,
                              MPVOID,
                              MPVOID) ;
                  /* Den Subclass der Originalfensterprozedur wieder zurÅcksetzen */
                  WinSubclassWindow (psubclass->instance.hwnd,
                                     psubclass->instance.pfnwp) ;
                  /* Gibt es eine Instanz vor dieser gefundenen? */
                  if   (psubclassPrev)
                       {
                       /* Wenn ja, die nÑchste Instanz mit der vorherigen verketten */
                       psubclassPrev->psubclassNext = psubclass->psubclassNext ;
                       }
                  else
                       {
                       /* Wenn nein, dann zeigt der Rootpointer ab jetzt auf das nÑchste Element */
                       psubclass_g = psubclass->psubclassNext ;
                       }
                  /* Element der aktuellen Instanz lîschen */
                  free (psubclass) ;
                  /* ZÑhler der Elemente/Instanzen vermindern */
                  ulCount_g-- ;
                  /* Wurde vielleicht die letzte Instanz entfernt? */
                  if   (!ulCount_g)
                       {
                       /* Ja, dann wird der Handle der eigenen DLL nicht mehr benîtigt */
                       DosFreeModule (hmodSelf_g) ;
                       /* AND Bitmap lîschen */
                       GpiDeleteBitmap (hbmAnd_g) ;
                       /* XOR Bitmap lîschen */
                       GpiDeleteBitmap (hbmXor_g) ;
                       /* Presentation Space lîschen */
                       GpiDestroyPS (hps_g) ;
                       /* Device Context lîschen */
                       DevCloseDC (hdc_g) ;
                       }
                  /* Aus der Schleife durch alle Instanzen springen */
                  break ;
                  }
             }
        }
   return TRUE ;
   }

/* Funktion zum Registrieren der Bubble Help eines Fensters */
BOOL EXPENTRY URegister (HWND    hwnd,
                         HMODULE hmod,
                         BOOL    fParent,
                         ULONG   ul)
   {
   BITMAPINFOHEADER2 bmp2 ;
   HAB               hab ;
   PSUBCLASS         psubclass ;
   PSUBCLASS         psubclassPrev ;
   SIZEL             sizl ;
   ULONG             ulI ;

   /* Aktuellen Anchor Block ermitteln */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Ist es die erste Registrierung? */
   if   (!ulCount_g)
        {
        /* Ja, einen Memory Device Context îffnen */
        hdc_g = DevOpenDC (hab,
                           OD_MEMORY,
                           "*",
                           0,
                           NULL,
                           NULLHANDLE) ;
        /* Grî·e irrelevant */
        sizl.cx = 0 ;
        sizl.cy = 0 ;
        /* Einen Presentation Space îffnen */
        hps_g = GpiCreatePS (hab,
                             hdc_g,
                             &sizl,
                             PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC) ;
        /* Den Modul Handle dieser DLL ermitteln */
        DosQueryModuleHandle ("BLASE",
                              &hmodSelf_g) ;
        /* AND Bitmap aus der DLL laden */
        hbmAnd_g = GpiLoadBitmap (hps_g,
                                  hmodSelf_g,
                                  IDB_AND,
                                  0,
                                  0) ;
        /* XOR Bitmap aus der DLL laden */
        hbmXor_g = GpiLoadBitmap (hps_g,
                                  hmodSelf_g,
                                  IDB_XOR,
                                  0,
                                  0) ;
        /* Struktur lîschen */
        memset (&bmp2,
                0,
                sizeof (BITMAPINFOHEADER2)) ;
        bmp2.cbFix = sizeof (BITMAPINFOHEADER2) ;
        /* Von der AND Bitmap den Header lesen */
        GpiQueryBitmapInfoHeader (hbmAnd_g,
                                  &bmp2) ;
        /* Die Grî·en der Bitmap merken */
        ulCX_g = bmp2.cx ;
        ulCY_g = bmp2.cy ;
        /* Einen Speicherberich fÅr dieses Element/Instanz anfordern */
        psubclass_g = calloc (1,
                              sizeof (SUBCLASS)) ;
        /* Beim ersten ist es zugleich das Rootelement */
        psubclass = psubclass_g ;
        }
   else
        {
        /* Nein, es existiert bereits eine Instanz */
        /* In einer Schleife bis ans Ende laufen */
        for  (ulI = 0,
               psubclass = psubclass_g;
              ulI < ulCount_g;
              ulI++,
               psubclassPrev = psubclass,
               psubclass = psubclass->psubclassNext) ;
        /* FÅr dieses Element/Instalz einen Speicherplatz besorgen */
        psubclassPrev->psubclassNext = calloc (1,
                                               sizeof (SUBCLASS)) ;
        /* Diesen am Ende der Instanzen anhÑngen */
        psubclass = psubclassPrev->psubclassNext ;
        }
   /* ZÑhler der Instanzen und Elemente erhîhen */
   ulCount_g++ ;
   /* Speicher freimachen */
   memset (&bmp2,
           0,
           sizeof (BITMAPINFOHEADER2)) ;
   bmp2.cbFix = sizeof (BITMAPINFOHEADER2) ;
   /* X Grî·e des Desktops lesen */
   bmp2.cx = WinQuerySysValue (HWND_DESKTOP,
                               SV_CXSCREEN) ;
   /* Y Grî·e des Desktops lesen */
   bmp2.cy = WinQuerySysValue (HWND_DESKTOP,
                               SV_CYSCREEN) ;
   /* Eine Ebene */
   bmp2.cPlanes = 1 ;
   /* Eine Bitmap anlegen mit der Grî·e des Desktops */
   psubclass->instance.hbmBackground = GpiCreateBitmap (hps_g,
                                                        &bmp2,
                                                        0,
                                                        NULL,
                                                        NULL) ;
   /* Einige Parameter speichern, ist es ein Dialog oder ein Frame */
   psubclass->instance.fParent = fParent ;
   /* Anchor Block */
   psubclass->instance.hab = hab ;
   /* Ressource Handle der Anwendung */
   psubclass->instance.hmod = hmod ;
   /* Dialog/Frame Handle der Anwendung */
   psubclass->instance.hwnd = hwnd ;
   /* Zu verwendende Timer ID */
   psubclass->instance.ulTimer = ul ;
   /* Dialog/Frame subclassen und Original Pointer auf Fensterprozedur speichern */
   psubclass->instance.pfnwp = WinSubclassWindow (psubclass->instance.hwnd,
                                                  (PFNWP) UWndProc) ;
   return TRUE ;
   }

/* Diese Fensterprozedur wird jetzt die Dialog/Frame Prozedur erweitern */
MRESULT EXPENTRY UWndProc (HWND   hwnd,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   PSUBCLASS psubclass ;

   {
   ULONG ulI ;

   /* In einer Schleife durch alle Instanzen/Elemente suchen */
   for  (ulI = 0,
          psubclass = psubclass_g;
         ulI < ulCount_g;
         ulI++,
          psubclass = psubclass->psubclassNext)
        {
        /* Der Windows Handle ist der Key. Wenn gefunden, dann ist es die richtige Instanz */
        if   (psubclass->instance.hwnd == hwnd)
             {
             break ;
             }
        }
   }
   /* Message Verarbeitung */
   switch (msg)
      {
      /* Das Frame/Dialog wird aktiviert/deaktiviert */
      case WM_ACTIVATE:
           /* Evtl. sichtbare BubbleHelp auf jeden Fall ausschalten */
           WinSendMsg (psubclass->instance.hwnd,
                       WMPUX_BUBBLEOFF,
                       MPVOID,
                       MPVOID) ;
           /* Aktiv/Deaktiv Status merken */
           psubclass->instance.fActive = SHORT1FROMMP (mp1) ;
           break ;
      /* Eine Benutzeraktion wurde aktiviert */
      case WM_COMMAND:
           /* Evtl. sichtbare BubbleHelp auf jeden Fall ausschalten */
           WinSendMsg (psubclass->instance.hwnd,
                       WMPUX_BUBBLEOFF,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Die Maus wurde bewegt */
      case WM_MOUSEMOVE:
           {
           HWND   hwndTemp ;
           POINTL aptl [4] ;
           RECTL  rcl ;
           SWP    swp ;

           /* Position der Maus ermitteln */
           WinQueryPointerPos (HWND_DESKTOP,
                               aptl) ;
           /* Fensterhandle unter dieser Position abfragen */
           hwndTemp = WinWindowFromPoint (HWND_DESKTOP,
                                          aptl,
                                          TRUE) ;
           /* Ist es ein neues Fenster? */
           if   (hwndTemp != psubclass->instance.hwndPrevious)
                {
                /* Ja, dann evtl. sichtbare BubbleHelp auf jeden Fall ausschalten */
                WinSendMsg (psubclass->instance.hwnd,
                            WMPUX_BUBBLEOFF,
                            MPVOID,
                            MPVOID) ;
                }
           /* Grî·e des Dialoges/Frames ermitteln */
           WinQueryWindowRect (psubclass->instance.hwnd,
                               &rcl) ;
           /* Ist es Dialog oder Frame? */
           if   (psubclass->instance.fParent)
                {
                /* Frame, Position des Fensters abfragen */
                WinQueryWindowPos (psubclass->instance.hwnd,
                                   &swp) ;
                /* Frame Start zur Grî·e addieren */
                rcl.xLeft = swp.x ;
                rcl.yBottom = swp.y ;
                rcl.xRight += swp.x ;
                rcl.yTop += swp.y ;
                }
           /* Ist Mauszeiger Åber einem neuen aktiven Fenster? */
           if   ((WinPtInRect (psubclass->instance.hab,
                               &rcl,
                               aptl)) &&
                 (psubclass->instance.fActive))
                {
                /* Ja, Timer fÅr BubbleHelp anwerfen lassen */
                WinSendMsg (psubclass->instance.hwnd,
                            WMPUX_BUBBLESTART,
                            MPVOID,
                            MPVOID) ;
                }
           break ;
           }
      /* Timer ist abgelaufen */
      case WM_TIMER:
           {
           POINTL aptl [4] ;
           RECTL  rcl ;
           SWP    swp ;

           /* Ist es auch der BubbleHelp Timer? */
           if   (SHORT1FROMMP (mp1) == psubclass->instance.ulTimer)
                {
                /* Ja, Position der Maus ermitteln */
                WinQueryPointerPos (HWND_DESKTOP,
                                    aptl) ;
                /* Ist BublleHelp noch nicht aktiv? */
                if   (!psubclass->instance.fBubble)
                     {
                     /* Ja, BubbleHelp fÅr ein Fenster aktivieren lassen */
                     WinSendMsg (psubclass->instance.hwnd,
                                 WMPUX_BUBBLEON,
                                 MPFROMHWND (WinWindowFromPoint (HWND_DESKTOP,
                                                                 aptl,
                                                                 TRUE)),
                                 MPVOID) ;
                     }
                else
                     {
                     /* Grî·e des Dialoges/Frames ermitteln */
                     WinQueryWindowRect (psubclass->instance.hwnd,
                                         &rcl) ;
                     /* Ist es Dialog oder Frame? */
                     if   (psubclass->instance.fParent)
                          {
                          /* Frame, Position des Fensters abfragen */
                          WinQueryWindowPos (psubclass->instance.hwnd,
                                             &swp) ;
                          /* Frame Start zur Grî·e addieren */
                          rcl.xLeft = swp.x ;
                          rcl.yBottom = swp.y ;
                          rcl.xRight += swp.x ;
                          rcl.yTop += swp.y ;
                          }
                     /* Ist Mauszeiger ausserhalb des Fensters? */
                     if   (!WinPtInRect (psubclass->instance.hab,
                                          &rcl,
                                          aptl))
                          {
                          /* Evtl. sichtbare BubbleHelp ausschalten */
                          WinSendMsg (psubclass->instance.hwnd,
                                      WMPUX_BUBBLEOFF,
                                      MPVOID,
                                      MPVOID) ;
                          }
                     }
                return (MRESULT) FALSE ;
                }
           break ;
           }
      /* Ausschalten der BubbleHelp */
      case WMPUX_BUBBLEOFF:
           {
           HPS hps ;

           /* LÑuft Timer? */
           if   (psubclass->instance.fTimer)
                {
                /* Ja, stoppen */
                WinStopTimer (psubclass->instance.hab,
                              psubclass->instance.hwnd,
                              psubclass->instance.ulTimer) ;
                /* Neuen Status des Timers merken */
                psubclass->instance.fTimer = FALSE ;
                }
           /* Ist BubbleHelp sichtbar? */
           if   (psubclass->instance.fBubble)
                {
                /* Presentation Space des Desktops besorgen */
                hps = WinGetScreenPS (HWND_DESKTOP) ;
                /* Background Bitmap selektieren */
                GpiSetBitmap (hps_g,
                              psubclass->instance.hbmBackground) ;
                /* Alten Hintergrund Åber sichtbare BubbleHelp zeichnen */
                GpiBitBlt (hps,
                           hps_g,
                           4,
                           psubclass->instance.aptl,
                           ROP_SRCCOPY,
                           BBO_IGNORE) ;
                /* Presentation Space wieder freigeben */
                WinReleasePS (hps) ;
                /* Neuen Status der BubbleHelp merken */
                psubclass->instance.fBubble = FALSE ;
                }
           /* Es gibt kein BubbleHelp Fenster mehr */
           psubclass->instance.hwndPrevious = NULLHANDLE ;
           return (MRESULT) FALSE ;
           }
      /* Einschalten der BubbleHelp */
      case WMPUX_BUBBLEON:
           {
           CHAR        sz [128] ;
           FATTRS      fat ;
           FONTMETRICS fm ;
           HDC         hdc ;
           HPS         hps ;
           LONG        lX ;
           LONG        lY ;
           POINTL      aptl [4] ;
           POINTL      ptl ;
           RECTL       rcl ;
           SIZEF       sizfx ;
           ULONG       ulDrawn ;
           ULONG       ulHeight ;
           ULONG       ulI ;
           ULONG       ulLength ;

           /* Ist BubbleHelp bereits sichtbar? */
           if   (!psubclass->instance.fBubble)
                {
                /* Nein, mit Hilfe der ID des Kontrollelementes und dem Ressourcehandle der Anwendung */
                /*   einen beschreibenden Text lesen */
                if   (WinLoadString (psubclass->instance.hab,
                                     psubclass->instance.hmod,
                                     WinQueryWindowUShort (HWNDFROMMP (mp1),
                                                           QWS_ID),
                                     sizeof (sz),
                                     sz))
                     {
                     /* Nur wenn Text vorhanden einen Desktop Fenster Device Context îffnen */
                     hdc = WinOpenWindowDC (HWND_DESKTOP) ;
                     /* Einen Presentation Spave fÅr den Desktop îffnen */
                     hps = WinGetScreenPS (HWND_DESKTOP) ;
                     /* Aktives Fenster mit BubbleHelp merken */
                     psubclass->instance.hwndPrevious = HWNDFROMMP (mp1) ;
                     /* Mauszeiger Position abfragen */
                     WinQueryPointerPos (HWND_DESKTOP,
                                         &ptl) ;
                     /* Diese Position ist Startkoordinate der BubbleHelp */
                     aptl[0].x = ptl.x ;
                     aptl[0].y = ptl.y ;
                     /* Die Grî·e der BubbleHelp entspricht der Grî·e der beiden AND/XOR Bitmaps */
                     aptl[1].x = ptl.x + ulCX_g ;
                     aptl[1].y = ptl.y + ulCY_g ;
                     aptl [2] = aptl [0] ;
                     aptl [3] = aptl [1] ;
                     /* Position und Grî·e der BubbleHelp fÅr den Repaint merken */
                     psubclass->instance.aptl [0] = aptl [0] ;
                     psubclass->instance.aptl [1] = aptl [1] ;
                     psubclass->instance.aptl [2] = aptl [2] ;
                     psubclass->instance.aptl [3] = aptl [3] ;

                     /* Background Bitmap selektieren */
                     GpiSetBitmap (hps_g,
                                   psubclass->instance.hbmBackground) ;
                     /* Hintergrund der BubbleHelp kopieren und archivieren */
                     GpiBitBlt (hps_g,
                                hps,
                                4,
                                aptl,
                                ROP_SRCCOPY,
                                BBO_IGNORE) ;
                     /* Grî·e der beiden AND/XOR Bitmaps festlegen */
                     /* Startkoordinate entspricht dem gesicherten Hintergrund */
                     aptl[2].x = 0 ;
                     aptl[2].y = 0 ;
                     aptl[3].x = ulCX_g ;
                     aptl[3].y = ulCY_g ;
                     /* AND Bitmap selektieren */
                     GpiSetBitmap (hps_g,
                                   hbmAnd_g) ;
                     /* AND Bitmap ausgeben */
                     GpiBitBlt (hps,
                                hps_g,
                                4,
                                aptl,
                                ROP_SRCAND,
                                BBO_IGNORE) ;
                     /* XOR Bitmap selektieren */
                     GpiSetBitmap (hps_g,
                                   hbmXor_g) ;
                     /* XOR Bitmap ausgeben */
                     GpiBitBlt (hps,
                                hps_g,
                                4,
                                aptl,
                                ROP_SRCINVERT,
                                BBO_IGNORE) ;
                     /* Der auszugebende Text mu· in der BubbleHelp so positioniert werden, */
                     /* da· er nicht die Blase Åberschreibt */
                     rcl.xLeft = aptl[0].x + 6 ;
                     rcl.xRight = aptl[1].x - 12 ;
                     rcl.yBottom = aptl[0].y + 6 ;
                     rcl.yTop = aptl[1].y - 12 ;
                     /* Strukur lîschen */
                     memset (&fat,
                             0,
                             sizeof (FATTRS)) ;
                     fat.usRecordLength = sizeof (FATTRS) ;
                     /* Einen transformierbaren Outline Font ermitteln */
                     fat.fsFontUse = FATTR_FONTUSE_OUTLINE | FATTR_FONTUSE_TRANSFORMABLE ;
                     /* Sollte schon Helv sein (mîglichst klein) */
                     strcpy (fat.szFacename,
                             "Helv") ;
                     /* Mit der Grî·e 8 */
                     fat.lAveCharWidth = 8 ;
                     fat.lMaxBaselineExt = 8 ;
                     /* Einen solchen Font anfordern */
                     GpiCreateLogFont (hps,
                                       NULL,
                                       1,
                                       &fat) ;
                     /* Diesen Font verwenden */
                     GpiSetCharSet (hps,
                                    1) ;
                     /* Horizontale Bildschirm Grî·e ermitteln */
                     DevQueryCaps (hdc,
                                   CAPS_HORIZONTAL_RESOLUTION,
                                   1,
                                   &lX) ;
                     /* Vertikale Bildschirm Grî·e ermitteln */
                     DevQueryCaps (hdc,
                                   CAPS_VERTICAL_RESOLUTION,
                                   1,
                                   &lY) ;
                     /* Koordinaten umrechnen */
                     ptl.x = 254 * 100 * lX / 7200000 ;
                     ptl.y = 254 * 100 * lY / 7200000 ;
                     /* Koordinaten konvertieren */
                     GpiConvert (hps,
                                 CVTC_DEVICE,
                                 CVTC_PAGE,
                                 1,
                                 &ptl) ;
                     sizfx.cx = MAKEFIXED (ptl.x, 0) ;
                     sizfx.cy = MAKEFIXED (ptl.y, 0) ;
                     /* In diesen Rahmen soll der Text ausgegeben werden */
                     GpiSetCharBox (hps,
                                    &sizfx) ;
                     /* Fontmetrics dieses Fonts ermitteln */
                     GpiQueryFontMetrics (hps,
                                          sizeof (FONTMETRICS),
                                          &fm) ;
                     /* Hîhe dieses Fonts abfragen */
                     ulHeight = fm.lMaxBaselineExt ;
                     /* LÑnge des auszugebenden Textes */
                     ulLength = strlen (sz) ;
                     /* In einer Schleife von oben nach unten */
                     for  (ulI = 0; ulI < ulLength; ulI += ulDrawn, rcl.yTop -= ulHeight)
                          {
                          /* Text ausgeben. Die Y Koordinate wird um die Hîhe des Fonts pro Zeile vermindert */
                          /* Durch den WORDBREAK werden immer nur ganze Wîrter ausgegeben */
                          ulDrawn = WinDrawText (hps,
                                                 -1,
                                                 &sz [ulI],
                                                 &rcl,
                                                 0,
                                                 0,
                                                 DT_LEFT | DT_TEXTATTRS | DT_TOP | DT_WORDBREAK) ;
                          }
                     /* Der Font wird nicht mehr benîtigt */
                     GpiDeleteSetId (hps,
                                     LCID_DEFAULT) ;
                     /* Presentation Space lîschen */
                     WinReleasePS (hps) ;
                     /* Device Context lîschen */
                     DevCloseDC (hdc) ;
                     /* BubbleHelp ist jetzt sichtbar */
                     psubclass->instance.fBubble = TRUE ;
                     }
                }
           return (MRESULT) FALSE ;
           }
      /* Aktivierung der BubbleHelp */
      case WMPUX_BUBBLESTART:
           /* Ist Timer noch nicht aktiv? */
           if   (!psubclass->instance.fTimer)
                {
                /* Ja, Timer mit 800ms Verzîgerung starten */
                WinStartTimer (psubclass->instance.hab,
                               psubclass->instance.hwnd,
                               psubclass->instance.ulTimer,
                               800) ;
                /* Neuen Status des Timers vermerken */
                psubclass->instance.fTimer = TRUE ;
                }
           return (MRESULT) FALSE ;
      }
   /* Original Fensterprozedur des Dialoges/Frames jetzt aufrufen */
   return psubclass->instance.pfnwp (hwnd,
                                     msg,
                                     mp1,
                                     mp2) ;
   }

