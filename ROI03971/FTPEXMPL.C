/* $Header: y:/projekte/redaktion os!2 inside/Gute Verbindung/Source/rcs/FTPEXMPL.C 1.2 1997/01/03 17:29:32 HaWi Exp $ */

/* Presentation Manager Kopfdatei einbinden */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C einbinden */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* FTP einbinden */
#define  OS2
#include "ftpapi.h"

/* Anwendungskopfdatei einbinden */
#include "FTPEXMPL.H"

/* Modulglobale Variablen */
BOOL fMinimized_g = FALSE ;
BOOL fTrace_g = FALSE ;
CHAR sz_g [256] = "" ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszClass_g = "WCP_FTPEXMPL" ;

/* Fensterprozedur fuer das Hauptfenster */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Message trifft gerade ein? */
   switch (msg)
      {
      /* Anwendung wurde geschlossen */
      case WM_CLOSE:
           {
           PWND pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           pwnd->fClosePending = TRUE ;
           _beginthread (ThreadLogoff,
                         NULL,
                         80000,
                         pwnd) ;
           return (MRESULT) FALSE ;
           }
      /* Was wird gewnscht? */
      case WM_COMMAND:
           {
           CHAR         sz [CCHMAXPATH] ;
           PUSRRECLOKAL pusrreclokal ;
           PUSRRECLOKAL pusrreclokal2 ;
           PWND         pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              case DID_CANCEL:
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              case IDM_DATEIENTFERNTEDATEIENLOESCHEN:
                   _beginthread (ThreadLoeschenEntfernt,
                                 NULL,
                                 80000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              case IDM_DATEIENTFERNTEDATEIENUEBERTRAGEN:
                   _beginthread (ThreadTransferEntfernt,
                                 NULL,
                                 80000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              case IDM_DATEILOKALEDATEIENLOESCHEN:
                   if   ((pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                                                  CM_QUERYRECORDEMPHASIS,
                                                                  MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                  MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        while (pusrreclokal)
                           {
                           strcpy (sz,
                                   pwnd->szLokal) ;
                           if   (pwnd->szLokal [strlen (pwnd->szLokal) - 1] != '\\')
                                {
                                strcat (sz,
                                        "\\") ;
                                }
                           strcat (sz,
                                   pusrreclokal->szFilename) ;
                           remove (sz) ;
                           pusrreclokal2 = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                                                    CM_QUERYRECORDEMPHASIS,
                                                                    MPFROMP (pusrreclokal),
                                                                    MPFROMSHORT (CRA_SELECTED))) ;
                           WinSendMsg (pwnd->hwndLokal,
                                       CM_REMOVERECORD,
                                       MPFROMP (&pusrreclokal),
                                       MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
                           pusrreclokal = pusrreclokal2 ;
                           }
                        }
                   return (MRESULT) FALSE ;
              case IDM_DATEILOKALEDATEIENUEBERTRAGEN:
                   _beginthread (ThreadTransferLokal,
                                 NULL,
                                 80000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      case WM_CONTROL:
           {
           PSZ             psz ;
           PUSRRECENTFERNT pusrrecentfernt ;
           PUSRRECLOKAL    pusrreclokal ;
           PWND            pwnd ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              case IDCO_ENTFERNT:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case CN_ENTER:
                           if   ((pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                                             CM_QUERYRECORDEMPHASIS,
                                                                             MPFROMP ((PRECORDCORE) CMA_FIRST),
                                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                                {
                                switch (pusrrecentfernt->recordtype)
                                   {
                                   case RECORDTYPEDIRECTORY:
                                        if   ((pusrrecentfernt->szFilename [0] == '.') &&
                                              (pusrrecentfernt->szFilename [1] == '.'))
                                             {
                                             if   ((psz = strrchr (pwnd->szEntfernt,
                                                                   '/')) != NULL)
                                                  {
                                                  *psz = '\0' ;   
                                                  }
                                             else
                                                  {
                                                  pwnd->szEntfernt [0] = '.' ;
                                                  pwnd->szEntfernt [1] = '\0' ;
                                                  }
                                             }
                                        else
                                             {
                                             if   (*pwnd->szEntfernt == '.')
                                                  {
                                                  *pwnd->szEntfernt = '\0' ;
                                                  }
                                             else
                                                  {
                                                  strcat (pwnd->szEntfernt,
                                                          "/") ; 
                                                  }
                                             strcat (pwnd->szEntfernt,
                                                     pusrrecentfernt->szFilename) ;
                                             }
                                        _beginthread (ThreadEntfernt,
                                                      NULL,
                                                      80000,
                                                      pwnd) ;
                                        break ;
                                   }
                                }
                           break ;
                      }
                   break ;
              case IDCO_LOKAL:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case CN_ENTER:
                           if   ((pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                                                          CM_QUERYRECORDEMPHASIS,
                                                                          MPFROMP ((PRECORDCORE) CMA_FIRST),
                                                                          MPFROMSHORT (CRA_SELECTED)))) != NULL)
                                {
                                switch (pusrreclokal->recordtype)
                                   {
                                   case RECORDTYPEDIRECTORY:
                                        if   ((pusrreclokal->szFilename [0] == '.') &&
                                              (pusrreclokal->szFilename [1] == '.'))
                                             {
                                             if   ((psz = strrchr (pwnd->szLokal,
                                                                   '\\')) != NULL)
                                                  {
                                                  *psz = '\0' ;   
                                                  }
                                             if   (strlen (pwnd->szLokal) < 3)
                                                  {
                                                  strcat (pwnd->szLokal,
                                                          "\\") ; 
                                                  }
                                             }
                                        else
                                             {
                                             if   (strlen (pwnd->szLokal) > 3)
                                                  {
                                                  strcat (pwnd->szLokal,
                                                          "\\") ; 
                                                  }
                                             strcat (pwnd->szLokal,
                                                     pusrreclokal->szFilename) ;
                                             }
                                        DosSetCurrentDir (pwnd->szLokal) ;
                                        _beginthread (ThreadLokal,
                                                      NULL,
                                                      80000,
                                                      pwnd) ;
                                        break ;
                                   case RECORDTYPEDRIVE:
                                        DosSetDefaultDisk (*pusrreclokal->szFilename - '@') ;
                                        DosSetCurrentDir ("\\") ;
                                        _beginthread (ThreadLokal,
                                                      NULL,
                                                      80000,
                                                      pwnd) ;
                                        break ;
                                   }
                                }
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Verz”gerter Start */
      case WM_CREATE:
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           WinPostMsg (hwnd,
                       WMP_CREATE,
                       MPVOID,
                       MPVOID) ;
           break ;
      case WM_DESTROY:
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
      case WM_FTPAPI_XFER_UPDATE:
           {
           CHAR sz [CCHMAXPATH] ;
           CHAR sz2 [CCHMAXPATH] ;

           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_FTPEXAMPLED,
                          sizeof (sz),
                          sz) ;
           sprintf (sz2,
                    "%s%u",
                    sz,
                    (ULONG) LONGFROMMP (mp1)) ;
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz2) ;
           return (MRESULT) FALSE ;
           }
      case WM_MINMAXFRAME:
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Verz”gerter Start hier */
      case WM_SIZE:
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       MPVOID,
                       MPVOID) ;
           break ;
      case WMP_CREATE:
           {
           CNRINFO              ccinfo ;
           static CONTAINERINIT acnrinitLokal [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_FILENAME, offsetof (USRRECLOKAL, minirec.pszIcon) },
                                                    { "", CFA_DATE | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_DATE,     offsetof (USRRECLOKAL, cdateTemp)       },
                                                    { "", CFA_TIME | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_TIME,     offsetof (USRRECLOKAL, ctimeTemp)       },
                                                    { "", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR, CFA_CENTER, IDS_SIZE,     offsetof (USRRECLOKAL, ulSize)          }} ;
           static CONTAINERINIT acnrinitEntfernt [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_FILENAME, offsetof (USRRECENTFERNT, minirec.pszIcon) },
                                                       { "", CFA_DATE | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_DATE,     offsetof (USRRECENTFERNT, cdateTemp)       },
                                                       { "", CFA_TIME | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_TIME,     offsetof (USRRECENTFERNT, ctimeTemp)       },
                                                       { "", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR, CFA_CENTER, IDS_SIZE,     offsetof (USRRECENTFERNT, ulSize)          }} ;
           PWND                 pwnd ;
           SWP                  swp ;
           
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           pwnd->hwndEntfernt = WinCreateWindow (hwndClient_g,
                                                 WC_CONTAINER,
                                                 "",
                                                 CCS_AUTOPOSITION | CCS_MINIRECORDCORE | CCS_MULTIPLESEL | CCS_READONLY | WS_GROUP | WS_VISIBLE,
                                                 0,
                                                 swp.cy / 2 + 1,
                                                 swp.cx - 2,
                                                 swp.cy / 2 - 2,
                                                 hwndFrame_g,
                                                 HWND_TOP,
                                                 IDCO_ENTFERNT,
                                                 NULL,
                                                 NULL) ;
           TInitContainer (pwnd->hwndEntfernt,
                           NULLHANDLE,
                           &acnrinitEntfernt [0],
                           sizeof (acnrinitEntfernt) / sizeof (acnrinitEntfernt [0])) ;
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           ccinfo.flWindowAttr = CA_CONTAINERTITLE | CA_DETAILSVIEWTITLES | CA_TITLESEPARATOR | CV_DETAIL | CV_MINI ;
           ccinfo.pszCnrTitle = pwnd->szHeaderEntfernt ;
           WinSendMsg (pwnd->hwndEntfernt,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           pwnd->hwndLokal = WinCreateWindow (hwndClient_g,
                                              WC_CONTAINER,
                                              "",
                                              CCS_AUTOPOSITION | CCS_MINIRECORDCORE | CCS_MULTIPLESEL | CCS_READONLY | WS_GROUP | WS_VISIBLE,
                                              0,
                                              1,
                                              swp.cx - 2,
                                              swp.cy / 2 - 2,
                                              hwndFrame_g,
                                              HWND_TOP,
                                              IDCO_LOKAL,
                                              NULL,
                                              NULL) ;
           TInitContainer (pwnd->hwndLokal,
                           NULLHANDLE,
                           &acnrinitLokal [0],
                           sizeof (acnrinitLokal) / sizeof (acnrinitLokal [0])) ;
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           ccinfo.flWindowAttr = CA_CONTAINERTITLE | CA_DETAILSVIEWTITLES | CA_TITLESEPARATOR | CV_DETAIL | CV_MINI ;
           ccinfo.pszCnrTitle = pwnd->szHeaderLokal ;
           WinSendMsg (pwnd->hwndLokal,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           ftpwindow (hwndClient_g) ;
           _beginthread (ThreadLokal,
                         NULL,
                         80000,
                         pwnd) ;
           if   (WinDlgBox (HWND_DESKTOP,
                            hwndFrame_g,
                            EntfernterHostDlgWndProc,
                            NULLHANDLE,
                            IDD_ENTFERNTERHOST,
                            pwnd) == DID_CANCEL)
                {
                WinPostMsg (hwnd,
                            WM_CLOSE,
                            MPVOID,
                            MPVOID) ;
                }
           else
                {
                _beginthread (ThreadEntfernt,
                              NULL,
                              80000,
                              pwnd) ;
                }
           return (MRESULT) TRUE ;
           }
      case WMP_SIZE:
           {
           PWND pwnd ;
           SWP  swp ;

           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           if   (!fMinimized_g)
                {
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                WinSetWindowPos (pwnd->hwndEntfernt,
                                 HWND_TOP,
                                 0,
                                 swp.cy / 2 + 1,
                                 swp.cx - 2,
                                 swp.cy / 2 - 2,
                                 SWP_MOVE | SWP_SIZE) ;
                WinSetWindowPos (pwnd->hwndLokal,
                                 HWND_TOP,
                                 0,
                                 1,
                                 swp.cx - 2,
                                 swp.cy / 2 - 2,
                                 SWP_MOVE | SWP_SIZE) ;
                }
           return (MRESULT) FALSE ;
           }
      case WMP_WRITEFTPERROR:
           {
           ULONG aul [12] [2] = {{ FTPABORT,      IDS_FTPABORT      },
                                 { FTPCOMMAND,    IDS_FTPCOMMAND    },
                                 { FTPCONNECT,    IDS_FTPCONNECT    },
                                 { FTPDATACONN,   IDS_FTPDATACONN   },
                                 { FTPHOST,       IDS_FTPHOST       },
                                 { FTPLOCALFILE,  IDS_FTPLOCALFILE  },
                                 { FTPLOGIN,      IDS_FTPLOGIN      },
                                 { FTPNOPRIMARY,  IDS_FTPNOPRIMARY  },
                                 { FTPNOXLATETBL, IDS_FTPNOXLATETBL },
                                 { FTPPROXYTHIRD, IDS_FTPPROXYTHIRD },
                                 { FTPSERVICE,    IDS_FTPSERVICE    },
                                 { FTPSOCKET,     IDS_FTPSOCKET     }} ;

           ULONG ulI ;

           for  (ulI = 0; ulI < sizeof (aul) / sizeof (aul [0]); ulI++)
                {
                if   (LONGFROMMP (mp1) == aul [ulI] [0])
                     {
                     TWriteMessage (hwnd,
                                    NULLHANDLE,
                                    IDS_ERROR,
                                    aul [ulI] [1],
                                    MB_ERROR | MB_OK) ;
                     break ;
                     }
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standardverarbeitung fuer das Clientfenster */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

MRESULT EXPENTRY EntfernterHostDlgWndProc (HWND   hwndDlg,
                                           MSG    msg,
                                           MPARAM mp1,
                                           MPARAM mp2)
   {
   switch (msg)
      {
      case WM_COMMAND:
           {
           PDLGENTFERNTERHOST pdlg ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              case DID_OK:
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ENTFERNTERHOST_1,
                                        sizeof (pdlg->pwnd->szLokation),
                                        pdlg->pwnd->szLokation) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ENTFERNTERHOST_2,
                                        sizeof (pdlg->pwnd->szKennung),
                                        pdlg->pwnd->szKennung) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ENTFERNTERHOST_3,
                                        sizeof (pdlg->pwnd->szPasswort),
                                        pdlg->pwnd->szPasswort) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ENTFERNTERHOST_4,
                                        sizeof (pdlg->pwnd->szAccount),
                                        pdlg->pwnd->szAccount) ;
                   pdlg->pwnd->transfertype = (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                                IDR_ENTFERNTERHOST_1,
                                                                                BM_QUERYCHECK,
                                                                                MPVOID,
                                                                                MPVOID)) ? TRANSFERTYPEBINARY : TRANSFERTYPEASCII) ;
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      case WM_INITDLG:
           {
           PDLGENTFERNTERHOST pdlg ;

           pdlg = calloc (1,
                          sizeof (DLGENTFERNTERHOST)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           pdlg->pwnd = PVOIDFROMMP (mp2) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_ENTFERNTERHOST_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (260),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_ENTFERNTERHOST_2,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (260),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_ENTFERNTERHOST_3,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (260),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_ENTFERNTERHOST_4,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (260),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDR_ENTFERNTERHOST_1,
                              BM_SETCHECK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Applikationsstart */
INT main (INT argc,
          PSZ argv [])
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;
   ULONG ulI ;

   if   (argc > 1)
        {
        if   (argc < 4)
             {
             for  (ulI = 1; ulI < argc; ulI++)
                  {
                  switch (argv [ulI] [0])
                     {
                     case '-':
                     case '/':
                          switch (argv [ulI] [1])
                             {
                             case 't':
                             case 'T':
                                  fTrace_g = TRUE ;
                                  break ;
                             }
                          break ;
                     }
                  }
             }
        }
   /* Beim Presentation Manager anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* MessageQueue mit Standardgroesse anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse fuer Anwendung anlegen */
             if   (WinRegisterClass (hab_g,
                                     pszClass_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster erzeugen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszClass_g,
                                                           "FTP Example",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_FTPEXMPL,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* MessageLoop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Messages auf Fensterprozeduren verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Standardfenster wieder entfernen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* MessageQueue wieder entfernen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim Presentation Manager wieder abmelden */
        WinTerminate (hab_g) ;
        }
   /* Applikationsende */
   return 0 ;
   }

BOOL TCutString (PSZ  pszString,
                 CHAR ch)
   {
   PSZ psz ;

   if   (*pszString)
        {
        psz = pszString ;
        while (*pszString)
           {
           pszString++ ;
           }
        pszString-- ;
        while (pszString != psz)
           {
           if   (*pszString == ch)
                {
                *pszString = '\0' ;
                }
           else
                {
                break ;
                }
           pszString-- ;
           }
        return TXK_NOERROR ;
        }
   else
        {
        return TXK_ERROR ;
        }
   }

VOID ThreadEntfernt (VOID* pv)
   {
   BOOL            f ;
   BOOL            f2 ;
   BOOL            f4 ;
   CHAR            sz [3 * CCHMAXPATH] ;
   CHAR            sz2 [CCHMAXPATH] ;
   CHAR            sz3 [CCHMAXPATH] ;
   CHAR            sz4 [CCHMAXPATH] ;
   CHAR            sz5 [CCHMAXPATH] ;
   CHAR            sz6 [CCHMAXPATH] ;
   CHAR            sz7 [CCHMAXPATH] ;
   CHAR            sz8 [CCHMAXPATH] ;
   CNRINFO         ccinfo ;
   DATETIME        date ;
   FILE*           pfile ;
   HAB             hab ;
   HMQ             hmq ;
   PSZ             apsz [] = { "Jan",
                               "Feb",
                               "Mar",
                               "Apr",
                               "May",
                               "Jun",
                               "Jul",
                               "Aug",
                               "Sep",
                               "Oct",
                               "Nov",
                               "Dec" } ;
   PSZ             psz ;
   PUSRRECENTFERNT pusrrecentfernt ;
   PWND            pwnd ;
   RECORDINSERT    recins ;
   ULONG           ul ;
   ULONG           ul2 ;
   ULONG           ul3 ;
   ULONG           ul4 ;
   ULONG           ulI ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   WinSendMsg (pwnd->hwndEntfernt,
               CM_REMOVERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
   strcpy (pwnd->szHeaderEntfernt,
           pwnd->szLokation) ;
   strcat (pwnd->szHeaderEntfernt,
           "::") ;
   if   (!*pwnd->szEntfernt)
        {
        pwnd->szEntfernt [0] = '.' ; 
        pwnd->szEntfernt [1] = '\0' ; 
        }
   strcat (pwnd->szHeaderEntfernt,
           pwnd->szEntfernt) ;
   memset (&ccinfo,
           0,
           sizeof (CNRINFO)) ;
   ccinfo.pszCnrTitle = pwnd->szHeaderEntfernt ;
   WinSendMsg (pwnd->hwndEntfernt,
               CM_SETCNRINFO,
               MPFROMP (&ccinfo),
               MPFROMLONG (CMA_CNRTITLE)) ;
   if   (!ftpcd (pwnd->szLokation,
                 pwnd->szKennung,
                 pwnd->szPasswort,
                 pwnd->szAccount,
                 pwnd->szEntfernt))
        {
        strcpy (sz2,
                tmpnam (NULL)) ;
        if   (!ftpdir (pwnd->szLokation,
                       pwnd->szKennung,
                       pwnd->szPasswort,
                       pwnd->szAccount,
                       sz2,
                       "*"))
             {
             f = FALSE ;
             if   ((pfile = fopen (sz2,
                                   "r")) != NULL)
                  {
                  DosGetDateTime (&date) ;
                  while (!feof (pfile))
                     {
                     if   (fgets (sz,
                                  sizeof (sz) - 1,
                                  pfile))
                          {
                          TCutString (sz,
                                      ' ') ;
                          if   (strlen (sz) > 2)
                               {
                               f2 = FALSE ;
                               f4 = FALSE ;
                               if   ((psz = strstr (sz,
                                                    ":\n")) != NULL)
                                    {
                                    *psz = '\0' ;
                                    TCutString (sz,
                                                ' ') ;
                                    strcpy (sz3,
                                            pwnd->szHeaderLokal) ;
                                    if   (*sz3)
                                         {
                                         strcat (sz3,
                                                 "/") ;
                                         }
                                    strcat (sz3,
                                            sz) ;
                                    f = TRUE ;
                                    f2 = TRUE ;
                                    }
                               if   (!f)
                                    {
                                    TCutString (sz,
                                                '\n') ;
                                    TCutString (sz,
                                                ' ') ;
                                    if   ((psz = strstr (sz,
                                                         "->")) != NULL)
                                         {
                                         *psz = '\0' ;
                                         TCutString (sz,
                                                     ' ') ;
                                         if   ((psz = strrchr (sz,
                                                               ' ')) != NULL)
                                              {
                                              strcpy (sz3,
                                                      pwnd->szHeaderLokal) ;
                                              if   (*sz3)
                                                   {
                                                   strcat (sz3,
                                                           "/") ;
                                                   }
                                              strcpy (sz,
                                                      psz + 1) ;
                                              strcat (sz3,
                                                      sz) ;
                                              f2 = TRUE ;
                                              }
                                         else
                                              {
                                              f4 = TRUE ;
                                              }
                                         }
                                    if   (*sz == 'd')
                                         {
                                         if   ((psz = strrchr (sz,
                                                               ' ')) != NULL)
                                              {
                                              strcpy (sz3,
                                                      pwnd->szHeaderLokal) ;
                                              if   (*sz3)
                                                   {
                                                   strcat (sz3,
                                                           "/") ;
                                                   }
                                              strcpy (sz,
                                                      psz + 1) ;
                                              strcat (sz3,
                                                      sz) ;
                                              f2 = TRUE ;
                                              }
                                         else
                                              {
                                              f4 = TRUE ;
                                              }
                                         }
                                    if   ((sz [0] == 't') &&
                                          (sz [1] == 'o') &&
                                          (sz [2] == 't') &&
                                          (sz [3] == 'a') &&
                                          (sz [4] == 'l'))
                                         {
                                         f4 = TRUE ;
                                         }
                                    if   (*sz == '.')
                                         {
                                         f4 = TRUE ;
                                         }
                                    }
                               if   (!f4)
                                    {
                                    if   (f2)
                                         {
                                         pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                                                    CM_ALLOCRECORD,
                                                                                    MPFROMLONG (sizeof (USRRECENTFERNT) - sizeof (MINIRECORDCORE)),
                                                                                    MPFROMSHORT (1))) ;
                                         strcpy (pusrrecentfernt->szFilename,
                                                 sz) ;
                                         pusrrecentfernt->minirec.pszIcon = pusrrecentfernt->szFilename ;
                                         pusrrecentfernt->recordtype = RECORDTYPEDIRECTORY ;
                                         memset (&recins,
                                                 0,
                                                 sizeof (RECORDINSERT)) ;
                                         recins.cb = sizeof (RECORDINSERT) ;
                                         recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                         recins.zOrder = CMA_TOP ;
                                         recins.cRecordsInsert = 1 ;
                                         WinSendMsg (pwnd->hwndEntfernt,
                                                     CM_INSERTRECORD,
                                                     MPFROMP (pusrrecentfernt),
                                                     MPFROMP (&recins)) ;
                                         }
                                    else
                                         {
                                         if   (!f)
                                              {
                                              if   (strstr (sz,
                                                            ":") != NULL)
                                                   {
                                                   sscanf (sz,
                                                           "%s %s %s %s %u %s %u %u:%u %[^\n]",
                                                           sz3,
                                                           sz4,
                                                           sz5,
                                                           sz6,
                                                           &ul4,
                                                           sz7,
                                                           &ul,
                                                           &ul2,
                                                           &ul3,
                                                           sz8) ;
                                                   if   (*sz8 == '.')
                                                        {
                                                        break ;
                                                        }
                                                   pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                                                              CM_ALLOCRECORD,
                                                                                              MPFROMLONG (sizeof (USRRECENTFERNT) - sizeof (MINIRECORDCORE)),
                                                                                              MPFROMSHORT (1))) ;
                                                   strcpy (pusrrecentfernt->szFilename,
                                                           sz8) ;
                                                   pusrrecentfernt->cdateTemp.year = date.year ;
                                                   pusrrecentfernt->ctimeTemp.hours = ul2 ;
                                                   pusrrecentfernt->ctimeTemp.minutes = ul3 ;
                                                   pusrrecentfernt->ulSize = ul4 ;
                                                   }
                                              else
                                                   {
                                                   sscanf (sz,
                                                           "%s %s %s %s %u %s %u %u %[^\n]",
                                                           sz3,
                                                           sz4,
                                                           sz5,
                                                           sz6,
                                                           &ul4,
                                                           sz7,
                                                           &ul,
                                                           &ul2,
                                                           sz8) ;
                                                   if   (*sz8 == '.')
                                                        {
                                                        break ;
                                                        }
                                                   pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                                                              CM_ALLOCRECORD,
                                                                                              MPFROMLONG (sizeof (USRRECENTFERNT) - sizeof (MINIRECORDCORE)),
                                                                                              MPFROMSHORT (1))) ;
                                                   strcpy (pusrrecentfernt->szFilename,
                                                           sz8) ;
                                                   pusrrecentfernt->cdateTemp.year = ul2 ;
                                                   pusrrecentfernt->ulSize = ul4 ;
                                                   }
                                              pusrrecentfernt->minirec.pszIcon = pusrrecentfernt->szFilename ;
                                              for  (ulI = 0; ulI < sizeof (apsz) / sizeof (apsz [0]); ulI++)
                                                   {
                                                   if   (!stricmp (apsz [ulI],
                                                                   sz7))
                                                        {
                                                        pusrrecentfernt->cdateTemp.month = ulI + 1 ;
                                                        break ;
                                                        }
                                                   }
                                              pusrrecentfernt->cdateTemp.day = ul ;
                                              pusrrecentfernt->recordtype = RECORDTYPEFILE ;
                                              memset (&recins,
                                                      0,
                                                      sizeof (RECORDINSERT)) ;
                                              recins.cb = sizeof (RECORDINSERT) ;
                                              recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                              recins.zOrder = CMA_TOP ;
                                              recins.cRecordsInsert = 1 ;
                                              WinSendMsg (pwnd->hwndEntfernt,
                                                          CM_INSERTRECORD,
                                                          MPFROMP (pusrrecentfernt),
                                                          MPFROMP (&recins)) ;
                                              }
                                         }
                                    }
                               }
                          }
                     }
                  fclose (pfile) ;
                  if   (!fTrace_g)
                       {
                       remove (sz2) ;
                       }
                  }
             }
        else
             {
             WinPostMsg (hwndClient_g,
                         WMP_WRITEFTPERROR,
                         MPFROMLONG (ftperrno),
                         MPVOID) ;
             }
        }
   else
        {
        WinPostMsg (hwndClient_g,
                    WMP_WRITEFTPERROR,
                    MPFROMLONG (ftperrno),
                    MPVOID) ;
        }
   WinLoadString (hab_g,
                  NULLHANDLE,
                  IDS_FTPEXAMPLE,
                  sizeof (sz),
                  sz) ;
   WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                      FID_TITLEBAR),
                     sz) ;
   WinSendMsg (pwnd->hwndEntfernt,
               CM_INVALIDATERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

VOID ThreadLoeschenEntfernt (VOID* pv)
   {
   HAB             hab ;
   HMQ             hmq ;
   PUSRRECENTFERNT pusrrecentfernt ;
   PUSRRECENTFERNT pusrrecentfernt2 ;
   PWND            pwnd ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                              CM_QUERYRECORDEMPHASIS,
                                              MPFROMP ((PRECORDCORE) CMA_FIRST),
                                              MPFROMSHORT (CRA_SELECTED))) ;
   while (pusrrecentfernt)
      {
      if   (ftpdelete (pwnd->szLokation,
                       pwnd->szKennung,
                       pwnd->szPasswort,
                       pwnd->szAccount,
                       pusrrecentfernt->szFilename))
           {
           WinPostMsg (hwndClient_g,
                       WMP_WRITEFTPERROR,
                       MPFROMLONG (ftperrno),
                       MPVOID) ;
           break ;
           }
      pusrrecentfernt2 = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                  CM_QUERYRECORDEMPHASIS,
                                                  MPFROMP (pusrrecentfernt),
                                                  MPFROMSHORT (CRA_SELECTED))) ;
      WinSendMsg (pwnd->hwndEntfernt,
                  CM_REMOVERECORD,
                  MPFROMP (&pusrrecentfernt),
                  MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
      pusrrecentfernt = pusrrecentfernt2 ;
      }
   _beginthread (ThreadEntfernt,
                 NULL,
                 80000,
                 pwnd) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

