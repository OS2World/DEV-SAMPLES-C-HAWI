/* $Header: D:/Projekte/Tief im MLE!1/Source/rcs/MLE.C 1.5 1996/04/19 18:21:00 HaWi Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C Kopfdateien */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikations Kopfdatei */
#include "MLE.H"

/* Modulglobale Variablen */
BOOL fMinimized_g = FALSE ;
CHAR szFile_g [CCHMAXPATH] = "" ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszMLE_g = "MLE" ;

/* Dialogprozedur zum Bearbeiten - Ersetzen */
MRESULT EXPENTRY BearbeitenErsetzenDlgWndProc (HWND   hwndDlg,
                                               MSG    msg,
                                               MPARAM mp1,
                                               MPARAM mp2)
   {
   /* Welcher Event fliesst ein */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           CHAR    sz [CCHMAXPATH] ;
           MLECOMM mlecomm ;
           PDLGBE  pdlg ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion wurde ausgeloest? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel */
              case DID_CANCEL:
                   /* Im Client Flag setzen damit lange Suchen nicht fortgesetzt wird */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_STOP,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   /* Dialog entfernen lassen */
                   WinPostMsg (hwndClient_g,
                               WMP_DESTROY,
                               MPFROMHWND (hwndDlg),
                               MPVOID) ;
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok */
              case DID_OK:
                   /* Platz fuer Kommunikationsstruktur loeschen */
                   memset (&mlecomm,
                           0,
                           sizeof (MLECOMM)) ;
                   /* Dialog ID von diesem Dialog hinterlegen */
                   mlecomm.ulDialog = WinQueryWindowUShort (hwndDlg,
                                                            QWS_ID) ;
                   /* Exakte Suche? */
                   mlecomm.fExakt = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                     IDH_BE_1,
                                                                     BM_QUERYCHECK,
                                                                     MPVOID,
                                                                     MPVOID)) ;
                   /* Von oben suchen? */
                   mlecomm.fVonOben = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDH_BE_2,
                                                                       BM_QUERYCHECK,
                                                                       MPVOID,
                                                                       MPVOID)) ;
                   /* Alle Ersetzen? */
                   mlecomm.fAlle = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                    IDH_BE_3,
                                                                    BM_QUERYCHECK,
                                                                    MPVOID,
                                                                    MPVOID)) ;
                   /* Suchtext */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_BE_1,
                                        sizeof (mlecomm.szSearch),
                                        mlecomm.szSearch) ;
                   /* Ersatztext */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_BE_2,
                                        sizeof (mlecomm.szReplace),
                                        mlecomm.szReplace) ;
                   /* Suche im Client initiieren */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_SEARCH,
                               MPFROMP (&mlecomm),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Ersetzen */
              case IDP_BE_1:
                   /* Ersatztext lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_BE_2,
                                        sizeof (sz),
                                        sz) ;
                   /* Texte austauschen */
                   WinSendMsg (pdlg->hwndMLE,
                               MLM_INSERT,
                               MPFROMP (sz),
                               MPVOID) ;
                   /* Ersetzen Pushbutton disablen */
                   WinEnableControl (hwndDlg,
                                     IDP_BE_1,
                                     FALSE) ;
                   return (MRESULT) FALSE ;
              /* Stoppen */
              case IDP_BE_2:
                   /* Im Client Flag setzen damit lange Suche nicht fortgesetzt wird */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_STOP,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Der Dialog wird zerstoert */
      case WM_DESTROY:
           /* Pointer auf Instanzdaten aus Window Words lesen und loeschen */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGBE pdlg ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGBE)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Ersetzen Pushbutton erst einmal disablen */
           WinEnableControl (hwndDlg,
                             IDP_BE_1,
                             FALSE) ;
           /* L„nge der Eingabefelder festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_BE_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH - 1),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_BE_2,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH - 1),
                              MPVOID) ;
           break ;
           }
      /* Client teilt den Handle des MLEs mit */
      case WMP_REPLACE:
           {
           PDLGBE pdlg ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Ersetzen Pushbutton je nach Status der Suche im Client disablen/enablen */
           WinEnableControl (hwndDlg,
                             IDP_BE_1,
                             LONGFROMMP (mp1)) ;
           /* MLE Handle merken */
           pdlg->hwndMLE = HWNDFROMMP (mp2) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialog Prozedur des Bearbeiten - Suchen Dialoges */
MRESULT EXPENTRY BearbeitenSuchenDlgWndProc (HWND   hwndDlg,
                                             MSG    msg,
                                             MPARAM mp1,
                                             MPARAM mp2)
   {
   /* Welcher Event fliesst ein */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           MLECOMM mlecomm ;

           /* Welche Benutzeraktion wurde ausgeloest? */
           switch (SHORT1FROMMP (mp1))
              {
              case DID_CANCEL:
                   /* Im Client Flag setzen damit lange Suchen nicht fortgesetzt wird */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_STOP,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   /* Dialog entfernen lassen */
                   WinPostMsg (hwndClient_g,
                               WMP_DESTROY,
                               MPFROMHWND (hwndDlg),
                               MPVOID) ;
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              case DID_OK:
                   /* Platz fuer Kommunikationsstruktur loeschen */
                   memset (&mlecomm,
                           0,
                           sizeof (MLECOMM)) ;
                   /* Dialog ID von diesem Dialog hinterlegen */
                   mlecomm.ulDialog = WinQueryWindowUShort (hwndDlg,
                                                            QWS_ID) ;
                   /* Exakte Suche? */
                   mlecomm.fExakt = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                     IDH_BS_1,
                                                                     BM_QUERYCHECK,
                                                                     MPVOID,
                                                                     MPVOID)) ;
                   /* Von oben suchen? */
                   mlecomm.fVonOben = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDH_BS_2,
                                                                       BM_QUERYCHECK,
                                                                       MPVOID,
                                                                       MPVOID)) ;
                   /* Suchtext */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_BS_1,
                                        sizeof (mlecomm.szSearch),
                                        mlecomm.szSearch) ;
                   /* Suche im Client initiieren */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_SEARCH,
                               MPFROMP (&mlecomm),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              case IDP_BS_1:
                   /* Im Client Flag setzen damit lange Suchen nicht fortgesetzt wird */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_STOP,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Dialog wird gestartet */
      case WM_INITDLG:
           /* L„nge der Eingabefelder festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_BS_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH - 1),
                              MPVOID) ;
           break ;
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welcher Event fliesst ein */
   switch (msg)
      {
      /* Die Anwendung soll geschlossen werden */
      case WM_CLOSE:
           {
           PWND pwnd ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Ist die Datei veraendert worden? */
           if   (pwnd->fChanged)
                {
                /* Ja, dann Sicherheitsabfrage */
                if   (TWriteMessage (hwnd,
                                     NULLHANDLE,
                                     IDS_MLE,
                                     IDS_DIEDATEIWURDEVERAENDERT,
                                     MB_QUERY | MB_YESNO) == MBID_YES)
                     {
                     /* Wenn sichern erwuenscht, dann entsprechende Funktion aktivieren */
                     WinPostMsg (hwnd,
                                 WM_COMMAND,
                                 MPFROMSHORT (IDM_DATEISICHERN),
                                 MPFROMSHORT (CMDSRC_MENU)) ;
                     return (MRESULT) FALSE ;
                     }
                }
           break ;
           }
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           CHAR        sz [CCHMAXPATH] ;
           FILEDLG     fildlg ;
           FONTDLG     fntdlg ;
           FONTMETRICS fm ;
           LONG        l ;
           PWND        pwnd ;
           ULONG       ul ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Welche Benutzeraktion wurde ausgeloest? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Ausschneiden */
              case IDM_BEARBEITENAUSSCHNEIDEN:
                   /* Cut veranlassen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_CUT,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Einfuegen */
              case IDM_BEARBEITENEINFUEGEN:
                   /* Clipboard oeffnen */
                   if   (WinOpenClipbrd (hab_g))
                        {
                        /* Pointer auf Textinhalt holen */
                        if   ((ul = WinQueryClipbrdData (hab_g,
                                                         CF_TEXT)) != 0)
                             {
                             /* Wenn OK Text an aktueller Position einfuegen */
                             WinSendMsg (pwnd->hwndMLE,
                                         MLM_INSERT,
                                         MPFROMP ((PSZ) ul),
                                         MPVOID) ;
                             }
                        /* Clipboard wieder schliessen */
                        WinCloseClipbrd (hab_g) ;
                        }
                   return (MRESULT) FALSE ;
              /* Ersetzen */
              case IDM_BEARBEITENERSETZEN:
                   /* Ersetzen Dialog starten */
                   WinLoadDlg (HWND_DESKTOP,
                               hwndFrame_g,
                               BearbeitenErsetzenDlgWndProc,
                               NULLHANDLE,
                               IDD_BEARBEITENERSETZEN,
                               NULL) ;
                   return (MRESULT) FALSE ;
              /* Kopieren */
              case IDM_BEARBEITENKOPIEREN:
                   /* Copy veranlassen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_COPY,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Loeschen */
              case IDM_BEARBEITENLOESCHEN:
                   /* Delete veranlassen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_CLEAR,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Undo */
              case IDM_BEARBEITENRUECKGAENGIG:
                   /* Undo veranlassen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_UNDO,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Suchen */
              case IDM_BEARBEITENSUCHEN:
                   /* Suchen Dialog starten */
                   WinLoadDlg (HWND_DESKTOP,
                               hwndFrame_g,
                               BearbeitenSuchenDlgWndProc,
                               NULLHANDLE,
                               IDD_BEARBEITENSUCHEN,
                               NULL) ;
                   return (MRESULT) FALSE ;
              /* Neu */
              case IDM_DATEINEU:
                   /* MLE Inhalt loeschen */
                   TDeleteMLEContents (pwnd->hwndMLE) ;
                   /* Changed Flag zuruecksetzen */
                   pwnd->fChanged = FALSE ;
                   /* Alten Dateinamen loeschen */
                   *pwnd->szFile = '\0' ;
                   /* Menueintrag Sichern disablen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   /* Menueintrag Sichern als disablen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   /* Titelzeile mit Anwendungstitel und - unbekannt besetzen */
                   strcpy (sz,
                           pszMLE_g) ;
                   strcat (sz,
                           " - Unbekannt") ;
                   WinSetWindowText (pwnd->hwndTitlebar,
                                     sz) ;
                   return (MRESULT) FALSE ;
              /* Oeffnen */
              case IDM_DATEIOEFFNEN:
                   /* Kommunikationsstruktur Datei oeffnen initialisieren */
                   memset (&fildlg,
                           0,
                           sizeof (FILEDLG)) ;
                   fildlg.cbSize = sizeof (FILEDLG) ;
                   fildlg.fl = FDS_CENTER | FDS_OPEN_DIALOG ;
                   strcpy (fildlg.szFullFile,
                           "*.*") ;
                   /* Datei oeffnen Dialog ausgeben */
                   if   (WinFileDlg (HWND_DESKTOP,
                                     hwndFrame_g,
                                     &fildlg) &&
                         (fildlg.lReturn == DID_OK))
                        {
                        /* MLE Inhalt loeschen */
                        TDeleteMLEContents (pwnd->hwndMLE) ;
                        /* Dateiinhalt in MLE einlesen */
                        if   (TReadFileInMLE (fildlg.szFullFile,
                                              pwnd->hwndMLE))
                             {
                             /* Dateiname merken */
                             strcpy (pwnd->szFile,
                                     fildlg.szFullFile) ;
                             /* Dateiname in Titelzeile setzen */
                             strcpy (sz,
                                     pszMLE_g) ;
                             strcat (sz,
                                     " - ") ;
                             strcat (sz,
                                     pwnd->szFile) ;
                             WinSetWindowText (pwnd->hwndTitlebar,
                                               sz) ;
                             /* Changed Flag zuruecksetzen */
                             pwnd->fChanged = FALSE ;
                             /* Menueintrag Sichern disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             /* Menueintrag Sichern als disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Sichern */
              case IDM_DATEISICHERN:
                   /* Ist Dateiname bekannt? */
                   if   (*pwnd->szFile)
                        {
                        /* Ja, MLE auslesen und in Datei schreiben */
                        if   (TWriteFileFromMLE (pwnd->szFile,
                                                 pwnd->hwndMLE))
                             {
                             /* Changed Falg zuruecksetzen */
                             pwnd->fChanged = FALSE ;
                             /* Menueintrag Sichern disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             /* Menueintrag Sichern als disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             }
                        }
                   else
                        {
                        /* Nein, Sichern als aktivieren */
                        WinPostMsg (hwnd,
                                    WM_COMMAND,
                                    MPFROMSHORT (IDM_DATEISICHERNALS),
                                    MPFROMSHORT (CMDSRC_MENU)) ;
                        }
                   return (MRESULT) FALSE ;
              /* Sichern als */
              case IDM_DATEISICHERNALS:
                   /* Kommunikationsstruktur Datei sichern als initialisieren */
                   memset (&fildlg,
                           0,
                           sizeof (FILEDLG)) ;
                   fildlg.cbSize = sizeof (FILEDLG) ;
                   fildlg.fl = FDS_CENTER | FDS_SAVEAS_DIALOG ;
                   strcpy (fildlg.szFullFile,
                           "*.*") ;
                   /* Datei sichern als Dialog ausgeben */
                   if   (WinFileDlg (HWND_DESKTOP,
                                     hwndFrame_g,
                                     &fildlg) &&
                         (fildlg.lReturn == DID_OK))
                        {
                        /* MLE auslesen und in Datei schreiben */
                        if   (TWriteFileFromMLE (fildlg.szFullFile,
                                                 pwnd->hwndMLE))
                             {
                             /* Dateiname merken */
                             strcpy (pwnd->szFile,
                                     fildlg.szFullFile) ;
                             /* Dateiname in Titelzeile setzen */
                             strcpy (sz,
                                     pszMLE_g) ;
                             strcat (sz,
                                     " - ") ;
                             strcat (sz,
                                     pwnd->szFile) ;
                             WinSetWindowText (pwnd->hwndTitlebar,
                                               sz) ;
                             /* Changed Flag zuruecksetzen */
                             pwnd->fChanged = FALSE ;
                             /* Menueintrag Sichern disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             /* Menueintrag Sichern als disablen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_SETITEMATTR,
                                         MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                                         MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Hintergrundfarbe */
              case IDM_OPTIONENHINTERGRUNDFARBE:
                   /* ID fuer Dialog hinterlegen */
                   ul = IDM_OPTIONENHINTERGRUNDFARBE ;
                   /* Farbauswahl Dialog starten */
                   if   ((l = WinDlgBox (HWND_DESKTOP,
                                         hwndFrame_g,
                                         OptionenFarbeDlgWndProc,
                                         NULLHANDLE,
                                         IDD_OPTIONENFARBE,
                                         (PVOID) &ul)) != 9999)
                        {
                        /* Hintergrundfarbe setzen */
                        WinSendMsg (pwnd->hwndMLE,
                                    MLM_SETBACKCOLOR,
                                    MPFROMLONG (l),
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              /* Schriftart */
              case IDM_OPTIONENSCHRIFTART:
                   /* Kommunikationsstruktur Fontauswahl initialisieren */
                   memset (&fntdlg,
                           0,
                           sizeof (FONTDLG)) ;
                   fntdlg.cbSize = sizeof (FONTDLG) ;
                   /* Presentation Space anlegen */
                   fntdlg.hpsScreen = WinGetPS (pwnd->hwndMLE) ;
                   GpiQueryLogicalFont (fntdlg.hpsScreen,
                                        0,
                                        (PSTR8) fntdlg.fAttrs.szFacename,
                                        &fntdlg.fAttrs,
                                        sizeof (fntdlg.fAttrs)) ;
                   GpiQueryFontMetrics (fntdlg.hpsScreen,
                                        sizeof (fm),
                                        &fm) ;
                   fntdlg.pszFamilyname = sz ;
                   fntdlg.fl = FNTS_BITMAPONLY | FNTS_CENTER | FNTS_INITFROMFATTRS ;
                   fntdlg.flType = fm.fsType ;
                   fntdlg.clrFore = CLR_BLACK ;
                   fntdlg.clrBack = CLR_WHITE ;
                   fntdlg.sNominalPointSize = fm.sNominalPointSize ;
                   fntdlg.usWeight = fm.usWeightClass ;
                   fntdlg.usWidth = fm.usWidthClass ;
                   fntdlg.usFamilyBufLen = sizeof (sz) ;
                   /* Fontauswahl als Dialog ausgeben */
                   if   (WinFontDlg (HWND_DESKTOP,
                                     hwndFrame_g,
                                     &fntdlg) &&
                         (fntdlg.lReturn == DID_OK))
                        {
                        /* Font setzen */
                        WinSendMsg (pwnd->hwndMLE,
                                    MLM_SETFONT,
                                    MPFROMP (&fntdlg.fAttrs),
                                    MPVOID) ;
                        }
                   /* Presentation Space wieder loeschen */
                   WinReleasePS (fntdlg.hpsScreen) ;
                   return (MRESULT) FALSE ;
              /* Vordergrundfarbe */
              case IDM_OPTIONENVORDERGRUNDFARBE:
                   /* ID fuer Dialog hinterlegen */
                   ul = IDM_OPTIONENVORDERGRUNDFARBE ;
                   /* Farbauswahl Dialog starten */
                   if   ((l = WinDlgBox (HWND_DESKTOP,
                                         hwndFrame_g,
                                         OptionenFarbeDlgWndProc,
                                         NULLHANDLE,
                                         IDD_OPTIONENFARBE,
                                         (PVOID) &ul)) != 9999)
                        {
                        /* Vordergrundfarbe setzen */
                        WinSendMsg (pwnd->hwndMLE,
                                    MLM_SETTEXTCOLOR,
                                    MPFROMLONG (l),
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              /* Zeilenumbruch anschalten */
              case IDM_OPTIONENZEILENUMBRUCHAN:
                   /* Bei Menueeintrag Haken setzen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAN, TRUE),
                               MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED)) ;
                   /* Bei Menueeintrag Haken entfernen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAUS, TRUE),
                               MPFROM2SHORT (MIA_CHECKED, FALSE)) ;
                   /* Neue Einstellung dem MLE mitteilen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_SETWRAP,
                               MPFROMSHORT (TRUE),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Zeilenumbruch ausschalten */
              case IDM_OPTIONENZEILENUMBRUCHAUS:
                   /* Bei Menueeintrag Haken entfernen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAN, TRUE),
                               MPFROM2SHORT (MIA_CHECKED, FALSE)) ;
                   /* Bei Menueeintrag Haken setzen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAUS, TRUE),
                               MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED)) ;
                   /* Neue Einstellung dem MLE mitteilen */
                   WinSendMsg (pwnd->hwndMLE,
                               MLM_SETWRAP,
                               MPFROMSHORT (FALSE),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Kontrollelement Event trifft ein */
      case WM_CONTROL:
           {
           PWND pwnd ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           if   ((pwnd = WinQueryWindowPtr (hwndFrame_g,
                                            QWL_USER)) != NULL)
                {
                /* Welches Kontrollelement? */
                switch (SHORT1FROMMP (mp1))
                   {
                   /* Das MLE */
                   case IDF_MLE:
                        /* Welcher Event? */
                        switch (SHORT2FROMMP (mp1))
                           {
                           /* MLE Inhalt wurde veraendert */
                           case MLN_CHANGE:
                                /* War Inhalt bereits vorher veraendert worden? */
                                if   (!pwnd->fChanged)
                                     {
                                     /* Nein, also Changed Flag setzen */
                                     pwnd->fChanged = TRUE ;
                                     /* Ist Dateiname bekannt? */
                                     if   (*pwnd->szFile)
                                          {
                                          /* Ja, Menueintrag Sichern enablen */
                                          WinSendMsg (pwnd->hwndMenu,
                                                      MM_SETITEMATTR,
                                                      MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                                                      MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                                          }
                                     /* Menueintrag Sichern als auf jeden Fall enablen */
                                     WinSendMsg (pwnd->hwndMenu,
                                                 MM_SETITEMATTR,
                                                 MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                                                 MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                                     }
                                break ;
                           /* MLE will wissen ob es weitersuchen soll */
                           case MLN_SEARCHPAUSE:
                                /* Entsprechendes Flag pruefen */
                                if   (pwnd->fStop)
                                     {
                                     /* Ja, Flag zuruecksetzen */
                                     pwnd->fStop = FALSE ;
                                     /* Suche abbrechen */
                                     return (MRESULT) TRUE ;
                                     }
                                else
                                     {
                                     /* Nein, Suche nicht abbrechen */
                                     return (MRESULT) FALSE ;
                                     }
                           }
                        break ;
                   }
                }
           break ;
           }
      /* Das Frame Fenster wird angelegt */
      case WM_CREATE:
           /* Platz fuer Instanzdaten allokieren und in den Window Words ablegen */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Verzoegerter Programmstart */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Das Fenster wird wieder geschlossen */
      case WM_DESTROY:
           {
           OPTIONEN optionen ;
           PWND     pwnd ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Clipboard oeffnen */
           if   (WinOpenClipbrd (hab_g))
                {
                /* Ich habe kein Interesse mehr an Clipboard Events */
                WinSetClipbrdViewer (hab_g,
                                     NULLHANDLE) ;
                /* Clipboard schliessen */
                WinCloseClipbrd (hab_g) ;
                }
           /* Zeilenumbruch Einstellung auslesen */
           optionen.fWordWrap = LONGFROMMR (WinSendMsg (pwnd->hwndMLE,
                                                        MLM_QUERYWRAP,
                                                        MPVOID,
                                                        MPVOID)) ;
           /* Font Einstellung auslesen */
           WinSendMsg (pwnd->hwndMLE,
                       MLM_QUERYFONT,
                       MPFROMP (&optionen.fat),
                       MPVOID) ;
           /* Hintergrundfarbe Einstellung auslesen */
           optionen.lBackground = LONGFROMMR (WinSendMsg (pwnd->hwndMLE,
                                                          MLM_QUERYBACKCOLOR,
                                                          MPVOID,
                                                          MPVOID)) ;
           /* Vordergrundfarbe Einstellung auslesen */
           optionen.lForeground = LONGFROMMR (WinSendMsg (pwnd->hwndMLE,
                                                          MLM_QUERYTEXTCOLOR,
                                                          MPVOID,
                                                          MPVOID)) ;
           /* Offene Datei Einstellung auslesen */
           strcpy (optionen.szFile,
                   pwnd->szFile) ;
           /* MLE loeschen */
           WinDestroyWindow (pwnd->hwndMLE) ;
           /* Optionen in INI schreiben */
           PrfWriteProfileData (pwnd->hini,
                                "MLE",
                                "Optionen",
                                &optionen,
                                sizeof (OPTIONEN)) ;
           /* INI schliessen */
           PrfCloseProfile (pwnd->hini) ;
           /* Instanzdaten der Applikation loeschen */
           free (pwnd) ;
           break ;
           }
      /* Ein Clipboard Event trifft ein */
      case WM_DRAWCLIPBOARD:
           {
           BOOL  f ;
           PWND  pwnd ;
           ULONG ul ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Clipboard oeffnen */
           if   (WinOpenClipbrd (hab_g))
                {
                /* Inhalt des Clipboards pruefen. Ist es Text? */
                f = WinQueryClipbrdFmtInfo (hab_g,
                                            CF_TEXT,
                                            &ul) ;
                /* Einfuegen Menueeintrag abhaengig vom gefundenen Clipboard Inhalt setzen */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_BEARBEITENEINFUEGEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, (f) ? FALSE : MIA_DISABLED)) ;
                /* Clipboard wieder schliessen */
                WinCloseClipbrd (hab_g) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Wer soll Hintergrund des Clients zeichnen? */
      case WM_ERASEBACKGROUND:
           /* PM */
           return (MRESULT) TRUE ;
      /* Gueltigkeit mancher Bearbeiten Menueeintraege koennen erst auf Anforderung festgestellt werden */
      case WM_INITMENU:
           {
           PWND  pwnd ;
           ULONG ul ;
           ULONG ul2 ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Ist es das Submenu Bearbeiten? */
           if   (SHORT1FROMMP (mp1) == IDM_BEARBEITEN)
                {
                /* Ja */
                /* Vorderer Selektionspunkt ermitteln */
                ul = LONGFROMMR (WinSendMsg (pwnd->hwndMLE,
                                             MLM_QUERYSEL,
                                             MPFROMSHORT (MLFQS_MINSEL),
                                             MPVOID)) ;
                /* Hinterer Selektionspunkt ermitteln */
                ul2 = LONGFROMMR (WinSendMsg (pwnd->hwndMLE,
                                              MLM_QUERYSEL,
                                              MPFROMSHORT (MLFQS_MAXSEL),
                                              MPVOID)) ;
                /* Sind beide Selektionspunkte gleich, dann kann Ausschneiden nicht durchgefuehrt werden */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_BEARBEITENAUSSCHNEIDEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, (ul == ul2) ? MIA_DISABLED : FALSE)) ;
                /* Sind beide Selektionspunkte gleich, dann kann Kopieren nicht durchgefuehrt werden */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_BEARBEITENKOPIEREN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, (ul == ul2) ? MIA_DISABLED : FALSE)) ;
                /* Sind beide Selektionspunkte gleich, dann kann Loeschen nicht durchgefuehrt werden */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_BEARBEITENLOESCHEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, (ul == ul2) ? MIA_DISABLED : FALSE)) ;
                /* Kann etwas rueckgaengig gemacht werden? */
                ul = SHORT1FROMMR (WinSendMsg (pwnd->hwndMLE,
                                               MLM_QUERYUNDO,
                                               MPVOID,
                                               MPVOID)) ;
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_BEARBEITENRUECKGAENGIG, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, (ul) ? FALSE : MIA_DISABLED)) ;
                }
           break ;
           }
      /* Frame wurde minimiert/maximiert */
      case WM_MINMAXFRAME:
           /* Neuen Status merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Das Frame wurde in der Groesse veraendert */
      case WM_SIZE:
           /* Groessenaenderung verzoegert durchfuehren */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Die non-modalen Dialoge wollen geloscht werden */
      case WMP_DESTROY:
           /* Dialog wegwerfen */
           WinDestroyWindow (HWNDFROMMP (mp1)) ;
           return (MRESULT) FALSE ;
      /* Verzoegerter Applikationsstart hier */
      case WMP_MAININIT:
           {
           CHAR     sz [CCHMAXPATH] ;
           OPTIONEN optionen ;
           PWND     pwnd ;
           SWP      swp ;
           ULONG    ul ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Menue Handle besorgen */
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           /* Titelzeilen Handle besorgen */
           pwnd->hwndTitlebar = WinWindowFromID (hwndFrame_g,
                                                 FID_TITLEBAR) ;
           /* Clientgroesse ermitteln */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* MLE hier reinplazieren */
           pwnd->hwndMLE = WinCreateWindow (hwndClient_g,
                                            WC_MLE,
                                            "",
                                            MLS_HSCROLL | MLS_VSCROLL | MLS_WORDWRAP | WS_VISIBLE,
                                            0,
                                            0,
                                            swp.cx,
                                            swp.cy,
                                            hwndFrame_g,
                                            HWND_TOP,
                                            IDF_MLE,
                                            NULL,
                                            NULL) ;
           /* MLE Format setzen */
           WinSendMsg (pwnd->hwndMLE,
                       MLM_FORMAT,
                       MPFROMSHORT (MLFIE_NOTRANS),
                       MPVOID) ;
           /* Eine INI anlegen bzw. oeffnen */
           pwnd->hini = TLocateIni (hab_g,
                                    "MLE.INI") ;
           /* Groesse der Optionen Struktur */
           ul = sizeof (OPTIONEN) ;
           /* INI Inhalt lesen */
           if   (PrfQueryProfileData (pwnd->hini,
                                      "MLE",
                                      "Optionen",
                                      &optionen,
                                      &ul))
                {
                /* Hintergrundfarbe setzen */
                WinSendMsg (pwnd->hwndMLE,
                            MLM_SETBACKCOLOR,
                            MPFROMLONG (optionen.lBackground),
                            MPVOID) ;
                /* Vordergrundfarbe setzen */
                WinSendMsg (pwnd->hwndMLE,
                            MLM_SETTEXTCOLOR,
                            MPFROMLONG (optionen.lForeground),
                            MPVOID) ;
                /* Font setzen */
                WinSendMsg (pwnd->hwndMLE,
                            MLM_SETFONT,
                            MPFROMP (&optionen.fat),
                            MPVOID) ;
                }
           else
                {
                /* Zeilenumbruch ist standardmaessig ausgeschaltet */
                optionen.fWordWrap = FALSE ;
                }
           /* Zeilenumbruch dem MLE mitteilen */
           WinSendMsg (pwnd->hwndMLE,
                       MLM_SETWRAP,
                       MPFROMSHORT (optionen.fWordWrap),
                       MPVOID) ;
           /* Menueeintraege zum Zeilenumbruch an/aus klicken */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAN, TRUE),
                       MPFROM2SHORT (MIA_CHECKED, (optionen.fWordWrap) ? MIA_CHECKED : FALSE)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_OPTIONENZEILENUMBRUCHAUS, TRUE),
                       MPFROM2SHORT (MIA_CHECKED, (optionen.fWordWrap) ? FALSE : MIA_CHECKED)) ;
           /* Clipboard oeffnen */
           if   (WinOpenClipbrd (hab_g))
                {
                /* Ich will ueber Clipboard Events informiert werden */
                WinSetClipbrdViewer (hab_g,
                                     hwnd) ;
                /* Clipboard wieder schliessen */
                WinCloseClipbrd (hab_g) ;
                }
           /* War in den Optionen eine geoeffnete Datei oder wurde ein Dateiname uebergeben? */
           if   (*szFile_g || *optionen.szFile)
                {
                /* Datei einlesen */
                if   (TReadFileInMLE ((*szFile_g) ? szFile_g : optionen.szFile,
                                      pwnd->hwndMLE))
                     {
                     /* Dateiname merken */
                     strcpy (pwnd->szFile,
                             (*szFile_g) ? szFile_g : optionen.szFile) ;
                     /* Dateiname in Titelzeile ausgeben */
                     strcpy (sz,
                             pszMLE_g) ;
                     strcat (sz,
                             " - ") ;
                     strcat (sz,
                             pwnd->szFile) ;
                     /* Titelzeilentext setzen */
                     WinSetWindowText (pwnd->hwndTitlebar,
                                       sz) ;
                     }
                }
/*         else
                {
                strcpy (sz,
                        pszMLE_g) ;
                strcat (sz,
                        " - ") ;
                strcat (sz,
                        "Unbekannt") ;
                WinSetWindowText (pwnd->hwndTitlebar,
                                  sz) ;
                } */
           /* Changed Flag zuruecksetzen */
           pwnd->fChanged = FALSE ;
           /* Menueintrag Sichern disablen */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DATEISICHERN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           /* Menueintrag Sichern als disablen */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DATEISICHERNALS, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           return (MRESULT) FALSE ;
           }
      /* Der Suchdialog bzw. Ersetzendialog meldet eine Suche an */
      case WMP_SEARCH:
           {
           BOOL           f ;
           MLE_SEARCHDATA mlesrch ;
           PMLECOMM       pmlecomm ;
           PWND           pwnd ;
           ULONG          ul ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Pointer der Kommunikationsstruktur wurde vom Dialog uebergeben */
           pmlecomm = PVOIDFROMMP (mp1) ;
           /* MLE Such/Ersetzen Kommunikationsstruktur initialisieren */
           memset (&mlesrch,
                   0,
                   sizeof (MLE_SEARCHDATA)) ;
           mlesrch.cb = sizeof (MLE_SEARCHDATA) ;
           /* Die Werte meiner Kommunikationsstruktur umkopieren */
           mlesrch.pchFind = pmlecomm->szSearch ;
           mlesrch.pchReplace = pmlecomm->szReplace ;
           mlesrch.cchFind = strlen (pmlecomm->szSearch) ;
           mlesrch.cchReplace = strlen (pmlecomm->szReplace) ;
           mlesrch.iptStart = (pmlecomm->fVonOben) ? 0 : -1 ;
           mlesrch.iptStop = -1 ;
           ul = (pmlecomm->fExakt) ? MLFSEARCH_CASESENSITIVE : 0 ;
           ul |= (pmlecomm->fAlle) ? MLFSEARCH_CHANGEALL : MLFSEARCH_SELECTMATCH ;
           /* Suche/Ersetzen starten */
           f = LONGFROMMP (WinSendMsg (pwnd->hwndMLE,
                                       MLM_SEARCH,
                                       MPFROMLONG (ul),
                                       MPFROMP (&mlesrch))) ;
           /* Wenn dieser Event vom Ersetzen Dialog ausgeloest wurde, dann muss hier jetzt eventuell Text ausgetauscht werden */
           if   (pmlecomm->ulDialog == IDD_BEARBEITENERSETZEN)
                {
                WinSendMsg (pwnd->hwndDialog,
                            WMP_REPLACE,
                            MPFROMLONG (f),
                            MPFROMHWND (pwnd->hwndMLE)) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Verzoegerte Groessenaenderung des Clients hier */
      case WMP_SIZE:
           {
           PWND pwnd ;
           SWP  swp ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn Fenster nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Client Masse ermitteln */
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                /* MLE neu positionieren */
                WinSetWindowPos (pwnd->hwndMLE,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                }
           return (MRESULT) FALSE ;
           }
      /* Auf dem Suchen/Ersetzen Dialog wurde der Stop Pushbutton gedrueckt */
      case WMP_STOP:
           {
           PWND pwnd ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Stop Flag setzen, so dass bei einer MLE Anfrage dessen Suche abgebrochen werden kann  */
           pwnd->fStop = LONGFROMMP (mp1) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Fensterverarbeitung */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Programmstart */
INT main (INT  argc,
          CHAR *argv [])
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_AUTOICON | FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Wurde ein Parameter uebergeben? */
   if   (argc > 1)
        {
        /* Ja, temporaer merken */
        strcpy (szFile_g,
                argv [1]) ;
        }
   /* Anmeldung beim PM */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Applikationsklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszMLE_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszMLE_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Loop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Message an Fenster verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Fenster wieder loeschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue wieder loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab_g) ;
        }
   /* Applikationsende */
   return 0 ;
   }

/* Dialogprozedur Farbe auswaehlen */
MRESULT EXPENTRY OptionenFarbeDlgWndProc (HWND   hwndDlg,
                                          MSG    msg,
                                          MPARAM mp1,
                                          MPARAM mp2)
   {
   /* Welcher Event fliesst ein */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           LONG   l ;
           PDLGOF pdlg ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion wurde ausgeloest? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel */
              case DID_CANCEL:
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  9999) ;
                   return (MRESULT) FALSE ;
              /* Ok */
              case DID_OK:
                   /* Selektiertes Elemt ermitteln */
                   l = LONGFROMMR (WinSendMsg (pdlg->hwndValueset,
                                               VM_QUERYSELECTEDITEM,
                                               MPVOID,
                                               MPVOID)) ;
                   /* Wert des selektierten Elementes ermitteln */
                   l = LONGFROMMR (WinSendMsg (pdlg->hwndValueset,
                                               VM_QUERYITEM,
                                               MPFROMLONG (l),
                                               MPVOID)) ;
                   /* Dialog loeschen */
                   WinDismissDlg (hwndDlg,
                                  l) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Kontrollelement hat etwas zu melden */
      case WM_CONTROL:
           /* Welches Kontrollelement? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Das Valueset */
              case IDVS_OF_1:
                   /* Welcher Event? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Enter */
                      case VN_ENTER:
                           /* Ok Pushbutton simulieren */
                           WinPostMsg (hwndDlg,
                                       WM_COMMAND,
                                       MPFROMSHORT (DID_OK),
                                       MPFROM2SHORT (CMDSRC_PUSHBUTTON, TRUE)) ;
                           break ;
                      }
                   break ;
              }
           break ;
      /* Diaog wird geloescht */
      case WM_DESTROY:
           {
           PDLGOF pdlg ;

           /* Pointer auf Instanzdaten aus Window Words lesen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Valueset loeschen */
           WinDestroyWindow (pdlg->hwndValueset) ;
           /* Instanzdaten loeschen */
           free (pdlg) ;
           break ;
           }
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           CHAR    sz [128] ;
           LONG    l ;
           PDLGOF  pdlg ;
           SWP     swp ;
           ULONG   ulI ;
           ULONG   ulJ ;
           VSCDATA vscd ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGOF)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Kommunikationsstruktur des Valueset initialisieren */
           memset (&vscd,
                   0,
                   sizeof (VSCDATA)) ;
           vscd.cbSize = sizeof (VSCDATA) ;
           /* Zwei Reihen */
           vscd.usRowCount = 2 ;
           /* Acht Spalten je Reihe */
           vscd.usColumnCount = 8 ;
           /* Dialogmasse ermitteln */
           WinQueryWindowPos (hwndDlg,
                              &swp) ;
           /* Valueset auf Dialog plazieren */
           pdlg->hwndValueset = WinCreateWindow (hwndDlg,
                                                 WC_VALUESET,
                                                 "",
                                                 VS_BORDER | VS_COLORINDEX | WS_VISIBLE,
                                                 18,
                                                 60,
                                                 swp.cx - 36,
                                                 swp.cy - 100,
                                                 hwndDlg,
                                                 HWND_TOP,
                                                 IDVS_OF_1,
                                                 &vscd,
                                                 NULL) ;
           /* In einer Schleife durch alle Reihen */
           for  (ulI = 1, l = CLR_BACKGROUND; ulI <= 2; ulI++)
                {
                /* In einer Schleife durch alle Spalten */
                for  (ulJ = 1; ulJ < 8; ulJ++, l++)
                     {
                     /* Farbwerte im Valueset setzen */
                     WinSendMsg (pdlg->hwndValueset,
                                 VM_SETITEM,
                                 MPFROM2SHORT (ulI, ulJ),
                                 MPFROMLONG (l)) ;
                     }
                }
           /* Uebergabe Parameter vom Client pruefen */
           switch (*((PULONG) mp2))
              {
              /* Es soll eine Hintergrundfarbe ausgewaehlt werden */
              case IDM_OPTIONENHINTERGRUNDFARBE:
                   /* Ressource ID des Textes */
                   ulI = IDS_HINTERGRUNDFARBE ;
                   break ;
              /* Es soll eine Vordergrundfarbe ausgewaehlt werden */
              case IDM_OPTIONENVORDERGRUNDFARBE:
                   /* Ressource ID des Textes */
                   ulI = IDS_VORDERGRUNDFARBE ;
                   break ;
              }
         /* Text aus Ressource laden */
         WinLoadString (hab_g,
                        NULLHANDLE,
                        ulI,
                        sizeof (sz),
                        sz) ;
         /* Titeltext setzen */
         WinSetWindowText (hwndDlg,
                           sz) ;
         return (MRESULT) FALSE ;
         }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Ein Laufwerk wird auf READONLY geprueft */
BOOL TCheckForReadOnlyDrive (CHAR ch)
   {
   BOOL  f ;
   CHAR  szBuffer [] = "C:\\~TMPTMP.TM~" ;
   FILE* pfile ;

   /* Drive letter im konstanten Text austauschen */
   *szBuffer = ch ;
   /* Schreibzugriff versuchen */
   if   ((pfile = fopen (szBuffer,
                        "w")) != NULL)
        {
        /* Datei schliessen */
        fclose (pfile) ;
        /* Datei loeschen */
        remove (szBuffer) ;
        /* Laufwerk ist nicht READONLY */
        f = TERROR ;
        }
   else
        {
        /* Laufwerk ist READONLY */
        f = TNOERROR ;
        }
   return f ;
   }

/* MLE Inhalt loeschen */
BOOL TDeleteMLEContents (HWND hwndMLE)
   {
   ULONG ul ;
   ULONG ul2 ;

   /* MLE Update ausschalten */
   WinEnableWindow (hwndMLE,
                    FALSE) ;
   /* Textlaenge ermitteln */
   ul = LONGFROMMR (WinSendMsg (hwndMLE,
                                MLM_QUERYFORMATTEXTLENGTH,
                                MPFROMLONG (0),
                                MPFROMLONG (-1))) ;
   /* In einer Schleife kleine Brocken rausloeschen */
   while (ul)
      {
      ul2 = LONGFROMMR (WinSendMsg (hwndMLE,
                                    MLM_DELETE,
                                    MPFROMLONG (0),
                                    MPFROMLONG ((ul > 32768) ? 32768 : ul))) ;
      ul -= ul2 ;
      }
   /* MLE Update wieder einschalten */
   WinEnableWindow (hwndMLE,
                    TRUE) ;
   return TNOERROR ;
   }

/* Eine INI suchen und ggfs anlegen */
HINI TLocateIni (HAB hab,
                 PSZ pszIni)
   {
   CHAR  sz [CCHMAXPATH] ;
   FILE* pfile ;
   PPIB  ppib ;
   PSZ   psz ;
   PTIB  ptib ;
   ULONG ul ;

   /* Name der INI ohne Extension */
   strcpy (sz,
           pszIni) ;
   /* Im lokalen Verzeichnis oder im DPATH? */
   if   ((pfile = fopen (pszIni,
                         "rb")) != NULL)
        {
        /* Ja, INI wieder schliessen */
        fclose (pfile) ;
        }
   else
        {
        /* Nein, Prozessinformationen abfragen */
        DosGetInfoBlocks (&ptib,
                          &ppib) ;
        /* Name des Programmmodules abfragen */
        DosQueryModuleName (ppib->pib_hmte,
                            sizeof (sz),
                            sz) ;
        /* Ist diese Lokation evtl. READONLY? */
        if   (TCheckForReadOnlyDrive (*sz) == TNOERROR)
             {
             /* Ja, dann INI ins OS2 Verzeichnis setzen */
             strcpy (sz,
                     "C:\\OS2\\") ;
             strcat (sz,
                     pszIni) ;
             /* Bootlaufwerk im konstanten Text austauschen */
             DosQuerySysInfo (QSV_BOOT_DRIVE,
                              QSV_BOOT_DRIVE,
                              &ul,
                              sizeof (ul)) ;
             *sz = ul + '@' ;
             }
        else
             {
             /* Nein, dann letzten Backslash suchen */
             if   ((psz = strrchr (sz,
                                   '\\')) != NULL)
                  {
                  /* Und INI Name hinterhaengen */
                  strcpy ((psz + 1),
                          pszIni) ;
                  }
             }
        }
   /* Es wird auf jeden Fall eine neue oder existierende INI geoeffnet */
   return PrfOpenProfile (hab,
                          sz) ;
   }

/* Dateiinhalt in MLE lesen */
BOOL TReadFileInMLE (PSZ  pszFile,
                     HWND hwndMLE)
   {
   FILE* pfile ;
   PSZ   psz ;
   ULONG ul ;
   ULONG ul2 ;
   ULONG ul3 ;
   ULONG ul4 ;
   ULONG ul5 ;

   /* Datei oeffnen */
   if   ((pfile = fopen (pszFile,
                         "r")) != NULL)
        {
        ul2 = 0 ;
        /* Datei an's Ende fahren */
        fseek (pfile,
               0,
               SEEK_END) ;
        /* Dateigroesse ermitteln */
        ul = ftell (pfile) ;
        /* Datei wieder an Anfang fahren */
        rewind (pfile) ;
        /* Speicher allokieren */
        if   ((psz = malloc (65536)) != NULL)
             {
             /* Import/Export Puffer setzen */
             WinSendMsg (hwndMLE,
                         MLM_SETIMPORTEXPORT,
                         MPFROMP (psz),
                         MPFROMLONG (32768)) ;
             /* MLE Update ausschalten */
             WinEnableWindow (hwndMLE,
                              FALSE) ;
             /* In einer Schleife kleine Brocken lesen und in's MLE importieren */
             while (ul)
                {
                memset (psz,
                        0,
                        65536) ;
                ul4 = (ul > 32768) ? 32768 : ul ;
                fread (psz,
                       1,
                       ul4,
                       pfile) ;
                ul5 = strlen (psz) ;
                ul3 = LONGFROMMR (WinSendMsg (hwndMLE,
                                              MLM_IMPORT,
                                              MPFROMP (&ul2),
                                              MPFROMLONG (min (ul4, ul5)))) ;
                if   (!ul3)
                     {
                     break ;
                     }
                else
                     {
                     ul -= ul3 ;
                     }
                } ;
             /* MLE Update wieder einschalten */
             WinEnableWindow (hwndMLE,
                              TRUE) ;
             /* Datei schliessen */
             fclose (pfile) ;
             /* Speicher wieder freigeben */
             free (psz) ;
             return TNOERROR ;
             }
        else
             {
             /* Datei schliessen */
             fclose (pfile) ;
             return TERROR ;
             }
        }
   else
        {
        return TERROR ;
        }
   }

/* MLE Inhalt in Datei schreiben */
BOOL TWriteFileFromMLE (PSZ  pszFile,
                        HWND hwndMLE)
   {
   FILE* pfile ;
   PSZ   psz ;
   ULONG ul ;
   ULONG ul2 ;
   ULONG ul3 ;
   ULONG ul4 ;

   /* Datei oeffnen */
   if   ((pfile = fopen (pszFile,
                         "w")) != NULL)
        {
        ul2 = 0 ;
        /* Textlaenge ermitteln */
        ul = LONGFROMMR (WinSendMsg (hwndMLE,
                                     MLM_QUERYFORMATTEXTLENGTH,
                                     MPFROMLONG (ul2),
                                     MPFROMLONG (-1))) ;
        /* Speicher allokieren */
        if   ((psz = malloc (65536)) != NULL)
             {
             /* Import/Export Puffer setzen */
             WinSendMsg (hwndMLE,
                         MLM_SETIMPORTEXPORT,
                         MPFROMP (psz),
                         MPFROMSHORT (32768)) ;
             /* MLE Update ausschalten */
             WinEnableWindow (hwndMLE,
                              FALSE) ;
             /* In einer Schleife kleine Brocken aus MLE lesen und in Datei schreiben */
             while (ul)
                {
                memset (psz,
                        0,
                        65536) ;
                ul4 = (ul > 32768) ? 32768 : ul ;
                ul3 = LONGFROMMR (WinSendMsg (hwndMLE,
                                              MLM_EXPORT,
                                              MPFROMP (&ul2),
                                              MPFROMP (&ul4))) ;
                if   (!ul3)
                     {
                     break ;
                     }
                else
                     {
                     fwrite (psz,
                             1,
                             ul3,
                             pfile) ;
                     ul -= ul3 ;
                     }
                }
             /* MLE Update wieder einschalten */
             WinEnableWindow (hwndMLE,
                              TRUE) ;
             /* Datei schliessen */
             fclose (pfile) ;
             /* Speicher wieder freigeben */
             free (psz) ;
             return TNOERROR ;
             }
        else
             {
             /* Datei schliessen */
             fclose (pfile) ;
             return TERROR ;
             }
        }
   else
        {
        return TERROR ;
        }
   }

/* MessageBox ermittelt selbst Texte aus STRINGTABLE */
USHORT TWriteMessage (HWND    hwnd,
                      HMODULE hmod,
                      ULONG   ulTitle,
                      ULONG   ulText,
                      ULONG   ulStyle)
   {
   CHAR szText [256] ;
   CHAR szTitle [128] ;
   HAB hab ;

   /* Anchor Block ermitteln */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Titel aus Ressource lesen */
   WinLoadString (hab,
                  hmod,
                  ulTitle,
                  sizeof (szTitle),
                  szTitle) ;
   /* Text aus Ressource lesen */
   WinLoadString (hab,
                  hmod,
                  ulText,
                  sizeof (szText),
                  szText) ;
   /* Message Box ausgeben */
   return WinMessageBox (HWND_DESKTOP,
                         hwnd,
                         szText,
                         szTitle,
                         0,
                         ulStyle) ;
   }

