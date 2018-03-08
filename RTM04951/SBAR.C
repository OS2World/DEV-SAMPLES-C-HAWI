/* $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBAR.C 1.1 1995/05/25 15:37:25 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SBAR.H"
#include "SBAR.PH"

PSUBCLASS psubclass_g = NULL ;
ULONG     ulCount_g = 0 ;

BOOL EXPENTRY Deregister (HWND hwnd)
   {
   PSUBCLASS psubclass ;
   PSUBCLASS psubclassPrev ;
   ULONG     ulI ;

   /* Sind Åberhaupt Subclasses aktiv? */
   if   (ulCount_g)
        {
        /* Ja. Verkettete Liste durchsuchen */
        for  (ulI = 0,
               psubclassPrev = NULL,
               psubclass = psubclass_g;
              ulI < ulCount_g;
              ulI++,
               psubclassPrev = psubclass,
               psubclass = psubclass->psubclassNext)
             {
             /* Ist dies die Subclass die deregistriert werden soll? */
             if   (psubclass->hwndSubclass == hwnd)
                  {
                  /* Ja. Subclass zurÅcknehmen */
                  WinSubclassWindow (psubclass->hwndSubclass,
                                     psubclass->pfnwp) ;
                  /* Statusbar Fenster lîschen */
                  WinDestroyWindow (psubclass->hwndStatusbar) ;
                  /* Element aus Kette rauswerfen bzw. Root Pointer setzen */
                  if   (psubclassPrev)
                       {
                       psubclassPrev->psubclassNext = psubclass->psubclassNext ;
                       }
                  else
                       {
                       psubclass_g = psubclass->psubclassNext ;
                       }
                  /* Memory der Instanzdaten lîschen */
                  free (psubclass) ;
                  /* InstanzzÑhler herabsetzen */
                  ulCount_g-- ;
                  break ;
                  }
             }
        }
   return TRUE ;
   }

/* Registrieren einer Statuszeile */                  
BOOL EXPENTRY Register (HWND hwnd)
   {
   CLASSINFO clsi ;
   HAB       hab ;
   PSUBCLASS psubclass ;
   PSUBCLASS psubclassPrev ;
   ULONG     ulI ;

   /* Anchorblock ermitteln */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Sind bereits Statusbars in dieser Anwendung in Benutzung */
   if   (!ulCount_g)
        {
        /* Nein. Klasseninformationen zur STATIC Klasse ermitteln */
        WinQueryClassInfo (hab,
                           WC_STATIC,
                           &clsi) ;
        /* Subclassen der Static Klasse */
        WinRegisterClass (hab,
                          WCP_SBARSTATIC,
                          StaticWndProc,
                          0,
                          clsi.cbWindowData + sizeof (PVOID)) ;
        /* Speicherplatz fÅr Instanzdaten allokieren */
        psubclass_g = calloc (1,
                              sizeof (SUBCLASS)) ;
        /* Erstes Subclass in modulglobaler Variable eintragen */
        psubclass = psubclass_g ;
        }
   else
        {
        /* Ja. In einer Schleife das Ende der verketteten Liste suchen */
        for  (ulI = 0,
               psubclass = psubclass_g;
              ulI < ulCount_g;
              ulI++,
               psubclassPrev = psubclass,
               psubclass = psubclass->psubclassNext) ;
        /* Speicherplatz fÅr Instanzdaten allokieren */
        psubclassPrev->psubclassNext = calloc (1,
                                               sizeof (SUBCLASS)) ;
        /* Diese Subclass am Ende eintragen */
        psubclass = psubclassPrev->psubclassNext ;
        }
   /* Subclass ZÑhler erhîhen */
   ulCount_g++ ;
   /* Statusbar Klasse registrieren */
   WinRegisterClass (hab,
                     SK_CLASSNAME,
                     StatusbarWndProc,
                     0,
                     0) ;
   /* Fenster fÅr Statusbar anlegen */
   psubclass->hwndStatusbar = WinCreateWindow (hwnd,
                                               SK_CLASSNAME,
                                               "",
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               hwnd,
                                               HWND_TOP,
                                               FID_SBAR,
                                               NULL,
                                               NULL) ;
   /* Framewindow Handle fÅr Subclass merken */
   psubclass->hwndSubclass = hwnd ;
   /* Frame subclassen */
   psubclass->pfnwp = WinSubclassWindow (psubclass->hwndSubclass,
                                         (PFNWP) SubclassWndProc) ;
   return TRUE ;
   }