VOID ThreadLogoff (VOID* pv)
   {
   HAB  hab ;
   HMQ  hmq ;
   PWND pwnd ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   ftplogoff () ;
   if   (pwnd->fClosePending)
        {
        WinPostMsg (hwndClient_g,
                    WM_QUIT,
                    MPVOID,
                    MPVOID) ;
        }
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

VOID ThreadLokal (VOID* pv)
   {
   APIRET       rc ;
   CHAR         sz [CCHMAXPATH] ;
   CHAR         sz2 [CCHMAXPATH] ;
   CNRINFO      ccinfo ;
   FILEFINDBUF3 findbuf3 ;
   FILESTATUS3  fsts3 ;
   PFSQBUFFER2  pfsqbuf2 ;
   HAB          hab ;
   HDIR         hdir ;
   HMQ          hmq ;
   PSZ          psz ;
   PUSRRECLOKAL pusrreclokal ;
   PWND         pwnd ;
   RECORDINSERT recins ;
   ULONG        ul ;
   ULONG        ulI ;
   ULONG        ulJ ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   WinSendMsg (pwnd->hwndLokal,
               CM_REMOVERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
   DosQueryCurrentDisk (&ulI,
                        &ulJ) ;
   pwnd->szLokal [0] = ulI + '@' ;
   pwnd->szLokal [1] = ':' ;
   pwnd->szLokal [2] = '\\' ;
   ulJ = sizeof (pwnd->szLokal) - 3 ;
   DosQueryCurrentDir (ulI,
                       &pwnd->szLokal [3],
                       &ulJ) ;
   strcpy (pwnd->szHeaderLokal,
           pwnd->szLokal) ;
   memset (&ccinfo,
           0,
           sizeof (CNRINFO)) ;
   ccinfo.pszCnrTitle = pwnd->szHeaderLokal ;
   WinSendMsg (pwnd->hwndLokal,
               CM_SETCNRINFO,
               MPFROMP (&ccinfo),
               MPFROMLONG (CMA_CNRTITLE)) ;
   strcpy (sz,
           pwnd->szLokal) ;
   if   (strlen (sz) > 3)
        {
        strcat (sz,
                "\\") ;
        }
   strcat (sz,
           "*.*") ;
   hdir = HDIR_CREATE ;
   ul = 1 ;
   rc = DosFindFirst (sz,
                      &hdir,
                      FILE_ARCHIVED | FILE_DIRECTORY | FILE_NORMAL | FILE_READONLY,
                      &findbuf3,
                      sizeof (findbuf3),
                      &ul,
                      FIL_STANDARD) ;
   while (rc == NO_ERROR &&
          ul == 1)
      {
      if   (!((findbuf3.achName [0] == '.') &&
              (findbuf3.achName [1] == '\0')))
           {
           pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                                   CM_ALLOCRECORD,
                                                   MPFROMLONG (sizeof (USRRECLOKAL) - sizeof (MINIRECORDCORE)),
                                                   MPFROMSHORT (1))) ;
           strcpy (pusrreclokal->szFilename,
                   findbuf3.achName) ;
           pusrreclokal->minirec.pszIcon = pusrreclokal->szFilename ;
           strcpy (sz2,
                   pwnd->szLokal) ;
           if   (!(findbuf3.attrFile & FILE_DIRECTORY))
                {
                if   (strlen (sz2) > 3)
                     {
                     strcat (sz2,
                             "\\") ;
                     }
                strcat (sz2,
                        findbuf3.achName) ;
                pusrreclokal->recordtype = RECORDTYPEFILE ;
                }
           if   (!DosQueryPathInfo (sz2,
                                    FIL_STANDARD,
                                    &fsts3,
                                    sizeof (FILESTATUS3)))
                {
                pusrreclokal->cdateTemp.day = fsts3.fdateLastWrite.day ;
                pusrreclokal->cdateTemp.month = fsts3.fdateLastWrite.month ;
                pusrreclokal->cdateTemp.year = fsts3.fdateLastWrite.year + 1980 ;
                pusrreclokal->ctimeTemp.hours = fsts3.ftimeLastWrite.hours ;
                pusrreclokal->ctimeTemp.minutes = fsts3.ftimeLastWrite.minutes ;
                pusrreclokal->ctimeTemp.seconds = fsts3.ftimeLastWrite.twosecs * 2 ;
                pusrreclokal->ulSize = fsts3.cbFile ;
                }
           if   (findbuf3.attrFile & FILE_DIRECTORY)
                {
                if   (!((pusrreclokal->szFilename [0] == '.') &&
                        (pusrreclokal->szFilename [1] == '.')))
                     {
                     if   (strlen (sz2) > 3)
                          {
                          strcat (sz2,
                                  "\\") ;
                          }
                     }
                pusrreclokal->recordtype = RECORDTYPEDIRECTORY ;
                }
           memset (&recins,
                   0,
                   sizeof (RECORDINSERT)) ;
           recins.cb = sizeof (RECORDINSERT) ;
           recins.pRecordOrder = (PRECORDCORE) CMA_END ;
           recins.zOrder = CMA_TOP ;
           recins.cRecordsInsert = 1 ;
           WinSendMsg (pwnd->hwndLokal,
                       CM_INSERTRECORD,
                       MPFROMP (pusrreclokal),
                       MPFROMP (&recins)) ;
           }
      rc = DosFindNext (hdir,
                        &findbuf3,
                        sizeof (findbuf3),
                        &ul) ;
      }
   DosFindClose (hdir) ;
   ulJ = 8192 ;
   pfsqbuf2 = malloc (ulJ) ;
   DosError (FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION) ;
   for  (ulI = 3; ulI <= 26; ulI++)
        {
        sz [0] = ulI + '@' ;
        sz [1] = ':' ;
        sz [2] = '\0' ;
        memset (pfsqbuf2,
                0,
                ulJ) ;
        if   (!DosQueryFSAttach (sz,
                                 0,
                                 FSAIL_QUERYNAME,
                                 pfsqbuf2,
                                 &ulJ))
             {
             psz = pfsqbuf2->szName + pfsqbuf2->cbName + 1 ;
             if   (stricmp (psz,
                            "CDFS"))
                  {
                  pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                                          CM_ALLOCRECORD,
                                                          MPFROMLONG (sizeof (USRRECLOKAL) - sizeof (MINIRECORDCORE)),
                                                          MPFROMSHORT (1))) ;
                  strcpy (pusrreclokal->szFilename,
                          sz) ;
                  pusrreclokal->minirec.pszIcon = pusrreclokal->szFilename ;
                  strcpy (sz2,
                          sz) ;
                  strcat (sz2,
                          "\\") ;
                  if   (!DosQueryPathInfo (sz2,
                                           FIL_STANDARD,
                                           &fsts3,
                                           sizeof (FILESTATUS3)))
                       {
                       pusrreclokal->cdateTemp.day = fsts3.fdateLastWrite.day ;
                       pusrreclokal->cdateTemp.month = fsts3.fdateLastWrite.month ;
                       pusrreclokal->cdateTemp.year = fsts3.fdateLastWrite.year + 1980 ;
                       pusrreclokal->ctimeTemp.hours = fsts3.ftimeLastWrite.hours ;
                       pusrreclokal->ctimeTemp.minutes = fsts3.ftimeLastWrite.minutes ;
                       pusrreclokal->ctimeTemp.seconds = fsts3.ftimeLastWrite.twosecs * 2 ;
                       pusrreclokal->ulSize = fsts3.cbFile ;
                       }
                  pusrreclokal->recordtype = RECORDTYPEDRIVE ;
                  memset (&recins,
                          0,
                          sizeof (RECORDINSERT)) ;
                  recins.cb = sizeof (RECORDINSERT) ;
                  recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                  recins.zOrder = CMA_TOP ;
                  recins.cRecordsInsert = 1 ;
                  WinSendMsg (pwnd->hwndLokal,
                              CM_INSERTRECORD,
                              MPFROMP (pusrreclokal),
                              MPFROMP (&recins)) ;
                  }
             }
        }
   DosError (FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION) ;
   free (pfsqbuf2) ;
   WinSendMsg (pwnd->hwndLokal,
               CM_INVALIDATERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

VOID ThreadTransferEntfernt (VOID* pv)
   {
   CHAR            sz [CCHMAXPATH] ;
   HAB             hab ;
   HMQ             hmq ;
   PUSRRECENTFERNT pusrrecentfernt ;
   PWND            pwnd ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                              CM_QUERYRECORDEMPHASIS,
                                              MPFROMP ((PRECORDCORE) CMA_FIRST),
                                              MPFROMSHORT (CRA_SELECTED))) ;
   while (pusrrecentfernt)
      {
      strcpy (sz,
              pwnd->szLokal) ;
      if   (pwnd->szLokal [strlen (pwnd->szLokal) - 1] != '\\')
           {
           strcat (sz,
                   "\\") ;
           }
      strcat (sz,
              pusrrecentfernt->szFilename) ;
      if   (ftpget (pwnd->szLokation,
                    pwnd->szKennung,
                    pwnd->szPasswort,
                    pwnd->szAccount,
                    sz,
                    pusrrecentfernt->szFilename,
                    "w",
                    (pwnd->transfertype == TRANSFERTYPEBINARY) ? T_BINARY : T_ASCII))
           {
           WinPostMsg (hwndClient_g,
                       WMP_WRITEFTPERROR,
                       MPFROMLONG (ftperrno),
                       MPVOID) ;
           break ;
           }
      pusrrecentfernt = PVOIDFROMMR (WinSendMsg (pwnd->hwndEntfernt,
                                                 CM_QUERYRECORDEMPHASIS,
                                                 MPFROMP (pusrrecentfernt),
                                                 MPFROMSHORT (CRA_SELECTED))) ;
      }                                            
   _beginthread (ThreadLokal,
                 NULL,
                 80000,
                 pwnd) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }
   
VOID ThreadTransferLokal (VOID* pv)
   {
   CHAR         sz [CCHMAXPATH] ;
   HAB          hab ;
   HMQ          hmq ;
   PUSRRECLOKAL pusrreclokal ;
   PWND         pwnd ;

   pwnd = pv ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                           CM_QUERYRECORDEMPHASIS,
                                           MPFROMP ((PRECORDCORE) CMA_FIRST),
                                           MPFROMSHORT (CRA_SELECTED))) ;
   while (pusrreclokal)
      {
      strcpy (sz,
              pwnd->szLokal) ;
      if   (pwnd->szLokal [strlen (pwnd->szLokal) - 1] != '\\')
           {
           strcat (sz,
                   "\\") ;
           }
      strcat (sz,
              pusrreclokal->szFilename) ;
      if   (ftpput (pwnd->szLokation,
                    pwnd->szKennung,
                    pwnd->szPasswort,
                    pwnd->szAccount,
                    sz,
                    pusrreclokal->szFilename,
                    (pwnd->transfertype == TRANSFERTYPEBINARY) ? T_BINARY : T_ASCII))
           {
           WinPostMsg (hwndClient_g,
                       WMP_WRITEFTPERROR,
                       MPFROMLONG (ftperrno),
                       MPVOID) ;
           break ;
           }
      pusrreclokal = PVOIDFROMMR (WinSendMsg (pwnd->hwndLokal,
                                              CM_QUERYRECORDEMPHASIS,
                                              MPFROMP (pusrreclokal),
                                              MPFROMSHORT (CRA_SELECTED))) ;
      }
   WinLoadString (hab_g,
                  NULLHANDLE,
                  IDS_FTPEXAMPLE,
                  sizeof (sz),
                  sz) ;
   WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                      FID_TITLEBAR),
                     sz) ;
   _beginthread (ThreadEntfernt,
                 NULL,
                 80000,
                 pwnd) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

BOOL TInitContainer (HWND           hwnd,
                     HMODULE        hmod,
                     PCONTAINERINIT pcnrinit,
                     ULONG          ul)
   {
   FIELDINFOINSERT fiins ;
   HAB             hab ;
   PFIELDINFO      pfldinfo ;
   PFIELDINFO      pfldinfoFirst ;
   ULONG           ulI ;

   hab = WinQueryAnchorBlock (hwnd) ;
   pfldinfo = PVOIDFROMMR (WinSendMsg (hwnd,
                                       CM_ALLOCDETAILFIELDINFO,
                                       MPFROMLONG (ul),
                                       MPVOID)) ;
   pfldinfoFirst = pfldinfo ;
   for  (ulI = 0; ulI < ul; ulI++, pcnrinit++)
        {
        pfldinfo->flData = pcnrinit->ulFlData ;
        pfldinfo->flTitle = pcnrinit->ulFlTitle ;
        WinLoadString (hab,
                       hmod,
                       pcnrinit->ulIdTitle,
                       sizeof (pcnrinit->szTitle),
                       pcnrinit->szTitle) ;
        pfldinfo->pTitleData = pcnrinit->szTitle ;
        pfldinfo->offStruct = pcnrinit->ulOffStruct ;
        pfldinfo = pfldinfo->pNextFieldInfo ;
        }
   fiins.cb = sizeof (FIELDINFOINSERT) ;
   fiins.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST ;
   fiins.cFieldInfoInsert = ul ;
   fiins.fInvalidateFieldInfo = TRUE ;
   WinSendMsg (hwnd,
               CM_INSERTDETAILFIELDINFO,
               MPFROMP (pfldinfoFirst),
               MPFROMP (&fiins)) ;
   return TXK_NOERROR ;
   }

USHORT TWriteMessage (HWND    hwnd,
                      HMODULE hmod,
                      ULONG   ulTitle,
                      ULONG   ulText,
                      ULONG   ulStyle)
   {
   CHAR szText [256] ;
   CHAR szTitle [128] ;
   HAB hab ;

   hab = WinQueryAnchorBlock (hwnd) ;
   WinLoadString (hab,
                  hmod,
                  ulTitle,
                  sizeof (szTitle),
                  szTitle) ;
   WinLoadString (hab,
                  hmod,
                  ulText,
                  sizeof (szText),
                  szText) ;
   return WinMessageBox (HWND_DESKTOP,
                         hwnd,
                         szText,
                         szTitle,
                         0,
                         ulStyle) ;
   }

