/* $Header: D:\projekte\dde - die zweite\source\RCS\IOS2P016.C 1.1 1995/05/25 15:27:18 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOS2P016.H"

/* *******************************************************************
   *** Function: main                                              ***
   ******************************************************************* */
INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;
   
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)                         /* Hello PM */
        {
        if   ((hmq = WinCreateMsgQueue (hab,                                   /* Create Message Queue */
                                        0)) != NULLHANDLE)
             {
             WinDlgBox (HWND_DESKTOP,                                          /* Start Dialog */
                        HWND_DESKTOP,
                        DlgWndProc,
                        NULLHANDLE,
                        IDD_DDESPY,
                        NULL) ;
             WinDestroyMsgQueue (hmq) ;                                        /* Destroy Message Queue */
             }
        WinTerminate (hab) ;                                                   /* Bye PM */
        }
   return 0 ;
   }

/* *******************************************************************
   *** Function: DlgWndProc                                        ***
   ******************************************************************* */
MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   BOOL        f ;
   CHAR        szBuffer [8192] ;
   CONVCONTEXT cctxt ;
   PDDEINIT    pddei ;
   PDDESTRUCT  pdde ;
   PDLG        pdlg ;
   PSZ         psz ;
   PSZ         pszStart ;
   SHORT       s ;
   
   pdlg = WinQueryWindowPtr (hwndDlg,                                          /* Get Pointer to dialogs instance data */
                             QWL_USER) ;
   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:                                                 /* Close application */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              case IDP_DDESPY1:                                                /* Query all running DDE server */
                   WinSendMsg (hwndDlg,
                               WMP_CLEAR,
                               MPFROMLONG (1),
                               MPVOID) ;
                   pdlg->ulRequesttype = DDESPY_REQTYPAPP ;                    /* Save type of requested connection */
                   memset (&cctxt,
                           0,
                           sizeof (CONVCONTEXT)) ;
                   cctxt.cb = sizeof (CONVCONTEXT) ;
                   cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                   cctxt.idCountry = 49 ;
                   cctxt.usCodepage = 850 ;
                   WinDdeInitiate (hwndDlg,                                    /* C'mon guys, who's there? */
                                   "",
                                   "",
                                   &cctxt) ;
                   return (MRESULT) FALSE ;
              case IDP_DDESPY2:                                                /* Query all topics of selected application */
                   WinSendMsg (hwndDlg,
                               WMP_CLEAR,
                               MPFROMLONG (2),
                               MPVOID) ;
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,    /* Query application listbox */
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (pdlg->szApplication)),
                                    MPFROMP (pdlg->szApplication)) ;
                        pdlg->ulRequesttype = DDESPY_REQTYPTOP ;               /* Save type of requested connection */
                        memset (&cctxt,
                                0,
                                sizeof (CONVCONTEXT)) ;
                        cctxt.cb = sizeof (CONVCONTEXT) ;
                        cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                        cctxt.idCountry = 49 ;
                        cctxt.usCodepage = 850 ;
                        WinDdeInitiate (hwndDlg,
                                        pdlg->szApplication,
                                        "",
                                        &cctxt) ;
                        }
                   return (MRESULT) FALSE ;
              case IDP_DDESPY3:                                                /* Query all Items of a given appilcation/topic */
                   WinSendMsg (hwndDlg,                                        /* In fact only vlid for the System topic */
                               WMP_CLEAR,
                               MPFROMLONG (3),
                               MPVOID) ;
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,    /* Query application listbox */
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (pdlg->szApplication)),
                                    MPFROMP (pdlg->szApplication)) ;
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2, /* Query topic listbox */
                                                             LM_QUERYSELECTION,
                                                             MPFROMSHORT (LIT_FIRST),
                                                             MPVOID))) != LIT_NONE)
                             {
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_QUERYITEMTEXT,
                                         MPFROM2SHORT (s, sizeof (pdlg->szTopic)),
                                         MPFROMP (pdlg->szTopic)) ;
                             pdlg->ulRequesttype = DDESPY_REQTYPITM ;          /* Save type of requested connection */
                             memset (&cctxt,
                                     0,
                                     sizeof (CONVCONTEXT)) ;
                             cctxt.cb = sizeof (CONVCONTEXT) ;
                             cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                             cctxt.idCountry = 49 ;
                             cctxt.usCodepage = 850 ;
                             WinDdeInitiate (hwndDlg,
                                             pdlg->szApplication,
                                             pdlg->szTopic,
                                             &cctxt) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              case IDP_DDESPY4:                                                /* REQUEST */
                   WinSendMsg (hwndDlg,
                               WMP_CLEAR,
                               MPFROMLONG (4),
                               MPVOID) ;
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,    /* Query application listbox */
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (pdlg->szApplication)),
                                    MPFROMP (pdlg->szApplication)) ;
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2, /* Query topic listbox */
                                                             LM_QUERYSELECTION,
                                                             MPFROMSHORT (LIT_FIRST),
                                                             MPVOID))) != LIT_NONE)
                             {
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_QUERYITEMTEXT,
                                         MPFROM2SHORT (s, sizeof (pdlg->szTopic)),
                                         MPFROMP (pdlg->szTopic)) ;
                             *pdlg->szItem = '\0' ;
                             if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox3, /* Query item listbox */
                                                                  LM_QUERYSELECTION,
                                                                  MPFROMSHORT (LIT_FIRST),
                                                                  MPVOID))) != LIT_NONE)
                                  {
                                  WinSendMsg (pdlg->hwndListbox3,
                                              LM_QUERYITEMTEXT,
                                              MPFROM2SHORT (s, sizeof (pdlg->szItem)),
                                              MPFROMP (pdlg->szItem)) ;
                                  }
                             else
                                  {
                                  WinQueryDlgItemText (hwndDlg,                /* if item listbox empty query entryfield */
                                                       IDE_DDESPY1,
                                                       sizeof (pdlg->szItem),
                                                       pdlg->szItem) ;
                                  }
                             WinQueryDlgItemText (hwndDlg,                     /* query data (MLE) */
                                                  IDF_DDESPY1,
                                                  sizeof (pdlg->szData),
                                                  pdlg->szData) ;
                             pdlg->ulRequesttype = DDESPY_REQTYPREQ ;          /* save type of requested connection */
                             memset (&cctxt,
                                     0,
                                     sizeof (CONVCONTEXT)) ;
                             cctxt.cb = sizeof (CONVCONTEXT) ;
                             cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                             cctxt.idCountry = 49 ;
                             cctxt.usCodepage = 850 ;
                             WinDdeInitiate (hwndDlg,
                                             pdlg->szApplication,
                                             pdlg->szTopic,
                                             &cctxt) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              case IDP_DDESPY5:                                                /* ADVISE */
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (pdlg->szApplication)),
                                    MPFROMP (pdlg->szApplication)) ;
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                             LM_QUERYSELECTION,
                                                             MPFROMSHORT (LIT_FIRST),
                                                             MPVOID))) != LIT_NONE)
                             {
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_QUERYITEMTEXT,
                                         MPFROM2SHORT (s, sizeof (pdlg->szTopic)),
                                         MPFROMP (pdlg->szTopic)) ;
                             *pdlg->szItem = '\0' ;
                             if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox3,
                                                                  LM_QUERYSELECTION,
                                                                  MPFROMSHORT (LIT_FIRST),
                                                                  MPVOID))) != LIT_NONE)
                                  {
                                  WinSendMsg (pdlg->hwndListbox3,
                                              LM_QUERYITEMTEXT,
                                              MPFROM2SHORT (s, sizeof (pdlg->szItem)),
                                              MPFROMP (pdlg->szItem)) ;
                                  }
                             else
                                  {
                                  WinQueryDlgItemText (hwndDlg,
                                                       IDE_DDESPY1,
                                                       sizeof (pdlg->szItem),
                                                       pdlg->szItem) ;
                                  }
                             WinQueryDlgItemText (hwndDlg,
                                                  IDF_DDESPY1,
                                                  sizeof (pdlg->szData),
                                                  pdlg->szData) ;
                             pdlg->ulRequesttype = DDESPY_REQTYPPKE ;
                             memset (&cctxt,
                                     0,
                                     sizeof (CONVCONTEXT)) ;
                             cctxt.cb = sizeof (CONVCONTEXT) ;
                             cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                             cctxt.idCountry = 49 ;
                             cctxt.usCodepage = 850 ;
                             WinDdeInitiate (hwndDlg,
                                             pdlg->szApplication,
                                             pdlg->szTopic,
                                             &cctxt) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              case IDP_DDESPY6:
                   WinSendMsg (hwndDlg,
                               WMP_CLEAR,
                               MPFROMLONG (4),
                               MPVOID) ;
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (pdlg->szApplication)),
                                    MPFROMP (pdlg->szApplication)) ;
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                             LM_QUERYSELECTION,
                                                             MPFROMSHORT (LIT_FIRST),
                                                             MPVOID))) != LIT_NONE)
                             {
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_QUERYITEMTEXT,
                                         MPFROM2SHORT (s, sizeof (pdlg->szTopic)),
                                         MPFROMP (pdlg->szTopic)) ;
                             *pdlg->szItem = '\0' ;
                             if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox3,
                                                                  LM_QUERYSELECTION,
                                                                  MPFROMSHORT (LIT_FIRST),
                                                                  MPVOID))) != LIT_NONE)
                                  {
                                  WinSendMsg (pdlg->hwndListbox3,
                                              LM_QUERYITEMTEXT,
                                              MPFROM2SHORT (s, sizeof (pdlg->szItem)),
                                              MPFROMP (pdlg->szItem)) ;
                                  }
                             else
                                  {
                                  WinQueryDlgItemText (hwndDlg,
                                                       IDE_DDESPY1,
                                                       sizeof (pdlg->szItem),
                                                       pdlg->szItem) ;
                                  }
                             WinQueryDlgItemText (hwndDlg,
                                                  IDF_DDESPY1,
                                                  sizeof (pdlg->szData),
                                                  pdlg->szData) ;
                             pdlg->ulRequesttype = DDESPY_REQTYPADV ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY1),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY2),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY3),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY4),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY5),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY6),
                                              FALSE) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY7),
                                              TRUE) ;
                             memset (&cctxt,
                                     0,
                                     sizeof (CONVCONTEXT)) ;
                             cctxt.cb = sizeof (CONVCONTEXT) ;
                             cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                             cctxt.idCountry = 49 ;
                             cctxt.usCodepage = 850 ;
                             WinDdeInitiate (hwndDlg,
                                             pdlg->szApplication,
                                             pdlg->szTopic,
                                             &cctxt) ;
                             WinEnableWindow (WinWindowFromID (hwndDlg,
                                                               IDP_DDESPY7),
                                              TRUE) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              case IDP_DDESPY7:
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY1),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY2),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY3),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY4),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY5),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY6),
                                    TRUE) ;
                   WinEnableWindow (WinWindowFromID (hwndDlg,
                                                     IDP_DDESPY7),
                                    FALSE) ;
                   WinDdePostMsg (pdlg->hwndServer,
                                  hwndDlg,
                                  WM_DDE_TERMINATE,
                                  NULL,
                                  DDEPM_RETRY) ;
                   pdlg->hwndServer = NULLHANDLE ;
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_CONTROL:
           switch (SHORT1FROMMP (mp1))
              {
              case IDL_DDESPY1:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case LN_SELECT:
                           WinSendMsg (hwndDlg,
                                       WMP_CLEAR,
                                       MPFROMLONG (2),
                                       MPVOID) ;
                           WinEnableWindow (WinWindowFromID (hwndDlg,
                                                             IDP_DDESPY3),
                                            FALSE) ;
                           break ;
                      }
                   break ;
              case IDL_DDESPY2:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case LN_SELECT:
                           WinSendMsg (hwndDlg,
                                       WMP_CLEAR,
                                       MPFROMLONG (3),
                                       MPVOID) ;
                           if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                                LM_QUERYSELECTION,
                                                                MPFROMSHORT (LIT_FIRST),
                                                                MPVOID))) != LIT_NONE)
                                {
                                WinSendMsg (pdlg->hwndListbox2,
                                            LM_QUERYITEMTEXT,
                                            MPFROM2SHORT (s, sizeof (szBuffer)),
                                            MPFROMP (szBuffer)) ;
                                if   (stricmp (szBuffer,
                                               SZDDESYS_TOPIC) == 0)
                                     {
                                     f = TRUE ;
                                     }
                                else
                                     {
                                     f = FALSE ;
                                     }
                                WinEnableWindow (WinWindowFromID (hwndDlg,
                                                                  IDP_DDESPY3),
                                                 f) ;
                                WinEnableWindow (WinWindowFromID (hwndDlg,
                                                                  IDE_DDESPY1),
                                                 !f) ;
                                }
                           break ;
                      }
                   break ;
              }
           break ;
      case WM_DDE_ACK:
           if   (!PVOIDFROMMP (mp2))
                {
                return (MRESULT) FALSE ;
                }
           pdde = PVOIDFROMMP (mp2) ;
           f = FALSE ;
           if   (pdde->fsStatus & DDE_FBUSY)
                {
                WinMessageBox (HWND_DESKTOP,
                               HWND_DESKTOP,
                               "DDE Server busy",
                               "DDE Notification",
                               0,
                               MB_OK) ;
                }
           else
                {
                if   (pdde->fsStatus & DDE_NOTPROCESSED)
                     {
                     WinMessageBox (HWND_DESKTOP,
                                    HWND_DESKTOP,
                                    "Unsupported DDE transaction data",
                                    "DDE Notification",
                                    0,
                                    MB_OK) ;
                     }
                else
                     {
                     if   (!(pdde->fsStatus & DDE_FACK))
                          {
                          f = TRUE ;
                          }
                     }
                }
           if   (!f)
                {
                WinDdePostMsg (HWNDFROMMP (mp1),
                               hwndDlg,
                               WM_DDE_TERMINATE,
                               NULL,
                               DDEPM_RETRY) ;
                }
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
      case WM_DDE_DATA:
           if   (!PVOIDFROMMP (mp2))
                {
                return (MRESULT) FALSE ;
                }
           pdde = PVOIDFROMMP (mp2) ;
           switch (pdlg->ulRequesttype)
              {
              case DDESPY_REQTYPITM:
                   strcpy (szBuffer,
                           DDES_PABDATA (pdde)) ;
                   pszStart = szBuffer ;
                   psz = pszStart ;
                   while (*psz)
                      {
                      if   ((*psz == '\t') &&
                            (psz != pszStart))
                           {
                           *psz = '\0' ;
                           if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox3,
                                                                LM_INSERTITEM,
                                                                MPFROMSHORT (LIT_END),
                                                                MPFROMP (pszStart)))) == 0)
                                {
                                WinSendMsg (pdlg->hwndListbox3,
                                            LM_SELECTITEM,
                                            MPFROMSHORT (s),
                                            MPFROMSHORT (TRUE)) ;
                                }
                           *psz = '\t' ;
                           pszStart = psz + 1 ;
                           }
                      psz++ ;
                      }
                   if   (psz != pszStart)
                        {
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox3,
                                                             LM_INSERTITEM,
                                                             MPFROMSHORT (LIT_END),
                                                             MPFROMP (pszStart)))) == 0)
                             {
                             WinSendMsg (pdlg->hwndListbox3,
                                         LM_SELECTITEM,
                                         MPFROMSHORT (s),
                                         MPFROMSHORT (TRUE)) ;
                             }
                        }
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_TERMINATE,
                                  NULL,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPADV:
                   WinSetDlgItemText (hwndDlg,
                                      IDF_DDESPY1,
                                      DDES_PABDATA (pdde)) ;
                   break ;
              case DDESPY_REQTYPREQ:
                   WinSetDlgItemText (hwndDlg,
                                      IDF_DDESPY1,
                                      DDES_PABDATA (pdde)) ;
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_TERMINATE,
                                  NULL,
                                  DDEPM_RETRY) ;
                   break ;
              }
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
      case WM_DDE_INITIATEACK:
           if   (!PVOIDFROMMP (mp2))
                {
                return (MRESULT) FALSE ;
                }
           pddei = PVOIDFROMMP (mp2) ;
           switch (pdlg->ulRequesttype)
              {
              case DDESPY_REQTYPAPP:
                   if   ((SHORT) SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                           LM_SEARCHSTRING,
                                                           MPFROM2SHORT (LSS_CASESENSITIVE, LIT_FIRST),
                                                           MPFROMP (pddei->pszAppName))) == LIT_NONE)
                        {
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                             LM_INSERTITEM,
                                                             MPFROMSHORT (LIT_END),
                                                             MPFROMP (pddei->pszAppName)))) == 0)
                             {
                             WinSendMsg (pdlg->hwndListbox1,
                                         LM_SELECTITEM,
                                         MPFROMSHORT (s),
                                         MPFROMSHORT (TRUE)) ;
                             }
                        }
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_TERMINATE,
                                  NULL,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPTOP:
                   if   ((SHORT) SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                           LM_SEARCHSTRING,
                                                           MPFROM2SHORT (LSS_CASESENSITIVE, LIT_FIRST),
                                                           MPFROMP (pddei->pszTopic))) == LIT_NONE)
                        {
                        if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                             LM_INSERTITEM,
                                                             MPFROMSHORT (LIT_END),
                                                             MPFROMP (pddei->pszTopic)))) == 0)
                             {
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_SELECTITEM,
                                         MPFROMSHORT (s),
                                         MPFROMSHORT (TRUE)) ;
                             }
                        }
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_TERMINATE,
                                  NULL,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPITM:
                   pdde = MakeDDESeg (HWNDFROMMP (mp1),
                                      SZDDESYS_ITEM_SYSITEMS,
                                      0,
                                      CF_TEXT,
                                      NULL,
                                      0) ;
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_REQUEST,
                                  pdde,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPREQ:
                   pdde = MakeDDESeg (HWNDFROMMP (mp1),
                                      pdlg->szItem,
                                      0,
                                      CF_TEXT,
                                      pdlg->szData,
                                      strlen (pdlg->szData) + 1) ;
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_REQUEST,
                                  pdde,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPPKE:
                   pdde = MakeDDESeg (HWNDFROMMP (mp1),
                                      pdlg->szItem,
                                      DDE_FACKREQ,
                                      CF_TEXT,
                                      pdlg->szData,
                                      strlen (pdlg->szData) + 1) ;
                   WinDdePostMsg (HWNDFROMMP (mp1),
                                  hwndDlg,
                                  WM_DDE_POKE,
                                  pdde,
                                  DDEPM_RETRY) ;
                   break ;
              case DDESPY_REQTYPADV:
                   pdlg->hwndServer = HWNDFROMMP (mp1) ;
                   pdde = MakeDDESeg (pdlg->hwndServer,
                                      pdlg->szItem,
                                      0,
                                      CF_TEXT,
                                      pdlg->szData,
                                      strlen (pdlg->szData) + 1) ;
                   WinDdePostMsg (pdlg->hwndServer,
                                  hwndDlg,
                                  WM_DDE_ADVISE,
                                  pdde,
                                  DDEPM_RETRY) ;
                   break ;
              }
           return (MRESULT) FALSE ;
      case WM_DDE_TERMINATE:
           if   (HWNDFROMMP (mp1) == pdlg->hwndServer)
                {
                WinDdePostMsg (pdlg->hwndServer,
                               hwndDlg,
                               WM_DDE_TERMINATE,
                               NULL,
                               DDEPM_RETRY) ;
                pdlg->hwndServer = NULLHANDLE ;
                }
           return (MRESULT) FALSE ;
      case WM_DESTROY:
           free (pdlg) ;                                                       /* free instance data for this dialog */
           break ;
      case WM_INITDLG:
           pdlg = calloc (1,                                                   /* Allocate instance data for this dialog */
                          sizeof (DLG)) ;
           WinSetWindowPtr (hwndDlg,                                           /* Save instance data pointer in window words */
                            QWL_USER,
                            pdlg) ;
           pdlg->hwndListbox1 = WinWindowFromID (hwndDlg,                      /* Query and save some window control handles */
                                                 IDL_DDESPY1) ;
           pdlg->hwndListbox2 = WinWindowFromID (hwndDlg,
                                                 IDL_DDESPY2) ;
           pdlg->hwndListbox3 = WinWindowFromID (hwndDlg,
                                                 IDL_DDESPY3) ;
           WinEnableWindow (WinWindowFromID (hwndDlg,                          /* TERMINATE not availale yet */
                                             IDP_DDESPY7),
                            FALSE) ;
           return (MRESULT) FALSE ;
      case WMP_CLEAR:                                                          /* Clear out the controls */
           switch (LONGFROMMP (mp1))
              {
              case 1:
                   WinSendMsg (pdlg->hwndListbox1,
                               LM_DELETEALL,
                               MPVOID,
                               MPVOID) ;
              case 2:
                   WinSendMsg (pdlg->hwndListbox2,
                               LM_DELETEALL,
                               MPVOID,
                               MPVOID) ;
              case 3:
                   WinSendMsg (pdlg->hwndListbox3,
                               LM_DELETEALL,
                               MPVOID,
                               MPVOID) ;
                   WinSetDlgItemText (hwndDlg,
                                      IDE_DDESPY1,
                                      "") ;
              case 4:
                   WinSetDlgItemText (hwndDlg,
                                      IDF_DDESPY1,
                                      "") ;
              }
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

/* *******************************************************************
   *** Function: MakeDDESeg                                        ***
   ******************************************************************* */
PDDESTRUCT MakeDDESeg (HWND  hwndDest,                                         /* This function allocates memory for the DDE data area */
                       PSZ   pszItemName,
                       ULONG ulStatus,
                       ULONG ulFormat,
                       PVOID pvData,
                       ULONG ulDataLen)
   {
   PDDESTRUCT pdde ;
   PID        pid ;
   TID        tid ;
   ULONG      ul ;
   ULONG      ulTotal ;

   ul = strlen (pszItemName) + 1 ;                                             /* Lenght Itemname + terminating \0 */
   ulTotal = sizeof (DDESTRUCT) + ul + ulDataLen ;                             /* sizeof DDE data area plus sizeof data */
   if (!DosAllocSharedMem ((PPVOID) &pdde,                                     /* allocate SharedMemory for DDE data area */
                           NULL,
                           ulTotal,
                           PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_GIVEABLE))
      {
      WinQueryWindowProcess (hwndDest,                                         /* Query receiving process */
                             &pid,
                             &tid) ;
      DosGiveSharedMem (&pdde,                                                 /* allow this process access to this SharedMemory */
                        pid,
                        PAG_READ | PAG_WRITE) ;
      pdde->cbData = ulDataLen ;                                               /* copy data into DDE data area */
      pdde->fsStatus = ulStatus ;
      pdde->usFormat = ulFormat ;
      pdde->offszItemName = sizeof (DDESTRUCT) ;
      pdde->offabData = (ulDataLen && pvData) ? sizeof (DDESTRUCT) + ul : 0 ;
      memcpy (DDES_PSZITEMNAME (pdde),
              pszItemName,
              strlen (pszItemName) + 1) ;
      memcpy (DDES_PABDATA (pdde),
              pvData,
              ulDataLen) ;
      return pdde ;                                                            /* return DDE data area */
      }
   return NULL ;                                                               /* error */
   }
