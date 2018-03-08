/* $Header: D:\Projekte\Redaktion Toolbox\Getunnelt\Source\RCS\TUNNEL.C 1.1 1997/04/27 16:29:33 HAWI Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include <JEmpower.h>

#include "TUNNEL.H"

INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;
   
   /* Beim PM anmelden */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue mit Standardgr”áe anlegen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* Ein normaler Dialog reicht fr dieses Testprogramm */
             WinDlgBox (HWND_DESKTOP,
                        HWND_DESKTOP,
                        DlgWndProc,
                        NULLHANDLE,
                        IDD,
                        NULL) ;
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
   switch (msg)
      {
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Benutzer hat ESC gedrckt, Anwendung verlassen */
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_DESTROY:
           {
           PDLG pdlg ;
           
           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           jeAppletsDestroy (pdlg->hndApplet) ;
           /* Instanzdaten l”schen */
           free (pdlg) ;
           break ;
           }
      case WM_INITDLG:
           {
           PDLG pdlg ;
           SWP  swp ;
           
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Fenster Handle des Kontrollelementes abfragen */
           pdlg->hwndTunnel = WinWindowFromID (hwndDlg,
                                               IDT_1) ;
           WinQueryWindowPos (pdlg->hwndTunnel,
                              &swp) ;
           jeInitJavaRuntime () ;
           pdlg->hndApplet = jeAppletInit (&pdlg->hwndApplet,
                                           1,
                                           pdlg->hwndTunnel,
                                           "file:",
                                           "BounceItem.class",
                                           swp.cx,
                                           swp.cy,
                                           0,
                                           0) ;
           pdlg->hwndApplet = jeHWNDFromHandle (pdlg->hndApplet) ;
           WinShowWindow (pdlg->hwndApplet,
                          TRUE) ;
           jeAppletStart (pdlg->hndApplet) ;
           return (MRESULT) FALSE ;
           }
      case WM_MINMAXFRAME:
           {
           PSWP pswp ;
           
           pswp = PVOIDFROMMP (mp1) ;
           if   (pswp->fl & SWP_MAXIMIZE || pswp->fl & SWP_RESTORE)
                {
                jeAppletsUniconify (1) ;
                }
           if   (pswp->fl & SWP_MINIMIZE)
                {
                jeAppletsIconify (1) ;
                }
           break;
           }
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

