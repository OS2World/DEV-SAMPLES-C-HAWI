/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF03.C 1.2 1996/01/26 11:18:16 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Anwendungskopfdatei einbinden */
#include "PMEINF03.H"

/* Modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PMEINF03" ;

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
              /* Informationsdialog gewuenscht */
              case IDM_HINFORMATION:
                   /* Dialog starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              HInformationWndProc,
                              NULLHANDLE,
                              IDD_HINFORMATION,
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

/* Fensterprozedur des Informationsdialoges */
MRESULT EXPENTRY HInformationWndProc (HWND   hwndDlg,
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
           return (MRESULT) FALSE ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           SWP swp ;
           SWP swpParent ;
           SWP swpDesktop ;

           /* Position des Dialoges ermitteln */
           WinQueryWindowPos (hwndDlg,
                              &swp) ;
           /* Position des Framewindows ermitteln */
           WinQueryWindowPos (hwndFrame_g,
                              &swpParent) ;
           /* Position des Desktops ermitteln */
           WinQueryWindowPos (HWND_DESKTOP,
                              &swpDesktop) ;
           /* Position auf der Mitte des Frames berechnen */
           if   (((swpParent.cx - swp.cx) / 2 + swpParent.x) < 0 ||
                 ((swpParent.cy - swp.cy) / 2 + swpParent.y) < 0 ||
                 ((swpParent.cx - swp.cx) / 2 + swpParent.x + swp.cx) > swpDesktop.cx ||
                 ((swpParent.cy - swp.cy) / 2 + swpParent.y + swp.cy) > swpDesktop.cy )
                {
                swpParent = swpDesktop ;
                }
           /* Dialog auf dem Frame zentrieren */
           WinSetWindowPos (hwndDlg,
                            HWND_TOP,
                            (swpParent.cx - swp.cx) / 2 + swpParent.x,
                            (swpParent.cy - swp.cy) / 2 + swpParent.y,
                            0,
                            0,
                            SWP_MOVE) ;
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
                                                           "PM EinfÅhrung - Beispiel 3",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PMEINF03,
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