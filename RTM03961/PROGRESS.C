/* $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/PROGRESS.C 1.1 1996/03/06 13:54:17 HaWi Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C Kopfdateien */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Applikations Kopfdatei */
#include "PROGRESS.H"

/* Einige modulglobale Variablen */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszProgress_g = "Progress" ;

/* Client Window Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welcher Event? */
   switch (msg)
      {
      /* Applikation schliessen */
      case WM_CLOSE:
           /* Stop message processing */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Benutzeranweisung */
      case WM_COMMAND:
           {
           CHAR sz [128] ;
           PWND pwnd ;

           /* Instanzdaten des Main Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Was will der Benutzer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Systemmenu meldet schliessen */
              case DID_CANCEL:
                   /* Schliessen veranlassen */
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Progress dialog */
              case IDM_DIALOGPROGRESS:
                   /* Progressindikator starten */
                   pwnd->hwndPGS = TProgressCreate (HWND_DESKTOP,
                                                    hwndFrame_g) ;
                   /* Text fuer Progressindikator aus Ressource holen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_SIMPLERZAEHLER,
                                  sizeof (sz),
                                  sz) ;
                   /* Titel dem Progressindikator mitteilen */
                   WinSendMsg (pwnd->hwndPGS,
                               WMP_PGSETTEXT,
                               MPFROMP (sz),
                               MPVOID) ;
                   /* Mit 0 den Progressindikator starten */
                   WinSendMsg (pwnd->hwndPGS,
                               WMP_PGSETVALUE,
                               MPFROMLONG (0),
                               MPVOID) ;
                   /* Counter auf 0 setzen */
                   pwnd->ulCounter = 0 ;
                   /* Timer starten */
                   WinStartTimer (hab_g,
                                  hwnd,
                                  K_TIMER,
                                  100) ;
                   break ;
              }
           break ;
           }
      /* Main window anlegen */
      case WM_CREATE:
           /* Instanzdaten des Fensters allokieren und merken */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Kleine Startverzoegerung */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Main Fenster wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten holen und wieder freigeben */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      /* Hintergrund zeichnen lassen */
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
      /* Timer laeuft ein */
      case WM_TIMER:
           {
           PWND pwnd ;

           /* Instanzdaten des Main Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Welcher Timer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der richtige */
              case K_TIMER:
                   /* Counter um 5 erhoehen */
                   pwnd->ulCounter += 5 ;
                   /* Neuen Wert dem Progressindikator mitteilen */
                   WinSendMsg (pwnd->hwndPGS,
                               WMP_PGSETVALUE,
                               MPFROMLONG (pwnd->ulCounter),
                               MPVOID) ;
                   /* Wenn Counter ueber 100 */
                   if   (pwnd->ulCounter > 100)
                        {
                        /* Dann Timer stoppen */
                        WinStopTimer (hab_g,
                                      hwnd,
                                      K_TIMER) ;
                        /* und Progressindikator wieder entfernen */
                        WinDestroyWindow (pwnd->hwndPGS) ;
                        }
                   break ;
              }
           break ;
           }
      /* Eigentlicher Applikationsstart mit Verzoegerung */
      case WMP_MAININIT:
           /* Window zeigen */
           WinSetWindowPos (hwndFrame_g,
                            HWND_TOP,
                            0,
                            0,
                            0,
                            0,
                            SWP_ACTIVATE | SWP_SHOW) ;
           return (MRESULT) FALSE ;
      }
   /* Standardverarbeitung fuer normale Fenster */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Einstieg in die Anwendung */
INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_AUTOICON | FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Beim PM anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Anwendungsklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszProgress_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Hauptfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE,
                                                           &ulCreateFlags,
                                                           pszProgress_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_PROGRESS,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Schleife */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Message an Fenster verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Hauptfenster loeschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue wieder loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM abmelden */
        WinTerminate (hab_g) ;
        }
   /* und zurueck */
   return 0 ;
   }

/* Diese Funktion baut aus einem CDATE und einem CTIME einen benutzerdefiniertes Datums und Zeitformat */
VOID TDateTimeToString (PSZ    psz,
                        PCDATE pcdate,
                        PCTIME pctime)
   {
   CHAR     sz [3] ;
   CHAR     sz2 [3] ;
   CHAR     sz3 [4] ;
   DATETIME date ;
   ULONG    ulFormat ;
   ULONG    ulTime ;

   /* Sind die Uebergabestrukturen leer? */
   if   ((!pcdate->day) &&
         (!pcdate->month) &&
         (!pcdate->year) &&
         (!pctime->hours) &&
         (!pctime->minutes) &&
         (!pctime->seconds))
        {
        /* ja, dann aktuelle Tageszeit ermitteln */
        DosGetDateTime (&date) ;
        }
   else
        {
        /* Sonst die Uebergabewerte umfuellen */
        date.day = pcdate->day ;
        date.month = pcdate->month ;
        date.year = pcdate->year ;
        date.hours = pctime->hours ;
        date.minutes = pctime->minutes ;
        date.seconds = pctime->seconds ;
        }
   /* Gewuenschtes Datumsformat abfragen */
   ulFormat = PrfQueryProfileInt (HINI_PROFILE,
                                  "PM_National",
                                  "iDate",
                                  1) ;
   /* Gewuenschtes Zeitformat abfragen */
   ulTime = PrfQueryProfileInt (HINI_PROFILE,
                                "PM_National",
                                "iTime",
                                1) ;
   /* Datumstrenner abfragen */
   PrfQueryProfileString (HINI_PROFILE,
                          "PM_National",
                          "sDate",
                          ".",
                          sz,
                          sizeof (sz)) ;
   /* Zeittrenner abfragen */
   PrfQueryProfileString (HINI_PROFILE,
                          "PM_National",
                          "sTime",
                          ":",
                          sz2,
                          sizeof (sz2)) ;
   *sz3 = '\0' ;
   /* Wenn 12 Stunden Zeit gewuenscht */
   if   (!ulTime)
        {
        /* Nur wenn mehr Uhrzeit groesser als 11:59 */
        if   (date.hours > 11)
             {
             /* 12 Stunden abziehen */
             date.hours -= 12 ;
             /* und den Anhang (ueblicherweise PM) abfragen */
             PrfQueryProfileString (HINI_PROFILE,
                                    "PM_National",
                                    "s2359",
                                    "",
                                    sz3,
                                    sizeof (sz3)) ;
             }
        else
             {
             /* sonst den Anhang (ueblicherweise AM) abfragen */
             PrfQueryProfileString (HINI_PROFILE,
                                    "PM_National",
                                    "s1159",
                                    "",
                                    sz3,
                                    sizeof (sz3)) ;
             }
        }
   /* Welches Datumsformat ist gewuenscht? */
   switch (ulFormat)
      {
      /* MMDDYYYY */
      case 0:
           sprintf (psz,
                    "%02u%c%02u%c%04u %02u%c%02u%c%02u %s",
                    (ULONG) date.month,
                    *sz,
                    (ULONG) date.day,
                    *sz,
                    (ULONG) date.year,
                    (ULONG) date.hours,
                    *sz2,
                    (ULONG) date.minutes,
                    *sz2,
                    (ULONG) date.seconds,
                    sz3) ;
           break ;
      /* DDMMYYYY */
      case 1:
           sprintf (psz,
                    "%02u%c%02u%c%04u %02u%c%02u%c%02u %s",
                    (ULONG) date.day,
                    *sz,
                    (ULONG) date.month,
                    *sz,
                    (ULONG) date.year,
                    (ULONG) date.hours,
                    *sz2,
                    (ULONG) date.minutes,
                    *sz2,
                    (ULONG) date.seconds,
                    sz3) ;
           break ;
      /* YYYYMMDD */
      case 2:
           sprintf (psz,
                    "%04u%c%02u%c%02u %02u%c%02u%c%02u %s",
                    (ULONG) date.year,
                    *sz,
                    (ULONG) date.month,
                    *sz,
                    (ULONG) date.day,
                    (ULONG) date.hours,
                    *sz2,
                    (ULONG) date.minutes,
                    *sz2,
                    (ULONG) date.seconds,
                    sz3) ;
           break ;
      }
   }

/* Diese Funktion fuellt DATETIME Daten in CDATE und CTIME Strukturen */
VOID TFillDateTime (PCDATE    pcdate,
                    PCTIME    pctime,
                    PDATETIME pdate)
   {
   DATETIME date ;

   /* Ist ueberhaupt etwas drin? */
   if   ((!pdate->day) &&
         (!pdate->month) &&
         (!pdate->year) &&
         (!pdate->hours) &&
         (!pdate->minutes) &&
         (!pdate->seconds))
        {
        /* Nein, dann aktuelle Tageszeit nehmen */
        DosGetDateTime (&date) ;
        pcdate->day = date.day ;
        pcdate->month = date.month ;
        pcdate->year = date.year ;
        pctime->hours = date.hours ;
        pctime->minutes = date.minutes ;
        pctime->seconds = date.seconds ;
        }
   else
        {
        /* ansonsten einfach umfuellen */
        pcdate->day = pdate->day ;
        pcdate->month = pdate->month ;
        pcdate->year = pdate->year ;
        pctime->hours = pdate->hours ;
        pctime->minutes = pdate->minutes ;
        pctime->seconds = pdate->seconds ;
        }
   }

/* Diese Funktion kreiert den Progressindikator */
HWND EXPENTRY TProgressCreate (HWND hwndParent,
                               HWND hwndOwner)
   {
   /* Progressindikator Dialog wird non-modal gestartet */
   return WinLoadDlg (hwndParent,
                      hwndOwner,
                      TProgressProc,
                      NULLHANDLE,
                      IDD_PG,
                      NULL) ;
   }

/* Dialog Fenster Prozedur des Progressindikators */
MRESULT EXPENTRY TProgressProc (HWND   hwndDlg,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welcher Event? */
   switch (msg)
      {
      /* Der Benutzer will etwas */
      case WM_COMMAND:
           {
           CHAR sz [150] ;
           CHAR sz2 [27] ;

           /* Was will er? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Stop auf Progressindikator gedrueckt */
              case DID_CANCEL:
                   /* Pushbutton mit dem Stop jetzt disablen */
                   WinEnableControl (hwndDlg,
                                     DID_CANCEL,
                                     FALSE) ;
                   /* Titel des Progressindikators lesen */
                   WinQueryWindowText (hwndDlg,
                                       sizeof (sz),
                                       sz) ;
                   /* Aus der Ressource den Text "wird gestoppt besorgen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_WIRDGESTOPPT,
                                  sizeof (sz2),
                                  sz2) ;
                   /* diesen Text an den Titel des Progressindikators anhaengen */
                   strcat (sz,
                           sz2);
                   /* Neuen Text wieder in Titelzeile ausgeben */
                   WinSetWindowText (hwndDlg,
                                     sz) ;
                   /* An den Owner des Progressindiaktors den Stop uebermitteln */
                   WinSendMsg (WinQueryWindow (hwndDlg,
                                               QW_OWNER),
                               WMP_PGSTOP,
                               MPFROMHWND (hwndDlg),
                               MPVOID) ;
                   break ;
              }
           break ;
           }
         /* Progressindikator Dialog wird gestartet */
         case WM_INITDLG:
           {
           CDATE    cdateTemp ;
           CHAR     sz [150] ;
           CTIME    ctimeTemp ;
           DATETIME date ;
           PDLGPG   pdlg ;
           ULONG    ulI ;

           /* Platz fuer die Instanzdaten des Progressindikators besorgen */
           pdlg = calloc (1,
                          sizeof (DLGPG)) ;
           /* Pointer auf Instanzdaten in Window Words ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Progressindiaktor auf Owner zentrieren */
           TWindowCenter (hwndDlg,
                          WinQueryWindow (hwndDlg,
                                          QW_OWNER)) ;
           /* An den Sliderbar die Dicke der Ticks schicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDSL_PG1,
                              SLM_SETTICKSIZE,
                              MPFROM2SHORT (SMA_SETALLTICKS, 4),
                              MPVOID) ;
           /* In einer Schleife */
           for  (ulI = 0; ulI < 101; ulI += 25)
                {
                /* Alle 25 Ticks einen dickeren Tick malen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDSL_PG1,
                                   SLM_SETTICKSIZE,
                                   MPFROM2SHORT (ulI, 8),
                                   MPVOID) ;
                /* Prozentwerte fuer Beschriftung zusammenbauen */
                _ltoa (ulI,
                       sz,
                       10) ;
                strcat (sz,
                        "%");
                /* und ausgeben */
                WinSendDlgItemMsg (hwndDlg,
                                   IDSL_PG1,
                                   SLM_SETSCALETEXT,
                                   MPFROMSHORT (ulI),
                                   MPFROMP (sz)) ;
                }
           /* Datetime vorbesetzen */
           memset (&date,
                   0,
                   sizeof (DATETIME)) ;
           /* und nach CDATE und CTIME umsetzen */
           TFillDateTime (&cdateTemp,
                          &pdlg->ctimeTemp,
                          &date) ;
           ctimeTemp = pdlg->ctimeTemp ;
           /* Differenz zwischen gespeicherter Uhrzeit und aktueller Uhrzeit berechnen */
           TTimeDiff (&pdlg->ctimeTemp,
                      &ctimeTemp) ;
           /* Zeitwert in ein benutzerdefiniertes Format bringen */
           TTimeToString (sz,
                          &ctimeTemp) ;
           /* AM/PM (falls vorhanden) hinten abschneiden */
           sz [8] = '\0' ;
           /* Zeit ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDT_PG2,
                              sz) ;
           break ;
           }
      /* Cancel soll nicht moeglich sein */
      case WMP_PGNOCANCEL:
           {
           HWND hwnd ;

           /* Cancel Pushbutton disablen */
           WinEnableControl (hwndDlg,
                             DID_CANCEL,
                             FALSE) ;
           /* Systemmenue Handle besorgen */
           hwnd = WinWindowFromID (hwndDlg,
                                   FID_SYSMENU) ;
           /* Hide im Systemmenue abschalten */
           WinSendMsg (hwnd,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (SC_HIDE, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           /* Close im Systemmenue abschalten */
           WinSendMsg (hwnd,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (SC_CLOSE, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           return (MRESULT) FALSE ;
           }
      /* Abfrage des aktuellen Wertes im Progressindikator */
      case WMP_PGQUERYVALUE:
           /* Aktuelle Position des Sliderarms ermitteln und an Abfrager returnieren */
           return MRFROMLONG (WinSendDlgItemMsg (hwndDlg,
                                                 IDSL_PG1,
                                                 SLM_QUERYSLIDERINFO,
                                                 MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                                                 MPVOID)) ;
      /* Der Text des Progressindikators wird uebergeben */
      case WMP_PGSETTEXT:
           /* Text ausgeben */
           WinSetWindowText (hwndDlg,
                             PVOIDFROMMP (mp1)) ;
           return (MRESULT) FALSE ;
      /* Der aktuelle Wert des Progressindikators wird uebergeben */
      case WMP_PGSETVALUE:
           {
           CDATE    cdateTemp ;
           CHAR     sz [150] ;
           CTIME    ctimeTemp ;
           DATETIME date ;
           PDLGPG   pdlg ;

           /* Pointer auf Instanzdaten des Progressindikators holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Ist ueberhaupt ein Wert drin? */
           if   (LONGFROMMP (mp1))
                {
                /* Diesen Wert setzen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDSL_PG1,
                                   SLM_SETSLIDERINFO,
                                   MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE),
                                   MPFROMLONG (LONGFROMMP (mp1))) ;
                }
           /* Datetime vorbesetzen */
           memset (&date,
                   0,
                   sizeof (DATETIME)) ;
           /* und nach CDATE und CTIME umsetzen */
           TFillDateTime (&cdateTemp,
                          &ctimeTemp,
                          &date) ;
           /* Differenz zwischen gespeicherter Uhrzeit und aktueller Uhrzeit berechnen */
           TTimeDiff (&pdlg->ctimeTemp,
                      &ctimeTemp) ;
           /* Zeitwert in ein benutzerdefiniertes Format bringen */
           TTimeToString (sz,
                          &ctimeTemp) ;
           /* AM/PM (falls vorhanden) hinten abschneiden */
           sz [8] = '\0' ;
           /* Zeit ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDT_PG2,
                              sz) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Funktion zum Berechnen der Differenz zwischen zwei Zeitwerten (in CTIME) */
VOID TTimeDiff (PCTIME pctimeOrg,
                PCTIME pctime)
   {
   ULONG ul ;
   ULONG ulOrg ;

   ul = 0 ;
   ulOrg = 0 ;
   ul += pctime->seconds ;
   ul += pctime->minutes * 60 ;
   ul += pctime->hours * 3600 ;
   ulOrg += pctimeOrg->seconds ;
   ulOrg += pctimeOrg->minutes * 60 ;
   ulOrg += pctimeOrg->hours * 3600 ;
   ul -= ulOrg ;
   pctime->hours = ul / 3600 ;
   ul = ul % 3600 ;
   pctime->minutes = ul / 60 ;
   pctime->seconds = ul % 60 ;
   }

/* Diese Funktion baut aus einem CTIME einen benutzerdefiniertes Zeitformat */
VOID TTimeToString (PSZ    psz,
                    PCTIME pctime)
   {
   CHAR     sz [3] ;
   CHAR     sz3 [4] ;
   DATETIME date ;
   ULONG    ulTime ;

   /* Ist die Uebergabestruktur leer? */
   if   ((!pctime->hours) &&
         (!pctime->minutes) &&
         (!pctime->seconds))
        {
        /* ja, dann aktuelle Tageszeit ermitteln */
        DosGetDateTime (&date) ;
        }
   else
        {
        /* Sonst die Uebergabewerte umfuellen */
        date.hours = pctime->hours ;
        date.minutes = pctime->minutes ;
        date.seconds = pctime->seconds ;
        }
   /* Gewuenschtes Zeitformat abfragen */
   ulTime = PrfQueryProfileInt (HINI_PROFILE,
                                "PM_National",
                                "iTime",
                                1) ;
   /* Zeittrenner abfragen */
   PrfQueryProfileString (HINI_PROFILE,
                          "PM_National",
                          "sTime",
                          ":",
                          sz,
                          sizeof (sz)) ;
   *sz3 = '\0' ;
   /* Wenn 12 Stunden Zeit gewuenscht */
   if   (!ulTime)
        {
        /* Nur wenn mehr Uhrzeit groesser als 11:59 */
        if   (date.hours > 11)
             {
             /* 12 Stunden abziehen */
             date.hours -= 12 ;
             /* und den Anhang (ueblicherweise PM) abfragen */
             PrfQueryProfileString (HINI_PROFILE,
                                    "PM_National",
                                    "s2359",
                                    "",
                                    sz3,
                                    sizeof (sz3)) ;
             }
        else
             {
             /* sonst den Anhang (ueblicherweise AM) abfragen */
             PrfQueryProfileString (HINI_PROFILE,
                                    "PM_National",
                                    "s1159",
                                    "",
                                    sz3,
                                    sizeof (sz3)) ;
             }
        }
   /* Aufbau des benutzerdefinierten Zeittextes */
   sprintf (psz,
            "%02u%c%02u%c%02u %s",
            (ULONG) date.hours,
            *sz,
            (ULONG) date.minutes,
            *sz,
            (ULONG) date.seconds,
            sz3) ;
   }

/* Funktion zum Zentrieren eines Fensters auf einem anderen Fenster */
BOOL TWindowCenter (HWND hwnd,
                    HWND hwndParent)
   {
   SWP swp ;
   SWP swpParent ;
   SWP swpDesktop ;

   /* Fensterposition des zu positionierenden Fensters */
   WinQueryWindowPos (hwnd,
                      &swp) ;
   /* Fensterposition des Parents */
   WinQueryWindowPos (hwndParent,
                      &swpParent) ;
   /* Fensterposition des Desktops */
   WinQueryWindowPos (HWND_DESKTOP,
                      &swpDesktop) ;
   /* Fenster zentrieren */
   if   (((swpParent.cx - swp.cx) / 2 + swpParent.x) < 0 ||
         ((swpParent.cy - swp.cy) / 2 + swpParent.y) < 0 ||
         ((swpParent.cx - swp.cx) / 2 + swpParent.x + swp.cx) > swpDesktop.cx ||
         ((swpParent.cy - swp.cy) / 2 + swpParent.y + swp.cy) > swpDesktop.cy )
        {
        swpParent = swpDesktop ;
        }
   return WinSetWindowPos (hwnd,
                           HWND_TOP,
                           (swpParent.cx - swp.cx) / 2 + swpParent.x,
                           (swpParent.cy - swp.cy) / 2 + swpParent.y,
                           0,
                           0,
                           SWP_MOVE) ;
   }

