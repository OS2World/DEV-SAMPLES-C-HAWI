/* $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBARTEST.C 1.1 1995/05/25 15:37:54 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SBAR.H"

#include "SBARTEST.H"

HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;

MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   PWND pwnd ;
   
   pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,
                                             QW_PARENT),
                             QWL_USER) ;
   switch (msg)
      {
      case WM_CLOSE:
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WM_CREATE:
           pwnd = calloc (1,
                          sizeof (WND)) ;
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            pwnd) ;
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      case WM_DESTROY:
           free (pwnd) ;
           break ;
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
      case WM_MINMAXFRAME:
           pwnd->fMinimized = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      case WMP_MAININIT:
           WinSetWindowPos (hwndFrame_g,
                            HWND_TOP,
                            0,
                            0,
                            0,
                            0,
                            SWP_ACTIVATE | SWP_SHOW) ;
           return (MRESULT) FALSE ;
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

INT main (VOID)
   {
   HMQ      hmq ;
   QMSG     qmsg ;
   /* Statusbar Initialisierungsstruktur */
   SBARITEM asbi [] = {{{ "Feld 1" }, NULLHANDLE, 10, SK_LOOKIN,     DT_CENTER },
                       {{ "Feld 2" }, NULLHANDLE, 20, SK_LOOKOUT,    DT_LEFT   },
                       {{ "Feld 3" }, NULLHANDLE,  5, SK_LOOKIN,     DT_RIGHT  },
                       {{ "Feld 4" }, NULLHANDLE, 30, SK_LOOKIGNORE, DT_CENTER },
                       {{ "Feld 5" }, NULLHANDLE, -1, SK_LOOKIN,     DT_LEFT   }} ;
   ULONG    ulCreateFlags = FCF_MINMAX | FCF_SIZEBORDER | FCF_SHELLPOSITION | FCF_TITLEBAR | FCF_SYSMENU | FCF_TASKLIST ;

   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             if   (WinRegisterClass (hab_g,
                                     "SBARTEST",
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE,
                                                           &ulCreateFlags,
                                                           "SBARTEST",
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_SBARTEST,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Registrieren der Statuszeile */
                       Register (hwndFrame_g) ;
                       /* Senden der Statuszeilen Initialdaten */
                       WinSendMsg (hwndFrame_g,
                                   WMPSBAR_SETDATA,
                                   MPFROMLONG (sizeof (asbi) / sizeof (asbi [0])),
                                   MPFROMP (asbi)) ;
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Deregistrieren der Statuszeile */
                       Deregister (hwndFrame_g) ;
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }
