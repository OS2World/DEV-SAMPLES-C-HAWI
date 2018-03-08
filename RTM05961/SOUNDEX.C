/* $Header: D:/Projekte/Kleine Helferlein!3/Source/rcs/SOUNDEX.C 1.2 1996/04/27 12:54:57 HaWi Exp $ */

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
#include "SOUNDEX.H"

HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszSoundex_g = "Soundex" ;

/* Client Window Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   switch (msg)
      {
      /* Benutzeranweisung */
      case WM_COMMAND:
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
              /* Drives dialog */
              case IDM_DIALOGSOUNDEX:
                   /* Dialog starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              SoundexWndProc,
                              NULLHANDLE,
                              IDD_SOUNDEX,
                              NULL) ;
                   break ;
              }
           break ;
      /* Main window anlegen */
      case WM_CREATE:
           /* Kleine Startverzoegerung */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Hintergrund zeichnen lassen */
      case WM_ERASEBACKGROUND:
           return (MRESULT) TRUE ;
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

/* Prozedur des Drives Dialoges */
MRESULT EXPENTRY SoundexWndProc (HWND   hwndDlg,
                                 MSG    msg,
                                 MPARAM mp1,
                                 MPARAM mp2)
   {
   switch (msg)
      {
      /* Benutzeranforderung */
      case WM_COMMAND:
           {
           BOOL  f ;
           CHAR  sz [CCHMAXPATH] ;
           CHAR  sz2 [CCHMAXPATH] ;
           CHAR  sz3 [128] ;
           ULONG ul ;
           ULONG ulCount ;
           ULONG ulI ;

           /* Was wird gewuenscht? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel vom Systemmenu */
              case DID_CANCEL:
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ueberpruefen wurde gedrueckt */
              case IDP_SX1:
                   /* Eingabetext holen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SX1,
                                        sizeof (sz),
                                        sz) ;
                   /* Anzahl der Eintraege in der Listbox ermitteln */
                   ul = LONGFROMMR (WinSendDlgItemMsg (hwndDlg,
                                                       IDL_SX1,
                                                       LM_QUERYITEMCOUNT,
                                                       MPVOID,
                                                       MPVOID)) ;
                   /* Zaehler initialisieren */
                   ulCount = 0 ;
                   /* In einer Schleife durch alle Listbox Eintraege */
                   for  (ulI = 0; ulI < ul; ulI++)
                        {
                        /* Listbox Eintrag lesen */
                        WinSendDlgItemMsg (hwndDlg,
                                           IDL_SX1,
                                           LM_QUERYITEMTEXT,
                                           MPFROM2SHORT (ulI, sizeof (sz2)),
                                           MPFROMP (sz2)) ;
                        /* Mit Eingabetext phonetisch ueberpruefen */
                        if   (TSoundex (sz,
                                        sz2) == TNOERROR)
                             {
                             /* Treffer */
                             f = TRUE ;
                             /* Zaehler erhoehen */
                             ulCount++ ;
                             }
                        else
                             {
                             /* Kein Treffer */
                             f = FALSE ;
                             }
                        /* Treffer werden in Listbox markiert */
                        WinSendDlgItemMsg (hwndDlg,
                                           IDL_SX1,
                                           LM_SELECTITEM,
                                           MPFROMSHORT (ulI),
                                           MPFROMSHORT (f)) ;
                        /* Treffer Text aus Ressourcen lesen */
                        WinLoadString (hab_g,
                                       NULLHANDLE,
                                       IDS_BTREFFER,
                                       sizeof (sz2),
                                       sz2) ;
                        /* Ausgabetext zusammenbauen */
                        sprintf (sz3,
                                 "%u %s",
                                 ulCount,
                                 sz2) ;
                        /* Ausgabe der Anzahl Treffer */
                        WinSetDlgItemText (hwndDlg,
                                           IDT_SX1,
                                           sz3) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Kontrollelement will etwas */
      case WM_CONTROL:
           {
           BOOL  f ;
           CHAR  sz [CCHMAXPATH] ;
           ULONG aulEntryfields [] = { IDE_SX1 } ;
           ULONG ulI ;

           /* Welches Kontrollelement? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Das Eingabefeld */
              case IDE_SX1:
                   /* Was will es? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Inhalt hat sich veraendert */
                      case EN_CHANGE:
                           f = FALSE ;
                           /* In einer Schleife durch alle Eingabefelder */
                           for  (ulI = 0; ulI < sizeof (aulEntryfields) / sizeof (aulEntryfields [0]); ulI++)
                                {
                                /* Text auslesen */
                                WinQueryDlgItemText (hwndDlg,
                                                     aulEntryfields [ulI],
                                                     sizeof (sz),
                                                     sz) ;
                                /* Ist im Eingabefeld etwas drin? */
                                if   (*sz)
                                     {
                                     /* Ja */
                                     f = TRUE ;
                                     break ;
                                     }
                                }
                           /* Pushbutton enablen/disablen */
                           WinEnableControl (hwndDlg,
                                             IDP_SX1,
                                             f) ;
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Dialog Start */
      case WM_INITDLG:
           {
           BOOL  f ;
           CHAR  sz [CCHMAXPATH] ;
           ULONG aulEntryfields [] = { IDE_SX1 } ;
           ULONG ulI ;

           /* Textlaenge fuer Eingabefeld festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SX1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH),
                              MPVOID) ;
           /* Texte fuer Listbox in einem Rutsch aus Ressource lesen */
           TFillControls (WinWindowFromID (hwndDlg,
                                           IDL_SX1),
                          NULLHANDLE,
                          IDS_INITLISTBOXLOW,
                          IDS_INITLISTBOXHIGH) ;
           f = FALSE ;
                           /* In einer Schleife durch alle Eingabefelder */
           for  (ulI = 0; ulI < sizeof (aulEntryfields) / sizeof (aulEntryfields [0]); ulI++)
                {
                /* Text auslesen */
                WinQueryDlgItemText (hwndDlg,
                                     aulEntryfields [ulI],
                                     sizeof (sz),
                                     sz) ;
                /* Ist im Eingabefeld etwas drin? */
                if   (*sz)
                     {
                     /* Ja */
                     f = TRUE ;
                     break ;
                     }
                }
           /* Pushbutton enablen/disablen */
           WinEnableControl (hwndDlg,
                             IDP_SX1,
                             f) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Default Dialog Prozedur */
   return WinDefDlgProc (hwndDlg,
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
                                     pszSoundex_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Hauptfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE,
                                                           &ulCreateFlags,
                                                           pszSoundex_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_SOUNDEX,
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

/* Hilfsfunktion zum lesen von mehreren Ressourcetexten */
BOOL TFillControls (HWND  hwnd,
                    ULONG hmod,
                    ULONG ulFrom,
                    ULONG ulTo)
   {
   CHAR  szBuffer [512] ;
   HAB   hab ;
   ULONG ulI ;

   /* Anchor Block besorgen */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* In einer Schleife ueber angegebenen Ressource ID Bereich */
   for  (ulI = ulFrom; ulI <= ulTo; ulI++)
        {
        /* Text aus Ressource lesen */
        WinLoadString (hab,
                       hmod,
                       ulI,
                       sizeof (szBuffer),
                       szBuffer) ;
        /* und in Listbox einfuegen */
        WinSendMsg (hwnd,
                    LM_INSERTITEM,
                    MPFROMSHORT (LIT_END),
                    MPFROMP (szBuffer)) ;
        }
   /* Kein Fehler */
   return TNOERROR ;
   }

/* Hilfsfunktion zum Ersetzen bestimmter Zeichen durch einen Ersatztext */
BOOL TReplaceCharacter (PSZ pszTarget,
                        PSZ pszSource)
   {
   BOOL      f ;
   CHAR      sz [230] ;
   CHAR      sz2 [2] ;
   TREPLACE  treplace [] = {{ '„', "ae"     }, /* Tabelle der zu ersetzenden Zeichen */
                            { '”', "oe"     },
                            { '', "ue"     },
                            { 'Ž', "Ae"     },
                            { '™', "Oe"     },
                            { 'š', "Ue"     },
                            { 'á', "ss"     },
                            { '1', "Eins"   },
                            { '2', "Zwei"   },
                            { '3', "Drei"   },
                            { '4', "Vier"   },
                            { '5', "Fuenf"  },
                            { '6', "Sechs"  },
                            { '7', "Sieben" },
                            { '8', "Acht"   },
                            { '9', "Neun"   },
                            { '0', "Null"   },
                            { '…', "a"      },
                            { 'ƒ', "a"      },
                            { ' ', "a"      },
                            { 'Š', "e"      },
                            { 'ˆ', "e"      },
                            { '‚', "e"      },
                            { '•', "o"      },
                            { '“', "o"      },
                            { '¢', "o"      },
                            { '—', "u"      },
                            { '–', "u"      },
                            { '£', "u"      },
                            { '·', "A"      },
                            { 'µ', "A"      },
                            { '', "E"      },
                            { 'Ô', "E"      },
                            { 'ã', "O"      },
                            { 'à', "O"      },
                            { 'ë', "U"      },
                            { 'é', "U"      }} ;
   PSZ       psz ;
   ULONG     ulI ;

   /* Text umkopieren */
   strcpy (sz,
           pszSource) ;
   /* Pointer auf String */
   psz = sz ;
   /* String mit einem Character */
   sz2 [1] = '\0' ;
   /* Ergebnisstring initialisieren */
   *pszTarget = '\0' ;
   /* In einer Schleife bis Textende */
   while (*psz)
      {
      f = FALSE ;
      /* In einer Schleife durch Tabelle */
      for  (ulI = 0; ulI < (sizeof (treplace) / sizeof (treplace [0])); ulI++)
           {
           /* Ist Zeichen enthalten? */
           if   (*psz == treplace[ulI].chSearch)
                {
                /* Ja, dann Austauschtext verwenden */
                f = TRUE ;
                strcat (pszTarget,
                        treplace[ulI].pszReplace) ;
                break ;
                }
           }
      /* Wenn Zeichen nicht in Tabelle */
      if   (!f)
           {
           /* dann das Zeichen anhaengen */
           *sz2 = *psz ;
           strcat (pszTarget,
                   sz2) ;
           }
      /* Naechstes Zeichen in Source */
      psz++ ;
      }
   return TNOERROR ;
   }

/* Hilfsfunktion mit phonetischem Vergleich */
BOOL TSoundex (PSZ psz1,
               PSZ psz2)
   {
   BOOL f ;
   CHAR ch1 ;
   CHAR ch2 ;
   CHAR szValue [] = "01230120022455012623010202" ; /* Tabelle der Zeichenwertigekeiten */
   PSZ  psz ;
   PSZ  pszNew1 ;
   PSZ  pszNew1Save ;
   PSZ  pszNew2 ;
   PSZ  pszNew2Save ;

   /* Fehlerkennung vorbesetzen */
   f = TERROR ;
   /* Nur wenn erster Buchstabe Text */
   if   (isascii (*psz1) &&
         isascii (*psz2))
        {
        /* Speicher fuer ersten Text */
        pszNew1 = calloc (2,
                          strlen (psz1)) ;
        /* Pointer merken */
        pszNew1Save = pszNew1 ;
        /* Sonderzeichen ersetzen */
        TReplaceCharacter (pszNew1,
                           psz1) ;
        /* Nur Grosschrift */
        strupr (pszNew1) ;
        /* Speicher fuer zweiten Text */
        pszNew2 = calloc (2,
                          strlen (psz2)) ;
        /* Pointer merken */
        pszNew2Save = pszNew2 ;
        /* Sonderzeichen ersetzen */
        TReplaceCharacter (pszNew2,
                           psz2) ;
        /* Nur Grosschrift */
        strupr (pszNew2) ;
        /* Erstes Zeichen bleibt immer erhalten */
        ch1 = *pszNew1++ ;
        /* Temporaerer Pointer */
        psz = pszNew1 ;
        /* In einer Schleife bis Textende */
        while (ch2 = *pszNew1++)
           {
           /* Ist Zeichen ASCII? */
           if   (isascii (ch2))
                {
                /* Ist Zeichen nicht gleich vorherigem Zeichen? */
                if   (szValue [ch2 - 'A'] != szValue [ch1 - 'A'])
                     {
                     /* Ist Wertigkeit des Zeichens groesser 0? */
                     if   (szValue [ch2 - 'A'] != '0')
                          {
                          /* Aktuelles Zeichen wird gemerkt */
                          ch1 = ch2 ;
                          /* Wertigkeit setzen */
                          *psz = szValue [ch2 - 'A'] ;
                          /* Naechste Soundexposition */
                          psz++ ;
                          }
                     }
                }
           }
        /* Ende des ersten Soundextextes */
        *psz = '\0' ;
        /* Erstes Zeichen bleibt immer erhalten */
        ch1 = *pszNew2++ ;
        /* Temporaerer Pointer */
        psz = pszNew2 ;
        /* In einer Schleife bis Textende */
        while (ch2 = *pszNew2++)
           {
           /* Ist Zeichen ASCII? */
           if   (isascii (ch2))
                {
                /* Ist Zeichen nicht gleich vorherigem Zeichen? */
                if   (szValue [ch2 - 'A'] != szValue [ch1 - 'A'])
                     {
                     /* Ist Wertigkeit des Zeichens groesser 0? */
                     if   (szValue [ch2 - 'A'] != '0')
                          {
                          /* Aktuelles Zeichen wird gemerkt */
                          ch1 = ch2 ;
                          /* Wertigkeit setzen */
                          *psz = szValue [ch2 - 'A'] ;
                          /* Naechste Soundexposition */
                          psz++ ;
                          }
                     }
                }
           }
        /* Ende des zweiten Soundextextes */
        *psz = '\0' ;
        /* Vergleich der beiden Soundexwerte */
        if   (strcmp (pszNew1Save,
                      pszNew2Save) == 0)
             {
             /* Treffer */
             f = TNOERROR ;
             }
        /* Teporaeren Speicher wieder freigeben */
        free (pszNew1Save) ;
        free (pszNew2Save) ;
        }
   /* Zurueck */
   return f ;
   }

