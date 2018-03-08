/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF04.C 1.3 1996/02/06 19:33:40 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Anwendungskopfdatei einbinden */
#include "PMEINF04.H"

/* Modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PM04" ;

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
              case IDM_DOEFFNEN:
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              DOeffnenWndProc,
                              NULLHANDLE,
                              IDD_DOEFFNEN,
                              NULL) ;
                   return (MRESULT) FALSE ;
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
              case DID_OK:
                   /* Dialog beenden */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Ein Kontrollelement hat eine Aktion zu vermelden */
      case WM_CONTROL:
           {
           CHAR  sz [65] ;
           CHAR  sz2 [65] ;
           SHORT s ;

           /* und welches Kontrollelement meldet sich? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Die Combobox meldet sich */
              case IDC_DO1:
                   /* und was will die Combobox? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein Eintrag ist selektiert worden */
                      case LN_SELECT:
                           /* Index des selektierten Eintrags ermitteln */
                           if   ((s = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDC_DO1,
                                                                       LM_QUERYSELECTION,
                                                                       MPFROMSHORT (LIT_FIRST),
                                                                       MPVOID))) != LIT_NONE)
                                {
                                /* Text des selektierten Eintrags ermitteln */
                                WinSendDlgItemMsg (hwndDlg,
                                                   IDC_DO1,
                                                   LM_QUERYITEMTEXT,
                                                   MPFROM2SHORT (s, sizeof (sz)),
                                                   MPFROMP (sz)) ;
                                /* Text im Ausgabefeld ausgeben */
                                WinSetDlgItemText (hwndDlg,
                                                   IDT_DO3,
                                                   sz) ;
                                }
                           break ;
                      }
                   break ;
              /* Eine der drei Checkboxen meldet sich */
              case IDH_DROSA:
              case IDH_DHELLBLAU:
              case IDH_DGELB:
                   /* und was wollen die Checkboxen? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Sie sind gedrueckt worden */
                      case BN_CLICKED:
                           /* Textpuffer loeschen */
                           *sz2 = '\0' ;
                           /* Ist die Rosa Checkbox selektiert? */
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DROSA,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                /* Ja, den Text der Checkbox ermitteln */
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DROSA,
                                                     sizeof (sz),
                                                     sz) ;
                                /* Diesen Text im Puffer hinten anhaengen */
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           /* Ist die Hellblau Checkbox selektiert? */
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DHELLBLAU,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                /* Ja, den Text der Checkbox ermitteln */
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DHELLBLAU,
                                                     sizeof (sz),
                                                     sz) ;
                                /* Diesen Text im Puffer hinten anhaengen */
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           /* Ist die Gelb Checkbox selektiert? */
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DGELB,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                /* Ja, den Text der Checkbox ermitteln */
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DGELB,
                                                     sizeof (sz),
                                                     sz) ;
                                /* Diesen Text im Puffer hinten anhaengen */
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           /* Zusammengebauten Text im Ausgabefeld ausgeben */
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO1,
                                              sz2) ;
                           break ;
                      }
                   break ;
              /* Die Listbox meldet sich */
              case IDL_DO1:
                   /* und was will die Listbox? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein Eintrag ist selektiert worden */
                      case LN_SELECT:
                           /* Index des selektierten Eintrags ermitteln */
                           if   ((s = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDL_DO1,
                                                                       LM_QUERYSELECTION,
                                                                       MPFROMSHORT (LIT_FIRST),
                                                                       MPVOID))) != LIT_NONE)
                                {
                                /* Text des selektierten Eintrags ermitteln */
                                WinSendDlgItemMsg (hwndDlg,
                                                   IDL_DO1,
                                                   LM_QUERYITEMTEXT,
                                                   MPFROM2SHORT (s, sizeof (sz)),
                                                   MPFROMP (sz)) ;
                                /* Text im Eingabefeld ausgeben */
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_DO1,
                                                   sz) ;
                                }
                           break ;
                      }
                   break ;
              /* Einer der drei Radiobuttons meldet sich */
              case IDR_DROSA:
              case IDR_DHELLBLAU:
              case IDR_DGELB:
                   /* und was wollen die Radiobuttons? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Sie sind gedrueckt worden */
                      case BN_CLICKED:
                           /* Text des entsprechenden Radiobuttons ermitteln */
                           WinQueryDlgItemText (hwndDlg,
                                                SHORT1FROMMP (mp1),
                                                sizeof (sz),
                                                sz) ;
                           /* Text im Ausgabefeld ausgeben */
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO2,
                                              sz) ;
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PSZ   apsz [] = { "Zeile 1",
                             "Zeile 2",
                             "Zeile 3",
                             "Zeile 4" } ;
           ULONG ulI ;

           /* Radiobutton Rosa anklicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_DROSA,
                              BM_CLICK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* Checkbox Rosa anklicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDH_DROSA,
                              BM_CLICK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* Checkbox Gelb anklicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDH_DGELB,
                              BM_CLICK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* In einer Schleife durch alle Texte von oben */
           for  (ulI = 0; ulI < sizeof (apsz) / sizeof (apsz [0]); ulI++)
                {
                /* Text in Combobox einfuegen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDC_DO1,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (apsz [ulI])) ;
                /* Text in Listbox einfuegen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDL_DO1,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (apsz [ulI])) ;
                }
           /* Eingabefeldlaenge der Combobox festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDC_DO1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           /* Eingabefeldlaenge des Eingabefeldes festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_DO1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           /* Erste Zeile in Combobox selektieren */
           WinSendDlgItemMsg (hwndDlg,
                              IDC_DO1,
                              LM_SELECTITEM,
                              MPFROMSHORT (0),
                              MPFROMSHORT (TRUE)) ;
           /* Erste Zeile in Listbox selektieren */
           WinSendDlgItemMsg (hwndDlg,
                              IDL_DO1,
                              LM_SELECTITEM,
                              MPFROMSHORT (0),
                              MPFROMSHORT (TRUE)) ;
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
                                                           "PM Einfhrung - Beispiel 4",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PMEINF04,
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