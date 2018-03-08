/* $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF01.C 1.2 1996/01/26 11:11:38 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* Anwendungskopfdatei einbinden */
#include "PMEINF01.H"

/* Modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PMEINF01" ;

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
      /* Client Hintergrund muss gezeichnet werden */
      case WM_ERASEBACKGROUND:
           /* PM soll selbst zeichnen */
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
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

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
                                                           "PM EinfÅhrung - Beispiel 1",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PMEINF01,
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