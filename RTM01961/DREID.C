/* $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREID.C 1.1 1995/10/04 16:42:21 HaWi Exp $ */

/* System Kopfdateien */
#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

/* Standard Kopfdateien */
#include <stdlib.h>
#include <string.h>

/* PMCX Kopfdatei */
#include "PMCX.H"

/* Kopfdatei dieser Klasse */
#include "DREID.H"
#include "DREID.PH"

/* Auskunftsfunktion. PMCX kann hiermit die Moeglichkeiten des Controls erkennen */
BOOL EXPENTRY DREIDQuery (PUSERINFO pui)
   {
   /* Version des Kontrollelementes */
   pui->ulMajor = DREID_MAJOR ;
   pui->ulMinor = DREID_MINOR ;
   /* Autor des Kontrollelementes */
   strcpy (pui->szAuthor,
           DREID_AUTHOR) ;
   /* Klassenname des Kontrollelementes */
   strcpy (pui->szClassname,
           DREID_CLASSNAME) ;
   /* Modulname des Kontrollelementes */
   strcpy (pui->szName,
           DREID_NAME) ;
   /* Anzahl Typen (zur Zeit nur 1 erlaubt) */
   pui->cTypes = CTYPES ;
   /* Standard Groesse wenn Kontrollelement benutzt wird */
   pui->utDefined[0].cx = 50 ;
   pui->utDefined[0].cy = 12 ;
   /* Standard Stile wenn Kontrollelement benutzt wird */
   pui->utDefined[0].flStyle = DREID_BOTTOM | DREID_IN | DREID_LEFT | DREID_VISIBLE ;
   /* Refresh wenn n”tig */
   pui->utDefined[0].flOptions = PMCXOPT_REFRESH ;
   /* Erlaubte Textl„nge */
   pui->utDefined[0].cMaxText = CCHMAXPATH ;
   /* ID des Settings Dialoges */
   pui->utDefined[0].idDlg = IDD_DREID ;
   /* Optionen werden als Bits verwaltet */
   pui->utDefined[0].flStyleType = STYLETYPE_BITFLAGS ;
   /* Keine privaten Daten ben”tigt */
   pui->utDefined[0].cCtlData = 0 ;
   /* Privates Kontrollelement */
   pui->utDefined[0].ulType = UTYPE_PRIVATE ;
   /* Beschreibung des Kontrollelementes */
   strcpy (pui->utDefined[0].szDescription,
           DREID_DESCRIPTION) ;
   /* Anzahl Settings */
   pui->utDefined[0].cMasks = 9 ;
   /* Die Settings. flStyleMask ist das Bit und idStyle die Stringtable Resource ID zu diesem Setting */
   pui->utDefined[0].stMasks[0].flStyleMask = DREID_BOTTOM ;
   pui->utDefined[0].stMasks[0].idStyle = DREID_BOTTOM ;
   pui->utDefined[0].stMasks[1].flStyleMask = DREID_CENTER ;
   pui->utDefined[0].stMasks[1].idStyle = DREID_CENTER ;
   pui->utDefined[0].stMasks[2].flStyleMask = DREID_DISABLED ;
   pui->utDefined[0].stMasks[2].idStyle = DREID_DISABLED ;
   pui->utDefined[0].stMasks[3].flStyleMask = DREID_IN ;
   pui->utDefined[0].stMasks[3].idStyle = DREID_IN ;
   pui->utDefined[0].stMasks[4].flStyleMask = DREID_LEFT ;
   pui->utDefined[0].stMasks[4].idStyle = DREID_LEFT ;
   pui->utDefined[0].stMasks[5].flStyleMask = DREID_OUT ;
   pui->utDefined[0].stMasks[5].idStyle = DREID_OUT ;
   pui->utDefined[0].stMasks[6].flStyleMask = DREID_RIGHT ;
   pui->utDefined[0].stMasks[6].idStyle = DREID_RIGHT ;
   pui->utDefined[0].stMasks[7].flStyleMask = DREID_TOP ;
   pui->utDefined[0].stMasks[7].idStyle = DREID_TOP ;
   pui->utDefined[0].stMasks[8].flStyleMask = DREID_VISIBLE ;
   pui->utDefined[0].stMasks[8].idStyle = DREID_VISIBLE ;
   /* Kein Fehler */
   return TRUE ;
   }

/* Funktion zum Registrieren des Kontrollelementes */
BOOL EXPENTRY DREIDRegister (HAB hab)
   {
   CLASSINFO clsi ;

   /* WC_STATIC ist Basis */
   WinQueryClassInfo (hab,
                      WC_STATIC,
                      &clsi) ;
   /* Neue Klasse registrieren mit der Anzahl Bytes fr die Verwaltung */
   return WinRegisterClass (hab,
                            DREID_CLASSNAME,
                            DREIDWndProc,
                            CS_CLIPSIBLINGS | CS_SIZEREDRAW | CS_SYNCPAINT,
                            clsi.cbWindowData + USER_CWINDOWWORDS) ;
   }

/* Diese Dialogprozedur wird vom PMCX aufgerufen und zeigt die m”glichen Einstellungen */
MRESULT EXPENTRY DREIDStyles (HWND   hwndDlg,
                              MSG    msg,
                              MPARAM mp1,
                              MPARAM mp2)
   {
   switch (msg)
      {
      /* Der Benutzer will etwas */
      case WM_COMMAND:
           {
           CHAR       sz [32] ;
           PUSERSTYLE pust ;
           ULONG      aul [9] [2] = {{ IDR_IN,       DREID_IN       },
                                     { IDR_OUT,      DREID_OUT      },
                                     { IDR_LEFT,     DREID_LEFT     },
                                     { IDR_CENTER,   DREID_CENTER   },
                                     { IDR_RIGHT,    DREID_RIGHT    },
                                     { IDR_BOTTOM,   DREID_BOTTOM   },
                                     { IDR_TOP,      DREID_TOP      },
                                     { IDH_VISIBLE,  DREID_VISIBLE  },
                                     { IDH_DISABLED, DREID_DISABLED }} ;
           ULONG      ulI ;

           /* PMCX Kontrollstruktur aus den Instanzdaten holen */
           pust = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              /* Dialog beenden */
              case DID_CANCEL:
                   /* Das war's */
                   WinDismissDlg (hwndDlg,
                                  FALSE) ;
                   break ;
              /* Einstellungen sichern */
              case DID_OK:
                   /* Text lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDF_TEXT,
                                        pust->cText,
                                        pust->pszText) ;
                   /* Text ID des Kontrollelementes */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ID,
                                        sizeof (pust->szid),
                                        pust->szid) ;
                   /* ID des Kontrollelementes */
                   WinQueryDlgItemText (hwndDlg,
                                        IDC_ID,
                                        sizeof (sz),
                                        sz) ;
                   /* nach numerisch wandeln */
                   pust->id = atol (sz) ;
                   /* Stil vorbesetzen */
                   pust->flStyle = 0 ;
                   /* In einer Schleife alle Radiobuttons und Checkboxen auswerten und Stil setzen */
                   for  (ulI = 0; ulI < sizeof (aul) / sizeof (aul [0]); ulI++)
                        {
                        if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                               aul [ulI] [0],
                                                               BM_QUERYCHECK,
                                                               MPVOID,
                                                               MPVOID)))
                             {
                             pust->flStyle |= aul [ulI] [1] ;
                             }
                        }
                   /* Das war's */
                   WinDismissDlg (hwndDlg,
                                  TRUE) ;
                   break ;
              /* Benutzer will Bidi Einstellungen vornehmen */
              case IDP_BIDI:
                   /* Helper Funktion im PMCX aufrufen */
                   pust->pfnGetBidi (hwndDlg) ;
                   break ;
              /* Benutzer will Fonts und Farben einstellen */
              case IDP_FONTS:
                   /* Helper Funktion im PMCX aufrufen */
                   pust->pfnGetFontClr (hwndDlg) ;
                   break ;
              }
           break ;
           }
      /* Dialog wird aufgerufen */
      case WM_INITDLG:
           {
           PUSERSTYLE pust ;

           /* PMCX Kontrollstruktur holen */
           pust = PVOIDFROMMP (mp2) ;
           /* und in den Instanzdaten speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pust) ;
           /* Nur wenn gltig */
           if   (pust)
                {
                /* Text des Kontrollelementes setzen */
                WinSetDlgItemText (hwndDlg,
                                   IDF_TEXT,
                                   pust->pszText) ;
                /* Text ID des Kontrollelementes setzen */
                WinSetDlgItemText (hwndDlg,
                                   IDE_ID,
                                   pust->szid) ;
                /* ID des Kontrollelementes setzen */
                WinSetDlgItemShort (hwndDlg,
                                    IDC_ID,
                                    pust->id,
                                    FALSE) ;
                /* IN ist Default Stil */
                WinSendDlgItemMsg (hwndDlg,
                                   IDR_IN,
                                   BM_SETCHECK,
                                   MPFROMSHORT (TRUE),
                                   MPVOID) ;
                /* Left ist ebenfalls Default Stil */
                WinSendDlgItemMsg (hwndDlg,
                                   IDR_LEFT,
                                   BM_SETCHECK,
                                   MPFROMSHORT (TRUE),
                                   MPVOID) ;
                /* Bottom ist ebenfalls Default Stil */
                WinSendDlgItemMsg (hwndDlg,
                                   IDR_BOTTOM,
                                   BM_SETCHECK,
                                   MPFROMSHORT (TRUE),
                                   MPVOID) ;
                /* Visible ist ebenfalls Default Stil */
                WinSendDlgItemMsg (hwndDlg,
                                   IDH_VISIBLE,
                                   BM_SETCHECK,
                                   MPFROMSHORT (TRUE),
                                   MPVOID) ;
                /* Die Standardeinstellungen auch in der Kontrollstruktur merken */
                pust->flStyle = DREID_BOTTOM | DREID_IN | DREID_LEFT | DREID_VISIBLE ;
                }
           return (MRESULT) FALSE ;
           }
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Eigentliche Fensterprozedur des Kontrollelementes */
MRESULT EXPENTRY DREIDWndProc (HWND   hwnd,
                               MSG    msg,
                               MPARAM mp1,
                               MPARAM mp2)
   {
   PWNDDREID pwnd ;

   /* Instanzdaten dieses Kontrollelementes lesen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWW_CDATA) ;
   switch (msg)
      {
      /* Fenster wird angelegt */
      case WM_CREATE:
           {
           CHAR          szText [CCHMAXPATH] ;
           CLASSINFO     clsi ;
           MRESULT       mr ;
           PCREATESTRUCT pcrst ;
           ULONG         ulStyle ;

           /* Platz fr Instanzdaten besorgen */
           pwnd = calloc (1,
                          sizeof (WNDDREID)) ;
           /* Instanzdaten in den Window Words speichern */
           WinSetWindowPtr (hwnd,
                            QWW_CDATA,
                            pwnd) ;
           /* WC_STATIC Einstellungen holen */
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           /* Anlagewerte holen */
           pcrst = PVOIDFROMMP (mp2) ;
           /* Adresse der Original WC_STATIC Fensterprozedur merken */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           /* SS_TEXT, sonst nichts */
           ulStyle = SS_TEXT ;
           /* Ist Bottom selektiert? */
           if   (pcrst->flStyle & DREID_BOTTOM)
                {
                /* Ja, im Fensterstil gegen Original austauschen */
                ulStyle |= DT_BOTTOM ;
                }
           /* Ist Center selektiert? */
           if   (pcrst->flStyle & DREID_CENTER)
                {
                /* Ja, im Fensterstil gegen Original austauschen */
                ulStyle |= DT_CENTER ;
                }
           /* Ist Disabled selektiert? */
           if   (pcrst->flStyle & DREID_DISABLED)
                {
                /* Ja, im Fensterstil gegen Original austauschen */
                ulStyle |= WS_DISABLED ;
                }
           /* Ist IN selektiert? */
           if   (pcrst->flStyle & DREID_IN)
                {
                /* Ja, im Kontrollelement merken */
                pwnd->ulStyle = DREID_IN ;
                }
           /* Ist Left selektiert? */
           if   (pcrst->flStyle & DREID_LEFT)
                {
                /* Ja, im Fensterstil gegen Original austauschen und im Kontrollelement merken */
                ulStyle |= DT_LEFT ;
                pwnd->ulSide = DREID_LEFT ;
                }
           /* Ist Out selektiert? */
           if   (pcrst->flStyle & DREID_OUT)
                {
                /* Ja, im Kontrollelement merken */
                pwnd->ulStyle = DREID_OUT ;
                }
           /* Ist Right selektiert? */
           if   (pcrst->flStyle & DREID_RIGHT)
                {
                /* Ja, im Fensterstil gegen Original austauschen und im Kontrollelement merken */
                ulStyle |= DT_RIGHT ;
                pwnd->ulSide = DREID_RIGHT ;
                }
           /* Ist Top selektiert? */
           if   (pcrst->flStyle & DREID_TOP)
                {
                /* Ja, im Fensterstil gegen Original austauschen */
                ulStyle |= DT_TOP ;
                }
           /* Ist Visible selektiert? */
           if   (pcrst->flStyle & DREID_VISIBLE)
                {
                /* Ja, im Fensterstil gegen Original austauschen */
                ulStyle |= WS_VISIBLE ;
                }
           /* Neue Stile setzen */
           pcrst->flStyle = ulStyle ;
           /* Originalfensterprozedur der WC_STATIC Klasse aufrufen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Stil dauerhaft austauschen */
           WinSetWindowULong (hwnd,
                              QWL_STYLE,
                              pcrst->flStyle) ;
           /* Liegt ein Text an? */
           if   (*pcrst->pszText)
                {
                /* Ja, ist Left eingestellt? */
                if   (pwnd->ulSide == DREID_LEFT)
                     {
                     /* Ja, ist bereits an erster Stelle ein Blank? */
                     if   (*pcrst->pszText != ' ')
                          {
                          /* Nein, dann Blank davor setzen */
                          strcpy (szText,
                                  " ") ;
                          strcat (szText,
                                  pcrst->pszText) ;
                          /* Neuen Text ausgeben */
                          WinSetWindowText (hwnd,
                                            szText) ;
                          }
                     }
                else
                     {
                     /* Ist Right eingestellt? */
                     if   (pwnd->ulSide == DREID_RIGHT)
                          {
                          /* Ja, ist bereits am Ende ein Blank? */
                          if   (*(pcrst->pszText + strlen (pcrst->pszText) - 1) != ' ')
                               {
                               /* Nein, dann Blank anh„ngen */
                               strcpy (szText,
                                       pcrst->pszText) ;
                               strcat (szText,
                                       " ") ;
                               /* Neuen Text ausgeben */
                               WinSetWindowText (hwnd,
                                                 szText) ;
                               }
                          }
                     }
                }
           /* Returnwert der Originalfensterklasse zurckliefern */
           return mr ;
           }
      /* Kontrollelement wird gel”scht */
      case WM_DESTROY:
           {
           MRESULT mr ;

           /* Ergebnis der Originalfensterprozedur holen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Instanzdaten wieder freigeben */
           free (pwnd) ;
           /* Returnwert der Originalfensterklasse zurckliefern */
           return mr ;
           }
      /* Das Kontrollelement soll gezeichnet werden */
      case WM_PAINT:
           /* Das machen wir nicht direkt, sondern in einer privaten Message. So hat der PM etwas Zeit */
           WinPostMsg (hwnd,
                       WMP_PAINT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Presentation Parameter des Kontrollelementes wurden ver„ndert */
      case WM_SETWINDOWPARAMS:
           {
           CHAR       szText [CCHMAXPATH] ;
           PWNDPARAMS pwprm ;

           /* Presentation Parameter holen */
           pwprm = PVOIDFROMMP (mp1) ;
           /* Wurde der Text ver„ndert */
           if   (pwprm->fsStatus & WPM_TEXT)
                {
                /* Ja, ist berhaupt etwas drin? */
                if   (*pwprm->pszText)
                     {
                     /* Ja, ist Left eingestellt? */
                     if   (pwnd->ulSide == DREID_LEFT)
                          {
                          /* Ja, ist bereits an erster Stelle ein Blank? */
                          if   (*pwprm->pszText != ' ')
                               {
                               /* Nein, dann Blank davor setzen */
                               strcpy (szText,
                                       " ") ;
                               strcat (szText,
                                       pwprm->pszText) ;
                               /* Neuen Text ausgeben */
                               WinSetWindowText (hwnd,
                                                 szText) ;
                               return (MRESULT) FALSE ;
                               }
                          }
                     /* Ist Right eingestellt? */
                     if   (pwnd->ulSide == DREID_RIGHT)
                          {
                          /* Ja, ist bereits am Ende ein Blank? */
                          if   (*(pwprm->pszText + strlen (pwprm->pszText) - 1) != ' ')
                               {
                               /* Nein, dann Blank anh„ngen */
                               strcpy (szText,
                                       pwprm->pszText) ;
                               strcat (szText,
                                       " ") ;
                               /* Neuen Text ausgeben */
                               WinSetWindowText (hwnd,
                                                 szText) ;
                               return (MRESULT) FALSE ;
                               }
                          }
                     }
                }
           break ;
           }
      /* Privater PAINT Event. Hier wird gemalt */
      case WMP_PAINT:
           {
           HPS    hps ;
           POINTL ptl ;
           RECTL  rcl ;
           ULONG  ulI ;
           ULONG  ulJ ;

           /* Ist IN eingestellt */
           if   (pwnd->ulStyle == DREID_IN)
                {
                /* Ja, entsprechende Farben verwenden */
                ulI = SYSCLR_BUTTONDARK ;
                ulJ = SYSCLR_BUTTONLIGHT ;
                }
           else
                {
                /* Nein, also fr OUT entsprechende Farben verwenden */
                ulI = SYSCLR_BUTTONLIGHT ;
                ulJ = SYSCLR_BUTTONDARK ;
              }
           /* Gr”áe des Fensters ermitteln */
           WinQueryWindowRect (hwnd,
                               &rcl) ;
           /* Breite und H”he um 1 vermindern */
           rcl.xRight-- ;
           rcl.yTop-- ;
           /* Presentation Space zum Zeichnen besorgen */
           hps = WinGetPS (hwnd) ;
           /* Wir fangen unten links an */
           ptl.x = 0 ;
           ptl.y = 0 ;
           /* Position setzen */
           GpiMove (hps,
                    &ptl) ;
           /* Eingestellte Farbe verwenden */
           GpiSetColor (hps,
                        ulI) ;
           /* und nach oben eine Linie ziehen */
           ptl.x = 0 ;
           ptl.y = rcl.yTop ;
           GpiLine (hps,
                    &ptl) ;
           /* Dann nach rechts oben Linie ziehen */
           ptl.x = rcl.xRight ;
           ptl.y = rcl.yTop ;
           GpiLine (hps,
                    &ptl) ;
           /* Andere Farbe jetzt */
           GpiSetColor (hps,
                        ulJ) ;
           /* nach unten rechts Linie ziehen */
           ptl.x = rcl.xRight ;
           ptl.y = 0 ;
           GpiLine (hps,
                    &ptl) ;
           /* und zum Ausgangspunkt zurck */
           ptl.x = 0 ;
           ptl.y = 0 ;
           GpiLine (hps,
                    &ptl) ;
           /* Presentation Space wieder freigeben */
           WinReleasePS (hps) ;
           return (MRESULT) FALSE ;
           }
      }
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }


