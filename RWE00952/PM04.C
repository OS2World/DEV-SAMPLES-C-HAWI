/* $Header: D:\projekte\fensterln mit os!2\source\RCS\PM04.C 1.1 1995/05/25 15:34:42 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PM04.H"

HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_PM03" ;

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
           switch (SHORT1FROMMP (mp1))
              {
              case IDM_DOEFFNEN:
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              DOeffnenWndProc,
                              NULLHANDLE,
                              IDD_DOEFFNEN,
                              NULL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
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
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
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

MRESULT EXPENTRY DOeffnenWndProc (HWND   hwndDlg,
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
              case DID_OK:
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_CONTROL:
           {
           CHAR  sz [65] ;
           CHAR  sz2 [65] ;
           SHORT s ;

           switch (SHORT1FROMMP (mp1))
              {
              case IDH_DO1:
              case IDH_DO2:
              case IDH_DO3:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case BN_CLICKED:
                           *sz2 = '\0' ;
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DO1,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DO1,
                                                     sizeof (sz),
                                                     sz) ;
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DO2,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DO2,
                                                     sizeof (sz),
                                                     sz) ;
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                  IDH_DO3,
                                                                  BM_QUERYCHECK,
                                                                  MPVOID,
                                                                  MPVOID)))
                                {
                                WinQueryDlgItemText (hwndDlg,
                                                     IDH_DO3,
                                                     sizeof (sz),
                                                     sz) ;
                                strcat (sz2,
                                        sz) ;
                                strcat (sz2,
                                        " ") ;
                                }
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO2,
                                              sz2) ;
                           break ;
                      }
                   break ;
              case IDL_DO1:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case LN_SELECT:
                           if   ((s = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDL_DO1,
                                                                       LM_QUERYSELECTION,
                                                                       MPFROMSHORT (LIT_FIRST),
                                                                       MPVOID))) != LIT_NONE)
                                {
                                WinSendDlgItemMsg (hwndDlg,
                                                   IDL_DO1,
                                                   LM_QUERYITEMTEXT,
                                                   MPFROM2SHORT (s, sizeof (sz)),
                                                   MPFROMP (sz)) ;
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_DO1,
                                                   sz) ;
                                }
                           break ;
                      }
                   break ;
              case IDR_DO1:
              case IDR_DO2:
              case IDR_DO3:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case BN_CLICKED:
                           WinQueryDlgItemText (hwndDlg,
                                                SHORT1FROMMP (mp1),
                                                sizeof (sz),
                                                sz) ;
                           WinSetDlgItemText (hwndDlg,
                                              IDT_DO1,
                                              sz) ;
                           break ;
                      }
                   break ;
              }
           break ;
           }
      case WM_INITDLG:
           {
           PSZ   apsz [] = { "Eintrag 1",
                             "Eintrag 2",
                             "Eintrag 3",
                             "Eintrag 4" } ;
           ULONG ulI ;

           WinSendMsg (hwndDlg,
                       WM_CONTROL,
                       MPFROM2SHORT (IDR_DO1, BN_CLICKED),
                       MPFROMHWND (WinWindowFromID (hwndDlg,
                                                    IDR_DO1))) ;
           WinSendMsg (hwndDlg,
                       WM_CONTROL,
                       MPFROM2SHORT (IDH_DO1, BN_CLICKED),
                       MPFROMHWND (WinWindowFromID (hwndDlg,
                                                    IDH_DO1))) ;
           WinSendMsg (hwndDlg,
                       WM_CONTROL,
                       MPFROM2SHORT (IDH_DO3, BN_CLICKED),
                       MPFROMHWND (WinWindowFromID (hwndDlg,
                                                    IDH_DO3))) ;
           for  (ulI = 0; ulI < sizeof (apsz) / sizeof (apsz [0]); ulI++)
                {
                WinSendDlgItemMsg (hwndDlg,
                                   IDL_DO1,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (apsz [ulI])) ;
                }
           WinSendDlgItemMsg (hwndDlg,
                              IDE_DO1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDL_DO1,
                              LM_SELECTITEM,
                              MPFROMSHORT (0),
                              MPFROMSHORT (TRUE)) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefDlgProc (hwndDlg,
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
                                                           "PM04",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PM04,
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
