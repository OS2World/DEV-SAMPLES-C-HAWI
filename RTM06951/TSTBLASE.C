/* $Header: D:/projekte/blasen\040im\040os!2/source/RCS/TSTBLASE.C 1.2 1995/07/03 10:23:14 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "TSTBLASE.H"
#include "BLASE.H"

/* Globales Modulhandle fÅr die Ressourcen */
HMODULE hmod_g ;

/* Einstieg in das Programm */
INT main (VOID)
   {
   APIRET arc ;
   CHAR sz [CCHMAXPATH] ;
   HAB hab ;
   HMQ hmq ;

   /* Anmeldung beim PM */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* Modulhandle fÅr die Ressource DLL ermitteln */
             arc = DosLoadModule (sz,
                                  sizeof (sz),
                                  "TSTBLASE",
                                  &hmod_g) ;
             /* Wenn erfolgreich */
             if   (!arc)
                  {
                  /* Dialog îffnen */
                  WinDlgBox (HWND_DESKTOP,
                             HWND_DESKTOP,
                             DlgWndProc,
                             hmod_g,
                             IDD,
                             NULL) ;
                  /* Message Queue wieder lîschen */
                  WinDestroyMsgQueue (hmq) ;
                  /* Modul mit Ressourcen wieder freigeben */
                  DosFreeModule (hmod_g) ;
                  }
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab) ;
        }
   return 0 ;
   }

/* Dialogprozedur */
MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   switch (msg)
      {
      /* Benutzer will etwas */
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Esc gedrÅckt */
              case DID_CANCEL:
                   /* Dialog wegwerden */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Dialog wird weggeworfen */
      case WM_DESTROY:
           /* Blasenhilfe deregistrieren. hwndDlg kennzeichnet die zu lîschende Instanz */
           UDeregister (hwndDlg) ;
           break ;
      /* Dialog wird geîffnet */
      case WM_INITDLG:
           /* Blasenhilfe registrieren */
           URegister (hwndDlg, /* Dialog Handle */
                      hmod_g,  /* Ressource Handle */
                      TRUE,    /* Dialog und kein Frame */
                      1) ;     /* ID des Timers (vermeidet Clash) */
           return (MRESULT) FALSE ;
      }
   /* Standardprozedur aktivieren */
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