/* Fensterprozedur der einzelnen Statusbar Elemente */
MRESULT EXPENTRY StaticWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   PWNDSTATIC pwnd ;

   /* Pointer auf Element Instanzdaten holen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      /* Element auf Statuszeile wird angelegt */
      case WM_CREATE:
           {
           CLASSINFO clsi ;

           /* Platz fÅr Element Instanzdaten allokieren */
           pwnd = calloc (1,
                          sizeof (WNDSTATIC)) ;
           /* Pointer in den Window Words merken */
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           /* Klasseninformationen zu Static lesen */
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           /* Static Klasse subclassen */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           break ;
           }
      /* Element auf Statuszeile wird gelîscht */
      case WM_DESTROY:
           {
           MRESULT mr ;

           /* Original Prozedur aufrufen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Instanzdaten der Elemente lîschen */
           free (pwnd) ;
           return mr ;
           }
      /* Element auf Statuszeile mu· gezeichnet werden */
      case WM_PAINT:
           {
           HPS     hps ;
           MRESULT mr ;
           POINTL  ptl ;
           RECTL   rcl ;
           ULONG   ulI ;
           ULONG   ulJ ;

           /* Original Fensterprozedur aufrufen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Soll 3D Look um Element gezeichnet werden? */
           if   (pwnd->ulLook != SK_LOOKIGNORE)
                {
                /* Ja */
                if   (pwnd->ulLook == SK_LOOKIN)
                     {
                     ulI = SYSCLR_BUTTONDARK ;
                     ulJ = SYSCLR_BUTTONLIGHT ;
                     }
                else
                     {
                     ulI = SYSCLR_BUTTONLIGHT ;
                     ulJ = SYSCLR_BUTTONDARK ;
                   }
                /* HPS besorgen */
                hps = WinGetPS (hwnd) ;
                /* Grî·e Elementfenster ermitteln */
                WinQueryWindowRect (hwnd,
                                    &rcl) ;
                /* 3D Rahmen durch vier Linien zeichnen */
                rcl.xRight-- ;
                rcl.yTop-- ;
                ptl.x = 0 ;
                ptl.y = 0 ;
                GpiMove (hps,
                         &ptl) ;
                GpiSetColor (hps,
                             ulI) ;
                ptl.x = 0 ;
                ptl.y = rcl.yTop ;
                GpiLine (hps,
                         &ptl) ;
                ptl.x = rcl.xRight ;
                ptl.y = rcl.yTop ;
                GpiLine (hps,
                         &ptl) ;
                GpiSetColor (hps,
                             ulJ) ;
                ptl.x = rcl.xRight ;
                ptl.y = 0 ;
                GpiLine (hps,
                         &ptl) ;
                ptl.x = 0 ;
                ptl.y = 0 ;
                GpiLine (hps,
                         &ptl) ;
                /* HPS wegwerfen */
                WinReleasePS (hps) ;
                }
           return mr ;
           }
      /* Ein Inhalt des Elementes wurde verÑndert */
      case WM_SETWINDOWPARAMS:
           {
           CHAR       szText [128] ;
           PWNDPARAMS pwprm ;

           pwprm = PVOIDFROMMP (mp1) ;
           /* Text geÑndert? */
           if   (pwprm->fsStatus & WPM_TEXT)
                {
                /* Ist Text gefÅllt? */
                if   (*pwprm->pszText)
                     {
                     /* LinkbÅndig? */
                     if   (pwnd->ulStyle == DT_LEFT)
                          {
                          /* Wenn noch kein Blank am Anfang */
                          if   (*pwprm->pszText != ' ')
                               {
                               /*dann eins vorne anfÅgen */
                               strcpy (szText,
                                       " ") ;
                               strcat (szText,
                                       pwprm->pszText) ;
                               WinSetWindowText (hwnd,
                                                 szText) ;
                               return (MRESULT) FALSE ;
                               }
                          }
                     /* RechtsbÅndig? */
                     if   (pwnd->ulStyle == DT_RIGHT)
                          {
                          /* Wenn noch kein Blank am Ende */
                          if   (*(pwprm->pszText + strlen (pwprm->pszText) - 1) != ' ')
                               {
                               /* dann eins hinten anfÅgen */
                               strcpy (szText,
                                       pwprm->pszText) ;
                               strcat (szText,
                                       " ") ;
                               WinSetWindowText (hwnd,
                                                 szText) ;
                               return (MRESULT) FALSE ;
                               }
                          }
                     }
                }
           break ;
           }
      /* Look des Elementes wird verÑndert */
      case WMPSBAR_SETLOOK:
           pwnd->ulLook = LONGFROMMP (mp1) ;
           return (MRESULT) FALSE ;
      /* Style des Elementes wird verÑndert */
      case WMPSBAR_SETSTYLE:
           pwnd->ulStyle = LONGFROMMP (mp1) ;
           return (MRESULT) FALSE ;
      }
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }

/* Fensterprozedur der Statuszeile */
MRESULT EXPENTRY StatusbarWndProc (HWND   hwnd,
                                   MSG    msg,
                                   MPARAM mp1,
                                   MPARAM mp2)
   {
   PSUBCLASS psubclass ;

   {
   ULONG ulI ;

   /* In einer Schleife in verketteter Liste nach passenden Instanzdaten suchen */
   for  (ulI = 0,
          psubclass = psubclass_g;
         ulI < ulCount_g;
         ulI++,
          psubclass = psubclass->psubclassNext)
        {
        if   (psubclass->hwndStatusbar == hwnd)
             {
             /* gefunden */
             break ;
             }
        }
   }
   switch (msg)
      {
      /* Statuszeile mu· gezeichnet werden */
      case WM_PAINT:
           {
           HPS    hps ;
           LONG   lCXToolbar ;
           POINTL ptl ;
           RECTL  rcl ;
           RECTL  rclSave ;
           ULONG  ul ;
           ULONG  ulI ;

           /* HPS besorgen */
           hps = WinBeginPaint (hwnd,
                                NULLHANDLE,
                                NULL) ;
           /* Grî·e der Statuszeile ermitteln */
           WinQueryWindowRect (hwnd,
                               &rclSave) ;
           /* Inhalt fÅllen */
           WinFillRect (hps,
                        &rclSave,
                        SYSCLR_MENU) ;
           /* Rahmen drumherum zeichnen */
           rclSave.xRight-- ;
           rclSave.yTop-- ;
           lCXToolbar = rclSave.xRight ;
           ptl.x = rclSave.xLeft ;
           ptl.y = rclSave.yBottom ;
           GpiMove (hps,
                    &ptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONLIGHT) ;
           ptl.x = rclSave.xLeft ;
           ptl.y = rclSave.yTop ;
           GpiLine (hps,
                    &ptl) ;
           ptl.x = rclSave.xRight ;
           ptl.y = rclSave.yTop ;
           GpiLine (hps,
                    &ptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONDARK) ;
           ptl.x = rclSave.xRight ;
           ptl.y = rclSave.yBottom ;
           GpiLine (hps,
                    &ptl) ;
           ptl.x = rclSave.xLeft ;
           ptl.y = rclSave.yBottom ;
           GpiLine (hps,
                    &ptl) ;
           rclSave.yBottom += 4 ;
           rclSave.yTop -= 4 ;
           /* In einer Schleife die Elemente abarbeiten */
           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                rcl = rclSave ;
                rcl.xLeft += 4 ;
                /* Element bis ans rechte Ende zeichnen */
                if   (psubclass->asbi[ulI].lSize == -1)
                     {
                     /* Ja */
                     rcl.xRight = lCXToolbar - 4 ;
                     }
                else
                     {
                     /* ansonsten Prozent Angabe in Pixel umrechnen */
                     rcl.xRight = rcl.xLeft + lCXToolbar * psubclass->asbi[ulI].lSize / 100 ;
                     }
                /* Ist Element bereits angelegt? */
                if   (psubclass->asbi[ulI].hwnd)
                     {
                     /* Ja. Nur neu positionieren */
                     WinSetWindowPos (psubclass->asbi[ulI].hwnd,
                                      HWND_TOP,
                                      rcl.xLeft,
                                      rcl.yBottom,
                                      rcl.xRight - rcl.xLeft,
                                      rcl.yTop - rcl.yBottom,
                                      SWP_MOVE | SWP_SHOW | SWP_SIZE) ;
                     }
                else
                     {
                     /* Nein. Element anlegen */
                     psubclass->asbi[ulI].hwnd = WinCreateWindow (hwnd,
                                                                  WCP_SBARSTATIC,
                                                                  "",
                                                                  psubclass->asbi[ulI].ulStyle | DT_VCENTER | SS_TEXT | WS_SYNCPAINT | WS_VISIBLE,
                                                                  rcl.xLeft,
                                                                  rcl.yBottom,
                                                                  rcl.xRight - rcl.xLeft,
                                                                  rcl.yTop - rcl.yBottom,
                                                                  hwnd,
                                                                  HWND_TOP,
                                                                  0,
                                                                  NULL,
                                                                  NULL) ;
                     /* 3D Look ans Element hinterherschicken */
                     WinSendMsg (psubclass->asbi[ulI].hwnd,
                                 WMPSBAR_SETLOOK,
                                 MPFROMLONG (psubclass->asbi[ulI].ulLook),
                                 MPVOID) ;
                     /* Style ans Element hinterherschicken */
                     WinSendMsg (psubclass->asbi[ulI].hwnd,
                                 WMPSBAR_SETSTYLE,
                                 MPFROMLONG (psubclass->asbi[ulI].ulStyle),
                                 MPVOID) ;
                     /* Hintergrundfarbe setzen */
                     ul = SYSCLR_MENU ;
                     WinSetPresParam (psubclass->asbi[ulI].hwnd,
                                      PP_BACKGROUNDCOLORINDEX,
                                      sizeof (ul),
                                      &ul) ;
                     /* Schriftart und Grî·e setzen */
                     WinSetPresParam (psubclass->asbi[ulI].hwnd,
                                      PP_FONTNAMESIZE,
                                      strlen ("8.Helv") + 1,
                                      "8.Helv") ;
                     /* Vordergrundfarbe setzen */
                     ul = SYSCLR_MENUTEXT ;
                     WinSetPresParam (psubclass->asbi[ulI].hwnd,
                                      PP_FOREGROUNDCOLORINDEX,
                                      sizeof (ul),
                                      &ul) ;
                     /* Text des Elementes ausgeben */
                     WinSetWindowText (psubclass->asbi[ulI].hwnd,
                                       psubclass->asbi[ulI].szText) ;
                     }
                rclSave.xLeft += lCXToolbar * psubclass->asbi[ulI].lSize / 100 + 4 ;
                }
           /* HPS wegwerfen */
           WinEndPaint (hps) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Subclass der Framewindow Prozedur */
MRESULT EXPENTRY SubclassWndProc (HWND   hwnd,
                                  MSG    msg,
                                  MPARAM mp1,
                                  MPARAM mp2)
   {
   PSUBCLASS psubclass ;

   {
   ULONG ulI ;

   /* In einer Schleife in verketteter Liste nach passenden Instanzdaten suchen */
   for  (ulI = 0,
          psubclass = psubclass_g;
         ulI < ulCount_g;
         ulI++,
          psubclass = psubclass->psubclassNext)
        {
        if   (psubclass->hwndSubclass == hwnd)
             {
             /* gefunden */
             break ;
             }
        }
   }
   switch (msg)
      {
      /* Fenster wurde aktiviert/deaktiviert */
      case WM_ACTIVATE:
           /* Wurde es aktiviert? */
           if   (SHORT1FROMMP (mp1))
                {
                /* Dann neues Zeichnen der Statuszeile auslîsen */
                WinInvalidateRect (psubclass->hwndStatusbar,
                                   NULL,
                                   TRUE) ;
                }
           break ;
      /* PM will Clientgrî·e aus Framegrî·e ermittelt haben (und umgekehrt) */
      case WM_CALCFRAMERECT:
           {
           BOOL   f ;
           PRECTL prcl ;

           /* Original Fensterprozedur aufrufen */
           if   ((f = LONGFROMMR (psubclass->pfnwp (hwnd,
                                                    msg,
                                                    mp1,
                                                    mp2))) == TRUE)
                {
                prcl = PVOIDFROMMP (mp1) ;
                /* SollClient berechnet werden? */
                if   (SHORT1FROMMP (mp2))
                     {
                     /* Ja. Unten Platz fÅr Statuszeile lassen */
                     prcl->yBottom += 30 ;
                     /* Hîhe entsprechend verringern */
                     prcl->yTop -= 30 ;
                     }
                }                   
           return MRFROMLONG (f) ;
           }
      /* PM erlaubt das Positionieren aller Frame Controls (inkl. unseres neuen) */
      case WM_FORMATFRAME:
           {
           PRECTL prcl ;
           PSWP   pswp ;
           ULONG  ul ;
           ULONG  ulI ;

           /* Original Fensterprozedur aufrufen */
           ul = LONGFROMMR (psubclass->pfnwp (hwnd,
                                              msg,
                                              mp1,
                                              mp2)) ;
           /* Pointer auf ein Array von Positionen aller Frame Controls */
           pswp = PVOIDFROMMP (mp1) ;
           /* In einer Schleife alle Controls durchsuchen */
           for  (ulI = 0; ulI < ul; ulI++)
                {
                /* Gibt es einen vertikalen Scrollbar? */
                if   (WinQueryWindowUShort (pswp[ulI].hwnd,
                                            QWS_ID) == FID_VERTSCROLL)
                     {
                     /* Ja. Flags setzen zum Verschieben und Grî·e verÑndern */
                     pswp[ulI].fl |= (SWP_MOVE | SWP_SIZE) ;
                     /* Hîhe um Hîhe Statuszeile verringern */
                     pswp[ulI].cy -= 30 ;
                     /* Unten Platz fÅr Statuszeile lassen */
                     pswp[ulI].y += 30 ;
                     break ;
                     }
                }
           /* In einer Schleife alle Controls durchsuchen */
           for  (ulI = 0; ulI < ul; ulI++)
                {
                /* Gibt es eine Clientwindow? */
                if   (WinQueryWindowUShort (pswp[ulI].hwnd,
                                            QWS_ID) == FID_CLIENT)
                     {
                     /* Ja. Flags setzen zum Verschieben und Grî·e verÑndern */
                     pswp[ulI].fl |= (SWP_MOVE | SWP_SIZE) ;
                     /* Hîhe um Hîhe Statuszeile verringern */
                     pswp[ulI].cy -= 30 ;
                     /* Unten Platz fÅr Statuszeile lassen */
                     pswp[ulI].y += 30 ;
                     break ;
                     }
                }
           /* Die letzte Position ist fÅr unser zusÑtzliches Control freigehalten worden */
           /* Flags wie beim Clientwindow */
           pswp[ul].fl = pswp[ulI].fl ;
           /* Hîhe entspricht 30 Pixels */
           pswp[ul].cy = 30 ;
           /* Breite wie beim Clientwindow */
           pswp[ul].cx = pswp[ulI].cx ;
           /* Y Startkoordinate wie beim Clientwindow -30 (wurde ja bereits von uns verÑndert) */
           pswp[ul].y = pswp[ulI].y - 30 ;
           /* X Startkoordinate wie beim Clientwindow */
           pswp[ul].x = pswp[ulI].x ;
           /* Egal wo angeordnet in ZORDER */
           pswp[ul].hwndInsertBehind = 0 ;
           /* Handle des Statuszeilen Fensters eintragen */
           pswp[ul].hwnd = psubclass->hwndStatusbar ;
           /* Soll Client direkt berechnet werden */
           if   ((prcl = PVOIDFROMMP (mp2)) != NULL)
                {
                /* Ja. Diese Daten zusÑtzlich korrigieren */
                /* Hîhe um Hîhe Statuszeile verringern */
                prcl->yTop -= 30 ;
                /* Unten Platz fÅr Statuszeile lassen */
                prcl->yBottom += 30 ;
                }
           /* Es gibt jetzt ein Frame Control mehr */
           return MRFROMLONG (ul + 1) ;
           }
      case WM_QUERYFRAMECTLCOUNT:
           /* Original Fensterprozedur aufrufen und eine Frame Extension addieren */
           return MRFROMSHORT (SHORT1FROMMR (psubclass->pfnwp (hwnd,
                                                               msg,
                                                               mp1,
                                                               mp2)) + 1) ;
      /* Initialdaten fÅr Statuszeile treffen ein */
      case WMPSBAR_SETDATA:
           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Anzahl Elemente merken */
                psubclass->ulCount = LONGFROMMP (mp1) ;
                /* Bereich leer machen */
                memset (psubclass->asbi,
                        0,
                        10 * sizeof (SBARITEM)) ;
                /* Initialdaten Åbernehmen */
                memcpy (psubclass->asbi,
                        PVOIDFROMMP (mp2),
                        psubclass->ulCount * sizeof (SBARITEM)) ;
                /* Neu Zeichnen der Statuszeile veranlassen */
                WinInvalidateRect (psubclass->hwndStatusbar,
                                   NULL,
                                   TRUE) ;
                }
           return (MRESULT) FALSE ;
      /* 3D Look eines Elementes soll verÑndert werden */
      case WMPSBAR_SETLOOK:
           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Neuen Look eintragen */
                psubclass->asbi[LONGFROMMP (mp1) - 1].ulLook = LONGFROMMP (mp2) ;
                /* und zusÑtzlich an das Element selbst schicken */
                WinSendMsg (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                            msg,
                            MPFROMLONG (psubclass->asbi[LONGFROMMP (mp1) - 1].ulLook),
                            MPVOID) ;
                /* Neu Zeichnen der Elementes auf der Statuszeile veranlassen */
                WinInvalidateRect (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                                   NULL,
                                   TRUE) ;
                }
           return (MRESULT) FALSE ;
      /* Grî·e eines Elementes soll verÑndert werden */
      case WMPSBAR_SETSIZE:
           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Neue Grî·e eintragen */
                psubclass->asbi[LONGFROMMP (mp1) - 1].lSize = LONGFROMMP (mp2) ;
                /* Neu Zeichnen der Statuszeile veranlassen */
                WinInvalidateRect (psubclass->hwndStatusbar,
                                   NULL,
                                   TRUE) ;
                }
           return (MRESULT) FALSE ;
      /* Style eines Elementes soll verÑndert werden */
      case WMPSBAR_SETSTYLE:
           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Neuen Style eintragen */
                psubclass->asbi[LONGFROMMP (mp1) - 1].ulStyle = LONGFROMMP (mp2) ;
                /* und zusÑtzlich an das Element selbst schicken */
                WinSendMsg (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                            msg,
                            MPFROMLONG (psubclass->asbi[LONGFROMMP (mp1) - 1].ulStyle),
                            MPVOID) ;
                /* Neu Zeichnen der Elementes auf der Statuszeile veranlassen */
                WinInvalidateRect (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                                   NULL,
                                   TRUE) ;
                }
           return (MRESULT) FALSE ;
      /* Text eines Elementes soll verÑndert werden */
      case WMPSBAR_SETTEXT:
           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Neuen Text eintragen */
                strcpy (psubclass->asbi[LONGFROMMP (mp1) - 1].szText,
                        PVOIDFROMMP (mp2)) ;
                /* und setzen */
                WinSetWindowText (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                                  psubclass->asbi[LONGFROMMP (mp1) - 1].szText) ;
                }
           return (MRESULT) FALSE ;
      /* Numerischer Wert eines Elementes soll verÑndert werden */
      case WMPSBAR_SETVALUE:
           {
           CHAR sz [128] ;

           /* Nicht mehr als 99 PlÑtze vorhanden */
           if   (LONGFROMMP (mp1) < 100)
                {
                /* Numerischen Wert nach Text wandeln */
                _ltoa (LONGFROMMP (mp2),
                       sz,
                       10) ;
                /* Neuen Text eintragen */
                strcpy (psubclass->asbi[LONGFROMMP (mp1) - 1].szText,
                        sz) ;
                /* und setzen */
                WinSetWindowText (psubclass->asbi[LONGFROMMP (mp1) - 1].hwnd,
                                  psubclass->asbi[LONGFROMMP (mp1) - 1].szText) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   return psubclass->pfnwp (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }
