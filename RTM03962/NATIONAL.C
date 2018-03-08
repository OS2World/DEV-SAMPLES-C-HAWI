/* $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/NATIONAL.C 1.1 1996/02/28 17:03:56 HaWi Exp $ */

/* System includes */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C includes */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Includes fuer neue PM Controls */
#include "CLNDR.H"

/* Applikation */
#include "NATIONAL.H"

HAB     hab_g = NULLHANDLE ;
HMODULE hmod_g = NULLHANDLE ;
HWND    hwndClient_g = NULLHANDLE ;
HWND    hwndFrame_g = NULLHANDLE ;
PSZ     pszNational_g = "National" ;

/* Client Window Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   switch (msg)
      {
      /* Applikation schliessen */
      case WM_CLOSE:
           /* Stop Message Verarbeitung */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* User Kommando */
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Sysmenu Kommando */
              case DID_CANCEL:
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Kalender Dialog */
              case IDM_DIALOGCALENDAR:
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              CalendarWndProc,
                              hmod_g,
                              IDD_DC,
                              NULL) ;
                   break ;
              }
           break ;
      /* Main window anlegen */
      case WM_CREATE:
           /* Start mit kleiner Verzoegerung */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Paint Background */
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
      /* Application start after small delay */
      case WMP_MAININIT:
           /* Show Window */
           WinSetWindowPos (hwndFrame_g,
                            HWND_TOP,
                            0,
                            0,
                            0,
                            0,
                            SWP_ACTIVATE | SWP_SHOW) ;
           return (MRESULT) FALSE ;
      }
   /* Default Window Prozedure */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Fensterprozedur des Kalender Dialoges */
MRESULT EXPENTRY CalendarWndProc (HWND   hwndDlg,
                                  MSG    msg,
                                  MPARAM mp1,
                                  MPARAM mp2)
   {
   switch (msg)
      {
      /* User Request */
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel */
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok */
              case DID_OK:
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Dialog startet hier */
      case WM_INITDLG:
           /* Dieser Termin soll im Kalender in jedem Jahr am 01.04. angezeigt werden */
           WinSendDlgItemMsg (hwndDlg,
                              IDT_DC1,
                              WMPAX_INSERTAPP,
                              MPFROMLONG (AK_INSERTIGNORE),
                              MPFROMSH2CH (0, 4, 1)) ;
           /* Dieser Termin soll im Kalender in 1996 am 02.04. angezeigt werden */
           WinSendDlgItemMsg (hwndDlg,
                              IDT_DC1,
                              WMPAX_INSERTAPP,
                              MPFROMLONG (AK_INSERTIGNORE),
                              MPFROMSH2CH (1996, 4, 2)) ;
           /* Dieser Termin soll im Kalender in jedem Jahr am 03.04. angezeigt werden */
           WinSendDlgItemMsg (hwndDlg,
                              IDT_DC1,
                              WMPAX_INSERTAPP,
                              MPFROMLONG (AK_INSERTIGNORE),
                              MPFROMSH2CH (0, 4, 3)) ;
           /* Dieser Termin soll im Kalender in 1996 am 04.04. angezeigt werden */
           WinSendDlgItemMsg (hwndDlg,
                              IDT_DC1,
                              WMPAX_INSERTAPP,
                              MPFROMLONG (AK_INSERTIGNORE),
                              MPFROMSH2CH (1996, 4, 4)) ;
           /* Kalender invalidieren */
           WinSendDlgItemMsg (hwndDlg,
                              IDT_DC1,
                              WMPAX_INVALIDATE,
                              MPVOID,
                              MPVOID) ;
           return (MRESULT) FALSE ;
      }
   /* Default Dialog Window Prozedure */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

INT main (VOID)
   {
   CHAR  sz [128] ;
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_AUTOICON | FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Beim PM anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Ressource DLL laden */
             if   (!DosLoadModule (NULL,
                                   0,
                                   "NATIONAL",
                                   &hmod_g))
                  {
                  /* Register Kalender Control */
                  if   (ARegisterClass (hab_g))
                       {
                       /* Register Fensterklasse */
                       if   (WinRegisterClass (hab_g,
                                               pszNational_g,
                                               ClientWndProc,
                                               CS_SIZEREDRAW,
                                               0))
                            {
                            /* Main window anlegen */
                            if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                                     WS_ANIMATE,
                                                                     &ulCreateFlags,
                                                                     pszNational_g,
                                                                     "",
                                                                     0,
                                                                     hmod_g,
                                                                     IDM_NATIONAL,
                                                                     &hwndClient_g)) != NULLHANDLE)
                                 {
                                 /* Applikationstitel aus Ressourcen lesen */
                                 WinLoadString (hab_g,
                                                hmod_g,
                                                IDS_NATIONAL,
                                                sizeof (sz),
                                                sz) ;
                                 /* Applikationstitel setzen */
                                 WinSetDlgItemText (hwndFrame_g,
                                                    FID_TITLEBAR,
                                                    sz) ;
                                 /* Message Schleife */
                                 while (WinGetMsg (hab_g,
                                                   &qmsg,
                                                   0,
                                                   0,
                                                   0))
                                    {
                                    WinDispatchMsg (hab_g,
                                                    &qmsg) ;
                                    }
                                 /* Main window loeschen */
                                 WinDestroyWindow (hwndFrame_g) ;
                                 }
                            }
                       }
                  /* Ressource DLL wieder freigeben */
                  DosFreeModule (hmod_g) ;
                  }
             /* Message Queue wieder loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

