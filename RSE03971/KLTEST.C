/* $Header: Y:/Projekte/Redaktion SE/Knopfleiste/Source/rcs/KLTEST.C 1.1 1996/12/29 13:49:43 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* Knopfleiste einbinden */
#include "KLEISTE.H"

/* Anwendungskopfdatei einbinden */
#include "KLTEST.H"

/* C einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Modulglobale Variablen */
CHAR sz_g [256] = "" ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_KLEISTE" ;

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
      /* Was wird gewÅnscht? */
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              case DID_CANCEL:
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENAUSSCHNEIDENAKTIVIEREN:
              case IDM_SCHALTENAUSSCHNEIDENDEAKTIVIEREN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ACTIVATEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENAUSSCHNEIDEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENAUSSCHNEIDENAKTIVIEREN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENAUSSCHNEIDENANSCHALTEN:
              case IDM_SCHALTENAUSSCHNEIDENAUSSCHALTEN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ENABLEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENAUSSCHNEIDEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENAUSSCHNEIDENANSCHALTEN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENEINFUEGENAKTIVIEREN:
              case IDM_SCHALTENEINFUEGENDEAKTIVIEREN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ACTIVATEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENEINFUEGEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENEINFUEGENAKTIVIEREN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENEINFUEGENANSCHALTEN:
              case IDM_SCHALTENEINFUEGENAUSSCHALTEN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ENABLEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENEINFUEGEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENEINFUEGENANSCHALTEN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENKOPIERENAKTIVIEREN:
              case IDM_SCHALTENKOPIERENDEAKTIVIEREN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ACTIVATEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENKOPIEREN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENKOPIERENAKTIVIEREN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENKOPIERENANSCHALTEN:
              case IDM_SCHALTENKOPIERENAUSSCHALTEN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ENABLEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENKOPIEREN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENKOPIERENANSCHALTEN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENLOESCHENAKTIVIEREN:
              case IDM_SCHALTENLOESCHENDEAKTIVIEREN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ACTIVATEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENLOESCHEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENLOESCHENAKTIVIEREN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_SCHALTENLOESCHENANSCHALTEN:
              case IDM_SCHALTENLOESCHENAUSSCHALTEN:
                   WinSendMsg (hwndFrame_g,
                               WMPKLEISTE_ENABLEBUTTON,
                               MPFROMLONG (IDM_BEARBEITENLOESCHEN),
                               MPFROMLONG ((SHORT1FROMMP (mp1) == IDM_SCHALTENLOESCHENANSCHALTEN) ? TRUE : FALSE)) ;
                   return (MRESULT) FALSE ;
              case IDM_BEARBEITENAUSSCHNEIDEN:
                   strcpy (sz_g,
                           "Bearbeiten-Ausschneiden gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_BEARBEITENEINFUEGEN:
                   strcpy (sz_g,
                           "Bearbeiten-EinfÅgen gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_BEARBEITENKOPIEREN:
                   strcpy (sz_g,
                           "Bearbeiten-Kopieren gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_BEARBEITENLOESCHEN:
                   strcpy (sz_g,
                           "Bearbeiten-Lîschen gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_DATEIOEFFNEN:
                   strcpy (sz_g,
                           "Datei-ôffnen gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_DATEISCHLIESSEN:
                   strcpy (sz_g,
                           "Datei-Schlie·en gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_HILFE:
                   strcpy (sz_g,
                           "Hilfe gedrÅckt") ;
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Verzîgerter Start */
      case WM_CREATE:
           WinPostMsg (hwnd,
                       WMP_CREATE,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Text ausgeben */
      case WM_PAINT:
           {
           HPS    hps ;
           POINTL ptl ;
           
           ptl.x = 100 ;
           ptl.y = 100 ;
           hps = WinBeginPaint (hwnd,
                                0,
                                NULL) ;
           GpiErase (hps) ;
           GpiCharStringAt (hps,
                            &ptl,
                            strlen (sz_g),
                            sz_g) ;
           WinEndPaint (hps) ;
           return (MRESULT) FALSE ;
           }
      /* Verzîgerter Start hier */
      case WMP_CREATE:
           WinSetWindowPos (hwndFrame_g,
                            HWND_TOP,
                            0,
                            0,
                            0,
                            0,
                            SWP_ACTIVATE | SWP_SHOW) ;
           return (MRESULT) TRUE ;
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
   HMQ         hmq ;
   /* Buttonbar             Nr ACTIVATE ENABLE BUTTONID            MENUID */
   KLEISTEITEM aitem [] = {{  1, TRUE,  TRUE,  IDKLEISTE_OPEN,     IDM_DATEIOEFFNEN           },
                           {  2, TRUE,  TRUE,  IDKLEISTE_CLOSE,    IDM_DATEISCHLIESSEN        },
                           {  3, TRUE,  TRUE,  0,                  0                          },
                           {  4, TRUE,  TRUE,  IDKLEISTE_CUT,      IDM_BEARBEITENAUSSCHNEIDEN },
                           {  5, TRUE,  TRUE,  IDKLEISTE_COPY,     IDM_BEARBEITENKOPIEREN     },
                           {  6, TRUE,  FALSE, IDKLEISTE_PASTE,    IDM_BEARBEITENEINFUEGEN    },
                           {  7, TRUE,  TRUE,  IDKLEISTE_TRASH,    IDM_BEARBEITENLOESCHEN     },
                           {  8, TRUE,  TRUE,  0,                  0                          },
                           {  9, TRUE,  TRUE,  IDKLEISTE_EXIT,     DID_CANCEL                 },
                           { 10, TRUE,  TRUE,  IDKLEISTE_HELP,     IDM_HILFE                  }} ;
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
                                                           WS_ANIMATE,
                                                           &ulCreateFlags,
                                                           pszClass_g,
                                                           "Knopfleiste",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_KLTEST,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Buttonbar registrieren */
                       KLeisteRegister (hwndFrame_g) ;
                       /* Daten des Buttonbar setzen */
                       WinSendMsg (hwndFrame_g,
                                   WMPKLEISTE_INSERTDATA,
                                   MPFROMLONG (sizeof (aitem) / sizeof (aitem [0])),
                                   MPFROMP (aitem)) ;
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
                       /* Buttonbar deregistrieren */
                       KLeisteDeregister (hwndFrame_g) ;
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
