/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF05.C 1.1 1996/02/07 12:16:54 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien einbinden */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Anwendungskopfdatei einbinden */
#include "PMEINF05.H"

/* Modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PM05" ;

/* Fensterprozedur fuer das Hauptfenster */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Message trifft gerade ein? */
   switch (msg)
      {
      /* Anwendung wurde geschlossen */
      case WM_CLOSE:
           /* Diese an uns selbst geschickte Message unterbricht das 'while' der MessageLoop */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Der Benutzer wuenscht eine Aktion */
      case WM_COMMAND:
           /* und welche Aktion wird gewuenscht? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Dialog soll aufgerufen werden */
              case IDM_DIALOGOEFFNEN:
                   /* Dialog starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              DOeffnenWndProc,
                              NULLHANDLE,
                              IDD_DIALOGOEFFNEN,
                              NULL) ;
                   return (MRESULT) FALSE ;
              /* Standarddialog fuer Dateien soll gestartet werden */
              case IDM_STANDARDDIALOGEDATEIOEFFNEN:
                   {
                   FILEDLG fildlg ;

                   /* šbergabestruktur loeschen */
                   memset (&fildlg,
                           0,
                           sizeof (FILEDLG)) ;
                   /* Groesse der Struktur hinterlegen */
                   fildlg.cbSize = sizeof (FILEDLG) ;
                   /* Einige Flags */
                   fildlg.fl = FDS_CENTER | FDS_SAVEAS_DIALOG ;
                   /* Muster fuer Dateien */
                   strcpy (fildlg.szFullFile,
                           "\\*.*") ;
                   /* Dialog aktivieren */
                   if   (WinFileDlg (HWND_DESKTOP,
                                     hwndFrame_g,
                                     &fildlg) &&
                         (fildlg.lReturn == DID_OK))
                        {
                        }
                   return (MRESULT) FALSE ;
                   }
              case IDM_STANDARDDIALOGEFARBAUSWAHL:
                   {
                   CHAR        szBuffer [CCHMAXPATH] ;
                   FONTDLG     fntdlg ;
                   FONTMETRICS fm ;

                   /* šbergabestruktur loeschen */
                   memset (&fntdlg,
                           0,
                           sizeof (FONTDLG)) ;
                   /* Groesse der Struktur hinterlegen */
                   fntdlg.cbSize = sizeof (FONTDLG) ;
                   /* PS fuer Farbauswahl besorgen */
                   fntdlg.hpsScreen = WinGetPS (hwndFrame_g) ;
                   /* Logischen Font ermitteln */
                   GpiQueryLogicalFont (fntdlg.hpsScreen,
                                        0,
                                        (PSTR8) fntdlg.fAttrs.szFacename,
                                        &fntdlg.fAttrs,
                                        sizeof (fntdlg.fAttrs)) ;
                   /* FontMetrics ermitteln */
                   GpiQueryFontMetrics (fntdlg.hpsScreen,
                                        sizeof (fm),
                                        &fm) ;
                   fntdlg.pszFamilyname = szBuffer ;
                   /* Einige Flags */
                   fntdlg.fl = FNTS_BITMAPONLY | FNTS_CENTER | FNTS_INITFROMFATTRS ;
                   fntdlg.flType = fm.fsType ;
                   /* Foreground Farbe */
                   fntdlg.clrFore = CLR_BLACK ;
                   /* Background Farbe */
                   fntdlg.clrBack = CLR_WHITE ;
                   fntdlg.sNominalPointSize = fm.sNominalPointSize ;
                   fntdlg.usWeight = fm.usWeightClass ;
                   fntdlg.usWidth = fm.usWidthClass ;
                   fntdlg.usFamilyBufLen = sizeof (szBuffer) ;
                   /* Dialog aktivieren */
                   if (WinFontDlg (HWND_DESKTOP,
                                   hwndFrame_g,
                                   &fntdlg) &&
                       (fntdlg.lReturn == DID_OK))
                      {
                      }
                   /* Presentation Space wieder freigeben */
                   WinReleasePS (fntdlg.hpsScreen) ;
                   return (MRESULT) FALSE ;
                   }
              }
           break ;
      /* Client Fenster wird erzeugt */
      case WM_CREATE:
           {
           PWND pwnd ;

           /* Speicherbereich fuer die Instanzdaten allokieren */
           pwnd = calloc (1,
                          sizeof (WND)) ;
           /* Pointer auf Instanzdaten in Window Words des Clientfensters speichern */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            pwnd) ;
           /* Verzoegerter Applikationsstart mit einer privaten Message */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
           }
      /* Clientfenster wird geloescht */
      case WM_DESTROY:
           /* Speicherbereich der Instanzdaten aus Windows Words holen und wieder freigeben */
           free (WinQueryWindowPtr (hwndFrame_g,
                                    QWL_USER)) ;
           break ;
      /* Client Hintergrund muss gezeichnet werden */
      case WM_ERASEBACKGROUND:
           /* PM soll selbst zeichnen */
           return (MRESULT) TRUE ;
      /* Verzoegerter Applikationsstart erfolgt hier */
      case WMP_MAININIT:
           {
           PWND pwnd ;

           /* Pointer auf Instanzdaten aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Fenster Handle des Menues dort ablegen */
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standardverarbeitung fuer das Clientfenster */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Fensterprozedur fuer den Dialog */
MRESULT EXPENTRY DOeffnenWndProc (HWND   hwndDlg,
                                  MSG    msg,
                                  MPARAM mp1,
                                  MPARAM mp2)
   {
   /* Welche Message trifft gerade ein? */
   switch (msg)
      {
      /* Der Benutzer wuenscht eine Aktion */
      case WM_COMMAND:
           /* und welche Aktion wird gewuenscht? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel Knopf gedreuckt */
              case DID_CANCEL:
                   /* Dialog beenden */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* OK Knopf gedreuckt */
              case IDP_DO1:
                   /* Dialog beenden */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Ein Kontrollelement hat eine Aktion zu vermelden */
      case WM_CONTROL:
           {
           BOOKTEXT booktxt ;
           CHAR     sz [128] ;
           CHAR     sz2 [16] ;
           MRESULT  mr ;
           PDLG     pdlg ;
           PUSRREC  pusrrec ;
           ULONG    ulI ;

           /* Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* und welches Kontrollelement meldet sich? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Container meldet sich */
              case IDCO_DO1:
                   /* und was will der Container? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Doppelklick */
                      case CN_ENTER:
                           /* Record der selektierten Zeile ermitteln */
                           if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pdlg->hwndContainer,
                                                                     CM_QUERYRECORDEMPHASIS,
                                                                     MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                     MPFROMSHORT (CRA_SELECTED)))) != NULL)
                                {
                                /* Ausgabetext bauen */
                                sprintf (sz,
                                         "Doppelklick auf Containerspalte %s / %s",
                                         pusrrec->pszColumn1,
                                         pusrrec->pszColumn2) ;
                                /* Text im Ausgabefeld ausgeben */
                                WinSetDlgItemText (hwndDlg,
                                                   IDT_DO1,
                                                   sz) ;
                                }
                           break ;
                      }
                   break ;
              /* Das Notebook meldet sich */
              case IDNO_DO1:
                   /* und was will das Notebook? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Neue Seite */
                      case BKN_PAGESELECTED:
                           /* Kommunikationsstruktur loeschen */
                           memset (&booktxt,
                                   0,
                                   sizeof (BOOKTEXT)) ;
                           /* Wo soll Text eingestellt werden */
                           booktxt.pString = sz2 ;
                           booktxt.textLen = sizeof (sz2) ;
                           /* Tab Text ermitteln */
                           WinSendMsg (pdlg->hwndNotebook,
                                       BKM_QUERYTABTEXT,
                                       MPFROMLONG (((PPAGESELECTNOTIFY) PVOIDFROMMP (mp2))->ulPageIdNew),
                                       MPFROMP (&booktxt)) ;
                           /* Ausgabetext bauen */
                           sprintf (sz,
                                    "Notebook Seite %s eingestellt",
                                    sz2) ;
                           /* Text im Ausgabefeld ausgeben */
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO1,
                                              sz) ;
                           break ;
                      }
                   break ;
              /* Der SliderBar meldet sich */
              case IDSL_DO1:
                   /* und was will der SliderBar? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Der Arm wurde bewegt */
                      case SLN_CHANGE:
                      case SLN_SLIDERTRACK:
                           /* SliderBar Position ermitteln */
                           ulI = LONGFROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                IDSL_DO1,
                                                                SLM_QUERYSLIDERINFO,
                                                                MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                                                                MPVOID)) ;
                           /* Ausgabetext bauen */
                           sprintf (sz,
                                    "SliderBar Verh„ltnis %u / %u eingestellt",
                                    10 + (ulI * 10),
                                    90 - (ulI * 10)) ;
                           /* Text im Ausgabefeld ausgeben */
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO1,
                                              sz) ;
                           break ;
                      }
                   break ;
              /* Der ValueSet meldet sich */
              case IDVS_DO1:
                   /* und was will der ValueSet? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein Eintrag ist selektiert worden */
                      case VN_SELECT:
                           /* Index des selektierten Eintrags ermitteln */
                           mr = WinSendDlgItemMsg (hwndDlg,
                                                   IDVS_DO1,
                                                   VM_QUERYSELECTEDITEM,
                                                   MPVOID,
                                                   MPVOID) ;
                           /* Ausgabetext bauen */
                           sprintf (sz,
                                    "ValueSet Reihe %u Spalte %u selektiert",
                                    (ULONG) SHORT1FROMMR (mr),
                                    (ULONG) SHORT2FROMMR (mr)) ;
                           /* Text im Ausgabefeld ausgeben */
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO1,
                                              sz) ;
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Dialog wird gestoppt */
      case WM_DESTROY:
           {
           /* Speicherbereich der Instanzdaten aus Windows Words holen und wieder freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
           }
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           CHAR                 sz [32] ;
           CNRINFO              ccinfo ;
           static CONTAINERINIT acnrinit [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_COLUMN1, offsetof (USRREC, pszColumn1) },
                                               { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_COLUMN2, offsetof (USRREC, pszColumn2) }} ;
           FIELDINFOINSERT      fiins ;
           LONG                 l ;
           PDLG                 pdlg ;
           PFIELDINFO           pfldinfo ;
           PFIELDINFO           pfldinfoFirst ;
           PSZ                  apsz [] = { "Zeile 1",
                                            "Zeile 2",
                                            "Zeile 3" } ;
           PSZ                  aapsz [] [2] = {{ "Zeile 1 Spalte 1", "Zeile 1 Spalte 2" },
                                                { "Zeile 2 Spalte 1", "Zeile 2 Spalte 2" },
                                                { "Zeile 3 Spalte 1", "Zeile 3 Spalte 2" }} ;
           PUSRREC              pusrrec ;
           RECORDINSERT         recins ;
           ULONG                ulI ;
           ULONG                ulJ ;

           /* Speicherbereich fuer die Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Pointer auf Instanzdaten in Window Words des Dialoges speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Handle des Containers merken */
           pdlg->hwndContainer = WinWindowFromID (hwndDlg,
                                                  IDCO_DO1) ;
           /* Handle des Notebooks merken */
           pdlg->hwndNotebook = WinWindowFromID (hwndDlg,
                                                 IDNO_DO1) ;
           /* Platz fuer die Spaltendefinitionen des Conatiners allokieren */
           pfldinfo = PVOIDFROMMR (WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCDETAILFIELDINFO,
                                               MPFROMLONG (sizeof (acnrinit) / sizeof (acnrinit [0])),
                                               MPVOID)) ;
           /* In einer Schleife alle Spalten beschreiben */
           for  (ulI = 0, pfldinfoFirst = pfldinfo; ulI < sizeof (acnrinit) / sizeof (acnrinit [0]); ulI++, pfldinfo = pfldinfo->pNextFieldInfo)
                {
                /* Spaltenflags */
                pfldinfo->flData = acnrinit[ulI].ulFlData ;
                /* Flags fuer die Spaltenueberschriften */
                pfldinfo->flTitle = acnrinit[ulI].ulFlTitle ;
                /* Ueberschrift aus der Ressource lesen */
                WinLoadString (hab_g,
                               NULLHANDLE,
                               acnrinit[ulI].ulIdTitle,
                               sizeof (acnrinit[ulI].szTitle),
                               acnrinit[ulI].szTitle) ;
                /* Titel der Spalten setzen */
                pfldinfo->pTitleData = acnrinit[ulI].szTitle ;
                /* Offset zu den Daten in der Record Struktur */
                pfldinfo->offStruct = acnrinit[ulI].ulOffStruct ;
                }
           /* Spaltenstruktur loeschen */
           memset (&fiins,
                   0,
                   sizeof (FIELDINFOINSERT)) ;
           /* Groesse der Spaltenstruktur */
           fiins.cb = sizeof (FIELDINFOINSERT) ;
           /* Wir fangen vorne an */
           fiins.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST ;
           /* Anzahl der Spalten */
           fiins.cFieldInfoInsert = sizeof (acnrinit) / sizeof (acnrinit [0]) ;
           /* Invalidieren nach Einfuegen der Spalten */
           fiins.fInvalidateFieldInfo = TRUE ;
           /* Spalten setzen */
           WinSendMsg (pdlg->hwndContainer,
                       CM_INSERTDETAILFIELDINFO,
                       MPFROMP (pfldinfoFirst),
                       MPFROMP (&fiins)) ;
           /* Ueberschrift des Containers aus der Ressource laden */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_HEADER,
                          sizeof (pdlg->szHeader),
                          pdlg->szHeader) ;
           /* Container Kommunikationsstruktur loeschen */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Einige Flags fuer den Container */
           ccinfo.flWindowAttr = CA_CONTAINERTITLE | CA_DETAILSVIEWTITLES | CA_TITLESEPARATOR | CV_DETAIL | CV_MINI ;
           /* Adresse der Containerueberschrift */
           ccinfo.pszCnrTitle = pdlg->szHeader ;
           /* Container definieren */
           WinSendMsg (pdlg->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           /* In einer Schleife einige Zeilen in Container einfuegen */
           for  (ulI = 0; ulI < sizeof (aapsz) / sizeof (aapsz [0]); ulI++)
                {
                /* Einen Container Record allokieren */
                pusrrec = PVOIDFROMMR (WinSendMsg (pdlg->hwndContainer,
                                                   CM_ALLOCRECORD,
                                                   MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                   MPFROMSHORT (1))) ;
                /* Text fuer Spalte 1 */
                strcpy (pusrrec->szColumn1,
                        aapsz [ulI] [0]) ;
                /* Text fuer Spalte 2 */
                strcpy (pusrrec->szColumn2,
                        aapsz [ulI] [1]) ;
                /* Container benoetigt Pointer */
                pusrrec->pszColumn1 = pusrrec->szColumn1 ;
                pusrrec->pszColumn2 = pusrrec->szColumn2 ;
                /* Einfuegestruktur loeschen */
                memset (&recins,
                        0,
                        sizeof (RECORDINSERT)) ;
                /* Groesse der Einfuegestruktur */
                recins.cb = sizeof (RECORDINSERT) ;
                /* Einen Record eintragen */
                recins.cRecordsInsert = 1 ;
                /* Wir fangen oben an */
                recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                /* Oberste Hierarchie */
                recins.zOrder = CMA_TOP ;
                /* Record in Container einfuegen */
                WinSendMsg (pdlg->hwndContainer,
                            CM_INSERTRECORD,
                            MPFROMP (pusrrec),
                            MPFROMP (&recins)) ;
                }
           /* Container invalidieren */
           WinSendMsg (pdlg->hwndContainer,
                       CM_INVALIDATERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_REPOSITION)) ;
           /* In zwei Schleifen die ValueSet Inhalte einfuegen */
           /* Schleife 1 sind die Zeilen */
           for  (ulI = 1, l = CLR_BACKGROUND; ulI <= 2; ulI++)
                {
                /* Schleife 2 sind die Spalten */
                for  (ulJ = 1; ulJ <= 8; ulJ++, l++)
                     {
                     /* Wert in ValueSet einfuegen */
                     WinSendDlgItemMsg (hwndDlg,
                                        IDVS_DO1,
                                        VM_SETITEM,
                                        MPFROM2SHORT (ulI, ulJ),
                                        MPFROMLONG (l)) ;
                     }
                }
           /* 26 Notebook Tabs generieren */
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETDIMENSIONS,
                       MPFROM2SHORT (26, 26),
                       MPFROMSHORT (BKA_MAJORTAB)) ;
           /* Keine Minortabs benoetigt */
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETDIMENSIONS,
                       MPFROM2SHORT (0, 0),
                       MPFROMSHORT (BKA_MINORTAB)) ;
           /* Keine Seitenknoepfe */
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETDIMENSIONS,
                       MPFROM2SHORT (0, 0),
                       MPFROMSHORT (BKA_PAGEBUTTON)) ;
           /* Hintergrundfarbe des Notebooks setzen */
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETNOTEBOOKCOLORS,
                       MPFROMLONG (SYSCLR_FIELDBACKGROUND),
                       MPFROMSHORT (BKA_BACKGROUNDPAGECOLORINDEX)) ;
           /* In einer Schleife die 26 Notebook Laschen beschriften */
           for  (ulI = 0; ulI < 26; ulI++)
                {
                /* Eine Seite einfuegen */
                pdlg->aulPageId [ulI] = LONGFROMMR (WinSendMsg (pdlg->hwndNotebook,
                                                                BKM_INSERTPAGE,
                                                                MPFROMLONG (0),
                                                                MPFROM2SHORT (BKA_MAJOR, BKA_LAST))) ;
                /* Text fuer die Laschen konstruieren */
                sz [0] = 'A' + ulI ;
                sz [1] = '\0' ;
                /* Text der Laschen setzen */
                WinSendMsg (pdlg->hwndNotebook,
                            BKM_SETTABTEXT,
                            MPFROMLONG (pdlg->aulPageId [ulI]),
                            MPFROMP (sz)) ;
                }
           /* 10 Schritte beim SliderBar setzen */
           for  (ulI = 0; ulI < 10; ulI++)
                {
                /* Dicke der Ticks setzen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDSL_DO1,
                                   SLM_SETTICKSIZE,
                                   MPFROM2SHORT (ulI, 8),
                                   MPVOID) ;
                /* Text der Ticks konstruieren */
                _ltoa ((ulI + 1) * 10,
                       sz,
                       10) ;
                /* Text der Ticks setzen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDSL_DO1,
                                   SLM_SETSCALETEXT,
                                   MPFROMSHORT (ulI),
                                   MPFROMP (sz)) ;
                }
           /* Spinbutton Texte setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDSP_DO1,
                              SPBM_SETARRAY,
                              MPFROMP (&apsz [0]),
                              MPFROMSHORT (sizeof (apsz) / sizeof (apsz [0]))) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standardverarbeitung fuer den Dialog */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Applikationsstart */
INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Beim Presentation Manager anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* MessageQueue mit Standardgroesse anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse fuer Anwendung anlegen */
             if   (WinRegisterClass (hab_g,
                                     pszClass_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster erzeugen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszClass_g,
                                                           "PM Einfhrung - Beispiel 5",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PMEINF05,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* MessageLoop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Messages auf Fensterprozeduren verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Standardfenster wieder entfernen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* MessageQueue wieder entfernen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim Presentation Manager wieder abmelden */
        WinTerminate (hab_g) ;
        }
   /* Applikationsende */
   return 0 ;
   }


