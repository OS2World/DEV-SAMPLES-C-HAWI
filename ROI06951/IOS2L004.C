/* $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2L004.C 1.1 1995/05/25 18:33:37 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include <IOS2L004.H>

BOOL EXPENTRY IOS2L_RegisterClass (const HAB hab)
   {
   CLASSINFO clsi ;
   
   WinQueryClassInfo (hab,
                      WC_ENTRYFIELD,
                      &clsi) ;
   return WinRegisterClass (hab,
                            WCP_IOS2L,
                            (PFNWP) IOS2L_WndProc,
                            0,
                            clsi.cbWindowData + sizeof (PVOID)) ;
   }

typedef struct
   {
   PFNWP pfnwp ;
   } WND, *PWND ;

MRESULT EXPENTRY IOS2L_WndProc (const HWND hwnd,
                                const MSG msg,
                                const MPARAM mp1,
                                const MPARAM mp2)
   {
   CLASSINFO clsi ;
   CHAR      szBuffer [CCHMAXPATH] ;
   CHAR      szBuffer2 [CCHMAXPATH] ;
   PDRAGINFO pdinfo ;
   PDRAGITEM pditem ;
   PWND      pwnd ;

   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   switch (msg)
      {
      case DM_DRAGOVER:
           pdinfo = mp1 ;
           DrgAccessDraginfo (pdinfo) ;
           pditem = DrgQueryDragitemPtr (pdinfo,
                                         0) ;
           if   (DrgVerifyRMF (pditem,
                               "DRM_OS2FILE",
                               NULL))
                {
                DrgFreeDraginfo (pdinfo) ;
                return MRFROM2SHORT (DOR_DROP, DO_COPY) ;
                }
           else
                {
                DrgFreeDraginfo (pdinfo) ;
                return MRFROM2SHORT (DOR_NEVERDROP, 0) ;
                }
      case DM_DROP:
           pdinfo = mp1 ;
           DrgAccessDraginfo (pdinfo) ;
           pditem = DrgQueryDragitemPtr (pdinfo,
                                         0) ;
           DrgQueryStrName (pditem->hstrContainerName,
                            CCHMAXPATH,
                            szBuffer) ;
           DrgQueryStrName (pditem->hstrSourceName,
                            CCHMAXPATH,
                            szBuffer2) ;
           strcat (szBuffer,
                   szBuffer2) ;
           WinSetWindowText (hwnd,
                             szBuffer) ;
           DrgFreeDraginfo (pdinfo) ;
           break ;
      case WM_CREATE:
           pwnd = calloc (1,
                          sizeof (WND)) ;
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           WinQueryClassInfo (WinQueryAnchorBlock (hwnd),
                              WC_ENTRYFIELD,
                              &clsi) ;
           pwnd->pfnwp = clsi.pfnWindowProc ;
           WinPostMsg (hwnd,
                       EM_SETTEXTLIMIT,
                       MPFROMSHORT (CCHMAXPATH),
                       MPVOID) ;
           break ;
      case WM_DESTROY:
           pwnd->pfnwp (hwnd,
                        msg,
                        mp1,
                        mp2) ;
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            NULL) ;
           free (pwnd) ;
           return (MRESULT) FALSE ;
      }
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }
