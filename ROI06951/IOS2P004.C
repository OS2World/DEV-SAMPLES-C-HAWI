/* $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2P004.C 1.1 1995/05/25 18:34:03 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "IOS2P004.H"
#include "IOS2L004.H"

/* *******************************************************************
   *** Function: main                                              ***
   ******************************************************************* */
INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;
   
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             if   (IOS2L_RegisterClass (hab))
                  {
                  WinDlgBox (HWND_DESKTOP,
                             HWND_DESKTOP,
                             (PFNWP) IOS2P_DlgWndProc,
                             NULLHANDLE,
                             IDD_IOS2P,
                             NULL) ;
                  }
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab) ;
        }
   return 0 ;
   }

/* *******************************************************************
   *** Function: IOS2P_DlgWndProc                                  ***
   ******************************************************************* */
MRESULT EXPENTRY IOS2P_DlgWndProc (const HWND hwndDlg,
                                   const MSG msg,
                                   const MPARAM mp1,
                                   const MPARAM mp2)
   {
   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }
