/* $Header: D:/Projekte/Viel System!1/Source/rcs/SYSLVL.C 1.2 1995/11/27 16:48:34 HaWi Exp $ */

/* OS/2 Kopfdateien mit BASE und PM Support laden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* Einige C Kopfdateien laden */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikationskopfdatei laden */
#include "SYSLVL.H"

BOOL  fMinimized_g = FALSE ;
HAB   hab_g = NULLHANDLE ;
HWND  hwndClient_g = NULLHANDLE ;
HWND  hwndFrame_g = NULLHANDLE ;
PSZ   pszSyslvl_g = "Syslvl" ;
ULONG ulWait_g = 0 ;

/* Tabelle mit COUNTRY Informationen zu den SYSLEVEL Dateien */
COUNTRY country [] = {{ 'Z', ""               },
                      { '0', "United States"  },
                      { 'C', "Canada"         },
                      { 'D', "Denmark"        },
                      { 'F', "France"         },
                      { 'G', "German"         },
                      { 'H', "Netherlands"    },
                      { 'I', "Italy"          },
                      { 'J', "Japan"          },
                      { 'L', "Finland"        },
                      { 'N', "Norway"         },
                      { 'P', "Portugal"       },
                      { 'S', "Spain"          },
                      { 'U', "United Kingdom" },
                      { 'W', "Sweden"         }} ;
/* Tabelle mit EDITION Informationen zu den SYSLEVEL Dateien */
EDITION edition [] = {{ 15, "Other"                  },
                      {  0, "OS/2 Standard Edition"  },
                      {  1, "OS/2 Extended Edition"  },
                      {  2, "OS/2 Extended Services" }} ;
/* Tabelle mit SYSID Informationen zu den SYSLEVEL Dateien */
SYSID   sysid [] = {{ "ZZZZ", ""                                             },
                    { "1000", "OS/2 SE"                                      },
                    { "1010", "OS/2 Toolkit"                                 },
                    { "2000", "OS/2 Communications Manager"                  },
                    { "2010", "OS/2 Feature Group 1 (3270 Emulator)"         },
                    { "2020", "OS/2 Feature Group 2 (Async)"                 },
                    { "4000", "OS/2 Database Manager"                        },
                    { "4010", "OS/2 Query Manager"                           },
                    { "4AFB", "OS/2 Shareware Collection by Harald Wilhelm"  },
                    { "5000", "OS/2 LAN Requester"                           },
                    { "5010", "OS/2 LAN Server"                              },
                    { "C010", "LAN Manager Utilities/2"                      },
                    { "C020", "IBM X25 Net Manager"                          },
                    { "C040", "Communications Subsystem for OS/2 EE"         },
                    { "C060", "IBM Snap Dump"                                },
                    { "C070", "ISDN Coprocessor Support Program Version 1.2" },
                    { "C080", "C Set/2"                                      },
                    { "C081", "Workframe/2"                                  }} ;

/* Client Window Message Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Messages sind von Interesse? */
   switch (msg)
      {
      /* Schliessen wird angezeigt */
      case WM_CLOSE:
           /* Schliessen veranlassen */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Benutzaktion wird angezeigt */
      case WM_COMMAND:
           {
           HWND    hwndTemp ;
           PUSRREC pusrrec ;
           PWND    pwnd ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Was will benutzer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* View Menueeintrag selektiert */
              case IDM_SVIEW:
                   /* Den selektierten Container Eintrag ermitteln */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMP ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Einen Dialog modal starten */
                        hwndTemp = WinLoadDlg (HWND_DESKTOP,
                                               hwndFrame_g,
                                               SWndProc,
                                               NULLHANDLE,
                                               IDD_S,
                                               NULL) ;
                        /* Daten hinterherschicken */
                        WinSendMsg (hwndTemp,
                                    WMP_MAININIT,
                                    MPFROMP (&pusrrec->sysfile),
                                    MPVOID) ;
                        /* Aus dem modalen wird ein non-modaler Dialog */
                        WinProcessDlg (hwndTemp) ;
                        /* Dialog loeschen, wird bei dieser Art der Verarbeitung (WinProcessDlg) benoetigt */
                        WinDestroyWindow (hwndTemp) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Kontrollelement zeigt etwas an */
      case WM_CONTROL:
           /* Weclhes Kontrollelement? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Container */
              case IDCO_SYSLEVELS:
                   /* Uns was zeigt der Container an? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Benutzer hat ENTER (Doppelklick) gedrueckt */
                      case CN_ENTER:
                           /* Der Einfachheit halber einen COMMAND Event ausloesen */
                           /* Das trudelt dann als WM_COMMAND ein und erspart uns das zweifache Programmieren */
                           WinPostMsg (hwnd,
                                       WM_COMMAND,
                                       MPFROMSHORT (IDM_SVIEW),
                                       MPFROM2SHORT (CMDSRC_PUSHBUTTON, TRUE)) ;
                           break ;
                      }
                   break ;
              }
           break ;
      /* Der Pointer ist ueber einem Kontrollelement */
      case WM_CONTROLPOINTER:
           /* Is der WAIT Status an? */
           if   (ulWait_g)
                {
                /* Ja, also WAIT Pointer setzen */
                return (MRESULT) WinQuerySysPointer (HWND_DESKTOP,
                                                     SPTR_WAIT,
                                                     FALSE) ;
                }
           break ;
      /* Das Client Window wird erzeugt */
      case WM_CREATE:
           /* Einen Speicherbereich fuer die Instanzdaten allokieren und in den WindowWords speichern */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Verzoegerter Start */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Client Window wird geloescht */
      case WM_DESTROY:
           /* Speicherbereich mit den Instanzdaten wieder freigeben */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      /* Anfrage des PM wer den Client Bereich zeichnet */
      case WM_ERASEBACKGROUND:
           /* PM soll selbst zeichnen */
           return (MRESULT) TRUE ;
      /* PM zeigt an, dass Fenster minimiert oder maximiert wurde */
      case WM_MINMAXFRAME:
           /* Status merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* PM zeigt an, dass Fenster in der Groesse veraendert wurde */
      case WM_SIZE:
           /* Verzoegert abarbeiten */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       mp1,
                       mp2) ;
           break ;
      /* Verzoegerter Start */
      case WMP_MAININIT:
           {
           CHAR                 sz [128] ;
           CNRINFO              ccinfo ;
           static CONTAINERINIT acnrinit [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_SYSLEVEL,   offsetof (USRREC, minirec.pszIcon)  },
                                               { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_MODUL,      offsetof (USRREC, pszModul)         },
                                               { "", CFA_DATE | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,   CFA_CENTER, IDS_DATUM,      offsetof (USRREC, cdate)            },
                                               { "", CFA_TIME | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,   CFA_CENTER, IDS_UHRZEIT,    offsetof (USRREC, ctime)            },
                                               { "", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR, CFA_CENTER, IDS_GROESSE,    offsetof (USRREC, ulGroesse)        }} ;
           PWND                 pwnd ;
           SWP                  swp ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Einge Parameter fuer den Thread fuellen */
           /* Groesse der Struktur (Thunking!) */
           pwnd->thread.usLength = sizeof (THREAD) ;
           /* Wer soll zum Abschluss informiert werden */
           pwnd->thread.hwnd = hwnd ;
           /* Adresse an der die SYSLEVEL Kette aufgebaut werden soll */
           pwnd->thread.ppsysfile = &pwnd->psysfile ;
           /* Adresse mit dem Zaehler der Syslevel Elemente */
           pwnd->thread.pulCount = &pwnd->ulCount ;
           /* Titel der Anwendung lesen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_PROGRAMM,
                          sizeof (sz),
                          sz) ;
           /* Titel setzen */
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz) ;
           /* Groesse der Client Area ermitteln */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* Container im Client Area erzeugen */
           pwnd->hwndContainer = WinCreateWindow (hwndClient_g,
                                                  WC_CONTAINER,
                                                  "",
                                                  CCS_AUTOPOSITION | CCS_MINIICONS | CCS_MINIRECORDCORE | CCS_READONLY | CCS_SINGLESEL | WS_GROUP,
                                                  0,
                                                  0,
                                                  swp.cx,
                                                  swp.cy,
                                                  hwndFrame_g,
                                                  HWND_TOP,
                                                  IDCO_SYSLEVELS,
                                                  NULL,
                                                  NULL) ;
           /* Font fuer den Container setzen */
           WinSetPresParam (pwnd->hwndContainer,
                            PP_FONTNAMESIZE,
                            sizeof ("8.Helv"),
                            "8.Helv") ;
           /* Container Ueberschrift lesen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_TITEL,
                          sizeof (pwnd->szTitel),
                          pwnd->szTitel) ;
           /* Container Spalten definieren */
           TInitContainer (pwnd->hwndContainer,
                           NULLHANDLE,
                           &acnrinit [0],
                           sizeof (acnrinit) / sizeof (acnrinit [0])) ;
           /* Container Informationsstruktur leeren */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Einige Attribute fuer den Container definieren */
           ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
           /* Containertitel */
           ccinfo.pszCnrTitle = pwnd->szTitel ;
           /* Containereinstellungen vornehmen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           /* Lesen der SYSLEVEL Informationen */
           WinPostMsg (hwnd,
                       WMP_REREAD,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Hier wird das Lesen der SYSLEVEL Informationen angestossen */
      case WMP_REREAD:
           {
           CNRINFO ccinfo ;
           PWND    pwnd ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Eine "Bitte warten" Ueberschrift fuer den Container ausgeben */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_WAIT,
                          sizeof (pwnd->szTitel),
                          pwnd->szTitel) ;
           /* Container Informationsstruktur leeren */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Containertitel */
           ccinfo.pszCnrTitle = pwnd->szTitel ;
           /* Containereinstellungen vornehmen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE)) ;
           /* Alle alten Records aus Container rauswerfen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_REMOVERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
           /* WAIT Status einschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           /* Suchthread starten */
           _beginthread (Thread,
                         NULL,
                         80000,
                         &pwnd->thread) ;
           return (MRESULT) FALSE ;
           }
      /* Verzoegerte Groessenaenderung */
      case WMP_SIZE:
           {
           PWND pwnd ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Container an neue Client area anpassen */
                WinSetWindowPos (pwnd->hwndContainer,
                                 HWND_TOP,
                                 0,
                                 0,
                                 SHORT1FROMMP (mp2),
                                 SHORT2FROMMP (mp2),
                                 SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE) ;
                }
           return (MRESULT) FALSE ;
           }
      /* THREAD ist fertig */
      case WMP_THREADFINISHED:
           {
           CNRINFO      ccinfo ;
           PSYSFILE     psysfile ;
           PUSRREC      pusrrec ;
           PWND         pwnd ;
           RECORDINSERT recins ;
           ULONG        ulI ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Container Ueberschrift lesen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_TITEL,
                          sizeof (pwnd->szTitel),
                          pwnd->szTitel) ;
           /* Container Informationsstruktur leeren */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Containertitel */
           ccinfo.pszCnrTitle = pwnd->szTitel ;
           /* Containereinstellungen vornehmen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE)) ;
           /* Wurden ueberhaupt SYSLEVEL Dateien gefunden? */
           if   (pwnd->psysfile)
                {
                /* Ja, in einer Schleife alle SYSLEVEL Dateien abarbeiten */
                for  (psysfile = pwnd->psysfile, ulI = 0; ulI < pwnd->ulCount; ulI++, psysfile++)
                     {
                     /* Einen Container Record allokieren */
                     pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                        CM_ALLOCRECORD,
                                                        MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                        MPFROMSHORT (1))) ;
                     /* Daten in Record Struktur speichern */
                     pusrrec->sysfile = *psysfile ;
                     /* Syslevel Name */
                     pusrrec->minirec.pszIcon = pusrrec->sysfile.systable.szSysName ;
                     /* Modulname */
                     pusrrec->pszModul = pusrrec->sysfile.szLocation ;
                     /* Moduldatum */
                     pusrrec->cdate.day = pusrrec->sysfile.fdate.day ;
                     pusrrec->cdate.month = pusrrec->sysfile.fdate.month ;
                     pusrrec->cdate.year = pusrrec->sysfile.fdate.year + 1980 ;
                     /* Moduluhrzeit */
                     pusrrec->ctime.hours = pusrrec->sysfile.ftime.hours ;
                     pusrrec->ctime.minutes = pusrrec->sysfile.ftime.minutes ;
                     pusrrec->ctime.seconds = pusrrec->sysfile.ftime.twosecs * 2 ;
                     /* Modulgroesse */
                     pusrrec->ulGroesse = pusrrec->sysfile.ulSize ;
                     /* RECORDINSERT Struktur leeren */
                     memset (&recins,
                             0,
                             sizeof (RECORDINSERT)) ;
                     recins.cb = sizeof (RECORDINSERT) ;
                     /* Record hinten anhaengen */
                     recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                     /* Nur eine Rootkette */
                     recins.zOrder = CMA_TOP ;
                     /* Einen Record einfuegen */
                     recins.cRecordsInsert = 1 ;
                     /* Rein mit dem Record */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INSERTRECORD,
                                 MPFROMP (pusrrec),
                                 MPFROMP (&recins)) ;
                     }
                /* SYSLEVEL Informationen freigeben */
                free (pwnd->psysfile) ;
                /* Pointer zuruecksetzen */
                pwnd->psysfile = NULL ;
                /* Zaehler auch zuruecksetzen */
                pwnd->ulCount = 0 ;
                }
           /* Container invalidieren */
           WinSendMsg (pwnd->hwndContainer,
                       CM_INVALIDATERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
           /* WAIT Processing abschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (FALSE),
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* WAIT Verarbeitung */
      case WMP_WAIT:
           /* Nur wenn WAIT angeschaltet wird oder Status gefuellt */
           if   (LONGFROMMP (mp1) || ulWait_g)
                {
                /* Status Rauf- oder Runterzaehlen */
                (LONGFROMMP (mp1)) ? ulWait_g++ : ulWait_g-- ;
                /* Nur wenn STATUS ein Veraendern der Eieruhr bewirkt */
                if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                      (!LONGFROMMP (mp1) && !ulWait_g))
                     {
                     /* Neuen Pointer setzen */
                     WinSetPointer (HWND_DESKTOP,
                                    WinQuerySysPointer (HWND_DESKTOP,
                                                        (LONGFROMMP (mp1)) ? SPTR_WAIT : SPTR_ARROW,
                                                        FALSE)) ;
                     /* Frame sperren */
                     WinEnableWindow (WinQueryWindow (hwnd,
                                                      QW_PARENT),
                                      !LONGFROMMP (mp1)) ;
                     }
                }
           return (MRESULT) FALSE ;
      }
   /* Aufruf der Standarverarbeitung */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Programmstart */
INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_AUTOICON | FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Beim PM anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Eine MessageQueue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Anwendungsklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszSyslvl_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster oeffnen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszSyslvl_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_SYSLVL,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Messageloop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Messages verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Standardfenster wieder loeschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue wieder loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab_g) ;
        }
   /* Applikationsende */
   return 0 ;
   }

/* Rekursive Funktion zum Durchsuchen von Laufwerken und Verzeichnissen nach SYSLEVEL Dateien */
BOOL ScanDirectory (PSZ       pszDirectory,
                    PSYSFILE* ppsysfile,
                    PULONG    pulFiles)
   {
   APIRET       rc ;
   CHAR         szBuffer [CCHMAXPATH] ;
   CHAR         szBuffer2 [CCHMAXPATH] ;
   FILE*        file ;
   FILEFINDBUF3 findbuf3 ;
   HDIR         hdir ;
   PBYTE        pb ;
   PBYTE        pbSave ;
   PSYSFILE     psysfile ;
   ULONG        ulSearchCount ;

   /* Startverzeichnis der Suche uebernehmen */
   strcpy (szBuffer,
           pszDirectory) ;
   /* Wildcard hinten anhaengen */
   strcat (szBuffer,
           "\\*.*") ;
   /* Immer einen neuen Handle erzeugen lassen */
   hdir = HDIR_CREATE ;
   /* Immer eine Datei nach der anderen */
   ulSearchCount = 1 ;
   /* Suche starten */
   rc = DosFindFirst (szBuffer,
                      &hdir,
                      FILE_ARCHIVED | FILE_DIRECTORY | FILE_NORMAL | FILE_READONLY,
                      &findbuf3,
                      sizeof (findbuf3),
                      &ulSearchCount,
                      FIL_STANDARD) ;
   /* Fehlerfrei? */
   if   (rc == NO_ERROR)
        {
        /* Ja, solange etwas gefunden wurde und fehlerfrei, weitersuchen */
        while ((ulSearchCount == 1) &&
               (rc == NO_ERROR))
           {
           /* Startet der Dateiname mit SYSLEVEL? */
           if   (!strnicmp (findbuf3.achName,
                            "SYSLEVEL.",
                            strlen ("SYSLEVEL.")))
                {
                /* Ja, voll qualifizierten Dateinamen generieren */
                strcpy (szBuffer2,
                        pszDirectory) ;
                strcat (szBuffer2,
                        "\\") ;
                strcat (szBuffer2,
                        findbuf3.achName) ;
                /* Diese Datei oeffnen */
                if   ((file = fopen (szBuffer2,
                                     "rb")) != NULL)
                     {
                     /* Platz fuer den Dateiinhalt allokieren */
                     if   ((pb = calloc (1,
                                         findbuf3.cbFile)) != NULL)
                          {
                          /* Pointer sichern */
                          pbSave = pb ;
                          /* Datei einlesen */
                          fread (pb,
                                 findbuf3.cbFile,
                                 1,
                                 file) ;
                          /* Geklappt? */
                          if   (!ferror (file))
                               {
                               /* Ja, SYSLEVEL Datei ueberpruefen */
                               /* Ist Signatur vorhanden? */
                               if   ((((PSYSHEADER) pb)->usSignature == (USHORT) 0xFFFF) &&
                                     (!strncmp (((PSYSHEADER) pb)->achSignature,
                                                SYSLEVEL,
                                                strlen (SYSLEVEL))))
                                    {
                                    /* Ja, ist Pointer fuer SYSLEVEL Kette leer? */
                                    if   (*ppsysfile == NULL)
                                         {
                                         /* Ja, dann Platz fuer den ersten Eintrag schaffen */
                                         *ppsysfile = calloc (1,
                                                              sizeof (SYSFILE)) ;
                                         }
                                    else
                                         {
                                         /* Nein, weitere Eintraege hinten anhaengen */
                                         *ppsysfile = realloc (*ppsysfile,
                                                               (*pulFiles + 1) * sizeof (SYSFILE)) ;
                                         }
                                    /* An das Ende springen */
                                    psysfile = *ppsysfile ;
                                    psysfile += *pulFiles ;
                                    (*pulFiles)++ ;
                                    /* Lokation der Datei speichern */
                                    strcpy (psysfile->szLocation,
                                            szBuffer2) ;
                                    /* Einige Daten zur Datei ebenfalls merken */
                                    psysfile->fdate = findbuf3.fdateLastWrite ;
                                    psysfile->ftime = findbuf3.ftimeLastWrite ;
                                    psysfile->ulSize = findbuf3.cbFile ;
                                    /* Dateiinhalte ebenfalls reinkopieren */
                                    psysfile->sysheader = *((PSYSHEADER) pb) ;
                                    pb += ((PSYSHEADER) pb)->ulTableOffset ;
                                    psysfile->systable = *((PSYSTABLE) pb) ;
                                    }
                               }
                          /* Speicherbereich wieder freigeben */
                          free (pbSave) ;
                          }
                     else
                          {
                          /* Fehler zurueckliefern */
                          return FALSE ;
                          }
                     /* Datei auf jeden Fall schliessen */
                     fclose (file) ;
                     }
                }
           /* War Eintrag ein Verzeichnis */
           if   (findbuf3.attrFile & FILE_DIRECTORY)
                {
                /* Nur wenn nicht ROOT Eintraege */
                if   (!(((findbuf3.achName [0] == '.') &&
                        (findbuf3.cchName == 1)) ||
                       ((findbuf3.achName [0] == '.') &&
                        (findbuf3.achName [1] == '.') &&
                        (findbuf3.cchName == 2))))
                     {
                     /* Voll qualifizierten Verzeichniseintrag generieren */
                     strcpy (szBuffer,
                             pszDirectory) ;
                     strcat (szBuffer,
                             "\\") ;
                     strcat (szBuffer,
                             findbuf3.achName) ;
                     /* Diese Funktion rekursiv aufrufen */
                     if   (!ScanDirectory (szBuffer,
                                           ppsysfile,
                                           pulFiles))
                          {
                          /* Zurueckgelieferte Fehler zurueckliefern */
                          return FALSE ;
                          }
                     }
                }
           /* Weiter suchen */
           rc = DosFindNext (hdir,
                             &findbuf3,
                             sizeof (findbuf3),
                             &ulSearchCount) ;
           }
        /* Suchkette schliessen */
        DosFindClose (hdir) ;
        }
   /* Kein Fehler */
   return TRUE ;
   }

/* Dialogprozedur */
MRESULT EXPENTRY SWndProc (HWND   hwndDlg,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   /* Welche Message soll verarbeitet werden? */
   switch (msg)
      {
      /* Dialogstart */
      case WM_INITDLG:
           /* Eingabefeldgroessen mancher Felder erhoehen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SMODUL,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SSYSTEM,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (80),
                              MPVOID) ;
           return (MRESULT) FALSE ;
      /* Die Daten vom Client kommen */
      case WMP_MAININIT:
           {
           BYTE     b ;
           CHAR     szBuffer [CCHMAXPATH] ;
           PFDATE   pfdate ;
           PRELEASE prelease ;
           PSYSFILE psysfile ;
           ULONG    ulI ;

           /* Das sind die Daten */
           psysfile = PVOIDFROMMP (mp1) ;
           /* In einer Schleife die EDITION Daten in die entsprechende Combobox fuellen */
           for  (ulI = 0; ulI < (sizeof (edition) / sizeof (edition [0])); ulI++)
                {
                WinSendDlgItemMsg (hwndDlg,
                                   IDC_SSYSEDITION,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (edition[ulI].psz)) ;
                }
           /* In einer Schleife die COUNTRY Daten in die entsprechende Combobox fuellen */
           for  (ulI = 0; ulI < (sizeof (country) / sizeof (country [0])); ulI++)
                {
                WinSendDlgItemMsg (hwndDlg,
                                   IDC_SCOUNTRY,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (country[ulI].psz)) ;
                }
           /* Dialogtitel lesen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_VIEW,
                          sizeof (szBuffer),
                          szBuffer) ;
           /* Dialogtitel setzen */
           WinSetWindowText (hwndDlg,
                             szBuffer) ;
           /* Modulname ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SMODUL,
                              psysfile->szLocation) ;
           /* Datum/Uhrzeit der SYSLEVEL mit Hilfe einer eigenen Packstruktur bearbeiten */
           pfdate = (PFDATE) &psysfile->systable.usSysDate ;
           /* Jahr bearbeiten */
           ulI = pfdate->year ;
           /* Wenn Jahr groesser 0 */
           if   (ulI)
                {
                /* Dann DOS Datumsstart addieren */
                ulI += 1980 ;
                }
           /* Datum zusammenbauen */
           sprintf (szBuffer,
                    "%04u%02u%02u",
                    ulI,
                    (ULONG) pfdate->month,
                    (ULONG) pfdate->day) ;
           /* Datum ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SDATE,
                              szBuffer) ;
           /* SYSID zusammenbauen */
           sprintf (szBuffer,
                    "%04X",
                    (ULONG) psysfile->systable.usSysId) ;
           /* SYSID ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SSYSID,
                              szBuffer) ;
           /* In einer Schleife die SYSID suchen */
           for  (ulI = 0; ulI < (sizeof (sysid) / sizeof (sysid [0])); ulI++)
                {
                /* Wenn gefunden */
                if   (!strcmp (szBuffer,
                               sysid[ulI].pszSysId))
                     {
                     /* Text hierzu ausgeben */
                     WinSetDlgItemText (hwndDlg,
                                        IDT_SSYSID,
                                        sysid[ulI].pszPart) ;
                     break ;
                     }
                }
           /* Systembezeichnung ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SSYSTEM,
                              psysfile->systable.szSysName) ;
           /* Platz fuer Komponent ID leeren */
           memset (szBuffer,
                   0,
                   sizeof (szBuffer)) ;
           /* Komponent ID ist nicht NULL terminiert */
           memcpy (szBuffer,
                   psysfile->systable.szCompId,
                   9) ;
           /* Komponent ID ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SCOMPONENT,
                              szBuffer) ;
           /* Type ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_STYPE,
                              psysfile->systable.szType) ;
           /* In einer Schleife die EDITION suchen */
           for  (ulI = 0; ulI < (sizeof (edition) / sizeof (edition [0])); ulI++)
                {
                /* Wenn gefunden */
                if   (edition[ulI].ul == (ULONG) psysfile->systable.bSysEdition)
                     {
                     /* Comboboxeintrag selektieren */
                     WinSendDlgItemMsg (hwndDlg,
                                        IDC_SSYSEDITION,
                                        LM_SELECTITEM,
                                        MPFROMSHORT (ulI),
                                        MPFROMSHORT (TRUE)) ;
                     break ;
                     }
                }
           /* Release der SYSLEVEL mit Hilfe einer eigenen Packstruktur bearbeiten */
           prelease = (PRELEASE) &psysfile->systable.bSysVersion ;
           /* Major */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SMAJOR,
                               prelease->major,
                               FALSE) ;
           /* Minor */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SMINOR,
                               prelease->minor,
                               FALSE) ;
           /* Modify */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SMODIFY,
                               psysfile->systable.bSysModify,
                               FALSE) ;
           /* Refresh */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SREFRESH,
                               psysfile->systable.bRefreshLevel,
                               FALSE) ;
           /* CSD bearbeiten */
           strncpy (szBuffer,
                    psysfile->systable.achCsdLevel,
                    8) ;
           /* Ist nur sieben Zeichen lang */
           szBuffer [7] = '\0' ;
           /* Countryinformation ermitteln */
           b = szBuffer [2] ;
           szBuffer [2] = '\0' ;
           /* CSD Prefix ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_SCSDPREFIX,
                              szBuffer) ;
           /* Countryinformation wieder zuruecksetzen */
           szBuffer [2] = b ;
           /* Country des SYSLEVEL CSDs mit Hilfe einer eigenen Packstruktur bearbeiten */
           for  (ulI = 0; ulI < (sizeof (country) / sizeof (country [0])); ulI++)
                {
                /* Wenn gefunden */
                if   (country[ulI].ch == szBuffer [2])
                     {
                     /* Comboboxeintrag selektieren */
                     WinSendDlgItemMsg (hwndDlg,
                                        IDC_SCOUNTRY,
                                        LM_SELECTITEM,
                                        MPFROMSHORT (ulI),
                                        MPFROMSHORT (TRUE)) ;
                     break ;
                     }
                }
           /* Aktuellen CSD Level ausgeben */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SCSDCURRENT,
                               atol (&szBuffer [3]),
                               FALSE) ;
           strncpy (szBuffer,
                    psysfile->systable.achCsdPrev,
                    8) ;
           szBuffer [7] = '\0' ;
           /* Vorherigen CSD Level ausgeben */
           WinSetDlgItemShort (hwndDlg,
                               IDE_SCSDPREVIOUS,
                               atol (&szBuffer [3]),
                               FALSE) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standarverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Thread zum Suchen der SYSLEVEL Dateien */
VOID Thread (VOID* pv)
   {
   CHAR       szBuffer [4] ;
   FSALLOCATE fsalloc ;
   HAB        hab ;
   HMQ        hmq ;
   PTHREAD    pthread ;
   ULONG      ulI ;

   /* Threaddaten holen */
   pthread = pv ;
   /* Diesen Thread beim PM anmelden */
   hab = WinInitialize (0) ;
   /* Eine Message Queue fuer diesen Thread anfordern */
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   /* Wenn Shutdown dann nimmt dieser Thread daran teil */
   WinCancelShutdown (hmq,
                      TRUE) ;
   /* HARDERROR abschalten */
   DosError (FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION) ;
   /* Von C: bis Z: alle Laufwerke testen */
   for  (ulI = 3; ulI <= 26; ulI++)
        {
        /* Liegen Informationen zum Laufwerk vor? */
        if   (!DosQueryFSInfo (ulI,
                               FSIL_ALLOC,
                               &fsalloc,
                               sizeof (fsalloc)))
             {
             /* Ja, dann dieses Laufwerk auch nach SYSLEVEL Dateien durchsuchen */
             szBuffer [0] = ulI + '@' ;
             szBuffer [1] = ':' ;
             szBuffer [2] = '\0' ;
             /* Rekursive Funktion zum Durchsuchen von Laufwerken und Verzeichnissen nach SYSLEVEL Dateien */
             ScanDirectory (szBuffer,
                            pthread->ppsysfile,
                            pthread->pulCount) ;
             }
        }
   /* HARDERROR wieder einschalten */
   DosError (FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION) ;
   /* Client ueber das Ende des THREADS informieren */
   WinSendMsg (pthread->hwnd,
               WMP_THREADFINISHED,
               MPVOID,
               MPVOID) ;
   /* Message Queue dieses Threads loeschen */
   WinDestroyMsgQueue (hmq) ;
   /* Diesen Thread beim PM wieder abmelden */
   WinTerminate (hab) ;
   /* THREAD Ende */
   _endthread () ;
   }

/* T-Helper Funktion zum Initialisieren von Containern */
BOOL TInitContainer (HWND           hwnd,     /* Container Handle */
                     HMODULE        hmod,     /* Ressource Handle */
                     PCONTAINERINIT pcnrinit, /* Initialisierungsstruktur */
                     ULONG          ul)       /* ANzahl Elemente in Initialisiewrungsstruktur */
   {
   FIELDINFOINSERT fiins ;
   HAB             hab ;
   PFIELDINFO      pfldinfo ;
   PFIELDINFO      pfldinfoFirst ;
   ULONG           ulI ;

   /* Anchor Block besorgen */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Anzahl Spalten anmelden */
   pfldinfo = PVOIDFROMMR (WinSendMsg (hwnd,
                                       CM_ALLOCDETAILFIELDINFO,
                                       MPFROMLONG (ul),
                                       MPVOID)) ;
   /* Pointer auf erste Spalte merken */
   pfldinfoFirst = pfldinfo ;
   /* In einer Schleife alle Spalten abarbeiten */
   for  (ulI = 0; ulI < ul; ulI++, pcnrinit++)
        {
        /* Datenflags */
        pfldinfo->flData = pcnrinit->ulFlData ;
        /* Titelflags */
        pfldinfo->flTitle = pcnrinit->ulFlTitle ;
        /* Spaltenueberschrift aus Ressource lesen */
        WinLoadString (hab,
                       hmod,
                       pcnrinit->ulIdTitle,
                       sizeof (pcnrinit->szTitle),
                       pcnrinit->szTitle) ;
        pfldinfo->pTitleData = pcnrinit->szTitle ;
        /* Offset zu den Daten */
        pfldinfo->offStruct = pcnrinit->ulOffStruct ;
        /* Naechste Spalte */
        pfldinfo = pfldinfo->pNextFieldInfo ;
        }
   /* Spalten einfuegen */
   memset (&fiins,
           0,
           sizeof (FIELDINFOINSERT)) ;
   fiins.cb = sizeof (FIELDINFOINSERT) ;
   fiins.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST ;
   fiins.cFieldInfoInsert = ul ;
   fiins.fInvalidateFieldInfo = TRUE ;
   /* rein mit den Spalten */
   WinSendMsg (hwnd,
               CM_INSERTDETAILFIELDINFO,
               MPFROMP (pfldinfoFirst),
               MPFROMP (&fiins)) ;
   return TNOERROR ;
   }


