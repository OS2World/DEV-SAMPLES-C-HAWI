/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF02.C 1.2 1996/01/26 11:16:13 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Anwendungskopfdatei einbinden */
#include "PMEINF02.H"

/* Modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PMEINF02" ;

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
           {
           PWND pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* und welche Aktion wird gewuenscht? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Farbe Rosa ausgewaehlt */
              case IDM_FROSA:
                   /* Gewaehlte Farbe in den Instanzdaten merken */
                   pwnd->lColor = CLR_PINK ;
                   /* Clientfenster invalidieren */
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              /* Farbe Hellblau ausgewaehlt */
              case IDM_FHELLBLAU:
                   /* Gewaehlte Farbe in den Instanzdaten merken */
                   pwnd->lColor = CLR_CYAN ;
                   /* Clientfenster invalidieren */
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              /* Farbe Gelb ausgewaehlt */
              case IDM_FGELB:
                   /* Gewaehlte Farbe in den Instanzdaten merken */
                   pwnd->lColor = CLR_YELLOW ;
                   /* Clientfenster invalidieren */
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
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
      /* Fenster wurde invalidiert und muss neu gezeichnet werden */
      case WM_PAINT:
           {
           HPS   hps ;
           PWND  pwnd ;
           RECTL rcl ;

           /* Pointer auf Instanzdaten aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Ist ueberhaupt eine Farbe ausgewaehlt? */
           if   (!pwnd->lColor)
                {
                /* Nein, also Standardfarbe fuer Hintergrund verwenden */
                pwnd->lColor = SYSCLR_APPWORKSPACE ;
                }
           /* Einen Presentation Space zum Zeichnen erzeugen */
           hps = WinBeginPaint (hwndClient_g,
                                NULLHANDLE,
                                &rcl) ;
           /* Clientfenster mit gewuenschter Farbe ausfuellen */
           WinFillRect (hps,
                        &rcl,
                        pwnd->lColor) ;
           /* Presentation Space wieder loeschen */
           WinEndPaint (hps) ;
           break ;
           }
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
                                                           "PM EinfÅhrung - Beispiel 2",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PMEINF02,
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