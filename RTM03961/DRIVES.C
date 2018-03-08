/* $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/DRIVES.C 1.1 1996/03/05 18:04:09 HaWi Exp $ */

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
#include "DRIVES.H"

HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszDrives_g = "Drives" ;

/* Client Window Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
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
              case IDM_DIALOGDRIVES:
                   /* Dialog starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              DDWndProc,
                              NULLHANDLE,
                              IDD_DD,
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
MRESULT EXPENTRY DDWndProc (HWND   hwndDlg,
                            MSG    msg,
                            MPARAM mp1,
                            MPARAM mp2)
   {
   switch (msg)
      {
      /* Benutzeranforderung */
      case WM_COMMAND:
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel vom Systemmenu */
              case DID_CANCEL:
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok vom Pushbutton */
              case DID_OK:
                   /* Dialog entfernen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Ein Kontrollelement meldet sich */
      case WM_CONTROL:
           {
           APIRET       rc ;
           CHAR         sz [CCHMAXPATH] ;
           FILEFINDBUF3 findbuf3 ;
           HDIR         hdir ;
           SHORT        s ;
           ULONG        ulSearchCount ;

           /* Wer meldet sich? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Drives Listbox */
              case IDL_DD1:
                   /* und was will es? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein Eintrag wurde selektiert */
                      case LN_SELECT:
                           /* Index des selektierten Eintrag ermitteln */
                           if   ((s = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                       IDL_DD1,
                                                                       LM_QUERYSELECTION,
                                                                       MPFROMSHORT (LIT_FIRST),
                                                                       MPVOID))) != LIT_NONE)
                                {
                                /* Text des selektierten Eintrags ermitteln */
                                WinSendDlgItemMsg (hwndDlg,
                                                   IDL_DD1,
                                                   LM_QUERYITEMTEXT,
                                                   MPFROM2SHORT (s, sizeof (sz)),
                                                   MPFROMP (sz)) ;
                                /* Alle Eintraege der Verzeichnislistbox loeschen */
                                WinSendDlgItemMsg (hwndDlg,
                                                   IDL_DD2,
                                                   LM_DELETEALL,
                                                   MPVOID,
                                                   MPVOID) ;
                                /* An den Laufwerksbuchstaben Suchmaske anhaengen */
                                strcat (sz,
                                        ":\\*.*") ;
                                /* Directory Handle erzeugen lassen */
                                hdir = HDIR_CREATE ;
                                /* Einer nach dem anderen */
                                ulSearchCount = 1 ;
                                /* Ich will alle Verzeichnisse lesen */
                                rc = DosFindFirst (sz,
                                                   &hdir,
                                                   FILE_ARCHIVED | FILE_DIRECTORY | FILE_READONLY,
                                                   &findbuf3,
                                                   sizeof (findbuf3),
                                                   &ulSearchCount,
                                                   FIL_STANDARD) ;
                                /* Nur wenn kein Fehler */
                                if   (rc == NO_ERROR)
                                     {
                                     /* und ein Verzeichnis gefunden wurde */
                                     while ((ulSearchCount == 1) &&
                                            (rc == NO_ERROR))
                                        {
                                        /* Sicherheitsabfrage ob es wirklich auch ein Verzeichnis ist */
                                        if   (findbuf3.attrFile & FILE_DIRECTORY)
                                             {
                                             /* Die beiden Rueckeintraege ignorieren */
                                             if   (!(((findbuf3.achName [0] == '.') &&
                                                     (findbuf3.cchName == 1)) ||
                                                    ((findbuf3.achName [0] == '.') &&
                                                     (findbuf3.achName [1] == '.') &&
                                                     (findbuf3.cchName == 2))))
                                                  {
                                                  /* Verzeichnis in zweiter Listbox eintragen */
                                                  WinSendDlgItemMsg (hwndDlg,
                                                                     IDL_DD2,
                                                                     LM_INSERTITEM,
                                                                     MPFROMSHORT (LIT_END),
                                                                     MPFROMP (findbuf3.achName)) ;
                                                  }
                                             }
                                        /* Naechstes Verzeichnis suchen */
                                        rc = DosFindNext (hdir,
                                                          &findbuf3,
                                                          sizeof (findbuf3),
                                                          &ulSearchCount) ;
                                        }
                                     /* Directory Suchhandle wieder schliessen */
                                     DosFindClose (hdir) ;
                                     }
                                }
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Dialog Start */
      case WM_INITDLG:
           {
           CHAR       sz [2] ;
           FSALLOCATE fsalloc ;
           ULONG      ulI ;

           /* Harte Fehlermeldungen abschalten */
           DosError (FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION) ;
           /* Ueber alle Laufwerke scannen */
           for  (ulI = 3; ulI <= 26; ulI++)
                {
                /* Gibt es unter diesem Laufwerk Informationen? */
                if   (!DosQueryFSInfo (ulI,
                                       FSIL_ALLOC,
                                       &fsalloc,
                                       sizeof (fsalloc)))
                     {
                     /* Ja, dann Nummer zu Laufwerksbuchstaben machen */
                     sz [0] = ulI + '@' ;
                     sz [1] = '\0' ;
                     /* Laufwerksbuchstabe in erster Listbox eintragen */
                     WinSendDlgItemMsg (hwndDlg,
                                        IDL_DD1,
                                        LM_INSERTITEM,
                                        MPFROMSHORT (LIT_END),
                                        MPFROMP (sz)) ;
                     }
                }
           /* Harte Fehlermeldungen wieder eintragen */
           DosError (FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION) ;
           /* Ersten Eintrag in Laufwerkslistbox auf jeden Fall selektieren */
           WinSendDlgItemMsg (hwndDlg,
                              IDL_DD1,
                              LM_SELECTITEM,
                              MPFROMSHORT (0),
                              MPFROMSHORT (TRUE)) ;
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
                                     pszDrives_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Hauptfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE,
                                                           &ulCreateFlags,
                                                           pszDrives_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_DRIVES,
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

