/* $Header: D:\projekte\dde - das dicke ende\source\RCS\IOS2P008.C 1.1 1995/05/25 15:24:59 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOS2P008.H"

/* *******************************************************************
   *** Function: ClientWndProc                                     ***
   ******************************************************************* */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   BOOL        f ;
   CONVCONTEXT cctxt ;
   DDECONV     ddeconv ;
   HPS         hps ;
   HWND        hwndTemp ;
   PDDEINIT    pddei ;
   PSTARTUP    pstartup ;
   static PSZ  apszDDETopics [] = { SZDDESYS_TOPIC,                            /* Array of all Topics */
                                    DDEP_TOPIC1,
                                    DDEP_TOPIC2,
                                    DDEP_TOPIC3,
                                    DDEP_TOPIC4,
                                    DDEP_TOPIC5,
                                    DDEP_TOPIC6 } ;
   static PSZ  apszListbox1 [] = { "Listbox1 - Item 1",                        /* Startdata for left Listbox */
                                   "Listbox1 - Item 2",
                                   "Listbox1 - Item 3" } ;
   static PSZ  apszListbox2 [] = { "Listbox2 - Item 1",                        /* Startdata for right Listbox */
                                   "Listbox2 - Item 2",
                                   "Listbox2 - Item 3" } ;
   PWND        pwnd ;
   RECTL       rcl ;
   SHORT       s ;
   ULONG       ulI ;
   ULONG       ulJ ;
   
   pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,                             /* get pointer to instancedata */
                                             QW_PARENT),
                             QWL_USER) ;
   switch (msg)
      {
      case WM_CLOSE:
         for (ulI = 0;                                                         /* Query all DDE connections for active ones */
              ulI < DDEP_MAXDDELINKS;                                          /* issue a private terminating message */
              ulI++)
            {
            if (pwnd->ahwndDDE [ulI])
               {
               WinSendMsg (pwnd->ahwndDDE [ulI],
                           WMP_DDEDESTROY,
                           MPVOID,
                           MPVOID) ;
               WinDestroyWindow (pwnd->ahwndDDE [ulI]) ;
               }
            }
         WinDestroyWindow (pwnd->hwndListbox1) ;                               /* Destroy left Listbox */
         WinDestroyWindow (pwnd->hwndListbox2) ;                               /* Destroy right listbox */
         WinPostMsg (hwnd,                                                     /* Messageloop breaker */
                     WM_QUIT,
                     MPVOID,
                     MPVOID) ;
         free (pwnd) ;                                                         /* free instancedata */
         return (MRESULT) FALSE ;
      case WM_CONTROL:
         switch (SHORT1FROMMP (mp1))
            {
            case IDL_IOS2P_1:
               switch (SHORT2FROMMP (mp1))
                  {
                  case LN_SELECT:
                     for (ulI = 0;                                             /* If the user selects an item in the left listbox */
                          ulI < DDEP_MAXDDELINKS;                              /* then all active DDE connections are checked */
                          ulI++)                                               /* and informed that DDE data changed */
                        {
                        if (pwnd->ahwndDDE [ulI])
                           {
                           WinPostMsg (pwnd->ahwndDDE [ulI],
                                       WMP_DDEUPDATE,
                                       MPVOID,
                                       MPVOID) ;
                           }
                        }
                     break ;
                  }
               break ;
            }
         break ;
      case WM_CREATE:
         pwnd = calloc (1,                                                     /* allocate instancedata */
                        sizeof (WND)) ;
         WinSetWindowPtr (WinQueryWindow (hwnd,                                /* Place address of instancedata in window words */
                                          QW_PARENT),
                          QWL_USER,
                          pwnd) ;
         WinPostMsg (hwnd,                                                     /* My own start */
                     WMP_CREATE,
                     MPVOID,
                     MPVOID) ;
         break ;
      case WM_DDE_ACK:
         if (PVOIDFROMMP (mp2))                                                /* DDE Client did send ACK */
            {
            DosFreeMem (PVOIDFROMMP (mp2)) ;
            }
         return (MRESULT) TRUE ;
      case WM_DDE_INITIATE:
         pddei = (PDDEINIT) PVOIDFROMMP (mp2) ;                                /* A DDE Request comes in */
         if (!stricmp (DDEP_APPLICATION,                                       /* AppName specified? */
                       pddei->pszAppName))
            {
            f = FALSE ;
            for (ulI = 0;                                                      /* Topicname specified? */
                 ulI < sizeof (apszDDETopics) / sizeof (apszDDETopics [0]);
                 ulI++)
               {
               if (!stricmp (apszDDETopics [ulI],
                             pddei->pszTopic))
                  {
                  f = TRUE ;
                  break ;
                  }
               }
            if (f)                                                             /* Yep! */
               {
               f = FALSE ;
               for (ulI = 0;                                                   /* Is there room for one more DDE connection? */
                    ulI < DDEP_MAXDDELINKS;
                    ulI++)
                  {
                  if (!pwnd->ahwndDDE [ulI])
                     {
                     f = TRUE ;
                     break ;
                     }
                  }
               if (f)                                                          /* Found free place for a new DDE connections */
                  {
                  pwnd->ahwndDDE [ulI] = WinCreateWindow (HWND_OBJECT,         /* Create DDE object window */
                                                          WCP_DDE,
                                                          "",
                                                          WS_DISABLED,
                                                          0,
                                                          0,
                                                          0,
                                                          0,
                                                          HWND_OBJECT,
                                                          HWND_BOTTOM,
                                                          ulI,
                                                          NULL,
                                                          NULL) ;
                  if (pwnd->ahwndDDE [ulI] != NULLHANDLE)                      /* OK? */
                     {
                     strcpy (ddeconv.szTopic,                                  /* Fill in startdata for this DDE object instance */
                             pddei->pszTopic) ;
                     ddeconv.hwndDDEClient = HWNDFROMMP (mp1) ;
                     ddeconv.hwndParent = hwnd ;
                     WinSendMsg (pwnd->ahwndDDE [ulI],
                                 WMP_DDECREATE,
                                 MPFROMP (&ddeconv),
                                 MPVOID) ;
                     memset (&cctxt,
                             0,
                             sizeof (cctxt)) ;
                     cctxt.cb = sizeof (cctxt) ;
                     cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                     cctxt.idCountry = 49 ;
                     cctxt.usCodepage = 850 ;
                     WinDdeRespond (HWNDFROMMP (mp1),                          /* Send ACK to requesting application */
                                    pwnd->ahwndDDE [ulI],
                                    DDEP_APPLICATION,
                                    pddei->pszTopic,
                                    &cctxt) ;
                     }
                  }
               }
            }
         if ((!*pddei->pszAppName) ||                                          /* Empty Appname and empty topicname? */
             (!*pddei->pszTopic))
            {
            for (ulI = 0;                                                      /* Send all topics to requesting application */
                 ulI < sizeof (apszDDETopics) / sizeof (apszDDETopics [0]);    /* There must be one connection per topic */
                 ulI++)                                                        /* Client has to terminate all unwanted topics */
               {
               f = FALSE ;
               for (ulJ = 0;
                    ulJ < DDEP_MAXDDELINKS;
                    ulJ++)
                  {
                  if (!pwnd->ahwndDDE [ulJ])
                     {
                     f = TRUE ;
                     break ;
                     }
                  }
               if (f)
                  {
                  pwnd->ahwndDDE [ulJ] = WinCreateWindow (HWND_OBJECT,
                                                          WCP_DDE,
                                                          "",
                                                          WS_DISABLED,
                                                          0,
                                                          0,
                                                          0,
                                                          0,
                                                          HWND_OBJECT,
                                                          HWND_BOTTOM,
                                                          ulJ,
                                                          NULL,
                                                          NULL) ;
                  if (pwnd->ahwndDDE [ulJ] != NULLHANDLE)
                     {
                     strcpy (ddeconv.szTopic,
                             apszDDETopics [ulI]) ;
                     ddeconv.hwndDDEClient = HWNDFROMMP (mp1) ;
                     ddeconv.hwndParent = hwnd ;
                     WinSendMsg (pwnd->ahwndDDE [ulJ],
                                 WMP_DDECREATE,
                                 MPFROMP (&ddeconv),
                                 MPVOID) ;
                     memset (&cctxt,
                             0,
                             sizeof (cctxt)) ;
                     cctxt.cb = sizeof (cctxt) ;
                     cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                     cctxt.idCountry = 49 ;
                     cctxt.usCodepage = 850 ;
                     WinDdeRespond (HWNDFROMMP (mp1),
                                    pwnd->ahwndDDE [ulJ],
                                    DDEP_APPLICATION,
                                    apszDDETopics [ulI],
                                    &cctxt) ;
                     }
                  }
               }
            }
         return (MRESULT) TRUE ;
      case WM_MINMAXFRAME:
         pwnd->fMinimized = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;    /* Save Minimized style */
         break ;
      case WM_PAINT:
         hps = WinBeginPaint (hwnd,                                            /* Clear background of client area */
                              NULLHANDLE,
                              &rcl) ;
         WinFillRect (hps,
                      &rcl,
                      SYSCLR_FIELDBACKGROUND) ;
         WinEndPaint (hps) ;
         return (MRESULT) FALSE ;
      case WM_SIZE:
         WinPostMsg (hwnd,                                                     /* Our own PAINT function */
                     WMP_SIZE,
                     MPVOID,
                     MPVOID) ;
         break ;
      case WMP_CREATE:
         WinQueryWindowRect (hwnd,                                             /* Query client size */
                             &rcl) ;
         pwnd->hwndListbox1 = WinCreateWindow (hwnd,                           /* Create left listbox */
                                               WC_LISTBOX,
                                               "",
                                               WS_VISIBLE,
                                               10,
                                               10,
                                               (rcl.xRight - 30) / 2,
                                               rcl.yTop - 20,
                                               pwnd->hwndFrame,
                                               HWND_TOP,
                                               IDL_IOS2P_1,
                                               NULL,
                                               NULL) ;
         for (ulI = 0;                                                         /* Insert initial data into left listbox */
              ulI < sizeof (apszListbox1) / sizeof (apszListbox1 [0]);
              ulI++)
            {
            WinSendMsg (pwnd->hwndListbox1,
                        LM_INSERTITEM,
                        MPFROMSHORT (LIT_END),
                        MPFROMP (apszListbox1 [ulI])) ;
            }
         WinSendMsg (pwnd->hwndListbox1,                                       /* Select first listbox entry */
                     LM_SELECTITEM,
                     MPFROMSHORT (0),
                     MPFROMSHORT (TRUE)) ;
         pwnd->hwndListbox2 = WinCreateWindow (hwnd,                           /* Create right listbox */
                                               WC_LISTBOX,
                                               "",
                                               WS_VISIBLE,
                                               ((rcl.xRight - 30) / 2) + 20,
                                               10,
                                               (rcl.xRight - 30) / 2,
                                               rcl.yTop - 20,
                                               pwnd->hwndFrame,
                                               HWND_TOP,
                                               IDL_IOS2P_2,
                                               NULL,
                                               NULL) ;
         for (ulI = 0;                                                         /* Insert initial data */
              ulI < sizeof (apszListbox2) / sizeof (apszListbox2 [0]);
              ulI++)
            {
            WinSendMsg (pwnd->hwndListbox2,
                        LM_INSERTITEM,
                        MPFROMSHORT (LIT_END),
                        MPFROMP (apszListbox2 [ulI])) ;
            }
         WinSendMsg (pwnd->hwndListbox2,                                       /* Select first row */
                     LM_SELECTITEM,
                     MPFROMSHORT (0),
                     MPFROMSHORT (TRUE)) ;
         return (MRESULT) FALSE ;
      case WMP_DDECOUNT:                                                       /* DDE Client queried it's object window for count of DDE connections */
         ulJ = 0 ;
         for (ulI = 0;
              ulI < DDEP_MAXDDELINKS;
              ulI++)
            {
            if (pwnd->ahwndDDE [ulI])
               {
               ulJ++ ;
               }
            }
         return MRFROMLONG (ulJ) ;
      case WMP_DDEDESTROY:                                                     /* DDE Client terminated it's DDE object */
         WinDestroyWindow (HWNDFROMMP (mp1)) ;                                 /* DDE object window sends this message to free this DDE object */
         for (ulI = 0;
              ulI < DDEP_MAXDDELINKS;
              ulI++)
            {
            if (pwnd->ahwndDDE [ulI] == HWNDFROMMP (mp1))
               {
               pwnd->ahwndDDE [ulI] = NULLHANDLE ;
               break ;
               }
            }
         return (MRESULT) FALSE ;
      case WMP_DELETELISTBOXITEM:                                              /* DDE client want's to delete a listbox entry */
         if (LONGFROMMP (mp1) == IDL_IOS2P_1)                                  /* Left listbox? */
            {
            hwndTemp = pwnd->hwndListbox1 ;                                    /* Yes! */
            }
         else
            {
            hwndTemp = pwnd->hwndListbox2 ;                                    /* Right listbox! */
            }
         WinSendMsg (hwndTemp,                                                 /* Delete item */
                     LM_DELETEITEM,
                     MPFROMSHORT (SHORT1FROMMP (mp2)),
                     MPVOID) ;
         return (MRESULT) FALSE ;
      case WMP_INSERTLISTBOXITEM:                                              /* DDE client want's to insert listbox item */
         if (LONGFROMMP (mp1) == IDL_IOS2P_1)
            {
            hwndTemp = pwnd->hwndListbox1 ;
            }
         else
            {
            hwndTemp = pwnd->hwndListbox2 ;
            }
         WinSendMsg (hwndTemp,                                                 /* Insert text */
                     LM_INSERTITEM,
                     MPFROMSHORT (LIT_END),
                     MPFROMP (PVOIDFROMMP (mp2))) ;
         return (MRESULT) FALSE ;
      case WMP_QUERYLISTBOXITEM:                                               /* DDE client want's currently selected value */
         if (LONGFROMMP (mp1) == IDL_IOS2P_1)
            {
            hwndTemp = pwnd->hwndListbox1 ;
            }
         else
            {
            hwndTemp = pwnd->hwndListbox2 ;
            }
         s = SHORT1FROMMR (WinSendMsg (hwndTemp,                               /* Query index of selected item */
                                       LM_QUERYSELECTION,
                                       MPFROMSHORT (LIT_FIRST),
                                       MPVOID)) ;
         if (s != LIT_NONE)                                                    /* Something selected? */
            {
            WinSendMsg (hwndTemp,                                              /* Yep, return item text */
                        LM_QUERYITEMTEXT,
                        MPFROM2SHORT (s, 64),
                        MPFROMP (PVOIDFROMMP (mp2))) ;
            return MRFROMLONG (FALSE) ;
            }
         else
            {
            return MRFROMLONG (TRUE) ;
            }
      case WMP_SIZE:                                                           /* Resize listboxes */
         if (!pwnd->fMinimized)                                                /* only when window visible */
            {
            WinQueryWindowRect (hwnd,                                          /* Query client size */
                                &rcl) ;
            WinSetWindowPos (pwnd->hwndListbox1,                               /* Left listbox */
                             HWND_TOP,
                             10,
                             10,
                             (rcl.xRight - 30) / 2,
                             rcl.yTop - 20,
                             SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE) ;
            WinSetWindowPos (pwnd->hwndListbox2,                               /* Right listbox */
                             HWND_TOP,
                             ((rcl.xRight - 30) / 2) + 20,
                             10,
                             (rcl.xRight - 30) / 2,
                             rcl.yTop - 20,
                             SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE) ;
            }
         return (MRESULT) FALSE ;
      case WMP_STARTUP:                                                        /* Startdata from main() */
         pstartup = (PSTARTUP) PVOIDFROMMP (mp1) ;                             /* Copy into instancedata */
         pwnd->hab = pstartup->hab ;
         pwnd->hwndClient = pstartup->hwndClient ;
         pwnd->hwndFrame = pstartup->hwndFrame ;
         return (MRESULT) FALSE ;
      }
   return (WinDefWindowProc (hwnd,
                             msg,
                             mp1,
                             mp2)) ;
   }

/* *******************************************************************
   *** Function: DDEWndProc                                        ***
   ******************************************************************* */
MRESULT EXPENTRY DDEWndProc (HWND   hwnd,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   BOOL       f ;
   CHAR       szBuffer [2048] ;
   PDDECONV   pddeconv ;
   PDDESTRUCT pdde ;
   PDDESTRUCT pddeNew ;
   PWNDDDE    pwnddde ;
   ULONG      ulI ;
   
   pwnddde = WinQueryWindowPtr (hwnd,                                          /* Instancedata from this DDE object window */
                                QWL_USER) ;
   switch (msg)
      {
      case WM_CREATE:                                                          /* Create a new DDE object window */
         pwnddde = calloc (1,                                                  /* allocate instance data for this DDE object */
                           sizeof (WNDDDE)) ;
         WinSetWindowPtr (hwnd,                                                /* Save pointer in window words */
                          QWL_USER,
                          pwnddde) ;
         break ;
      case WM_DDE_ACK:                                                         /* DDE client sends ACK */
         if (PVOIDFROMMP (mp2))
            {
            DosFreeMem (PVOIDFROMMP (mp2)) ;
            }
         return (MRESULT) FALSE ;
      case WM_DDE_ADVISE:                                                      /* Automatic DDE connection requested */
         if (!PVOIDFROMMP (mp2))                                               /* Oops, no data */
            {
            return (MRESULT) FALSE ;
            }
         pwnddde->ulRequestType = msg ;                                        /* Save type of connection (ADVISE) */
         pwnddde->hwndDDEClient = HWNDFROMMP (mp1) ;
         pdde = (PDDESTRUCT) PVOIDFROMMP (mp2) ;                               /* Get DDE struct */
         f = FALSE ;
         strcpy (pwnddde->szItemName,                                          /* Copy itemname in instance data */
                 DDES_PSZITEMNAME (pdde)) ;
         if (!stricmp (DDEP_TOPIC1,                                            /* This topic allows ADVISE connection */
                       pwnddde->szTopic))
            {
            f = TRUE ;
            }
         if (f)                                                                /* ADVISE connection is allowed */
            {
            pwnddde->fImmediate = (pdde->fsStatus & DDE_FNODATA) ? FALSE : TRUE ; /* If update of data ACK or direct send data to client? */
            if (pdde->fsStatus & DDE_FACKREQ)                                  /* DDE client want's ACK if ADVISE possible */
               {
               pddeNew = MakeDDESeg (HWNDFROMMP (mp1),
                                     DDES_PSZITEMNAME (pdde),
                                     DDE_FACK,
                                     CF_TEXT,
                                     NULL,
                                     0) ;
               WinDdePostMsg (HWNDFROMMP (mp1),
                              hwnd,
                              WM_DDE_ACK,
                              pddeNew,
                              DDEPM_RETRY) ;
               }
            }
         else                                                                  /* Negative ACK because requested Topic does not allow ADVISE */
            {
            pddeNew = MakeDDESeg (HWNDFROMMP (mp1),
                                  DDES_PSZITEMNAME (pdde),
                                  DDE_NOTPROCESSED,
                                  CF_TEXT,
                                  NULL,
                                  0) ;
            WinDdePostMsg (HWNDFROMMP (mp1),
                           hwnd,
                           WM_DDE_ACK,
                           pddeNew,
                           DDEPM_RETRY) ;
            }
         DosFreeMem (pdde) ;
         return (MRESULT) FALSE ;
      case WM_DDE_POKE:                                                        /* DDE client sends data */
         if (!PVOIDFROMMP (mp2))
            {
            return (MRESULT) FALSE ;
            }
         pwnddde->ulRequestType = msg ;                                        /* Save type of connection (POKE) */
         pwnddde->hwndDDEClient = HWNDFROMMP (mp1) ;
         pdde = (PDDESTRUCT) PVOIDFROMMP (mp2) ;                               /* Get DDE struct */
         f = FALSE ;
         if (!stricmp (DDEP_TOPIC3,                                            /* Insert in left listbox? */
                       pwnddde->szTopic))
            {
            f = TRUE ;                                                         /* Yes! */
            WinSendMsg (pwnddde->hwndParent,                                   /* Send this to client window */
                        WMP_INSERTLISTBOXITEM,
                        MPFROMLONG (IDL_IOS2P_1),
                        MPFROMP (DDES_PABDATA (pdde))) ;
            }
         if (!stricmp (DDEP_TOPIC4,                                            /* Insert in right listbox? */
                       pwnddde->szTopic))
            {
            f = TRUE ;                                                         /* Yes! */
            WinSendMsg (pwnddde->hwndParent,                                   /* Send this to client window */
                        WMP_INSERTLISTBOXITEM,
                        MPFROMLONG (IDL_IOS2P_2),
                        MPFROMP (DDES_PABDATA (pdde))) ;
            }
         if (!stricmp (DDEP_TOPIC5,                                            /* Delete item from left listbox? */
                       pwnddde->szTopic))
            {
            f = TRUE ;                                                         /* Yes! */
            WinSendMsg (pwnddde->hwndParent,                                   /* Send this to client window */
                        WMP_DELETELISTBOXITEM,
                        MPFROMLONG (IDL_IOS2P_1),
                        MPFROMSHORT (atol (DDES_PABDATA (pdde)))) ;
            }
         if (!stricmp (DDEP_TOPIC6,                                            /* Delete item from right listbox? */
                       pwnddde->szTopic))
            {
            f = TRUE ;                                                         /* Yes! */
            WinSendMsg (pwnddde->hwndParent,                                   /* Send this to client window */
                        WMP_DELETELISTBOXITEM,
                        MPFROMLONG (IDL_IOS2P_2),
                        MPFROMSHORT (atol (DDES_PABDATA (pdde)))) ;
            }
         if (f)                                                                /* Everything's allowed */
            {
            if (pdde->fsStatus & DDE_FACKREQ)                                  /* ACK requested? */
               {
               pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                         /* Yes */
                                     DDES_PSZITEMNAME (pdde),
                                     DDE_FACK,
                                     CF_TEXT,
                                     DDES_PABDATA (pdde),
                                     strlen (DDES_PABDATA (pdde)) + 1) ;
               WinDdePostMsg (HWNDFROMMP (mp1),
                              hwnd,
                              WM_DDE_ACK,
                              pddeNew,
                              DDEPM_RETRY) ;
               }
            }
         else                                                                  /* Don't know requested DDE topic */
            {
            pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                            /* Negative ACK */
                                  DDES_PSZITEMNAME (pdde),
                                  DDE_NOTPROCESSED,
                                  CF_TEXT,
                                  DDES_PABDATA (pdde),
                                  strlen (DDES_PABDATA (pdde)) + 1) ;
            WinDdePostMsg (HWNDFROMMP (mp1),
                           hwnd,
                           WM_DDE_ACK,
                           pddeNew,
                           DDEPM_RETRY) ;
            }
         DosFreeMem (PVOIDFROMMP (mp2)) ;
         return (MRESULT) FALSE ;
      case WM_DDE_REQUEST:                                                     /* DDE request from DDE client */
         if (!PVOIDFROMMP (mp2))
            {
            return (MRESULT) FALSE ;
            }
         pwnddde->ulRequestType = msg ;                                        /* Save type of connection (REQUEST) */
         pwnddde->hwndDDEClient = HWNDFROMMP (mp1) ;
         pdde = (PDDESTRUCT) PVOIDFROMMP (mp2) ;                               /* Get DDE struct */
         f = FALSE ;
         if (!stricmp (SZDDESYS_ITEM_FORMATS,                                  /* System Topic "Formats" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;
            strcpy (szBuffer,                                                  /* TAB delimited text is allowed */
                    SZFMT_TEXT) ;
            }
         if (!stricmp (SZDDESYS_ITEM_HELP,                                     /* System Topic "Help" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;
            strcpy (szBuffer,                                                  /* Send something back */
                    "Helptext for DDE") ;
            }
         if (!stricmp (SZDDESYS_ITEM_RESTART,                                  /* System Topic "Restart" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;                                                         /* Send AppName back */
            strcpy (szBuffer,
                    "IOS2P008.EXE") ;
            }
         if (!stricmp (SZDDESYS_ITEM_STATUS,                                   /* System Topic "Status" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;
            ulI = LONGFROMMR (WinSendMsg (pwnddde->hwndParent,                 /* Query count of DDE connections from client window */
                                          WMP_DDECOUNT,
                                          MPVOID,
                                          MPVOID)) ;
            sprintf (szBuffer,                                                 /* Send count back */
                     "%s%u",
                     "Active DDE Connections: ",
                     ulI) ;
            }
         if (!stricmp (SZDDESYS_ITEM_SYSITEMS,                                 /* System Topic "SysItems" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;                                                         /* Send all SysItems */
            sprintf (szBuffer,
                     "%s\t%s\t%s\t%s\t%s\t%s",
                     SZDDESYS_ITEM_FORMATS,
                     SZDDESYS_ITEM_HELP,
                     SZDDESYS_ITEM_RESTART,
                     SZDDESYS_ITEM_STATUS,
                     SZDDESYS_ITEM_SYSITEMS,
                     SZDDESYS_ITEM_TOPICS) ;
            }
         if (!stricmp (SZDDESYS_ITEM_TOPICS,                                   /* System Topic "Topics" */
                       DDES_PSZITEMNAME (pdde)))
            {
            f = TRUE ;                                                         /* Send all Topics */
            sprintf (szBuffer,
                     "%s\t%s\t%s\t%s\t%s\t%s\t%s",
                     SZDDESYS_TOPIC,
                     DDEP_TOPIC1,
                     DDEP_TOPIC2,
                     DDEP_TOPIC3,
                     DDEP_TOPIC4,
                     DDEP_TOPIC5,
                     DDEP_TOPIC6) ;
            }
         if (f)                                                                /* Valid System Item encountered */
            {
            pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                            /* Positive ACK */
                                  DDES_PSZITEMNAME (pdde),
                                  DDE_FRESPONSE,
                                  CF_TEXT,
                                  szBuffer,
                                  strlen (szBuffer) + 1) ;
            WinDdePostMsg (HWNDFROMMP (mp1),
                           hwnd,
                           WM_DDE_DATA,
                           pddeNew,
                           DDEPM_RETRY) ;
            DosFreeMem (pdde) ;
            return (MRESULT) FALSE ;
            }
         f = FALSE ;                                                           /* Now look for our own topics */
         if (!stricmp (DDEP_TOPIC1,                                            /* Query item from left listbox */
                       pwnddde->szTopic))
            {
            if (!LONGFROMMR (WinSendMsg (pwnddde->hwndParent,                  /* Get data from client window */
                                         WMP_QUERYLISTBOXITEM,
                                         MPFROMLONG (IDL_IOS2P_1),
                                         MPFROMP (szBuffer))))
               {
               f = TRUE ;
               }
            }
         if (!stricmp (DDEP_TOPIC2,                                            /* Query item from right listbox */
                       pwnddde->szTopic))
            {
            if (!LONGFROMMR (WinSendMsg (pwnddde->hwndParent,                  /* Get data from client window */
                                         WMP_QUERYLISTBOXITEM,
                                         MPFROMLONG (IDL_IOS2P_2),
                                         MPFROMP (szBuffer))))
               {
               f = TRUE ;
               }
            }
         if (f)
            {
            pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                            /* Send data to DDE client */
                                  DDES_PSZITEMNAME (pdde),
                                  DDE_FRESPONSE,
                                  CF_TEXT,
                                  szBuffer,
                                  strlen (szBuffer) + 1) ;
            WinDdePostMsg (HWNDFROMMP (mp1),
                           hwnd,
                           WM_DDE_DATA,
                           pddeNew,
                           DDEPM_RETRY) ;
            }
         else                                                                  /* Error */
            {
            pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                            /* Negative ACK */
                                  DDES_PSZITEMNAME (pdde),
                                  DDE_NOTPROCESSED,
                                  CF_TEXT,
                                  NULL,
                                  0) ;
            WinDdePostMsg (HWNDFROMMP (mp1),
                           hwnd,
                           WM_DDE_ACK,
                           pddeNew,
                           DDEPM_RETRY) ;
            }
         DosFreeMem (pdde) ;
         return (MRESULT) FALSE ;
      case WM_DDE_TERMINATE:                                                   /* DDE client stops connection */
         WinDdePostMsg (HWNDFROMMP (mp1),                                      /* Always ACK */
                        hwnd,
                        WM_DDE_TERMINATE,
                        NULL,
                        DDEPM_RETRY) ;
         WinPostMsg (pwnddde->hwndParent,                                      /* Inform client window to free blocked connection */
                     WMP_DDEDESTROY,
                     MPFROMHWND (hwnd),
                     MPVOID) ;
         return (MRESULT) FALSE ;
      case WM_DDE_UNADVISE:                                                    /* No longer ADVISE */
         if (PVOIDFROMMP (mp2))
            {
            pdde = (PDDESTRUCT) PVOIDFROMMP (mp2) ;                            /* Get DDE Struct */
            if (pdde->fsStatus & DDE_FACKREQ)                                  /* ACK? */
               {
               pddeNew = MakeDDESeg (HWNDFROMMP (mp1),                         /* Yes */
                                     DDES_PSZITEMNAME (pdde),
                                     DDE_FACK,
                                     CF_TEXT,
                                     NULL,
                                     0) ;
               WinDdePostMsg (HWNDFROMMP (mp1),
                              hwnd,
                              WM_DDE_ACK,
                              pddeNew,
                              DDEPM_RETRY) ;
               }
            pwnddde->ulRequestType = 0 ;                                       /* Free connection type ... just in case */
            DosFreeMem (pdde) ;
            }
         return (MRESULT) FALSE ;
      case WM_DESTROY:                                                         /* DDE object window deleted */
         free (pwnddde) ;                                                      /* free instance data for this connection */
         break ;
      case WMP_DDECREATE:                                                      /* Startdata from client window */
         pddeconv = (PDDECONV) PVOIDFROMMP (mp1) ;
         pwnddde->hwndParent = pddeconv->hwndParent ;                          /* Handle of Parent (Client window) */
         pwnddde->hwndDDEClient = pddeconv->hwndDDEClient ;                    /* HWND of DDE Partner */
         strcpy (pwnddde->szTopic,                                             /* Requested topic */
                 pddeconv->szTopic) ;
         return (MRESULT) FALSE ;
      case WMP_DDEDESTROY:                                                     /* Application terminated. Client window want's DDE object to terminate connection */
         WinDdePostMsg (pwnddde->hwndDDEClient,
                        hwnd,
                        WM_DDE_TERMINATE,
                        NULL,
                        DDEPM_RETRY) ;
         return (MRESULT) FALSE ;
      case WMP_DDEUPDATE:                                                      /* Application data changed (in client window) */
         if (pwnddde->ulRequestType == WM_DDE_ADVISE)                          /* Do we hold an ADVISE connection? */
            {
            f = FALSE ;                                                        /* Yes! */
            if (!stricmp (DDEP_TOPIC1,                                         /* Only for this topic allowed */
                          pwnddde->szTopic))
               {
               f = TRUE ;                                                      /* Query data from client window */
               WinSendMsg (pwnddde->hwndParent,
                           WMP_QUERYLISTBOXITEM,
                           MPFROMLONG (IDL_IOS2P_1),
                           MPFROMP (szBuffer)) ;
               }
            if (f)
               {
               if (pwnddde->fImmediate)                                        /* ACK or data to DDE client? */
                  {
                  pddeNew = MakeDDESeg (pwnddde->hwndDDEClient,                /* Send data immediately */
                                        pwnddde->szItemName,
                                        0,
                                        CF_TEXT,
                                        szBuffer,
                                        strlen (szBuffer) + 1) ;
                  WinDdePostMsg (pwnddde->hwndDDEClient,
                                 hwnd,
                                 WM_DDE_DATA,
                                 pddeNew,
                                 DDEPM_RETRY) ;
                  }
               else                                                            /* DDE Client want's ACK when data changes */
                  {
                  pddeNew = MakeDDESeg (pwnddde->hwndDDEClient,
                                        pwnddde->szItemName,
                                        DDE_FNODATA,
                                        CF_TEXT,
                                        NULL,
                                        0) ;
                  WinDdePostMsg (pwnddde->hwndDDEClient,
                                 hwnd,
                                 WM_DDE_DATA,
                                 pddeNew,
                                 DDEPM_RETRY) ;
                  }
               }
            }
         return (MRESULT) FALSE ;
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* *******************************************************************
   *** Function: main                                              ***
   ******************************************************************* */
INT main (VOID)                                                                /*  Application start */
   {
   HMQ          hmq ;
   PSTARTUP     pstartup ;
   QMSG         qmsg ;
   static ULONG ulCreateFlags = FCF_MINMAX | FCF_SIZEBORDER |
                                FCF_SHELLPOSITION | FCF_TITLEBAR |
                                FCF_SYSMENU | FCF_TASKLIST ;
   
   if ((pstartup = calloc (1,                                                  /* Startup values for Client proc */
                           sizeof (STARTUP))) != (PSTARTUP) NULL)
      {
      if ((pstartup->hab = WinInitialize (QV_OS2)) != NULLHANDLE)              /* Hello PM */
         {
         if ((hmq = WinCreateMsgQueue (pstartup->hab,                          /* Create Messagequeue */
                                       0)) != NULLHANDLE)
            {
            if (WinRegisterClass (pstartup->hab,                               /* Register Class for DDE Objectwindows */
                                  WCP_DDE,
                                  (PFNWP) DDEWndProc,
                                  0,
                                  sizeof (PVOID)))                             /* Additional memory for DDE instancedata pointer */
               {
               if (WinRegisterClass (pstartup->hab,                            /* Register application */
                                     WCP_IOS2P,
                                     (PFNWP) ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  if ((pstartup->hwndFrame = WinCreateStdWindow (HWND_DESKTOP, /* Create Standard window */
                                                                 WS_VISIBLE | WS_ANIMATE,
                                                                 &ulCreateFlags,
                                                                 WCP_IOS2P,
                                                                 "",
                                                                 0,
                                                                 NULLHANDLE,
                                                                 IDW_IOS2P,
                                                                 &pstartup->hwndClient)) != NULLHANDLE)
                     {
                     WinSendMsg (pstartup->hwndClient,                         /* Send initial data to client window proc */
                                 WMP_STARTUP,
                                 MPFROMP (pstartup),
                                 MPVOID) ;
                     while (WinGetMsg (pstartup->hab,                          /* Message loop */
                                       &qmsg,
                                       0,
                                       0,
                                       0))
                        {
                        WinDispatchMsg (pstartup->hab,
                                        &qmsg) ;
                        }
                     WinDestroyWindow (pstartup->hwndFrame) ;                  /* Destroy application frame */
                     }
                  }
               }
            WinDestroyMsgQueue (hmq) ;                                         /* Delete Message queue */
            }
         WinTerminate (pstartup->hab) ;                                        /* Bye PM */
         }
      free (pstartup) ;                                                        /* Free startup data area */
      }
   return 0 ;
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
