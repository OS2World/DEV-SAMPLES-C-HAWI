/* $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/CLNDR.C 1.1 1996/02/28 17:03:55 HaWi Exp $ */

/* OS/ Kopfdateien */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C Kopfdateien */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Oeffentliche und private Kopfdatei des Kalenders */
#include "CLNDR.H"
#include "CLNDR.PH"

/* Funktion zum Registrieren des Kalender Controls */
BOOL EXPENTRY ARegisterClass (HAB hab)
   {
   CLASSINFO clsi ;

   /* Da von WC_STATIC geerbt werden soll, dieses abfragen */
   WinQueryClassInfo (hab,
                      WC_STATIC,
                      &clsi) ;
   /* Die integrierte STATIC Klasse direkt mit registrieren */
   WinRegisterClass (hab,
                     WCP_CLNDRSTATIC,
                     StaticWndProc,
                     0,
                     clsi.cbWindowData + sizeof (PVOID)) ;
   /* Eigentliches Kalender Control registrieren */
   return WinRegisterClass (hab,
                            AXK_CLASSNAME,
                            AWndProc,
                            0,
                            clsi.cbWindowData + sizeof (PVOID)) ;
   }

/* Fensterprozedur des Kalender Controls */
MRESULT EXPENTRY AWndProc (HWND   hwnd,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   PWND pwnd ;

   /* Instanzdaten aus Window Words holen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      /* Neues Kalender Control wird erzeugt */
      case WM_CREATE:
           {
           CHAR          sz [128] ;
           CLASSINFO     clsi ;
           PCREATESTRUCT pcrst ;
           PSZ           psz ;
           time_t        timeCurrent ;
           struct tm*    ptm ;
           ULONG         ul ;
           ULONG         ulI ;

           /* Platz fuer Instanzdaten allokieren */
           pwnd = calloc (1,
                          sizeof (WND)) ;
           /* Pointer auf Instanzdaten in Window Words ablegen */
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           /* Grunddaten der WC_STATIC Klasse besorgen */
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           /* Original Fensterprozedur der WC_STATIC merken */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           /* CREATE Daten besorgen */
           pcrst = PVOIDFROMMP (mp2) ;
           /* Ohne Angaben startet der Kalender am Montag */
           pwnd->ulStyle = AK_STYLEMONDAY ;
           /* Gibt es in den Ressourcen CTLDATA Daten? */
           psz = pcrst->pCtlData ;
           if   (psz)
                {
                /* Ja, an erster Stelle steht der Wochentag an dem Kalender starten soll */
                pwnd->ulStyle = *psz - '0' ;
                /* Hier folgen jetzt 7 Abkuerzugen fuer die Tage und 12 Namen fuer die Monate */
                sscanf (psz + 1,
                        "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
                        pwnd->aszDay [0],
                        pwnd->aszDay [1],
                        pwnd->aszDay [2],
                        pwnd->aszDay [3],
                        pwnd->aszDay [4],
                        pwnd->aszDay [5],
                        pwnd->aszDay [6],
                        pwnd->aszMonth [0],
                        pwnd->aszMonth [1],
                        pwnd->aszMonth [2],
                        pwnd->aszMonth [3],
                        pwnd->aszMonth [4],
                        pwnd->aszMonth [5],
                        pwnd->aszMonth [6],
                        pwnd->aszMonth [7],
                        pwnd->aszMonth [8],
                        pwnd->aszMonth [9],
                        pwnd->aszMonth [10],
                        pwnd->aszMonth [11]) ;
                }
           /* CREATE Daten nicht (!) an WC_STATIC durchreichen */
           pcrst->pCtlData = NULL ;
           /* Aktuelle Zeit besorgen */
           time (&timeCurrent) ;
           ptm = localtime (&timeCurrent) ;
           /* Aktuelle Zeit in den Instanzdaten speichern */
           pwnd->tm = *ptm ;
           /* Den aktuellen Wochentag, Monat und Jahr ebenfalls speichern */
           pwnd->ulDay = pwnd->tm.tm_mday ;
           pwnd->ulMonth = pwnd->tm.tm_mon ;
           pwnd->ulYear = pwnd->tm.tm_year ;
           /* Ein Kalender startet immer mit dem 1. Tag */
           pwnd->tm.tm_mday = 1 ;
           /* Ist der Font und die Fontgroesse bereits durch die Ressourcen bestimmt worden? */
           WinQueryPresParam (hwnd,
                              PP_FONTNAMESIZE,
                              0,
                              NULL,
                              sizeof (sz),
                              sz,
                              QPF_NOINHERIT) ;
           /* Das Fenster fuer den Monat und das Jahr anlegen */
           pwnd->hwndOutput = WinCreateWindow (hwnd,
                                               WCP_CLNDRSTATIC,
                                               "",
                                               DT_CENTER | DT_VCENTER | SS_TEXT | WS_SYNCPAINT | WS_VISIBLE,
                                               0,
                                               0,
                                               0,
                                               0,
                                               hwnd,
                                               HWND_TOP,
                                               FID_OUTPUT,
                                               NULL,
                                               NULL) ;
           /* Hintergrund ist der normale Dialog Hintergrund */
           ul = SYSCLR_DIALOGBACKGROUND ;
           WinSetPresParam (pwnd->hwndOutput,
                            PP_BACKGROUNDCOLORINDEX,
                            sizeof (ul),
                            &ul) ;
           /* Waren Font und Fontgroesse durch die Ressourcen bereits bestimmt? */
           if   (*sz)
                {
                /* Ja, dann diese auch fuer dieses Control anwenden */
                WinSetPresParam (pwnd->hwndOutput,
                                 PP_FONTNAMESIZE,
                                 strlen (sz) + 1,
                                 sz) ;
                }
           /* 3D Look dieses Kontrollelementes steht nach aussen */
           WinSendMsg (pwnd->hwndOutput,
                       WMPAX_SETLOOK,
                       MPFROMLONG (AK_LOOKOUT),
                       MPVOID) ;
           /* In einer Schleife durch alle 7 Wochentage */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndHeader) / sizeof (pwnd->ahwndHeader [0]); ulI++)
                {
                /* Fenster fuer die Wochentage anlegen */
                pwnd->ahwndHeader [ulI] = WinCreateWindow (hwnd,
                                                           WCP_CLNDRSTATIC,
                                                           pwnd->aszDay [ulI],
                                                           DT_CENTER | DT_VCENTER | SS_TEXT | WS_SYNCPAINT | WS_VISIBLE,
                                                           0,
                                                           0,
                                                           0,
                                                           0,
                                                           hwnd,
                                                           HWND_TOP,
                                                           FID_HEADER1 + ulI,
                                                           NULL,
                                                           NULL) ;
                /* Hintergrund ist der normale Dialog Hintergrund */
                WinSetPresParam (pwnd->ahwndHeader [ulI],
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (ul),
                                 &ul) ;
                /* Waren Font und Fontgroesse durch die Ressourcen bereits bestimmt? */
                if   (*sz)
                     {
                     /* Ja, dann diese auch fuer dieses Control anwenden */
                     WinSetPresParam (pwnd->ahwndHeader [ulI],
                                      PP_FONTNAMESIZE,
                                      strlen (sz) + 1,
                                      sz) ;
                     }
                /* 3D Look dieses Kontrollelementes steht nach aussen */
                WinSendMsg (pwnd->ahwndHeader [ulI],
                            WMPAX_SETLOOK,
                            MPFROMLONG (AK_LOOKOUT),
                            MPVOID) ;
                }
           /* In einer Schleife durch alle 42 (maximal 31 Tage koennen auf 6 Wochen in einem Monat verteilt sein) Monatstage */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndDay) / sizeof (pwnd->ahwndDay [0]); ulI++)
                {
                /* Fenster fuer die Monatstage anlegen */
                pwnd->ahwndDay [ulI] = WinCreateWindow (hwnd,
                                                        WCP_CLNDRSTATIC,
                                                        "",
                                                        DT_CENTER | DT_VCENTER | SS_TEXT | WS_SYNCPAINT | WS_VISIBLE,
                                                        0,
                                                        0,
                                                        0,
                                                        0,
                                                        hwnd,
                                                        HWND_TOP,
                                                        FID_DAY1 + ulI,
                                                        NULL,
                                                        NULL) ;
                /* Hintergrund ist der normale Dialog Hintergrund */
                WinSetPresParam (pwnd->ahwndDay [ulI],
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (ul),
                                 &ul) ;
                /* Waren Font und Fontgroesse durch die Ressourcen bereits bestimmt? */
                if   (*sz)
                     {
                     /* Ja, dann diese auch fuer dieses Control anwenden */
                     WinSetPresParam (pwnd->ahwndDay [ulI],
                                      PP_FONTNAMESIZE,
                                      strlen (sz) + 1,
                                      sz) ;
                     }
                /* 3D Look dieses Kontrollelementes ist nicht gesetzt */
                WinSendMsg (pwnd->ahwndDay [ulI],
                            WMPAX_SETLOOK,
                            MPFROMLONG (AK_LOOKIGNORE),
                            MPVOID) ;
                }
           /* Initialisierung der Listenverwaltung fuer Termine die der Kalender selbst verwalten soll */
           pwnd->plistroot = TListInit (sizeof (CDATE),
                                        0,
                                        0) ;
           break ;
           }
      /* Der Kalender wird wieder geschlossen */
      case WM_DESTROY:
           {
           MRESULT mr ;
           ULONG   ulI ;

           /* Original Fensterprozedur der WC_STATIC aufrufen */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Fenster der 42 Monatstage loeschen */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndDay) / sizeof (pwnd->ahwndDay [0]); ulI++)
                {
                WinDestroyWindow (pwnd->ahwndDay [ulI]) ;
                }
           /* Fenster der 7 Wochentage loeschen */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndHeader) / sizeof (pwnd->ahwndHeader [0]); ulI++)
                {
                WinDestroyWindow (pwnd->ahwndHeader [ulI]) ;
                }
           /* Fenster fuer Monat und Jahr loeschen */
           WinDestroyWindow (pwnd->hwndOutput) ;
           /* Listenverwaltung loeschen */
           TListDestroy (pwnd->plistroot) ;
           /* Speicher der Instanzdaten wieder freigeben */
           free (pwnd) ;
           return mr ;
           }
      case WM_PAINT:
           /* Verzoegertes PAINT */
           WinPostMsg (hwnd,
                       WMPAX_PAINT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Ein Monatstag meldet den Klick mit der Maus */
      case WMPAX_CONTROL:
           /* Diesen Tag mit Monat und Jahr koppeln und an PARENT (Dialog) melden */
           WinPostMsg (WinQueryWindow (hwnd,
                                       QW_OWNER),
                       WM_CONTROL,
                       mp1,
                       MPFROMSH2CH (pwnd->tm.tm_year + 1900, pwnd->tm.tm_mon + 1, LONGFROMMP (mp2))) ;
           return (MRESULT) FALSE ;
      /* Ein Termin wird dem Kalender zur Verwaltung uebergeben */
      case WMPAX_INSERTAPP:
           {
           CDATE cdateTemp ;

           /* Die Daten aus den Message Parametern rauspicken */
           cdateTemp.day = CHAR4FROMMP (mp2) ;
           cdateTemp.month = CHAR3FROMMP (mp2) ;
           cdateTemp.year = SHORT1FROMMP (mp2) ;
           /* Einen neuen Termin in die Listenverwaltung anhaengen */
           TListInsert (pwnd->plistroot,
                        TXK_LAST,
                        &cdateTemp) ;
           /* Soll der Kalender direkt invalidiert werden? */
           if   (LONGFROMMP (mp1) == AK_INSERTINVALIDATE)
                {
                /* Ja, REDRAW ausloesen */
                WinPostMsg (hwnd,
                            WMPAX_REDRAW,
                            MPVOID,
                            MPVOID) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Kalender soll einfach invalidiert werden */
      case WMPAX_INVALIDATE:
           /* REDRAW ausloesen */
           WinPostMsg (hwnd,
                       WMPAX_REDRAW,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Ein eigener PAINT */
      case WMPAX_PAINT:
           {
           HPS   hps ;
           RECTL rcl ;
           SWP   swp ;
           ULONG ulI ;
           ULONG ulJ ;

           /* Presentation Space fuer Kalender besorgen */
           hps = WinGetPS (hwnd) ;
           /* Groesse des Kalender Controls ermitteln */
           WinQueryWindowRect (hwnd,
                               &rcl) ;
           /* Inhalt mit Dialog Hintergrundfarbe fuellen */
           WinFillRect (hps,
                        &rcl,
                        SYSCLR_DIALOGBACKGROUND) ;
           /* Einen 3D Rahmen drumherum zeichnen */
           WinDrawBorder (hps,
                          &rcl,
                          1,
                          1,
                          0,
                          0,
                          DB_DEPRESSED) ;
           /* Presentation Space wieder freigeben */
           WinReleasePS (hps) ;
           /* In einer Schleife durch alle sieben Wochentage */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndHeader) / sizeof (pwnd->ahwndHeader [0]); ulI++)
                {
                /* Wochentag neu positionieren */
                WinSetWindowPos (pwnd->ahwndHeader [ulI],
                                 HWND_TOP,
                                 rcl.xLeft + 4 + (ulI * ((rcl.xRight - rcl.xLeft - 8) / 7)),
                                 ((rcl.yTop - rcl.yBottom) / 8) * 6 + 2,
                                 (rcl.xRight - rcl.xLeft - 8) / 7,
                                 (rcl.yTop - rcl.yBottom) / 8 - 4,
                                 SWP_MOVE | SWP_SIZE) ;
                /* Wochentag zeichnen lassen */
                WinInvalidateRect (pwnd->ahwndHeader [ulI],
                                   NULL,
                                   FALSE) ;
                }
           /* Position des letzten Wochentages ermitteln */
           WinQueryWindowPos (pwnd->ahwndHeader [ulI - 1],
                              &swp) ;
           /* Ueberschrift darf nicht laenger sein als das der letzte Wochentag (kann sonst durch Rundungsfehler schief werden */
           WinSetWindowPos (pwnd->hwndOutput,
                            HWND_TOP,
                            rcl.xLeft + 4,
                            ((rcl.yTop - rcl.yBottom) / 8) * 7 + 2,
                            swp.x + swp.cx - 4,
                            (rcl.yTop - rcl.yBottom) / 8 - 4,
                            SWP_MOVE | SWP_SIZE) ;
           /* Ueberschrift neu zeichnen lassen */
           WinInvalidateRect (pwnd->hwndOutput,
                              NULL,
                              FALSE) ;
           /* In einer Schleife durch 6 moegliche Wochen eines Monats */
           for  (ulI = 0; ulI < 6; ulI++)
                {
                /* In einer Schleife von 7 Tagen je moeglicher Woche */
                for  (ulJ = 0; ulJ < 7; ulJ++)
                     {
                     /* Monatstag neu positionieren */
                     WinSetWindowPos (pwnd->ahwndDay [(ulI * 7) + ulJ],
                                      HWND_TOP,
                                      rcl.xLeft + 4 + (ulJ * ((rcl.xRight - rcl.xLeft - 8) / 7)),
                                      ((rcl.yTop - rcl.yBottom) / 8) * (5 - ulI) + 2,
                                      (rcl.xRight - rcl.xLeft - 8) / 7,
                                      (rcl.yTop - rcl.yBottom) / 8 - 4,
                                      SWP_MOVE | SWP_SIZE) ;
                     /* Monatstag zeichnen lassen */
                     WinInvalidateRect (pwnd->ahwndDay [(ulI * 7) + ulJ],
                                        NULL,
                                        FALSE) ;
                     }
                }
           /* REDRAW ausloesen */
           WinPostMsg (hwnd,
                       WMPAX_REDRAW,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Auskuntsfunktion liefert aktuell sichtbaren Monat und Tag */
      case WMPAX_QUERYDATE:
           return MRFROMSH2CH (pwnd->tm.tm_year + 1900, pwnd->tm.tm_mon + 1, 0) ;
      /* Hier wird der Inhalt der Kalenderfenster berechnet und ausgegeben */
      case WMPAX_REDRAW:
           {
           CDATE      cdateTemp ;
           CHAR       sz [24] ;
           time_t     timeCurrent ;
           struct tm* ptm ;
           ULONG      ul ;
           ULONG      ulI ;
           ULONG      ulJ ;

           /* Die gespeicherte Zeit kalkulieren lassen */
           timeCurrent = mktime (&pwnd->tm) ;
           ptm = localtime (&timeCurrent) ;
           /* und wieder zwischenspeichern */
           pwnd->tm = *ptm ;
           /* Bezeichnung des Monats mit der Jahresangabe zusammenbauen */
           sprintf (sz,
                    "%s %04u",
                    pwnd->aszMonth [pwnd->tm.tm_mon],
                    (ULONG) pwnd->tm.tm_year + 1900) ;
           /* Dies in der Ueberschrift Fenster ausgeben */
           WinSetWindowText (pwnd->hwndOutput,
                             sz) ;
           /* In einer Schleife durch alle 42 Monatstage */
           for  (ulI = 0; ulI < sizeof (pwnd->ahwndDay) / sizeof (pwnd->ahwndDay [0]); ulI++)
                {
                /* Den 3D Look abschalten */
                WinSendMsg (pwnd->ahwndDay [ulI],
                            WMPAX_SETLOOK,
                            MPFROMLONG (AK_LOOKIGNORE),
                            MPVOID) ;
                /* Den Inhalt loeschen */
                WinSetWindowText (pwnd->ahwndDay [ulI],
                                  "") ;
                }
           /* An welchem Wochentag soll der Kalender starten */
           switch (pwnd->ulStyle)
              {
              /* Sonntag */
              case AK_STYLESUNDAY:
                   /* Zeitfunktionen sind immer Sonntag basierend, deshalb keine Korrektur */
                   ul = pwnd->tm.tm_wday ;
                   break ;
              /* Montag */
              case AK_STYLEMONDAY:
                   {
                   ULONG aul [] = { 6,
                                    0,
                                    1,
                                    2,
                                    3,
                                    4,
                                    5 } ;

                   /* Ermittlung mit welcher Korrektur von links der erste Monatstag beginnen soll.
                      So muss ein Sonntag bei Start am Montag ganz hinten (6) stehen */
                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              /* Dienstag */
              case AK_STYLETUESDAY:
                   {
                   ULONG aul [] = { 5,
                                    6,
                                    0,
                                    1,
                                    2,
                                    3,
                                    4 } ;

                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              /* Mittwoch */
              case AK_STYLEWEDNESDAY:
                   {
                   ULONG aul [] = { 4,
                                    5,
                                    6,
                                    0,
                                    1,
                                    2,
                                    3 } ;

                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              /* Donnerstag */
              case AK_STYLETHURSDAY:
                   {
                   ULONG aul [] = { 3,
                                    4,
                                    5,
                                    6,
                                    0,
                                    1,
                                    2 } ;

                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              /* Freitag */
              case AK_STYLEFRIDAY:
                   {
                   ULONG aul [] = { 2,
                                    3,
                                    4,
                                    5,
                                    6,
                                    0,
                                    1 } ;

                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              /* Samstag */
              case AK_STYLESATURDAY:
                   {
                   ULONG aul [] = { 1,
                                    2,
                                    3,
                                    4,
                                    5,
                                    6,
                                    0 } ;

                   ul = aul [pwnd->tm.tm_wday] ;
                   break ;
                   }
              }
           /* Es gibt maximal 31 Tage pro Monat */
           for  (ulI = 0; ulI < 31; ulI++)
                {
                /* Am 28. Februar muss ein wenig geprueft werden */
                if   ((ulI == 28) &&
                      (pwnd->tm.tm_mon == 1))
                     {
                     /* Ist das Jahr nicht (!) durch 4 teilbar? */
                     if   (((pwnd->tm.tm_year + 1900) % 4) != 0)
                          {
                          /* Dann endet der Februar am 28. */
                          break ;
                          }
                     /* Ist das Jahr durch 100 teilbar? */
                     if   (((pwnd->tm.tm_year + 1900) % 100) == 0)
                          {
                          /* Aber nicht durch 400 teilbar? */
                          if   (((pwnd->tm.tm_year + 1900) % 400) != 0)
                               {
                               /* Dann endet der Februar auch in diesem Fall am 28. */
                               break ;
                               }
                          }
                     }
                /* Ist es der 29. Februar? */
                if   ((ulI == 29) &&
                      (pwnd->tm.tm_mon == 1))
                     {
                     /* Ja, dann ist der Februar auf jeden Fall jetzt zu Ende */
                     break ;
                     }
                /* Diese Monate haben alle 30 Monatstage */
                if   ((ulI == 30) &&
                      ((pwnd->tm.tm_mon == 1) ||
                       (pwnd->tm.tm_mon == 3) ||
                       (pwnd->tm.tm_mon == 5) ||
                       (pwnd->tm.tm_mon == 8) ||
                       (pwnd->tm.tm_mon == 10)))
                     {
                     /* Also ist hier Schluss */
                     break ;
                     }
                /* Rennen wir gerade ueber den aktuellen Tag? */
                if   (((ulI + 1) == pwnd->ulDay) &&
                      (pwnd->tm.tm_mon == pwnd->ulMonth) &&
                      (pwnd->tm.tm_year == pwnd->ulYear))
                     {
                     /* Ja, dieser erhaelt einen 3D Look nach aussen */
                     WinSendMsg (pwnd->ahwndDay [ul + ulI],
                                 WMPAX_SETLOOK,
                                 MPFROMLONG (AK_LOOKOUT),
                                 MPVOID) ;
                     }
                /* Nummer des Tages in Control ausgeben. Als Offset wird der Wochentag, mit dem der Kalender startet, dazu addiert */
                WinSetDlgItemShort (hwnd,
                                    FID_DAY1 + ul + ulI,
                                    ulI + 1,
                                    FALSE) ;
                }
           /* Enthaelt unsere Listenverwaltung einige Termine die angezeigt werden wollen */
           ulJ = TListCount (pwnd->plistroot) ;
           /* In einer Schleife durch alle gespeicherten Termine */
           for  (ulI = 0; ulI < ulJ; ulI++)
                {
                /* Eintrag an Position besorgen */
                TListQuery (pwnd->plistroot,
                            TXK_INDEX,
                            (PVOID) ulI,
                            &cdateTemp) ;
                /* Ist das Jahr == 0 (Jedes Jahr)? */
                if   (cdateTemp.year == 0)
                     {
                     /* Stimmt der Monat mit dem Termin ueberein */
                     if   (cdateTemp.month == pwnd->tm.tm_mon + 1)
                          {
                          /* Der aktuelle Tag hat eine eigene Darstellung, also nicht ueberschreiben */
                          if   (pwnd->tm.tm_mday != cdateTemp.day)
                               {
                               /* Dann bekommt der Tag einen 3D Look, allerdings nach innen */
                               WinSendMsg (pwnd->ahwndDay [ul + cdateTemp.day - 1],
                                           WMPAX_SETLOOK,
                                           MPFROMLONG (AK_LOOKIN),
                                           MPVOID) ;
                               }
                          }
                     }
                else
                     {
                     /* Stimmt das Jahr und der Monat mit dem Termin ueberein */
                     if   ((cdateTemp.year == pwnd->tm.tm_year + 1900) &&
                           (cdateTemp.month == pwnd->tm.tm_mon + 1))
                          {
                          /* Stimmt der Tag mit dem Termin ueberein? */
                          if   (pwnd->tm.tm_mday != cdateTemp.day)
                               {
                               /* Der aktuelle Tag hat eine eigene Darstellung, also nicht ueberschreiben */
                               WinSendMsg (pwnd->ahwndDay [ul + cdateTemp.day - 1],
                                           WMPAX_SETLOOK,
                                           MPFROMLONG (AK_LOOKIN),
                                           MPVOID) ;
                               }
                          }
                     }
                }
           return (MRESULT) FALSE ;
           }
      /* Das Kalender Control wird von aussen auf einen anderen Termin dirigiert */
      case WMPAX_SETDATE:
           {
           BOOL f ;

           f = TRUE ;
           /* Was soll gemacht werden? */
           switch (LONGFROMMP (mp1))
              {
              /* Naechster Monat */
              case AK_DATENEXTMONTH:
                   /* Ist Monat kleiner Dezember? */
                   if   (pwnd->tm.tm_mon < 11)
                        {
                        /* Einfach um 1 erhoehen */
                        pwnd->tm.tm_mon++ ;
                        }
                   else
                        {
                        /* Im Dezember (wenn Jahr kleiner 2037) */
                        if   (pwnd->tm.tm_year < 137)
                             {
                             /* Das Jahr erhoehen und Monat auf Januar stellen */
                             pwnd->tm.tm_year++ ;
                             pwnd->tm.tm_mon = 0 ;
                             }
                        }
                   break ;
              /* Naechstes Jahr */
              case AK_DATENEXTYEAR:
                   /* Wenn Jahr noch nicht auf 2037 */
                   if   (pwnd->tm.tm_year < 137)
                        {
                        /* Jahr erhoehen */
                        pwnd->tm.tm_year++ ;
                        }
                   break ;
              /* Vorheriger Monat */
              case AK_DATEPREVMONTH:
                   /* Wenn Monat noch nicht auf Januar */
                   if   (pwnd->tm.tm_mon > 0)
                        {
                        /* Einfach um 1 vermindern */
                        pwnd->tm.tm_mon-- ;
                        }
                   else
                        {
                        /* Im Januar wenn Jahr groesser 1970 */
                        if   (pwnd->tm.tm_year > 70)
                             {
                             /* Jahr vermindern und Monat auf Dezember setzen */
                             pwnd->tm.tm_year-- ;
                             pwnd->tm.tm_mon = 11 ;
                             }
                        }
                   break ;
              /* Vorheriges Jahr */
              case AK_DATEPREVYEAR:
                   /* Wenn Jahr noch nicht auf 1970 */
                   if   (pwnd->tm.tm_year > 70)
                        {
                        /* Jahr vermindern */
                        pwnd->tm.tm_year-- ;
                        }
                   break ;
              /* Falsche Anweisung */
              default:
                   f = FALSE ;
                   break ;
              }
           /* Wenn Dateum veraendert wurde */
           if   (f)
                {
                /* REDRAW ausloesen */
                WinPostMsg (hwnd,
                            WMPAX_REDRAW,
                            MPVOID,
                            MPVOID) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der WC_STATIC Standardverarbeitung */
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }

/* Fensterprozedur fuer die 3D Kontrollelemente innerhalb des Kalenders */
MRESULT EXPENTRY StaticWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   PWNDSTATIC pwnd ;

   /* Instanzdaten aus Window Words holen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      /* Die Maus wurde geklickt */
      case WM_BUTTON1DBLCLK:
      case WM_BUTTON1UP:
           {
           CHAR sz [3] ;

           /* Den Text des Kontrollelementes lesen */
           WinQueryWindowText (hwnd,
                               sizeof (sz),
                               sz) ;
           /* An das Kalender Control den Klick und den betreffenden Tag melden */
           WinPostMsg (WinQueryWindow (hwnd,
                                       QW_OWNER),
                       WMPAX_CONTROL,
                       MPFROM2SHORT (WinQueryWindowUShort (WinQueryWindow (hwnd,
                                                                           QW_OWNER),
                                                           QWS_ID), (msg == WM_BUTTON1DBLCLK) ? AXN_ENTER : AXN_SELECT),
                       MPFROMLONG (atol (sz))) ;
           }
           break ;
      /* Ein Fenster dieser 3D STATIC Klasse wird erzeugt */
      case WM_CREATE:
           {
           CLASSINFO clsi ;

           /* Platz fuer Instanzdaten schaffen */
           pwnd = calloc (1,
                          sizeof (WNDSTATIC)) ;
           /* Pointer auf Instanzdaten in Window Words des Controls ablegen */
           WinSetWindowPtr (hwnd,
                           QWL_USER,
                            pwnd) ;
           /* Originaldaten der WC_STATIC besorgen */
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           /* Original Fensterprozedur der WC_STATIC merken */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           break ;
           }
      /* Ein 3D STATIC wird geloescht */
      case WM_DESTROY:
           {
           MRESULT mr ;

           /* Original Verarbeitung der WC_STATIC aktivieren */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Instanzdaten freigeben */
           free (pwnd) ;
           return mr ;
           }
      case WM_PAINT:
           /* Verzeogerten PAINT ausloesen */
           WinPostMsg (hwnd,
                       WMPAX_PAINT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Verzoegerten PAINT hier durchfueheren */
      case WMPAX_PAINT:
           {
           HPS   hps ;
           RECTL rcl ;

           /* Soll ueberhaupt õD gezeichnet werden? */
           if   (pwnd->ulLook != AK_LOOKIGNORE)
                {
                /* Ja, Groesse des Fensters ermitteln */
                WinQueryWindowRect (hwnd,
                                    &rcl) ;
                /* oben und unten 1 fuer Rahmen abziehen */
                rcl.xRight-- ;
                rcl.yTop-- ;
                /* Presentation Space besorgen */
                hps = WinGetPS (hwnd) ;
                /* 3D Rahmen (innen oder aussen= zeichnen */
                WinDrawBorder (hps,
                               &rcl,
                               1,
                               1,
                               0,
                               0,
                               (pwnd->ulLook == AK_LOOKIN) ? DB_DEPRESSED : DB_RAISED) ;
                /* Presentation Space wieder freigeben */
                WinReleasePS (hps) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Der 3D Look wird von aussen angegeben */
      case WMPAX_SETLOOK:
           pwnd->ulLook = LONGFROMMP (mp1) ;
           return (MRESULT) FALSE ;
      }
   /* Aufruf der WC_STATIC Standardverarbeitung */
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }

/* Diese Funktion ermittelt die Anzahl von Eintragen in der Listenverwaltung */
ULONG TListCount (PVOID pv)
   {
   return ((PLISTROOT) pv)->ulCount ;
   }

/* Diese Funktion loescht einen bestimmten Eintrag aus der Listenverwaltung */
BOOL TListDelete (PVOID pv,
                  ULONG ul,
                  PVOID pv2)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = TXK_ERROR ;
   plistroot = pv ;
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        if   (ul == TXK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = TXK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = TXK_LAST ;
                  }
             }
        switch (ul)
           {
           case TXK_FIRST:
                f = TXK_NOERROR ;
                break ;
           case TXK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = TXK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          f = TXK_NOERROR ;
                          break ;
                          }
                     }
                break ;
           case TXK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      case TXK_STRING:
                           if   ((strcmp (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      case TXK_ULONG:
                           if   (*((PULONG) (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      }
                   if   (f == TXK_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = TXK_PTR2NEXTINCURR) != NULL) ;
                break ;
           case TXK_LAST:
                pb = plistroot->pvLast ;
                f = TXK_NOERROR ;
                break ;
           }
        if   (f == TXK_NOERROR)
             {
             if   (!TXK_PTR2PREVINCURR)
                  {
                  plistroot->pvFirst = TXK_PTR2NEXTINCURR ;
                  }
             else
                  {
                  TXK_PTR2NEXTINPREV = TXK_PTR2NEXTINCURR ;
                  }
             if   (!TXK_PTR2NEXTINCURR)
                  {
                  plistroot->pvLast = TXK_PTR2PREVINCURR ;
                  }
             else
                  {
                  TXK_PTR2PREVINNEXT = TXK_PTR2PREVINCURR ;
                  }
             free (pb) ;
             plistroot->ulCount-- ;
             }
        }
   return f ;
   }

/* Diese Funktion loescht eine komplette Listenverwaltung */
BOOL TListDestroy (PVOID pv)
   {
   PBYTE     pb ;
   PBYTE     pbSave ;
   PLISTROOT plistroot ;

   plistroot = pv ;
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        pbSave = pb ;
        do
           {
           pb = TXK_PTR2NEXTINCURR ;
           free (pbSave) ;
           } while ((pbSave = pb) != NULL) ;
        }
   free (pv) ;
   return TXK_NOERROR ;
   }

/* Mit dieser Funktion wird die Listenverwaltung initialisiert */
PVOID TListInit (ULONG ulDataSize,
                 ULONG ulKeyOffset,
                 ULONG ulKeyStyle)
   {
   PLISTROOT plistroot ;

   plistroot = calloc (1,
                       sizeof (LISTROOT)) ;
   plistroot->ulDataSize = ulDataSize ;
   plistroot->ulKeyOffset = ulKeyOffset ;
   plistroot->ulKeyStyle = ulKeyStyle ;
   return plistroot ;
   }

/* Diese Funktion fuegt ein Element in der Listenverwaltung ein */
BOOL TListInsert (PVOID pv,
                  ULONG ul,
                  PVOID pv2)
   {
   BOOL      f ;
   PBYTE     pb ;
   PBYTE     pbPrev ;
   PBYTE     pbSave ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = TXK_ERROR ;
   plistroot = pv ;
   pb = calloc (1,
                plistroot->ulDataSize + TXK_HEADERDATA) ;
   memcpy (TXK_PTR2DATAINCURR,
           pv2,
           plistroot->ulDataSize) ;
   if   (!plistroot->pvFirst)
        {
        plistroot->pvFirst = pb ;
        plistroot->pvLast = pb ;
        f = TXK_NOERROR ;
        }
   else
        {
        if   (ul == TXK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = TXK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 >= plistroot->ulCount)
                  {
                  ul = TXK_LAST ;
                  }
             }
        switch (ul)
           {
           case TXK_FIRST:
                TXK_PTR2NEXTINCURR = plistroot->pvFirst ;
                TXK_PTR2PREVINNEXT = pb ;
                plistroot->pvFirst = pb ;
                f = TXK_NOERROR ;
                break ;
           case TXK_INDEX:
                pbSave = pb ;
                for  (ulI = 0, pbPrev = NULL, pb = plistroot->pvFirst;
                      ulI < plistroot->ulCount;
                      ulI++, pbPrev = pb, pb = TXK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          *((PBYTE *) (pbSave + TXK_HEADERNEXT)) = *((PBYTE *) (pbPrev + TXK_HEADERNEXT)) ;
                          *((PBYTE *) (pbPrev + TXK_HEADERNEXT)) = pbSave ;
                          *((PBYTE *) (pbSave + TXK_HEADERPREV)) = TXK_PTR2PREVINCURR ;
                          TXK_PTR2PREVINCURR = pbSave ;
                          f = TXK_NOERROR ;
                          break ;
                          }
                     }
                break ;
           case TXK_LAST:
                TXK_PTR2PREVINCURR = plistroot->pvLast ;
                *((PBYTE *) ((PBYTE) plistroot->pvLast + TXK_HEADERNEXT)) = pb ;
                plistroot->pvLast = pb ;
                f = TXK_NOERROR ;
                break ;
           }
        }
   if   (f == TXK_NOERROR)
        {
        plistroot->ulCount++ ;
        }
   return f ;
   }


/* Diese Funktion holt einen Eintrag aus der Listenverwaltung */
BOOL TListQuery (PVOID pv,
                 ULONG ul,
                 PVOID pv2,
                 PVOID pv3)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = TXK_ERROR ;
   plistroot = pv ;
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        if   (ul == TXK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = TXK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = TXK_LAST ;
                  }
             }
        switch (ul)
           {
           case TXK_FIRST:
                f = TXK_NOERROR ;
                break ;
           case TXK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = TXK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          f = TXK_NOERROR ;
                          break ;
                          }
                     }
                break ;
           case TXK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      case TXK_STRING:
                           if   ((strcmp (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      case TXK_ULONG:
                           if   (*((PULONG) (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      }
                   if   (f == TXK_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = TXK_PTR2NEXTINCURR) != NULL) ;
                break ;
           case TXK_LAST:
                pb = plistroot->pvLast ;
                f = TXK_NOERROR ;
                break ;
           }
        if   (f == TXK_NOERROR)
             {
             memcpy (pv3,
                     TXK_PTR2DATAINCURR,
                     plistroot->ulDataSize) ;
             }
        }
   return f ;
   }

/* Diese Funktion ersetzt einen bestimmten Eintrag in der Listenverwaltung */
BOOL TListUpdate (PVOID pv,
                  ULONG ul,
                  PVOID pv2,
                  PVOID pv3)
   {
   BOOL      f ;
   PBYTE     pb ;
   PLISTROOT plistroot ;
   ULONG     ulI ;

   f = TXK_ERROR ;
   plistroot = pv ;
   if   ((pb = plistroot->pvFirst) != NULL)
        {
        if   (ul == TXK_INDEX)
             {
             if   ((ULONG) pv2 == 0)
                  {
                  ul = TXK_FIRST ;
                  }
             if   ((ULONG) pv2 + 1 == plistroot->ulCount)
                  {
                  ul = TXK_LAST ;
                  }
             }
        switch (ul)
           {
           case TXK_FIRST:
                f = TXK_NOERROR ;
                break ;
           case TXK_INDEX:
                for  (ulI = 0;
                      ulI < plistroot->ulCount;
                      ulI++, pb = TXK_PTR2NEXTINCURR)
                     {
                     if   (ulI == (ULONG) pv2)
                          {
                          f = TXK_NOERROR ;
                          break ;
                          }
                     }
                break ;
           case TXK_KEY:
                do
                   {
                   switch (plistroot->ulKeyStyle)
                      {
                      case TXK_STRING:
                           if   ((strcmp (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset,
                                          pv2)) == 0)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      case TXK_ULONG:
                           if   (*((PULONG) (TXK_PTR2DATAINCURR + plistroot->ulKeyOffset)) == (ULONG) pv2)
                                {
                                f = TXK_NOERROR ;
                                }
                           break ;
                      }
                   if   (f == TXK_NOERROR)
                        {
                        break ;
                        }
                   } while ((pb = TXK_PTR2NEXTINCURR) != NULL) ;
                break ;
           case TXK_LAST:
                pb = plistroot->pvLast ;
                f = TXK_NOERROR ;
                break ;
           }
        if   (f == TXK_NOERROR)
             {
             memcpy (TXK_PTR2DATAINCURR,
                     pv3,
                     plistroot->ulDataSize) ;
             }
        }
   return f ;
   }

