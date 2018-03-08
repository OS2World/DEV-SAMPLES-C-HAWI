/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM02.C 1.1 1995/05/25 15:32:49 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PM02.H"

HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PM02" ;

MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   switch (msg)
      {
      case WM_CLOSE:
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WM_COMMAND:
           {
           PWND pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              case IDM_FBLAU:
                   pwnd->lColor = CLR_BLUE ;
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_FGRUEN:
                   pwnd->lColor = CLR_GREEN ;
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              case IDM_FROT:
                   pwnd->lColor = CLR_RED ;
                   WinInvalidateRect (hwndClient_g,
                                      NULL,
                                      FALSE) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      case WM_CREATE:
           {
           PWND pwnd ;

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
           }
      case WM_DESTROY:
           {
           PWND pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           free (pwnd) ;
           break ;
           }
      case WM_PAINT:
           {
           HPS   hps ;
           PWND  pwnd ;
           RECTL rcl ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           if   (!pwnd->lColor)
                {
                pwnd->lColor = SYSCLR_APPWORKSPACE ;
                }
           hps = WinBeginPaint (hwndClient_g,
                                NULLHANDLE,
                                &rcl) ;
           WinFillRect (hps,
                        &rcl,
                        pwnd->lColor) ;
           WinEndPaint (hps) ;
           break ;
           }
      case WMP_MAININIT:
           {
           PWND pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             if   (WinRegisterClass (hab_g,
                                     pszClass_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszClass_g,
                                                           "PM02",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PM02,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }
