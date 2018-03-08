/* $Header: D:\Projekte\Redaktion Toolbox\Geschwindigkeitsrausch\Source\RCS\PERFORM.C 1.3 1997/12/01 16:47:56 HAWI Exp $ */

#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PERFORM.H"

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
             if   (PRegisterClass (hab))
                  {
                  /* Ein normaler Dialog reicht fr dieses Programm */
                  WinDlgBox (HWND_DESKTOP,
                             HWND_DESKTOP,
                             DlgWndProc,
                             NULLHANDLE,
                             IDD_PERFORM,
                             NULL) ;
                  }
             /* Message Queue wieder wegwerfen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab) ;
        }
   /* und tschá */
   return 0 ;
   }

/* Fensterprozedur des Dialoges */
MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   /* Welche Message soll verarbeitet werden */
   switch (msg)
      {
      /* Der Benutzer startet eine Aktion */
      case WM_COMMAND:
           /* Was will er? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Benutzer hat ESC gedrckt, Anwendung verlassen */
              case DID_CANCEL:
                   /* Dialog wegwerfen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Der Benutzer hat de Ok Button gedrckt, Anwendung verlassen */
              case DID_OK:
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Ein Event eines Kontrollelementes trifft ein */
      case WM_CONTROL:
           {
           PDLG  pdlg ;
           ULONG ulCPU ;
           ULONG ulCPUs ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welches Kontrollelement will was? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Spinbutton */
              case IDSP_P1:
                   /* Was will der Spinbutton? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein neuer Eintrag wurde selektiert */
                      case SPBN_CHANGE:
                           /* Die gesetzten Groessen abfragen */
                           WinSendMsg (pdlg->hwndSpinbutton,
                                       SPBM_QUERYLIMITS,
                                       MPFROMP (&ulCPUs),
                                       MPFROMP (&ulCPU)) ;
                           /* Die ausgew„hlte CPU abfragen */
                           WinSendMsg (pdlg->hwndSpinbutton,
                                       SPBM_QUERYVALUE,
                                       MPFROMP (&ulCPU),
                                       MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
                           /* Der Performanceklasse die Anzahl CPUs und die zu beobachtende CPU mitteilen */
                           WinSendDlgItemMsg (hwndDlg,
                                              IDT_P2,
                                              WMPPX_SETCPU,
                                              MPFROMLONG (ulCPUs),   /* Anzahl CPUs */
                                              MPFROMLONG (ulCPU)) ;  /* Diese CPU monitoren */
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Der Dialog wird beendet */
      case WM_DESTROY:
           {
           PDLG pdlg ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Instanzdaten l”schen */
           free (pdlg) ;
           break ;
           }
      /* Der Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLG  pdlg ;
           ULONG ul ;

           /* Speicherbereich fr die Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Handle des Spinbuttons merken */
           pdlg->hwndSpinbutton = WinWindowFromID (hwndDlg,
                                                   IDSP_P1) ;
           DosQuerySysInfo (26, /* QSV_NUMPROCESSORS */
                            26, /* QSV_NUMPROCESSORS */
                            &ul,
                            sizeof (ULONG)) ;
           /* Wenn 0, dann kein SMP also eine CPU vorhanden */
           if   (ul == 0)
                {
                ul = 1 ;
                }
           /* Anzahl CPUs setzen */
           WinSendMsg (pdlg->hwndSpinbutton,
                       SPBM_SETLIMITS,
                       MPFROMLONG (ul),
                       MPFROMLONG (1)) ;
           /* Den aktuellen Wert des Spinbuttons setzen */
           WinSendMsg (pdlg->hwndSpinbutton,
                       SPBM_SETCURRENTVALUE,
                       MPFROMLONG (1),
                       0) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Standardverarbeitung fr alle anderen Events */
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

/* Diese Funktion registriert die neue Fensterklasse */
BOOL PRegisterClass (HAB hab)
   {
   CLASSINFO clsi ;

   /* Die Daten der Standard Ausgabefeldklasse abfragen */
   WinQueryClassInfo (hab,
                      WC_STATIC,
                      &clsi) ;
   /* Unsere eigene Klasse registrieren mit zus„tzlichem Platz fr einen VOID Pointer */
   return WinRegisterClass (hab,
                            PXK_CLASSNAME,
                            (PFNWP) PWndProc,
                            0,
                            clsi.cbWindowData + sizeof (PVOID)) ;
   }

/* Das ist die Messageverarbeitung der neuen Fensterklasse */
MRESULT EXPENTRY PWndProc (HWND   hwnd,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   PWND pwnd ;

   /* Pointer auf Instanzdaten holen */
   pwnd = WinQueryWindowPtr (hwnd,
                             QWL_USER) ;
   /* Welche Message soll verarbeitet werden */
   switch (msg)
      {
      /* Das Fenster wird angelegt */
      case WM_CREATE:
           {
           CLASSINFO clsi ;
           RECTL     rcl ;

           /* Speicherbereich fr die Instanzdaten allokieren */
           pwnd = calloc (1,
                          sizeof (WND)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwnd,
                            QWL_USER,
                            pwnd) ;
           /* Den Handle Anchor Block merken */
           pwnd->hab = WinQueryAnchorBlock (hwnd) ;
           /* Die Daten der Standard Ausgabefeldklasse ermitteln */
           WinQueryClassInfo (pwnd->hab,
                              WC_STATIC,
                              &clsi) ;
           /* Hier erfolgt die Original Messageverarbeitung der Ausgabefeldklasse,
              diese wird von uns ja nur "gesubclassed" */
           pwnd->pfnwp = clsi.pfnWindowProc ;
           /* Fenstergr”áe abfragen */
           WinQueryWindowRect (hwnd,
                               &rcl) ;
           /* Anzahl horizontale Pixel im Fenster minus 2 * 1 Pixel fr die beiden R„nder */
           pwnd->ulMaxColumns = rcl.xRight - rcl.xLeft - 2 ;
           /* Anzahl vertikaler Pixel im Fenster minus 2 * 1 Pixel fr die beiden R„nder */
           pwnd->ulMaxRows = rcl.yTop - rcl.yBottom - 2 ;
           /* Ein Array mit Korrdinatenp„archen fr die Anzahl der horizontalen Pixel allokieren */
           pwnd->paptl = calloc (pwnd->ulMaxColumns,
                                 sizeof (POINTL)) ;
           break ;
           }
      /* Das Fenster wird geschlossen */
      case WM_DESTROY:
           {
           MRESULT mr ;

           /* Zun„chst soll die Original Ausgabefeldklasse ihre Arbeit verrichten */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Ist der Pointer auf die CPU Daten gltig? */
           if   (pwnd->pcpuutil)
                {
                /* Ja, dann diesen Speicherbereich l”schen */
                free (pwnd->pcpuutil) ;
                }
           /* Ist der Timer aktiv? */
           if   (pwnd->fTimer)
                {
                /* Ja, dann Timer stoppen */
                WinStopTimer (pwnd->hab,
                              hwnd,
                              PXK_TIMER) ;
                }
           /* Instanzdaten l”schen */
           free (pwnd) ;
           /* Das Ergebnis der Original Ausgabefeldklasse zurckliefern */
           return mr ;
           }
      /* Das Fenster will bemalt werden */
      case WM_PAINT:
           {
           DOUBLE    d ;
           HPS       hps ;
           MRESULT   mr ;
           MYCPUUTIL mycpuutil ;
           RECTL     rcl ;
           ULONG     ul ;
           ULONG     ulI ;

           /* Zun„chst soll die Original Ausgabefeldklasse ihre Arbeit verrichten */
           mr = pwnd->pfnwp (hwnd,
                             msg,
                             mp1,
                             mp2) ;
           /* Aufruf des Performance API Befehles */
           if   (DosPerfSysCall (CMD_KI_RDCNT,
                                 (ULONG) pwnd->pcpuutil,
                                 0,
                                 0) == 0)
                {
                /* CPU Nummer 0 basieren */
                ul = pwnd->ulCPU - 1 ;
                /* Aus den HIGH/LOW Wert P„archen DOUBLEs machen */
                mycpuutil.dBusy = LL2F(pwnd->pcpuutil[ul].ulBusyLow, pwnd->pcpuutil[ul].ulBusyLow) ;
                mycpuutil.dIdle = LL2F(pwnd->pcpuutil[ul].ulIdleLow, pwnd->pcpuutil[ul].ulIdleLow) ;
                mycpuutil.dIntr = LL2F(pwnd->pcpuutil[ul].ulIntrLow, pwnd->pcpuutil[ul].ulIntrLow) ;
                mycpuutil.dTime = LL2F(pwnd->pcpuutil[ul].ulTimeLow, pwnd->pcpuutil[ul].ulTimeLow) ;
                /* Haben wir einen Vergangenheitswert? */
                if   (pwnd->mycpuutilPrev.dTime)
                     {
                     /* Ist der Monitor bereits am rechten Rand angelangt? */
                     if   (pwnd->ulColumns >= pwnd->ulMaxColumns - 2)
                          {
                          /* Ja, dann alle Pixel P„archen um 1 nach links shiften */
                          for  (ulI = 0; ulI < pwnd->ulMaxColumns - 2; ulI++)
                               {
                               /* Pixel P„archen shiften */
                               pwnd->paptl [ulI] = pwnd->paptl [ulI + 1] ;
                               /* Spaltenz„hler (x Koordinate) neu setzen */
                               pwnd->paptl[ulI].x = ulI + 1 ;
                               }
                          }
                     else
                          {
                          /* Nein, dann Spaltenz„hler um 1 erh”hen */
                          pwnd->ulColumns++ ;
                          }
                     /* Neuen Ausschlag berechnen */
                     d = (mycpuutil.dBusy - pwnd->mycpuutilPrev.dBusy) / (mycpuutil.dTime - pwnd->mycpuutilPrev.dTime) * 100 ;
                     /* In das nun neue Pixelp„archen die neue x Koordinate eintragen */
                     pwnd->paptl[pwnd->ulColumns].x = pwnd->ulColumns ;
                     /* Ausschlag gibt die y Koordinate */
                     pwnd->paptl[pwnd->ulColumns].y = pwnd->ulMaxRows * d / 100 ;
                     /* Wird der untere Rand berschritten? */
                     if   (pwnd->paptl[pwnd->ulColumns].y <= 0)
                          {
                          /* Ja, also setzen */
                          pwnd->paptl[pwnd->ulColumns].y = 1 ;
                          }
                     /* Wird der oeber Rand berschritten? */
                     if   (pwnd->paptl[pwnd->ulColumns].y >= pwnd->ulMaxRows)
                          {
                          /* Ja, also abschneiden */
                          pwnd->paptl[pwnd->ulColumns].y = pwnd->ulMaxRows ;
                          }
                     /* Presentation Space mit Fensterhandle ermitteln */
                     hps = WinGetPS (hwnd) ;
                     /* Die Fenstergr”áe abfragen */
                     WinQueryWindowRect (hwnd,
                                         &rcl) ;
                     /* Den Fensterhintergrund zeichnen, alte Werte l”schen */
                     WinFillRect (hps,
                                  &rcl,
                                  SYSCLR_DIALOGBACKGROUND) ;
                     /* Die Fensterumrandung zeichnen */
                     WinDrawBorder (hps,
                                    &rcl,
                                    1,
                                    1,
                                    0,
                                    0,
                                    DB_DEPRESSED) ;
                     /* Den HPS Zeiger auf das erste Pixel P„archen plazieren */
                     GpiMove (hps,
                              &pwnd->paptl [0]) ;
                     /* Die Anzahl der vorliegenden Spalten bezeichnen lassen */
                     GpiPolyLine (hps,
                                  pwnd->ulColumns,
                                  &pwnd->paptl [0]) ;
                     /* Presentation Space wieder freigeben */
                     WinReleasePS (hps) ;
                     }
                /* Die aktuellen Daten als Vergangenheitswert sichern */
                pwnd->mycpuutilPrev = mycpuutil ;
                }
           /* Das Ergebnis der Original Ausgabefeldklasse zurckliefern */
           return mr ;
           }
      /* Ein Timer meldet sich */
      case WM_TIMER:
           /* Welcher ist es? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Ist es meiner? */
              case PXK_TIMER:
                   /* Ja, dann Fenster invalidieren. Dies fhrt zu einer WM_PAINT Message */
                   WinInvalidateRect (hwnd,
                                      NULL,
                                      FALSE) ;
                   break ;
              }
           return (MRESULT) FALSE ;
      /* Die Anwendung meldet die Anzahl der CPUs und die Nummer der CPU, die durch diese
         Fensterinstanz "gemonitored" werden soll */
      case WMPPX_SETCPU:
           /* Gesamtanzahl CPUs */
           pwnd->ulCPUs = LONGFROMMP (mp1) ;
           /* Diese CPU soll berwacht werden */
           pwnd->ulCPU = LONGFROMMP (mp2) ;
           /* Ist der Pointer auf die CPU Daten gltig? */
           if   (pwnd->pcpuutil)
                {
                /* Ja, dann diesen Speicherbereich l”schen */
                free (pwnd->pcpuutil) ;
                }
           /* Den Speicherbereich auf die CPU Daten mit der neuen Gesamtanzahl allokieren */
           pwnd->pcpuutil = calloc (pwnd->ulCPUs,
                                    sizeof (CPUUTIL)) ;

           /* Ist der Timer noch nicht aktiv? */
           if   (!pwnd->fTimer)
                {
                /* Ja, also Timer starten */
                WinStartTimer (pwnd->hab,
                               hwnd,
                               PXK_TIMER,
                               1000) ;
                /* Flag setzen, daá Timer aktiv */
                pwnd->fTimer = TRUE ;
                }
           /* Mit dem ersten Pixel P„archen links beginnen */
           pwnd->ulColumns = 0 ;
           /* Das erste Pixelop„archen auf 1 setzen um Umrandung nicht zu berschreiben */
           pwnd->paptl[pwnd->ulColumns].x = 1 ;
           pwnd->paptl[pwnd->ulColumns].y = 1 ;
           return (MRESULT) FALSE ;
      }
   /* Aufruf der Original Ausgabefeld Fensterklasse */
   return pwnd->pfnwp (hwnd,
                       msg,
                       mp1,
                       mp2) ;
   }

