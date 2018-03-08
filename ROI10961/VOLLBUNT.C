/* $Header: D:/Projekte/Voll bunt/Source/rcs/VOLLBUNT.C 1.1 1996/07/31 19:25:18 HaWi Exp $ */

/* OS/2 Kopfdateien einlesen */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien einlesen */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikationskopfdatei einlesen */
#include "VOLLBUNT.H"

/* Einige Applikations globale Definitionen */
BOOL    fMinimized_g = FALSE ;      /* Ist Frame minimized? */
HAB     hab_g = NULLHANDLE ;        /* Handle Anchor Block */
HINI    hini_g = NULLHANDLE ;       /* Handle Applikations INI */
HWND    hwndClient_g = NULLHANDLE ; /* Handle Client Area */
HWND    hwndFrame_g = NULLHANDLE ;  /* Handle Frame Window */
PSZ     pszVollBunt_g = "VollBunt" ; /* Applikationseintrag in INI */

/* Fensterprozedur des Dialoges */
MRESULT EXPENTRY VollBuntDlgWndProc (HWND   hwndDlg,
                                     MSG    msg,
                                     MPARAM mp1,
                                     MPARAM mp2)
   {
   /* Welche Message ist von Interesse? */
   switch (msg)
      {
      /* Dialog wird geschlossen */
      case WM_DESTROY:
           /* Presentation Parameter und Position des Dialoges in INI ablegen */
           SaveDialog (hini_g,
                       hwndDlg,
                       pszVollBunt_g) ;
           break ;
      /* Dialog wird geîffnet */
      case WM_INITDLG:
           /* Presentation Parameter und Position des Dialoges restaurieren */
           RestoreDialog (hini_g,
                          hwndDlg,
                          pszVollBunt_g) ;
           return (MRESULT) FALSE ;
      /* Der Benutzer hat einen Presentation Parameter auf dem Dialog fallen gelassen */
      case WM_PRESPARAMCHANGED:
           /* Diesen Presentation Parameter auf die Controls verteilen */
           DistributePresparamChange (hwndDlg,
                                      mp1) ;
           break ;
      }
   /* Aufruf der Standard Dialog Verarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Fensterprozedur des Hauptfensters */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Message ist von Interesse? */
   switch (msg)
      {
      /* Fenster wird geschlossen */
      case WM_CLOSE:
           /* Nur wenn Hauptfenster nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Presentation Parameter und Position des Hauptfensters in INI ablegen */
                SaveDialog (hini_g,
                            hwnd,
                            pszVollBunt_g) ;
                /* Fenster Koordinaten auch in INI ablegen */
                SaveWindow (hini_g,
                            WinQueryWindow (hwnd,
                                            QW_PARENT),
                            pszVollBunt_g) ;
                }
           break ;
      /* Eine Benutzeraktion wurde aktiviert */
      case WM_COMMAND:
           /* Welche? */
           switch (SHORT1FROMMP (mp1))
              {
              /* MenÅpunkt Dialog */
              case IDM_VOLLBUNT2:
                   /* Dialog ausgeben */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              VollBuntDlgWndProc,
                              NULLHANDLE,
                              IDD_VOLLBUNT,
                              NULL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Hauptfenster wird gestartet */
      case WM_CREATE:
           /* Verzîgerten Start aktivieren */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Soll PM das Client zeichnen? */
      case WM_ERASEBACKGROUND:
           /* Ja! */
           return (MRESULT) TRUE ;
      /* Fenster wurde minimiert bzw. maximiert */
      case WM_MINMAXFRAME:
           /* Status merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Verzîgerter Start */
      case WMP_MAININIT:
           /* Presentation Parameter und Position des Hauptfensters aus INI restaurieren */
           RestoreDialog (hini_g,
                          hwnd,
                          pszVollBunt_g) ;
           /* Fenster Koordinaten aus INI restaurieren */
           RestoreWindow (hini_g,
                          hwndFrame_g,
                          pszVollBunt_g) ;
           return (MRESULT) FALSE ;
      }
   /* Aufruf der Standard Fenster Verarbeitung */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Programmstart ohne Parameter also muss diese Funktion nicht im Code auftauchen*/
VOID _setuparg (VOID)
   {
   }

INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Anmeldung beim PM liefert Handle Anchor Block */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Anlegen einer Message Queue mit Standard Grî·e */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszVollBunt_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszVollBunt_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_VOLLBUNT,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Applikations INI anlegen bzw. îffnen */
                       hini_g = LocateIni (hab_g,
                                           "VOLLBUNT.INI") ;
                       /* Message Loop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Verteilung der Messages */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Applikations INI schlie·en */
                       PrfCloseProfile (hini_g) ;
                       /* Standardfenster wieder lîschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue lîschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Abmeldung beim PM */
        WinTerminate (hab_g) ;
        }
   /* Applikation beenden */
   return 0 ;
   }

/* Hilfsfunktion zur öberprÅfung eines Laufwerks auf READONLY */
BOOL CheckForReadOnlyDrive (CHAR ch) /* Drive letter */
   {
   BOOL  f ;
   CHAR  szBuffer [] = "C:\\~TMPTMP.TM~" ;
   FILE* pfile ;

   /* Laufwerksbuchstaben in Dateinamen einpflanzen */
   *szBuffer = ch ;
   /* Kann temporÑre Datei geschrieben werden */
   if   ((pfile = fopen (szBuffer,
                        "w")) != NULL)
        {
        /* Ja, temporÑre Datei wieder schlie·en */
        fclose (pfile) ;
        /* TemporÑre Datei lîschen */
        remove (szBuffer) ;
        /* Drive ist nicht READONLY */
        f = TERROR ;
        }
   else
        {
        /* Drive ist READONLY */
        f = TNOERROR ;
        }
   /* Ok */
   return f ;
   }

/* Hilfsfunktion zum Verteilen eines Presentation Parameters auf Kontrollelemente */
BOOL DistributePresparamChange (HWND   hwnd, /* Handle Dialog */
                                MPARAM mp)   /* mp1 der WM_PRESPARAMCHANGED Message */
   {
   CHAR  sz [128] ;
   HENUM henum ;
   HWND  hwndTemp ;
   RGB2  rgb2 ;

   /* Welcher Presentation Parameter? */
   switch (LONGFROMMP (mp))
      {
      /* Background Color */
      case PP_BACKGROUNDCOLOR:
           /* Den Wert des Presentation Parameters vom Dialog ermitteln */
           if   (WinQueryPresParam (hwnd,
                                    PP_BACKGROUNDCOLOR,
                                    0,
                                    NULL,
                                    sizeof (RGB2),
                                    &rgb2,
                                    QPF_NOINHERIT))
                {
                /* Enumeration Åber alle Kontrollelemente */
                henum = WinBeginEnumWindows (hwnd) ;
                /* In einer Schleife alle Kontrollelemente abarbeiten */
                while ((hwndTemp = WinGetNextWindow (henum)) != NULLHANDLE)
                   {
                   /* Presentation Parameter fÅr dieses Kontrollelement setzen */
                   WinSetPresParam (hwndTemp,
                                    PP_BACKGROUNDCOLOR,
                                    sizeof (RGB2),
                                    &rgb2) ;
                   }
                /* Ende der Enumeration */
                WinEndEnumWindows (henum) ;
                }
           break ;
      /* Font */
      case PP_FONTNAMESIZE:
           /* Den Wert des Presentation Parameters vom Dialog ermitteln */
           if   (WinQueryPresParam (hwnd,
                                    PP_FONTNAMESIZE,
                                    0,
                                    NULL,
                                    sizeof (sz),
                                    sz,
                                    QPF_NOINHERIT))
                {
                /* Enumeration Åber alle Kontrollelemente */
                henum = WinBeginEnumWindows (hwnd) ;
                /* In einer Schleife alle Kontrollelemente abarbeiten */
                while ((hwndTemp = WinGetNextWindow (henum)) != NULLHANDLE)
                   {
                   /* Presentation Parameter fÅr dieses Kontrollelement setzen */
                   WinSetPresParam (hwndTemp,
                                    PP_FONTNAMESIZE,
                                    strlen (sz) + 1,
                                    sz) ;
                   }
                /* Ende der Enumeration */
                WinEndEnumWindows (henum) ;
                }
           break ;
      /* Foreground Color */
      case PP_FOREGROUNDCOLOR:
           /* Den Wert des Presentation Parameters vom Dialog ermitteln */
           if   (WinQueryPresParam (hwnd,
                                    PP_FOREGROUNDCOLOR,
                                    0,
                                    NULL,
                                    sizeof (RGB2),
                                    &rgb2,
                                    QPF_NOINHERIT))
                {
                /* Enumeration Åber alle Kontrollelemente */
                henum = WinBeginEnumWindows (hwnd) ;
                /* In einer Schleife alle Kontrollelemente abarbeiten */
                while ((hwndTemp = WinGetNextWindow (henum)) != NULLHANDLE)
                   {
                   /* Presentation Parameter fÅr dieses Kontrollelement setzen */
                   WinSetPresParam (hwndTemp,
                                    PP_FOREGROUNDCOLOR,
                                    sizeof (RGB2),
                                    &rgb2) ;
                   }
                /* Ende der Enumeration */
                WinEndEnumWindows (henum) ;
                }
           break ;
      }
   /* Ok */
   return TNOERROR ;
   }

/* Hilfsfunktion zum Anlegen bzw. Lesen einer Applikations INI */
HINI LocateIni (HAB hab,    /* Handle Anchor Block */
                PSZ pszIni) /* Dateiname der INI */
   {
   CHAR  sz [CCHMAXPATH] ;
   FILE* pfile ;
   PPIB  ppib ;
   PSZ   psz ;
   PTIB  ptib ;
   ULONG ul ;

   /* Dateiname der INI vorbesetzen */
   strcpy (sz,
           pszIni) ;
   /* Ist INI im aktuellen Verzeichnis? */
   if   ((pfile = fopen (pszIni,
                         "rb")) != NULL)
        {
        /* Ja! */
        fclose (pfile) ;
        }
   else
        {
        /* Nein, Proze·informationen lesen */
        DosGetInfoBlocks (&ptib,
                          &ppib) ;
        /* EXE Name der Applikation ermitteln */
        DosQueryModuleName (ppib->pib_hmte,
                            sizeof (sz),
                            sz) ;
        /* Liegt das EXE auf einem READONLY Medium? */
        if   (CheckForReadOnlyDrive (*sz) == TNOERROR)
             {
             /* Ja, dann muss INI ins OS2 Verzeichnis */
             strcpy (sz,
                     "C:\\OS2\\") ;
             /* INI Dateiname anhÑngen */
             strcat (sz,
                     pszIni) ;
             /* Boot Drive ermitteln */
             DosQuerySysInfo (QSV_BOOT_DRIVE,
                              QSV_BOOT_DRIVE,
                              &ul,
                              sizeof (ul)) ;
             /* Boot Drive in den OS2 Pfad einblenden */
             *sz = ul + '@' ;
             }
        else
             {
             /* Nein, also INI zum EXE stecken */
             /* Den letzten Backslash suchen */
             if   ((psz = strrchr (sz,
                                   '\\')) != NULL)
                  {
                  /* An Stelle des EXE Namens den INI Namen einblenden */
                  strcpy ((psz + 1),
                          pszIni) ;
                  }
             }
        }
   /* Profil Handle zurÅckliefern */
   return PrfOpenProfile (hab,
                          sz) ;
   }

/* Hilfsfunktion zum Restaurieren von Presentation Parametern und Dialog Positionen */
BOOL RestoreDialog (HINI hini, /* Handle Applikations INI */
                    HWND hwnd, /* Fenster Handle */
                    PSZ  psz)  /* Applikations Kennung in INI */
   {
   CHAR  sz [128] ;
   CHAR  sz2 [128] ;
   HENUM henum ;
   HWND  hwndTemp ;
   PSZ   pszBackground = ".Background" ;
   PSZ   pszFont = ".Font" ;
   PSZ   pszForeground = ".Foreground" ;
   PSZ   pszXLeft = ".xLeft" ;
   PSZ   pszYBottom = ".yBottom" ;
   RGB2  rgb2 ;
   SWP   swp ;
   ULONG ul ;
   ULONG ulI ;
   ULONG ulTemp ;

   /* ID des Fensters ermitteln */
   ul = WinQueryWindowUShort (hwnd,
                              QWS_ID) ;
   /* Position des Fensters ermitteln */
   WinQueryWindowPos (hwnd,
                      &swp) ;
   /* Vorbereiten zum Lesen der X Koordinate */
   sprintf (sz,
            "%u%s",
            ul,
            pszXLeft) ;
   /* Grî·e eines SWP Wertes */
   ulI = sizeof (swp.x) ;
   /* X Koordinate aus INI lesen */
   if   (PrfQueryProfileData (hini,
                              psz,
                              sz,
                              &swp.x,
                              &ulI))
        {
        /* Vorbereiten zum Lesen der Y Koordinate */
        sprintf (sz,
                 "%u%s",
                 ul,
                 pszYBottom) ;
        /* Grî·e eines SWP Wertes */
        ulI = sizeof (swp.y) ;
        /* Y Koordinate aus INI lesen */
        PrfQueryProfileData (hini,
                             psz,
                             sz,
                             &swp.y,
                             &ulI) ;
        /* Neue Fensterposition setzen */
        WinSetWindowPos (hwnd,
                         HWND_TOP,
                         swp.x,
                         swp.y,
                         swp.cx,
                         swp.cy,
                         SWP_MOVE) ;
        }
   /* Enumeration Åber alle Kontrollelemente */
   henum = WinBeginEnumWindows (hwnd) ;
   /* In einer Schleife alle Kontrollelemente abarbeiten */
   while ((hwndTemp = WinGetNextWindow (henum)) != NULLHANDLE)
      {
      /* ID des Kontrollelementes besorgen */
      ulTemp = WinQueryWindowUShort (hwndTemp,
                                     QWS_ID) ;
      /* Vorbereiten zum Lesen der Background Farbe */
      sprintf (sz,
               "%u.%u%s",
               ul,
               ulTemp,
               pszBackground) ;
      /* Grî·e einer Farbe */
      ulI = sizeof (RGB2) ;
      /* Background Farbe aus INI lesen */
      if   (PrfQueryProfileData (hini,
                                 psz,
                                 sz,
                                 &rgb2,
                                 &ulI))
           {
           /* Presentation Parameter fÅr dieses Kontrollelement setzen */
           WinSetPresParam (hwndTemp,
                            PP_BACKGROUNDCOLOR,
                            sizeof (RGB2),
                            &rgb2) ;
           }
      /* Vorbereiten zum Lesen des Fonts */
      sprintf (sz,
               "%u.%u%s",
               ul,
               ulTemp,
               pszFont) ;
      /* Font aus INI lesen */
      if   (PrfQueryProfileString (hini,
                                   psz,
                                   sz,
                                   "",
                                   sz2,
                                   sizeof (sz2)))
           {
           /* Presentation Parameter fÅr dieses Kontrollelement setzen */
           WinSetPresParam (hwndTemp,
                            PP_FONTNAMESIZE,
                            strlen (sz2) + 1,
                            sz2) ;
           }
      /* Vorbereiten zum Lesen der Foreground Farbe */
      sprintf (sz,
               "%u.%u%s",
               ul,
               ulTemp,
               pszForeground) ;
      /* Grî·e einer Farbe */
      ulI = sizeof (RGB2) ;
      /* Foreground Farbe aus INI lesen */
      if   (PrfQueryProfileData (hini,
                                 psz,
                                 sz,
                                 &rgb2,
                                 &ulI))
           {
           /* Presentation Parameter fÅr dieses Kontrollelement setzen */
           WinSetPresParam (hwndTemp,
                            PP_FOREGROUNDCOLOR,
                            sizeof (RGB2),
                            &rgb2) ;
           }
      }
   /* Ende der Enumeration */
   WinEndEnumWindows (henum) ;
   /* Ok */
   return TNOERROR ;
   }

/* Hilfsfunktion zum Restaurieren einer Frame Grî·e und Position */
BOOL RestoreWindow (HINI hini, /* Handle Applikations INI */
                    HWND hwnd, /* Frame Handle */
                    PSZ  psz)  /* Applikations Kennung in INI */
   {
   SWP   swp ;
   ULONG ulI ;

   /* Grî·e eines SWP Wertes */
   ulI = sizeof (swp.x) ;
   /* X Koordinate aus INI lesen */
   if   (PrfQueryProfileData (hini,
                              psz,
                              "Window.xLeft",
                              &swp.x,
                              &ulI))
        {
        /* Grî·e eines SWP Wertes */
        ulI = sizeof (swp.cx) ;
        /* CX Koordinate aus INI lesen */
        PrfQueryProfileData (hini,
                             psz,
                             "Window.xRight",
                             &swp.cx,
                             &ulI) ;
        /* Grî·e eines SWP Wertes */
        ulI = sizeof (swp.y) ;
        /* Y Koordinate aus INI lesen */
        PrfQueryProfileData (hini,
                             psz,
                             "Window.yBottom",
                             &swp.y,
                             &ulI) ;
        /* Grî·e eines SWP Wertes */
        ulI = sizeof (swp.cy) ;
        /* CY Koordinate aus INI lesen */
        PrfQueryProfileData (hini,
                             psz,
                             "Window.yTop",
                             &swp.cy,
                             &ulI) ;
        /* Neue Fensterposition und -grî·e setzen */
        WinSetWindowPos (hwnd,
                         HWND_TOP,
                         swp.x,
                         swp.y,
                         swp.cx,
                         swp.cy,
                         SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW) ;
        }
   else
        {
        /* Standard Fensterposition und -grî·e setzen */
        WinSetWindowPos (hwnd,
                         HWND_TOP,
                         0,
                         0,
                         0,
                         0,
                         SWP_SHOW | SWP_ACTIVATE) ;
        }
   /* Ok */
   return TNOERROR ;
   }

/* Hilfsfunktion zum Sichern von Presentation Parametern und Dialog Positionen in der INI */
BOOL SaveDialog (HINI hini,
                 HWND hwnd,
                 PSZ  psz)
   {
   CHAR  sz [128] ;
   CHAR  sz2 [128] ;
   HENUM henum ;
   HWND  hwndTemp ;
   PSZ   pszBackground = ".Background" ;
   PSZ   pszFont = ".Font" ;
   PSZ   pszForeground = ".Foreground" ;
   PSZ   pszXLeft = ".xLeft" ;
   PSZ   pszYBottom = ".yBottom" ;
   RGB2  rgb2 ;
   SWP   swp ;
   ULONG ul ;
   ULONG ulTemp ;

   /* ID des Fensters ermitteln */
   ul = WinQueryWindowUShort (hwnd,
                              QWS_ID) ;
   /* Position des Fensters ermitteln */
   WinQueryWindowPos (hwnd,
                      &swp) ;
   /* Vorbereiten zum Schreiben der X Koordinate */
   sprintf (sz,
            "%u%s",
            ul,
            pszXLeft) ;
   /* Schreiben der X Koordinate */
   PrfWriteProfileData (hini,
                        psz,
                        sz,
                        &swp.x,
                        sizeof (swp.x)) ;
   /* Vorbereiten zum Schreiben der Y Koordinate */
   sprintf (sz,
            "%u%s",
            ul,
            pszYBottom) ;
   /* Schreiben der Y Koordinate */
   PrfWriteProfileData (hini,
                        psz,
                        sz,
                        &swp.y,
                        sizeof (swp.y)) ;
   /* Enumeration Åber alle Kontrollelemente */
   henum = WinBeginEnumWindows (hwnd) ;
   /* In einer Schleife alle Kontrollelemente abarbeiten */
   while ((hwndTemp = WinGetNextWindow (henum)) != NULLHANDLE)
      {
      /* ID des Kontrollelementes besorgen */
      ulTemp = WinQueryWindowUShort (hwndTemp,
                                     QWS_ID) ;
      /* Liegt Background Presentation Parameter vor? */
      if   (WinQueryPresParam (hwndTemp,
                               PP_BACKGROUNDCOLOR,
                               0,
                               NULL,
                               sizeof (RGB2),
                               &rgb2,
                               QPF_NOINHERIT))
           {
           /* Ja, Vorbereiten zum Schreiben des Background Presentation Parameter in INI */
           sprintf (sz,
                    "%u.%u%s",
                    ul,
                    ulTemp,
                    pszBackground) ;
           /* Schreiben des Background Presentation Parameter in INI */
           PrfWriteProfileData (hini,
                                psz,
                                sz,
                                &rgb2,
                                sizeof (RGB2)) ;
           }
      /* Liegt Font Presentation Parameter vor? */
      if   (WinQueryPresParam (hwndTemp,
                               PP_FONTNAMESIZE,
                               0,
                               NULL,
                               sizeof (sz2),
                               sz2,
                               QPF_NOINHERIT))
           {
           /* Ja, Vorbereiten zum Schreiben des Font Presentation Parameter in INI */
           sprintf (sz,
                    "%u.%u%s",
                    ul,
                    ulTemp,
                    pszFont) ;
           /* Schreiben des Font Presentation Parameter in INI */
           PrfWriteProfileString (hini,
                                  psz,
                                  sz,
                                  sz2) ;
           }
      /* Liegt Foreground Presentation Parameter vor? */
      if   (WinQueryPresParam (hwndTemp,
                               PP_FOREGROUNDCOLOR,
                               0,
                               NULL,
                               sizeof (RGB2),
                               &rgb2,
                               QPF_NOINHERIT))
           {
           /* Ja, Vorbereiten zum Schreiben des Foreground Presentation Parameter in INI */
           sprintf (sz,
                    "%u.%u%s",
                    ul,
                    ulTemp,
                    pszForeground) ;
           /* Schreiben des Foreground Presentation Parameter in INI */
           PrfWriteProfileData (hini,
                                psz,
                                sz,
                                &rgb2,
                                sizeof (RGB2)) ;
           }
      }
   /* Ende der Enumeration */
   WinEndEnumWindows (henum) ;
   /* Ok */
   return TNOERROR ;
   }

/* Hilfsfunktion zum Speichern von Grî·e und Position eines Frames in der INI */
BOOL SaveWindow (HINI hini, /* Applikations INI */
                 HWND hwnd, /* Frame Handle */
                 PSZ  psz)  /* Applikations Kennung in INI */
   {
   SWP swp ;

   /* Position des Fensters ermitteln */
   WinQueryWindowPos (hwnd,
                      &swp) ;
   /* Schreiben der X Koordinate */
   PrfWriteProfileData (hini,
                        psz,
                        "Window.xLeft",
                        &swp.x,
                        sizeof (swp.x)) ;
   /* Schreiben der Y Koordinate */
   PrfWriteProfileData (hini,
                        psz,
                        "Window.yBottom",
                        &swp.y,
                        sizeof (swp.y)) ;
   /* Schreiben der CX Koordinate */
   PrfWriteProfileData (hini,
                        psz,
                        "Window.xRight",
                        &swp.cx,
                        sizeof (swp.cx)) ;
   /* Schreiben der CYKoordinate */
   PrfWriteProfileData (hini,
                        psz,
                        "Window.yTop",
                        &swp.cy,
                        sizeof (swp.cy)) ;
   /* Ok */
   return TNOERROR ;
   }

