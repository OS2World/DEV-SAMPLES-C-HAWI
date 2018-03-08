/* $Header: D:/Projekte/Hin und Her/Source/rcs/EXTTEXT.C 1.1 1996/06/23 21:42:36 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "EXTTEXT.H"

INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;

   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             if   (RegisterClass (hab))
                  {
                  WinDlgBox (HWND_DESKTOP,
                             HWND_DESKTOP,
                             DlgWndProc,
                             NULLHANDLE,
                             IDD,
                             NULL) ;
                  }
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab) ;
        }
   return 0 ;
   }

MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   switch (msg)
      {
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

BOOL RegisterClass (HAB hab)
   {
   CLASSINFO clsi ;

   WinQueryClassInfo (hab,
                      WC_STATIC,
                      &clsi) ;
   return WinRegisterClass (hab,
                            WCP_EXTTEXT,
                            WndProc,
                            0,
                            clsi.cbWindowData + sizeof (PVOID)) ;
   }

MRESULT EXPENTRY WndProc (HWND   hwnd,
                          MSG    msg,
                          MPARAM mp1,
                          MPARAM mp2)
   {
   PWND pwnd ;

   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      case DM_DRAGOVER:
           {
           PDRAGINFO pdinfo ;
           PDRAGITEM pditem ;

           pdinfo = PVOIDFROMMP (mp1) ;
           if   (DrgAccessDraginfo (pdinfo))
                {
                if   ((pditem = DrgQueryDragitemPtr (pdinfo,
                                                     0)) != NULL)
                     {
                     if   (DrgVerifyRMF (pditem,
                                         "DRM_OS2FILE",
                                         NULL))
                          {
                          DrgFreeDraginfo (pdinfo) ;
                          return MRFROM2SHORT (DOR_DROP, DO_COPY) ;
                          }
                     }
                }
           DrgFreeDraginfo (pdinfo) ;
           break ;
           }
      case DM_DROP:
           {
           CHAR      sz [CCHMAXPATH] ;
           CHAR      sz2 [CCHMAXPATH] ;
           PDRAGINFO pdinfo ;
           PDRAGITEM pditem ;

           pdinfo = PVOIDFROMMP (mp1) ;
           if   (DrgAccessDraginfo (pdinfo))
                {
                if   ((pditem = DrgQueryDragitemPtr (pdinfo,
                                                     0)) != NULL)
                     {
                     DrgQueryStrName (pditem->hstrContainerName,
                                      CCHMAXPATH,
                                      sz) ;
                     DrgQueryStrName (pditem->hstrTargetName,
                                      CCHMAXPATH,
                                      sz2) ;
                     strcat (sz,
                             sz2) ;
                     WinSetWindowText (hwnd,
                                       sz) ;
                     }
                }
           DrgFreeDraginfo (pdinfo) ;
           break ;
           }
      case WM_CREATE:
           {
           CLASSINFO clsi ;

           pwnd = calloc (1,
                          sizeof (WND)) ;
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_STATIC,
                              &clsi) ;
           pwnd->pfnwp = clsi.pfnWindowProc ;
           break ;
           }
      case WM_DESTROY:
           pwnd->pfnwp (hwnd,
                        msg,
                        mp1,
                        mp2) ;
           free (pwnd) ;
           return (MRESULT) FALSE ;
      }
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }

