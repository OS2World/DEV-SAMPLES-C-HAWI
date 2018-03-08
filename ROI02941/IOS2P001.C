/* $Header: D:\projekte\gib mir die daten\source\RCS\IOS2P001.C 1.1 1995/05/25 15:49:29 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "IOS2P001.H"
#include "IOS2L001.H"

typedef struct
   {
   HWND    hwndListbox ;
   } IOS2PDLG, *PIOS2PDLG ;

CHAR  chDrive_g = '\0' ;
HAB   hab_g = NULLHANDLE ;
HWND  hwndIOS2L_g = NULLHANDLE ;
HWND  hwndIOS2P_g = NULLHANDLE ;
ULONG ulWait_g = 0 ;

/* *******************************************************************
   *** Function: main                                              ***
   ******************************************************************* */
INT main (VOID)
   {
   HMQ  hmq ;
   
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             WinDlgBox (HWND_DESKTOP,
                        HWND_DESKTOP,
                        (PFNWP) IOS2P_DlgWndProc,
                        NULLHANDLE,
                        IDD_IOS2P,
                        NULL) ;
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab_g) ;
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
   CHAR       szBuffer [CCHMAXPATH] ;
   PIOS2LDATA pios2ldataReceive ;
   PIOS2LDATA pios2ldataSend ;
   PIOS2LINFO pios2linfoReceive ;
   PIOS2LINFO pios2linfoSend ;
   PIOS2PDLG  pdlg ;
   SHORT      s ;

   pdlg = WinQueryWindowPtr (hwndDlg,
                             QWL_USER) ;
   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              case IDP_IOS2P_1:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                   pios2linfoSend->hwndSender = hwndDlg ;
                   pios2ldataSend = pios2linfoSend->pData ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_IOS2P_1,
                                        sizeof (pios2ldataSend->szName),
                                        pios2ldataSend->szName) ;
                   WinPostMsg (hwndIOS2L_g,
                               WMP_IOS2LINSERT,
                               MPFROMLONG (IOS2L_INSERTPART),
                               MPFROMP (pios2linfoSend)) ;
                   WinSetDlgItemText (hwndDlg,
                                      IDE_IOS2P_1,
                                      "") ;
                   return (MRESULT) FALSE ;
              case IDP_IOS2P_2:
                   s = WinQueryLboxSelectedItem (pdlg->hwndListbox) ;
                   if   (s != LIT_NONE)
                        {
                        WinSendMsg (hwndDlg,
                                    WMP_IOS2PWAIT,
                                    MPFROMLONG (TRUE),
                                    MPVOID) ;
                        WinQueryLboxItemText (pdlg->hwndListbox,
                                              s,
                                              szBuffer,
                                              sizeof (szBuffer)) ;
                        pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                        pios2linfoSend->hwndSender = hwndDlg ;
                        pios2ldataSend = pios2linfoSend->pData ;
                        pios2ldataSend->ulKey = atol (szBuffer) ;
                        WinPostMsg (hwndIOS2L_g,
                                    WMP_IOS2LDELETE,
                                    MPFROMLONG (IOS2L_DELETEPART),
                                    MPFROMP (pios2linfoSend)) ;
                        WinSetDlgItemText (hwndDlg,
                                           IDE_IOS2P_1,
                                           "") ;
                        }
                   return (MRESULT) FALSE ;
              case IDP_IOS2P_3:
                   s = WinQueryLboxSelectedItem (pdlg->hwndListbox) ;
                   if   (s != LIT_NONE)
                        {
                        WinSendMsg (hwndDlg,
                                    WMP_IOS2PWAIT,
                                    MPFROMLONG (TRUE),
                                    MPVOID) ;
                        WinQueryLboxItemText (pdlg->hwndListbox,
                                              s,
                                              szBuffer,
                                              sizeof (szBuffer)) ;
                        pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                        pios2linfoSend->hwndSender = hwndDlg ;
                        pios2ldataSend = pios2linfoSend->pData ;
                        pios2ldataSend->ulKey = atol (szBuffer) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_IOS2P_1,
                                             sizeof (pios2ldataSend->szName),
                                             pios2ldataSend->szName) ;
                        WinPostMsg (hwndIOS2L_g,
                                    WMP_IOS2LUPDATE,
                                    MPFROMLONG (IOS2L_UPDATEPART),
                                    MPFROMP (pios2linfoSend)) ;
                        }
                   return (MRESULT) FALSE ;
              case IDP_IOS2P_4:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_IOS2P_2,
                                        sizeof (szBuffer),
                                        szBuffer) ;
                   chDrive_g = *szBuffer ;
                   _beginthread (IOS2P_CreateDBThread,
                                 NULL,
                                 81919,
                                 (PVOID) 0) ;
                   return (MRESULT) FALSE ;
              case IDP_IOS2P_5:
                   if   ((IOS2L_DropDB ()) == IOS2L_NOERROR)
                        {
                        WinSendMsg (pdlg->hwndListbox,
                                    LM_DELETEALL,
                                    MPVOID,
                                    MPVOID) ;
                        WinSetDlgItemText (hwndDlg,
                                           IDE_IOS2P_1,
                                           "") ;
                        WinSetDlgItemText (hwndDlg,
                                           IDE_IOS2P_2,
                                           "C") ;
                        WinSetDlgItemShort (hwndDlg,
                                            IDT_IOS2P_1,
                                            0,
                                            FALSE) ;
                        WinSendMsg (hwndDlg,
                                    WMP_IOS2PDBOK,
                                    MPFROMLONG (FALSE),
                                    MPVOID) ;
                        }
                   else
                        {
                        WinMessageBox (HWND_DESKTOP,
                                       HWND_DESKTOP,
                                       "Fehler beim L”schen der Datenbank.",
                                       "Fehler",
                                       0,
                                       MB_OK | MB_ERROR) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_CONTROL:
           switch (SHORT1FROMMP (mp1))
              {
              case IDL_IOS2P_1:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case LN_SELECT:
                           s = WinQueryLboxSelectedItem (pdlg->hwndListbox) ;
                           if   (s != LIT_NONE)
                                {
                                WinSendMsg (hwndDlg,
                                            WMP_IOS2PWAIT,
                                            MPFROMLONG (TRUE),
                                            MPVOID) ;
                                WinQueryLboxItemText (pdlg->hwndListbox,
                                                      s,
                                                      szBuffer,
                                                      sizeof (szBuffer)) ;
                                pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                                pios2linfoSend->hwndSender = hwndDlg ;
                                pios2ldataSend = pios2linfoSend->pData ;
                                pios2ldataSend->ulKey = atol (szBuffer) ;
                                WinPostMsg (hwndIOS2L_g,
                                            WMP_IOS2LREQUEST,
                                            MPFROMLONG (IOS2L_REQUESTPART),
                                            MPFROMP (pios2linfoSend)) ;
                                }
                           break ;
                      }
                   break ;
              }
           break ;
      case WM_CONTROLPOINTER:
           if   (ulWait_g)
                {
                return (MRESULT) WinQuerySysPointer (HWND_DESKTOP,
                                                     SPTR_WAIT,
                                                     FALSE) ;
                }
           break ;
      case WM_DESTROY:
           WinDestroyWindow (hwndIOS2L_g) ;
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           hwndIOS2P_g = hwndDlg ;
           pdlg = calloc (1,
                          sizeof (IOS2PDLG)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           pdlg->hwndListbox = WinWindowFromID (hwndDlg,
                                                IDL_IOS2P_1) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_IOS2P_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_IOS2P_2,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (1),
                              MPVOID) ;
           IOS2L_Register (hwndDlg) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2PDBCREATED:
           pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
           pios2linfoSend->hwndSender = hwndDlg ;
           WinPostMsg (hwndIOS2L_g,
                       WMP_IOS2LREQUEST,
                       MPFROMLONG (IOS2L_REQUESTCOUNT),
                       MPFROMP (pios2linfoSend)) ;
           WinSendMsg (hwndDlg,
                       WMP_IOS2PDBOK,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2LOPENED:
           WinSendMsg (hwndDlg,
                       WMP_IOS2PWAIT,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           hwndIOS2L_g = HWNDFROMMP (mp1) ;
           pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
           pios2linfoSend->hwndSender = hwndDlg ;
           WinPostMsg (hwndIOS2L_g,
                       WMP_IOS2LREQUEST,
                       MPFROMLONG (IOS2L_REQUESTCOUNT),
                       MPFROMP (pios2linfoSend)) ;
           WinSendMsg (hwndDlg,
                       WMP_IOS2PDBOK,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2LOPENERROR:
           hwndIOS2L_g = HWNDFROMMP (mp1) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_IOS2P_2,
                              "C") ;
           WinSendMsg (hwndDlg,
                       WMP_IOS2PDBOK,
                       MPFROMLONG (FALSE),
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2LSEND:
           pios2linfoReceive = mp2 ;
           switch (LONGFROMMP (mp1))
              {
              case IOS2L_DELETEPART:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_ERROR:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Fehler beim L”schen.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      case IOS2L_NOERROR:
                           WinSendDlgItemMsg (hwndDlg,
                                              IDL_IOS2P_1,
                                              LM_DELETEALL,
                                              MPVOID,
                                              MPVOID) ;
                           pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                           pios2linfoSend->hwndSender = hwndDlg ;
                           WinPostMsg (hwndIOS2L_g,
                                       WMP_IOS2LREQUEST,
                                       MPFROMLONG (IOS2L_REQUESTCOUNT),
                                       MPFROMP (pios2linfoSend)) ;
                           break ;
                      case IOS2L_NOTFOUND:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Zu l”schendes Item nicht gefunden.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      }
                   break ;
              case IOS2L_INSERTPART:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_ERROR:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Fehler beim Einfgen.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      case IOS2L_NOERROR:
                           WinSendDlgItemMsg (hwndDlg,
                                              IDL_IOS2P_1,
                                              LM_DELETEALL,
                                              MPVOID,
                                              MPVOID) ;
                           pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                           pios2linfoSend->hwndSender = hwndDlg ;
                           WinPostMsg (hwndIOS2L_g,
                                       WMP_IOS2LREQUEST,
                                       MPFROMLONG (IOS2L_REQUESTCOUNT),
                                       MPFROMP (pios2linfoSend)) ;
                           break ;
                      case IOS2L_NOTFOUND:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Neuen Schlssel nicht gefunden.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      }
                   break ;
              case IOS2L_REQUESTALL:
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_END:
                           WinSendMsg (hwndDlg,
                                       WMP_IOS2PWAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           WinSendMsg (pdlg->hwndListbox,
                                       LM_SELECTITEM,
                                       MPFROMSHORT (0),
                                       MPFROMSHORT (TRUE)) ;
                           break ;
                      case IOS2L_ERROR:
                           WinSendMsg (hwndDlg,
                                       WMP_IOS2PWAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           break ;
                      case IOS2L_NOERROR:
                           pios2ldataReceive = pios2linfoReceive->pData ;
                           _ltoa (pios2ldataReceive->ulKey,
                                  szBuffer,
                                  10) ;
                           WinInsertLboxItem (pdlg->hwndListbox,
                                              LIT_END,
                                              szBuffer) ;
                           break ;
                      case IOS2L_NOTFOUND:
                           WinSendMsg (hwndDlg,
                                       WMP_IOS2PWAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           break ;
                      }
                   break ;
              case IOS2L_REQUESTCOUNT:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   WinSetDlgItemShort (hwndDlg,
                                       IDT_IOS2P_1,
                                       pios2linfoReceive->ulEID,
                                       FALSE) ;
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_ERROR:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Fehler beim Ermitteln der Anzahl.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      case IOS2L_NOERROR:
                           WinSendMsg (hwndDlg,
                                       WMP_IOS2PWAIT,
                                       MPFROMLONG (TRUE),
                                       MPVOID) ;
                           pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                           pios2linfoSend->hwndSender = hwndDlg ;
                           WinPostMsg (hwndIOS2L_g,
                                       WMP_IOS2LREQUEST,
                                       MPFROMLONG (IOS2L_REQUESTALL),
                                       MPFROMP (pios2linfoSend)) ;
                           break ;
                      case IOS2L_NOTFOUND:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Keine Anzahl ermittelt.",
                                          "Hinweis",
                                          0,
                                          MB_OK | MB_INFORMATION) ;
                           break ;
                      }
                   break ;
              case IOS2L_REQUESTPART:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_ERROR:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Fehler beim Lesen.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      case IOS2L_NOERROR:
                           pios2ldataReceive = pios2linfoReceive->pData ;
                           WinSetDlgItemText (hwndDlg,
                                              IDE_IOS2P_1,
                                              pios2ldataReceive->szName) ;
                           break ;
                      case IOS2L_NOTFOUND:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Zu lesendes Item nicht gefunden.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      }
                   break ;
              case IOS2L_UPDATEPART:
                   WinSendMsg (hwndDlg,
                               WMP_IOS2PWAIT,
                               MPFROMLONG (FALSE),
                               MPVOID) ;
                   switch (pios2linfoReceive->ulRC)
                      {
                      case IOS2L_ERROR:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Fehler beim Ersetzen.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      case IOS2L_NOERROR:
                           break ;
                      case IOS2L_NOTFOUND:
                           WinMessageBox (HWND_DESKTOP,
                                          HWND_DESKTOP,
                                          "Zu ersetzendes Item n. gefunden.",
                                          "Fehler",
                                          0,
                                          MB_OK | MB_ERROR) ;
                           break ;
                      }
                   break ;
              }
           IOS2L_FreeData (mp2) ;
           IOS2L_FreeInfo (mp2) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2PDBOK:
           WinEnableControl (hwndDlg,
                             IDE_IOS2P_1,
                             LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDE_IOS2P_2,
                             !LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDL_IOS2P_1,
                             LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDP_IOS2P_1,
                             LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDP_IOS2P_2,
                             LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDP_IOS2P_3,
                             LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDP_IOS2P_4,
                             !LONGFROMMP (mp1)) ;
           WinEnableControl (hwndDlg,
                             IDP_IOS2P_5,
                             LONGFROMMP (mp1)) ;
           return (MRESULT) FALSE ;
      case WMP_IOS2PWAIT:
           if   (!LONGFROMMP (mp1) && !ulWait_g)
                {
                return (MRESULT) FALSE ;
                }
           (LONGFROMMP (mp1)) ? ulWait_g++ : ulWait_g-- ;
           if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                 (!LONGFROMMP (mp1) && !ulWait_g))
                {
                WinSetPointer (HWND_DESKTOP,
                               WinQuerySysPointer (HWND_DESKTOP,
                                                   (LONGFROMMP (mp1)) ?
                                                            SPTR_WAIT :
                                                            SPTR_ARROW,
                                                   FALSE)) ;
                WinEnableWindow (hwndDlg,
                                 !LONGFROMMP (mp1)) ;
                }
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }
   
/* *******************************************************************
   *** Function: IOS2P_CreateDBThread                              ***
   ******************************************************************* */
VOID IOS2P_CreateDBThread (VOID * pv)
   {
   IOS2L_CreateDB (chDrive_g) ;
   WinPostMsg (hwndIOS2P_g,
               WMP_IOS2PDBCREATED,
               MPVOID,
               MPVOID) ;
   }

