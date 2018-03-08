/* $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKETEST.C 1.1 1995/05/25 18:46:20 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "CAKETEST.H"
#include "CAKE.H"

INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;
   
   /* Beim PM anmelden */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue mit Standardgrî·e anlegen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* 2D Grafik Kontrollelement registrieren */
             if   (CRegisterClass (hab))
                  {
                  /* Ein normaler Dialog reicht fÅr dieses Testprogramm */
                  WinDlgBox (HWND_DESKTOP,
                             HWND_DESKTOP,
                             DlgWndProc,
                             NULLHANDLE,
                             IDD,
                             NULL) ;
                  }
             /* Message Queue wieder wegwerfen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab) ;
        }
   return 0 ;
   }

MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   CHAR    szBuffer [52] ;
   CHAR    szBuffer2 [52] ;
   CINFO   cinfo ;
   CVALUE  cvalue ;
   PDLG    pdlg ;
   ULONG   ulI ;
   VALUES  boxvalues [] = {{ "1", CK_LAST,                  10 },
                           { "2", CK_LAST,                  20 },
                           { "3", CK_LAST,                  40 },
                           { "4", CK_LAST,                  80 },
                           { "5", CK_LAST,                 160 },
                           { "6", CK_LAST,                 320 },
                           { "7", CK_LAST,                 160 },
                           { "8", CK_LAST | CK_INVALIDATE,  80 }} ;

   /* Pointer auf Instanzdaten holen */
   pdlg = WinQueryWindowPtr (hwndDlg,
                             QWL_USER) ;
   switch (msg)
      {
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Benutzer hat ESC gedrÅckt, Anwendung verlassen */
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Der Benutzer hat Change gedrÅckt, verÑnderte Werte in Liste Åbernehmen */
              case IDP_1:
                   /* Ermitteln des Index des Kuchenteiles */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_1,
                                        sizeof (szBuffer),
                                        szBuffer) ;
                   /* Daten des Kuchenteiles ermitteln */
                   if   (LONGFROMMR (WinSendMsg (pdlg->hwndCake,
                                                 WMPC_QUERYCAKE,
                                                 MPFROM2SHORT (CK_INDEX, atol (szBuffer)),
                                                 MPFROMP (&cvalue))) != CKC_ERROR)
                        {
                        /* Neuer Wert des Kuchenteils abfragen */
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_2,
                                             sizeof (szBuffer2),
                                             szBuffer2) ;
                        /* Neuer Text des Kuchenteils abfragen */
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_3,
                                             sizeof (cvalue.szTitle),
                                             cvalue.szTitle) ;
                        /* Wert nach numerisch konvertieren */
                        cvalue.ulValue = atol (szBuffer2) ;
                        /* Neue Werte Åbergeben, neuzeichnen erforderlich */
                        WinSendMsg (pdlg->hwndCake,
                                    WMPC_UPDATECAKE,
                                    MPFROM2SHORT (CK_INDEX | CK_INVALIDATE, atol (szBuffer)),
                                    MPFROMP (&cvalue)) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_CONTROL:
           switch (SHORT1FROMMP (mp1))
              {
              case IDT_1:
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Benutzer hat auf Eintrag in Legende gedoppelklickt <g> */
                      case CKN_ENTER:
                           /* Kuchenteil lîschen und Control neu zeichnen */
                           WinSendMsg (pdlg->hwndCake,
                                       WMPC_REMOVECAKE,
                                       MPFROM2SHORT (CK_INDEX | CK_INVALIDATE, LONGFROMMP (mp2)),
                                       MPVOID) ;
                           /* CINFO Buffer lîschen */
                           memset (&cinfo,
                                   0,
                                   sizeof (cinfo)) ;
                           /* Control Infos lesen */
                           WinSendMsg (pdlg->hwndCake,
                                       WMPC_QUERYINFO,
                                       MPVOID,
                                       MPFROMP (&cinfo)) ;
                           /* Neue Anzahl der Kuchenteile ausgeben */
                           WinSetDlgItemShort (hwndDlg,
                                               IDT_2,
                                               cinfo.ulCount,
                                               FALSE) ;
                           /* Alle Eingabefelder lîschen */
                           for  (ulI = IDE_1; ulI <= IDE_3; ulI++)
                                {
                                WinSetDlgItemText (hwndDlg,
                                                   ulI,
                                                   "") ;
                                }
                           return (MRESULT) FALSE ;
                      /* Benutzer hat auf Eintrag in Legende geklickt */
                      case CKN_SELECT:
                           /* Daten zu Kuchenteil ermitteln */
                           if   (LONGFROMMR (WinSendMsg (pdlg->hwndCake,
                                                         WMPC_QUERYCAKE,
                                                         MPFROM2SHORT (CK_INDEX, LONGFROMMP (mp2)),
                                                         MPFROMP (&cvalue))) != CKC_ERROR)
                                {
                                /* Alle Werte dieses Kuchenteiles ausgeben */
                                WinSetDlgItemShort (hwndDlg,
                                                    IDE_1,
                                                    LONGFROMMP (mp2),
                                                    FALSE) ;
                                _ltoa (cvalue.ulValue,
                                       szBuffer,
                                       10) ;
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_2,
                                                   szBuffer) ;
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_3,
                                                   cvalue.szTitle) ;
                                }
                           return (MRESULT) FALSE ;
                      }
                   break ;
              }
           break ;
      case WM_DESTROY:
           /* Instanzdaten lîschen */
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           /* Insatnzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Fenster Handle des Kontrollelementes abfragen */
           pdlg->hwndCake = WinWindowFromID (hwndDlg,
                                             IDT_1) ;
           /* Grî·e der Eingabefelder einstellen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (9),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_2,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (9),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_3,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (sizeof (cvalue.szTitle) - 1),
                              MPVOID) ;
           /* In einer Schleife alle Initialwerte an Control Åbergeben */
           for  (ulI = 0; ulI < sizeof (boxvalues) / sizeof (boxvalues [0]); ulI++)
                {
                /* CVALUE Struktur lîschen */
                memset (&cvalue,
                        0,
                        sizeof (cvalue)) ;
                /* Wert dieses Kuchenteils */
                cvalue.ulValue = boxvalues[ulI].ulValue ;
                /* Text dieses Kuchenteils */
                strcpy (cvalue.szTitle,
                        boxvalues[ulI].pszTitle) ;
                /* Kuchenteil einfÅgen */
                WinSendMsg (pdlg->hwndCake,
                            WMPC_INSERTCAKE,
                            MPFROM2SHORT (boxvalues[ulI].ulFlags, 0),
                            MPFROMP (&cvalue)) ;
                }
           /* CINFO Struktur lîschen */
           memset (&cinfo,
                   0,
                   sizeof (cinfo)) ;
           /* Control Infos lesen */
           WinSendMsg (pdlg->hwndCake,
                       WMPC_QUERYINFO,
                       MPVOID,
                       MPFROMP (&cinfo)) ;
           /* Neue Anzahl der Kuchenteile ausgeben */
           WinSetDlgItemShort (hwndDlg,
                               IDT_2,
                               cinfo.ulCount,
                               FALSE) ;
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }
