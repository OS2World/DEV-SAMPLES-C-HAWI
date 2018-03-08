/* $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREIDTST.C 1.1 1995/10/04 13:08:27 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "DREIDTST.H"
#include "DREID.H"

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
             /* 3D Text Kontrollelement registrieren */
             if   (DREIDRegister (hab))
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
   PDLG pdlg ;

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
              }
           break ;
      case WM_DESTROY:
           /* Instanzdaten lîschen */
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           /* Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }


