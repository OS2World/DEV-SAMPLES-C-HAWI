/* $Header: D:/Projekte/Hin und Her/Source/rcs/DROPTEST.C 1.1 1996/06/23 21:42:35 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "DROPTEST.H"

BOOL fMinimized_g = FALSE ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszDropTest_g = "DropTest" ;

MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   switch (msg)
      {
      case DM_DRAGOVER:
           return MRFROM2SHORT (DOR_DROP, DO_COPY) ;
      case DM_DROP:
           {
           CHAR      sz [8096] ;
           CHAR      sz2 [2048] ;
           PDRAGINFO pdinfo ;
           PDRAGITEM pditem ;
           PWND      pwnd ;
           ULONG     ulI ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           *sz = '\0' ;
           WinSetWindowText (pwnd->hwndMLE,
                             "") ;
           pdinfo = PVOIDFROMMP (mp1) ;
           DrgAccessDraginfo (pdinfo) ;
           for  (ulI = 0; ulI < pdinfo->cditem; ulI++)
                {
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_OBJECTB,
                               sizeof (sz2),
                               sz2) ;
                sprintf (sz,
                         "%s%s%u\n",
                         sz,
                         sz2,
                         ulI + 1) ;
                pditem = DrgQueryDragitemPtr (pdinfo,
                                              ulI) ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_TYPEB,
                               sizeof (sz2),
                               sz2) ;
                strcat (sz,
                        sz2) ;
                DrgQueryStrName (pditem->hstrType,
                                 sizeof (sz2),
                                 sz2) ;
                strcat (sz,
                        sz2) ;
                strcat (sz,
                        "\n") ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_RMFB,
                               sizeof (sz2),
                               sz2) ;
                strcat (sz,
                        sz2) ;
                DrgQueryStrName (pditem->hstrRMF,
                                 sizeof (sz2),
                                 sz2) ;
                strcat (sz,
                        sz2) ;
                strcat (sz,
                        "\n") ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_CONTAINERB,
                               sizeof (sz2),
                               sz2) ;
                strcat (sz,
                        sz2) ;
                DrgQueryStrName (pditem->hstrContainerName,
                                 sizeof (sz2),
                                 sz2) ;
                strcat (sz,
                        sz2) ;
                strcat (sz,
                        "\n") ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_SOURCEB,
                               sizeof (sz2),
                               sz2) ;
                strcat (sz,
                        sz2) ;
                DrgQueryStrName (pditem->hstrSourceName,
                                 sizeof (sz2),
                                 sz2) ;
                strcat (sz,
                        sz2) ;
                strcat (sz,
                        "\n") ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_TARGETB,
                               sizeof (sz2),
                               sz2) ;
                strcat (sz,
                        sz2) ;
                DrgQueryStrName (pditem->hstrTargetName,
                                 sizeof (sz2),
                                 sz2) ;
                strcat (sz,
                        sz2) ;
                strcat (sz,
                        "\n") ;
                }
           WinSetWindowText (pwnd->hwndMLE,
                             sz) ;
           DrgFreeDraginfo (pdinfo) ;
           return (MRESULT) FALSE ;
           }
      case WM_CREATE:
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
      case WM_MINMAXFRAME:
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      case WM_SIZE:
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       mp1,
                       mp2) ;
           break ;
      case WMP_MAININIT:
           {
           CHAR sz [CCHMAXPATH] ;
           PWND pwnd ;
           SWP  swp ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_PROGRAMM,
                          sizeof (sz),
                          sz) ;
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz) ;
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           pwnd->hwndMLE = WinCreateWindow (hwndClient_g,
                                            WC_MLE,
                                            "",
                                            MLS_BORDER | MLS_DISABLEUNDO | MLS_HSCROLL | MLS_IGNORETAB | MLS_VSCROLL | MLS_WORDWRAP | WS_GROUP | WS_VISIBLE,
                                            0,
                                            0,
                                            swp.cx / 2,
                                            swp.cy,
                                            hwndFrame_g,
                                            HWND_TOP,
                                            IDF_DROP,
                                            NULL,
                                            NULL) ;
           WinSendMsg (pwnd->hwndMLE,
                       MLM_SETTEXTLIMIT,
                       MPFROMLONG (32767),
                       MPVOID) ;
           WinSendMsg (pwnd->hwndMLE,
                       MLM_FORMAT,
                       MPFROMSHORT (MLFIE_NOTRANS),
                       MPVOID) ;
           break ;
           }
      case WMP_SIZE:
           {
           CHAR  sz [CCHMAXPATH] ;
           HPS   hps ;
           PWND  pwnd ;
           RECTL rcl ;
           SWP   swp ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           if   (!fMinimized_g)
                {
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                WinSetWindowPos (pwnd->hwndMLE,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx / 2,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_DROPOBJECTSHERE,
                               sizeof (sz),
                               sz) ;
                hps = WinGetPS (hwndClient_g) ;
                rcl.xLeft = swp.cx / 2 ;
                rcl.yBottom = 0 ;
                rcl.xRight = swp.cx ;
                rcl.yTop = swp.cy ;
                WinDrawText (hps,
                             -1,
                             sz,
                             &rcl,
                             CLR_DARKBLUE,
                             SYSCLR_WINDOW,
                             DT_VCENTER | DT_CENTER) ;
                WinReleasePS (hps) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

VOID _setuparg (VOID)
   {
   }

INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             if   (WinRegisterClass (hab_g,
                                     pszDropTest_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszDropTest_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_DROPTEST,
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

