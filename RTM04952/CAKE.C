/* $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKE.C 1.1 1995/05/25 18:45:51 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CAKE.H"
#include "CAKE.PH"

/* Abfrage der Anzahl der Elemente in Liste */
ULONG CListCount (PVOID pv)
   {
   return ((PLISTROOT) pv)->ulCount ;
   }

/* Lîschen eines Listenelementes */
BOOL CListDelete (PVOID pv,
                  ULONG ul,
                  PVOID pv2)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = CKC_ERROR ;
   /* Rootpointer holen */
   plistroot = pv ;
   /* Existieren Åberhaupt Elemente? */
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        /* Umrechnung von Indexangaben auf mîgliche Standardpositionierungen */
        if   (ul == CK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = CK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = CK_LAST ;
                  }
             }
        /* Wie soll Element ermittelt werden? */
        switch (ul)
           {
           /* Erstes Element gewÅnscht */
           case CK_FIRST:
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           /* In einer Schleife bis zum gewÅnschten Index wandern */
           case CK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = CK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          /* Gefunden */
                          f = CKC_NOERROR ;
                          break ;
                          }
                     }
                break ;
           /* Ermittlung eines Elementes Åber seinen SchlÅssel */
           case CK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      /* Ist es ein TextschlÅssel? */
                      case CK_STRING:
                           if   ((strcmp (CK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      /* Ist es ein numerischer SchlÅssel */
                      case CK_ULONG:
                           if   (*((PULONG) (CK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      }
                   /* Wenn SchlÅssel gefunden dann Suche abbrechen */
                   if   (f == CKC_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = CK_PTR2NEXTINCURR) != NULL) ;
                break ;
           /* Letztes Element gewÅnscht */
           case CK_LAST:
                pb = plistroot->pvLast ;
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           }
        /* Element gefunden? */
        if   (f == CKC_NOERROR)
             {
             /* Gibt es ein vorheriges Element? */
             if   (!CK_PTR2PREVINCURR)
                  {
                  /* Nein, dann ist nÑchstes Element das neue erste Element <g> */
                  plistroot->pvFirst = CK_PTR2NEXTINCURR ;
                  }
             else
                  {
                  /* Ja, dann wird das nÑchste im vorherigen den nÑchsten im aktuellen Element aufnehmen */ 
                  CK_PTR2NEXTINPREV = CK_PTR2NEXTINCURR ;
                  }
             /* Gibt es ein nÑchstes Element? */
             if   (!CK_PTR2NEXTINCURR)
                  {
                  /* Nein, dann ist das vorherige das neue letzte Element <g> */
                  plistroot->pvLast = CK_PTR2PREVINCURR ;
                  }
             else
                  {
                  /* Ja, dann wird das vorherige im nÑchsten den vorherigen im aktuellen Element aufnehmen */
                  CK_PTR2PREVINNEXT = CK_PTR2PREVINCURR ;
                  }
             /* Aktuelles Element lîschen */
             free (pb) ;
             /* ZÑhler aller Elemente dekrementieren */
             plistroot->ulCount-- ;
             }
        }
   return f ;
   }

/* Wegwerfen einer kompletten Kette */
BOOL CListDestroy (PVOID pv)
   {
   PBYTE     pb ;
   PBYTE     pbSave ;
   PLISTROOT plistroot ;

   /* Rootpointer holen */
   plistroot = pv ;
   /* Existieren Åberhaupt Elemente? */
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        pbSave = pb ;
        /* In einer Schleife alle Elemente lîschen */
        do
           {
           pb = CK_PTR2NEXTINCURR ;
           free (pbSave) ;
           } while ((pbSave = pb) != NULL) ;
        }
   /* Root Element auch lîschen */
   free (pv) ;
   return CKC_NOERROR ;
   }

/* Iniialisierung einer neuen Kette */
PVOID CListInit (ULONG ulDataSize,
                 ULONG ulKeyOffset,
                 ULONG ulKeyStyle)
   {
   PLISTROOT plistroot ;

   /* Root Element allokieren */
   plistroot = calloc (1,
                       sizeof (LISTROOT)) ;
   /* Grî·e eines Blocks speichern */
   plistroot->ulDataSize = ulDataSize ;
   /* Offset zu einem mîglichen SchlÅssel speichern */
   plistroot->ulKeyOffset = ulKeyOffset ;
   /* Mîglichen SchlÅssel speichern */
   plistroot->ulKeyStyle = ulKeyStyle ;
   return plistroot ;
   }

/* Ein neues Listenelement einfÅgen */
BOOL CListInsert (PVOID pv,
                  ULONG ul,
                  PVOID pv2)
   {
   BOOL      f ;
   PBYTE     pb ;
   PBYTE     pbPrev ;
   PBYTE     pbSave ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = CKC_ERROR ;
   /* Rootpointer holen */
   plistroot = pv ;
   /* Platz fÅr Element plus Platz fÅr die beiden Kettenpointer allokieren */
   pb = calloc (1,
                plistroot->ulDataSize + CK_HEADERDATA) ;
   /* Daten des Elementes reinkopieren */
   memcpy (CK_PTR2DATAINCURR,
           pv2,
           plistroot->ulDataSize) ;
   /* Gibt es bereits ein Element in dieser Kette? */
   if   (!plistroot->pvFirst)
        {
        /* Nein, dann ist das neue zugleich erstes und letztes Element */
        plistroot->pvFirst = pb ;
        plistroot->pvLast = pb ;
        f = CKC_NOERROR ;
        }
   else
        {
        /* Ja, umrechnung von Indexangaben auf mîgliche Standardpositionierungen */
        if   (ul == CK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = CK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 >= plistroot->ulCount)
                  {
                  ul = CK_LAST ;
                  }
             }
        /* Wie soll Position ermittelt werden? */
        switch (ul)
           {
           /* An erster Stelle gewÅnscht */
           case CK_FIRST:
                /* Verkettung durchfÅhren */
                CK_PTR2NEXTINCURR = plistroot->pvFirst ;
                CK_PTR2PREVINNEXT = pb ;
                plistroot->pvFirst = pb ;
                f = CKC_NOERROR ;
                break ;
           /* In einer Schleife bis zur gewÅnschten Position wandern */
           case CK_INDEX:
                pbSave = pb ;
                for  (ulI = 0, pb = plistroot->pvFirst;
                      ulI < plistroot->ulCount;
                      ulI++, pbPrev = pb, pb = CK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          /* Verkettung durchfÅhren */
                          *((PBYTE *) (pbSave + CK_HEADERNEXT)) = *((PBYTE *) (pbPrev + CK_HEADERNEXT)) ;
                          *((PBYTE *) (pbPrev + CK_HEADERNEXT)) = pbSave ;
                          *((PBYTE *) (pbSave + CK_HEADERPREV)) = CK_PTR2PREVINCURR ;
                          CK_PTR2PREVINCURR = pbSave ;
                          f = CKC_NOERROR ;
                          break ;
                          }
                     }
                break ;
           /* Am Ende gewÅnscht */
           case CK_LAST:
                /* Verkettung durchfÅhren */
                CK_PTR2PREVINCURR = plistroot->pvLast ;
                *((PBYTE *) ((PBYTE) plistroot->pvLast + CK_HEADERNEXT)) = pb ;
                plistroot->pvLast = pb ;
                f = CKC_NOERROR ;
                break ;
           }
        }
   /* Element gefunden? */
   if   (f == CKC_NOERROR)
        {
        /* ZÑhler der Elemente erhîhen */
        plistroot->ulCount++ ;
        }
   return f ;
   }

/* Die Daten eines Listenelementes ermitteln */
BOOL CListQuery (PVOID pv,
                 ULONG ul,
                 PVOID pv2,
                 PVOID pv3)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = CKC_ERROR ;
   /* Rootpointer holen */
   plistroot = pv ;
   /* Existieren Åberhaupt Elemente? */
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        /* Umrechnung von Indexangaben auf mîgliche Standardpositionierungen */
        if   (ul == CK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = CK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = CK_LAST ;
                  }
             }
        /* Wie soll Element ermittelt werden? */
        switch (ul)
           {
           /* Erstes Element gewÅnscht */
           case CK_FIRST:
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           /* In einer Schleife bis zum gewÅnschten Index wandern */
           case CK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = CK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          /* Gefunden */
                          f = CKC_NOERROR ;
                          break ;
                          }
                     }
                break ;
           /* Ermittlung eines Elementes Åber seinen SchlÅssel */
           case CK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      /* Ist es ein TextschlÅssel? */
                      case CK_STRING:
                           if   ((strcmp (CK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      /* Ist es ein numerischer SchlÅssel */
                      case CK_ULONG:
                           if   (*((PULONG) (CK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      }
                   /* Wenn SchlÅssel gefunden dann Suche abbrechen */
                   if   (f == CKC_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = CK_PTR2NEXTINCURR) != NULL) ;
                break ;
           /* Letztes Element gewÅnscht */
           case CK_LAST:
                pb = plistroot->pvLast ;
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           }
        /* Element gefunden? */
        if   (f == CKC_NOERROR)
             {
             /* Daten lesen */
             memcpy (pv3,
                     CK_PTR2DATAINCURR,
                     plistroot->ulDataSize) ;
             }
        }
   return f ;
   }

/* Die Daten eines Listenelementes ersetzen */
BOOL CListUpdate (PVOID pv,
                  ULONG ul,
                  PVOID pv2,
                  PVOID pv3)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = CKC_ERROR ;
   /* Rootpointer holen */
   plistroot = pv ;
   /* Existieren Åberhaupt Elemente? */
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        /* Umrechnung von Indexangaben auf mîgliche Standardpositionierungen */
        if   (ul == CK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = CK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = CK_LAST ;
                  }
             }
        /* Wie soll Element ermittelt werden? */
        switch (ul)
           {
           /* Erstes Element gewÅnscht */
           case CK_FIRST:
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           /* In einer Schleife bis zum gewÅnschten Index wandern */
           case CK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = CK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          /* Gefunden */
                          f = CKC_NOERROR ;
                          break ;
                          }
                     }
                break ;
           /* Ermittlung eines Elementes Åber seinen SchlÅssel */
           case CK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      /* Ist es ein TextschlÅssel? */
                      case CK_STRING:
                           if   ((strcmp (CK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      /* Ist es ein numerischer SchlÅssel */
                      case CK_ULONG:
                           if   (*((PULONG) (CK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                /* Gefunden */
                                f = CKC_NOERROR ;
                                }
                           break ;
                      }
                   /* Wenn SchlÅssel gefunden dann Suche abbrechen */
                   if   (f == CKC_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = CK_PTR2NEXTINCURR) != NULL) ;
                break ;
           /* Letztes Element gewÅnscht */
           case CK_LAST:
                pb = plistroot->pvLast ;
                /* Gefunden */
                f = CKC_NOERROR ;
                break ;
           }
        /* Element gefunden? */
        if   (f == CKC_NOERROR)
             {
             /* Daten ersetzen */
             memcpy (CK_PTR2DATAINCURR,
                     pv3,
                     plistroot->ulDataSize) ;
             }
        }
   return f ;
   }

/* Neue Klasse registrieren */
BOOL EXPENTRY CRegisterClass (HAB hab)
   {
   CLASSINFO clsi ;
   
   /* Klassendaten der WC_STATIC ermitteln */
   WinQueryClassInfo (hab,
                      WC_STATIC,
                      &clsi) ;
   /* Lokation der neuen Fensterprozedur und Platz fÅr Instanzdaten nicht vergessen */
   return WinRegisterClass (hab,
                            CK_CLASSNAME,
                            CWndProc,
                            0,
                            clsi.cbWindowData + sizeof (PVOID)) ;
   }

/* Fensterprozedur des neuen Kontrollelementes */
MRESULT EXPENTRY CWndProc (HWND   hwnd,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   PWND pwnd ;

   /* Instanzdaten holen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      /* Benutzer hat geklickt */
      case WM_BUTTON1DBLCLK:
      case WM_BUTTON1UP:
           {
           CVALUE cvalue ;
           HPS    hps ;
           POINTL aptl [TXTBOX_COUNT] ;
           RECTL  rcl ;
           ULONG  ulHeight ;
           ULONG  ulI ;
           ULONG  ulJ ;
           ULONG  ulWidth ;
           ULONG  ulXRegion ;

           /* PS anlegen */
           hps = WinGetPS (hwnd) ;
           /* Anzahl der Elemente in Kette lesen */
           if   (CListCount (pwnd->plistroot))
                {
                /* Breich des Fensters abstecken */
                WinQueryWindowRect (hwnd,
                                    &rcl) ;
                /* In einer Schleife durch alle Elemente */
                for  (ulI = 0, ulWidth = 0;
                      ulI < CListCount (pwnd->plistroot);
                      ulI++)
                     {
                     /* Elementdaten holen */
                     CListQuery (pwnd->plistroot,
                                 CK_INDEX,
                                 (PVOID) ulI,
                                 &cvalue) ;
                     /* Breite dieses Elementtextes ermitteln */
                     GpiQueryTextBox (hps,
                                      strlen (cvalue.szTitle),
                                      cvalue.szTitle,
                                      TXTBOX_COUNT,
                                      aptl) ;
                     /* Grî·te Breite festhalten */
                     if   (aptl[TXTBOX_BOTTOMRIGHT].x > ulWidth)
                          {
                          ulWidth = aptl[TXTBOX_BOTTOMRIGHT].x ;
                          }
                     /* Texthîhe ermitteln */
                     ulHeight = aptl[TXTBOX_TOPLEFT].y ;
                     }
                ulWidth += 10 ;
                ulXRegion = rcl.xRight - ulWidth - 4 ;
                /* Wo wurde geklickt? */
                aptl[0].x = SHORT1FROMMP (mp1) ;
                aptl[0].y = SHORT2FROMMP (mp1) ;
                /* In einer Schleife durch alle Elemente prÅfen wo Klick erfolgte */
                for  (ulI = 0, ulJ = rcl.yTop - ulHeight - 4;
                      ulI < CListCount (pwnd->plistroot);
                      ulI++, ulJ -= ulHeight)
                     {
                     aptl[1].x = ulXRegion + 1 ;
                     aptl[1].y = ulJ ;
                     aptl[2].x = ulXRegion + ulWidth ;
                     aptl[2].y = ulJ + ulHeight ;
                     if   ((aptl[0].x > aptl[1].x) &&
                           (aptl[0].x < aptl[2].x) &&
                           (aptl[0].y > aptl[1].y) &&
                           (aptl[0].y < aptl[2].y))
                          {
                          /* Er hat ein Elemnt der Legende angeklickt entsprechende WM_CONTROL Message and Parent Window schicken */
                          WinPostMsg (WinQueryWindow (hwnd,
                                                      QW_PARENT),
                                      WM_CONTROL,
                                      MPFROM2SHORT (WinQueryWindowUShort (hwnd,
                                                                          QWS_ID), (msg == WM_BUTTON1DBLCLK) ? CKN_ENTER : CKN_SELECT),
                                      MPFROMLONG (ulI)) ;
                          break ;
                          }
                     }
                }
           /* PS wieder freigeben */
           WinReleasePS (hps) ;
           break ;
           }
      /* Control wird auf einem Fenster angelegt */
      case WM_CREATE:
           {
           CLASSINFO clsi ;

           /* Platz fÅr Instanzdaten allokieren */
           pwnd = calloc (1,
                          sizeof (WND)) ;
           /* Instanzdaten in WIndowWords ablegen */
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           /* Klassendaten zu WC_STATIC ermitteln */
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           /* Alter Pointer auf Fensterprozedur merken */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           /* Root Element einer neuen Liste anlegen */
           pwnd->plistroot = CListInit (sizeof (CVALUE),
                                        0,
                                        0) ;
           break ;
           }
      /* Control wird gelîscht */
      case WM_DESTROY:
           {
           MRESULT mr ;

           /* Liste mit allen Inhalten wegwerfen */
           CListDestroy (pwnd->plistroot) ;
           /* Originalprozedur zunÑchst aufrufen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Instanzdaten lîschen */
           free (pwnd) ;
           return mr ;
           }
      /* Fenster soll gezeichnet werden */
      case WM_PAINT:
           /* Eigenen PAINT Event auslîsen */
           WinPostMsg (hwnd,
                       WMPC_PAINT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Kuchenteil wird eingefÅgt */
      case WMPC_INSERTCAKE:
           {
           PCVALUE pcvalue ;
           ULONG   ul ;
           ULONG   ulRc ;

           /* Daten des neuen Elementes holen */
           pcvalue = PVOIDFROMMP (mp2) ;
           /* UnnÅtzes Flag (fÅr Listenverwaltung) ausmaskieren */
           ul = SHORT1FROMMP (mp1) ;
           ul &= ~CK_INVALIDATE ;
           /* Element in Liste an gewÅnschter Position einfÅgen */
           if   ((ulRc = CListInsert (pwnd->plistroot,
                                      ul,
                                      pcvalue)) != CKC_ERROR)
                {
                /* Wenn erfolgreich dann auf Wunsch auch neu zeichnen */
                if   (SHORT1FROMMP (mp1) & CK_INVALIDATE)
                     {
                     WinPostMsg (hwnd,
                                 WMPC_PAINT,
                                 MPVOID,
                                 MPVOID) ;
                     }
                }
           return MRFROMLONG (ulRc) ;
           }
      case WMPC_PAINT:
           {
           CVALUE cvalue ;
           HPS    hps ;
           LONG   alColors [3] ;
           LONG   lColor ;
           POINTL aptl [TXTBOX_COUNT] ;
           RECTL  rcl ;
           ULONG  ulHeight ;
           ULONG  ulI ;
           ULONG  ulJ ;
           ULONG  ulValues ;
           ULONG  ulWidth ;
           ULONG  ulXRegion ;
           ULONG  ulYRegion ;

           /* Presentation Space anlegen */
           hps = WinGetPS (hwnd) ;
           /* Fenstergrî·e ermitteln */
           WinQueryWindowRect (hwnd,
                               &rcl) ;
           /* Hintergrund des Fenster lîschen */
           WinFillRect (hps,
                        &rcl,
                        SYSCLR_DIALOGBACKGROUND) ;
           /* Rahmen um Fenster zeichnen */
           aptl[0].x = 0 ;
           aptl[0].y = 0 ;
           GpiMove (hps,
                    aptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONDARK) ;
           aptl[0].x = 0 ;
           aptl[0].y = rcl.yTop - 1 ;
           aptl[1].x = rcl.xRight - 1 ;
           aptl[1].y = rcl.yTop - 1 ;
           GpiPolyLine (hps,
                        2,
                        aptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONLIGHT) ;
           aptl[0].x = rcl.xRight - 1 ;
           aptl[0].y = 0 ;
           aptl[1].x = 0 ;
           aptl[1].y = 0 ;
           GpiPolyLine (hps,
                        2,
                        aptl) ;
           /* Farbinformationen holen */
           GpiQueryColorData (hps,
                              3,
                              alColors) ;
           /* In einer Schleife durch alle Elemente in Liste */
           for  (ulI = 0, ulValues = 0, ulWidth = 0;
                 ulI < CListCount (pwnd->plistroot);
                 ulI++)
                {
                /* Daten jedes Elementes lesen */
                CListQuery (pwnd->plistroot,
                            CK_INDEX,
                            (PVOID) ulI,
                            &cvalue) ;
                /* Werte aller Elemente addieren */
                ulValues += cvalue.ulValue ;
                /* Breite des Elementtextes ermitteln */
                GpiQueryTextBox (hps,
                                 strlen (cvalue.szTitle),
                                 cvalue.szTitle,
                                 TXTBOX_COUNT,
                                 aptl) ;
                /* Grî·te Breite ermitteln */
                if   (aptl[TXTBOX_BOTTOMRIGHT].x > ulWidth)
                     {
                     ulWidth = aptl[TXTBOX_BOTTOMRIGHT].x ;
                     }
                /* Hîhe ermitteln */
                ulHeight = aptl[TXTBOX_TOPLEFT].y ;
                }
           ulWidth += 10 ;
           /* Nochmals Fenstergrî·e ermitteln */
           WinQueryWindowRect (hwnd,
                               &rcl) ;
           /* Beschreibbare Grî·e des Fensters ermitteln (minus Legende) */
           ulXRegion = rcl.xRight - ulWidth - 4 ;
           ulYRegion = rcl.yTop - 4 ;
           /* Rahmen um Legende zeichnen */
           aptl[0].x = ulXRegion ;
           aptl[0].y = 2 ;
           GpiMove (hps,
                    aptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONLIGHT) ;
           aptl[0].x = ulXRegion ;
           aptl[0].y = rcl.yTop - 3 ;
           aptl[1].x = ulXRegion + ulWidth + 2 ;
           aptl[1].y = rcl.yTop - 3 ;
           GpiPolyLine (hps,
                        2,
                        aptl) ;
           GpiSetColor (hps,
                        SYSCLR_BUTTONDARK) ;
           aptl[0].x = ulXRegion + ulWidth + 2 ;
           aptl[0].y = 2 ;
           aptl[1].x = ulXRegion ;
           aptl[1].y = 2 ;
           GpiPolyLine (hps,
                        2,
                        aptl) ;
           /* In einer Schleife durch alle Kuchenteile */
           for  (ulI = 0, ulJ = rcl.yTop - ulHeight - 4, lColor = alColors [QCD_LCT_LOINDEX] + 1;
                 ulI < CListCount (pwnd->plistroot);
                 ulI++, ulJ -= ulHeight, lColor++)
                {
                /* Solange noch Platz in Legende */
                if   (ulJ >= 4)
                     {
                     /* Elementdaten ermitteln */
                     CListQuery (pwnd->plistroot,
                                 CK_INDEX,
                                 (PVOID) ulI,
                                 &cvalue) ;
                     /* Farbe fÅr Element in Legende ermitteln */
                     if   (lColor > alColors [QCD_LCT_HIINDEX])
                          {
                          lColor = alColors [QCD_LCT_LOINDEX] + 1 ;
                          }
                     /* NÑchste Farbe setzen */
                     GpiSetColor (hps,
                                  lColor) ;
                     /* Startposition fÅr Box setzen */
                     aptl[0].x = ulXRegion + 1 ;
                     aptl[0].y = ulJ ;
                     GpiMove (hps,
                              aptl) ;
                     aptl[0].x += 4 ;
                     aptl[0].y += 4 ;
                     /* Box zeichnen */
                     GpiBox (hps,
                             DRO_FILL,
                             aptl,
                             0,
                             0) ;
                     aptl[0].x = ulXRegion + 10 ;
                     aptl[0].y = ulJ ;
                     /* Textfarbe einstellen */
                     GpiSetColor (hps,
                                  SYSCLR_OUTPUTTEXT) ;
                     /* Text des Elementes neben Box ausgeben */
                     GpiCharStringAt (hps,
                                      aptl,
                                      strlen (cvalue.szTitle),
                                      cvalue.szTitle) ;
                     }
                }
           /* Sind Åberhaupt Elemente in Kette? */
           if   (CListCount (pwnd->plistroot))
                {
                /* Haben diese auch zeichenbare Werte? */
                if   (ulValues)
                     {
                     ulXRegion -= 4 ;
                     /* In einer Schleife durch alle Elemente */
                     for  (ulI = 0, ulJ = 0, lColor = alColors [QCD_LCT_LOINDEX] + 1;
                           ulI < CListCount (pwnd->plistroot);
                           ulI++, ulJ += (cvalue.ulValue * 360) / ulValues, lColor++)
                          {
                          /* Daten des Elementes ermitteln */
                          CListQuery (pwnd->plistroot,
                                      CK_INDEX,
                                      (PVOID) ulI,
                                      &cvalue) ;
                          /* Ausgebbarer Wert in diesem Element? */
                          if   (cvalue.ulValue)
                               {
                               /* Mitte setzen */
                               aptl[0].x = 2 + (ulXRegion / 2) ;
                               aptl[0].y = 2 + (ulYRegion / 2) ;
                               GpiMove (hps,
                                        aptl) ;
                               /* Farbe fÅr Element setzen */
                               if   (lColor > alColors [QCD_LCT_HIINDEX])
                                    {
                                    lColor = alColors [QCD_LCT_LOINDEX] + 1 ;
                                    }
                               GpiSetColor (hps,
                                            lColor) ;
                               /* Damit Farbe im KuchenstÅck ausgegeben wird */
                               GpiBeginArea (hps,
                                             BA_BOUNDARY) ;
                               /* KuchenstÅck zeichnen */
                               GpiPartialArc (hps,
                                              aptl,
                                              MAKEFIXED (min ((ulXRegion / 2), (ulYRegion / 2)), 0),
                                              MAKEFIXED (ulJ, 0),
                                              MAKEFIXED ((cvalue.ulValue * 360) / ulValues, 0)) ;
                               GpiEndArea (hps) ;
                               }
                          }
                     }
                }
           /* PS wieder wegwerfen */
           WinReleasePS (hps) ;
           return (MRESULT) FALSE ;
           }
      /* Daten eines Kuchenteiles sollen abgefragt werden */
      case WMPC_QUERYCAKE:
           /* Direkt im Aufruf retournieren */
           return MRFROMLONG (CListQuery (pwnd->plistroot,
                                          SHORT1FROMMP (mp1),
                                          (PVOID) SHORT2FROMMP (mp1),
                                          PVOIDFROMMP (mp2))) ;
           /* Daten des Control werden abgefragt */
      case WMPC_QUERYINFO:
           {
           PCINFO pcinfo ;

           /* Pointer fÅr Daten des Controls holen */
           pcinfo = PVOIDFROMMP (mp2) ;
           /* Daten dort reinkopieren */
           memcpy (pcinfo,
                   &pwnd->cinfo,
                   sizeof (CINFO)) ;
           /* Anzahl Elemente auch reinkopieren */
           pcinfo->ulCount = CListCount (pwnd->plistroot) ;
           return (MRESULT) FALSE ;
           }
      /* Kuchenelement soll entfernt werden */
      case WMPC_REMOVECAKE:
           {
           ULONG ul ;
           ULONG ulRc ;

           /* UnnÅtzes Flag (fÅr Listenverwaltung) ausmaskieren */
           ul = SHORT1FROMMP (mp1) ;
           ul &= ~CK_INVALIDATE ;
           if   ((ulRc = CListDelete (pwnd->plistroot,
                                      ul,
                                      (PVOID) SHORT2FROMMP (mp1))) != CKC_ERROR)
                {
                /* Wenn erfolgreich dann auf Wunsch auch neu zeichnen */
                if   (SHORT1FROMMP (mp1) & CK_INVALIDATE)
                     {
                     WinPostMsg (hwnd,
                                 WMPC_PAINT,
                                 MPVOID,
                                 MPVOID) ;
                     }
                }
           return MRFROMLONG (ulRc) ;
           }
      /* Daten des Controls werden geÑndert (im Moment ohne Bedeutung) */
      case WMPC_SETINFO:
           {
           PCINFO pcinfo ;

           /* Pointer auf neue Daten fÅr Control holen */
           pcinfo = PVOIDFROMMP (mp2) ;
           /* Daten reinkopieren */
           memcpy (&pwnd->cinfo,
                   pcinfo,
                   sizeof (CINFO)) ;
           /* Auf Wunsch neu zeichnen */
           if   (LONGFROMMP (mp1) & CK_INVALIDATE)
                {
                WinPostMsg (hwnd,
                            WMPC_PAINT,
                            MPVOID,
                            MPVOID) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Daten eines Kuchenteils sollen ersetzt werden */
      case WMPC_UPDATECAKE:
           {
           PCVALUE pcvalue ;
           ULONG   ul ;
           ULONG   ulRc ;

           /* Daten des neuen Elementes holen */
           pcvalue = PVOIDFROMMP (mp2) ;
           /* UnnÅtzes Flag (fÅr Listenverwaltung) ausmaskieren */
           ul = SHORT1FROMMP (mp1) ;
           ul &= ~CK_INVALIDATE ;
           /* Update der Daten durchfÅhren */
           if   ((ulRc = CListUpdate (pwnd->plistroot,
                                      ul,
                                      (PVOID) (ULONG) SHORT2FROMMP (mp1),
                                      pcvalue)) != CKC_ERROR)
                {
                /* Wenn erfolgreich dann auf Wunsch auch neu zeichnen */
                if   (SHORT1FROMMP (mp1) & CK_INVALIDATE)
                     {
                     WinPostMsg (hwnd,
                                 WMPC_PAINT,
                                 MPVOID,
                                 MPVOID) ;
                     }
                }
           return MRFROMLONG (ulRc) ;
           }
      }
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }
