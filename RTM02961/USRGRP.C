/* $Header: D:/PROJEKTE/Kleine Helferlein!1/Source/rcs/USRGRP.C 1.3 1995/12/01 13:11:06 HaWi Exp $ */

/* OS/2 Kopfdateien mit BASE und PM Support laden */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* Einige LAN Server Kopfdateien laden */
#define  INCL_32
#define  INCL_NETGROUP
#define  INCL_NETUSER
#include <netcons.h>
#include <access.h>

/* Einige C Kopfdateien laden */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikationskopfdatei laden */
#include "USRGRP.H"

BOOL  fMinimized_g = FALSE ;
HAB   hab_g = NULLHANDLE ;
HWND  hwndClient_g = NULLHANDLE ;
HWND  hwndFrame_g = NULLHANDLE ;
PSZ   pszUsrGrp_g = "UsrGrp" ;

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
           struct group_info_0* pgrpinf ;
           PCHAR                pch ;
           PWND                 pwnd ;
           ULONG                ulI ;
           struct user_info_0*  pusrinf ;
           USHORT               usEntriesRead ;
           USHORT               usTotalEntries ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Was will Benutzer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Gruppen scannen */
              case IDM_SGROUP:
                   /* Erst einmal pruefen wieviele Gruppen vorliegen */
                   pch = NULL ;
                   NetGroupEnum ("",
                                 ACCESS_NONE,
                                 pch,
                                 0,
                                 &usEntriesRead,
                                 &usTotalEntries) ;
                   /* Platz fuer diese Eintraeg allokieren */
                   pch = calloc (usTotalEntries,
                                 sizeof (struct group_info_0)) ;
                   /* Alle Gruppen lesen */
                   NetGroupEnum ("",
                                 ACCESS_NONE,
                                 pch,
                                 sizeof (struct group_info_0) * usTotalEntries,
                                 &usEntriesRead,
                                 &usTotalEntries) ;
                   /* In einer Schleife durch alle Gruppen */
                   pgrpinf = (struct group_info_0*) pch ;
                   for  (ulI = 1; ulI <= usEntriesRead; ulI++, pgrpinf++)
                        {
                        /* Name ist nicht NULL terminiert (mit Blank gepaddet) */
                        TCutString ((PSZ) pgrpinf->grpi0_name,
                                    ' ') ;
                        /* Gruppe in Listbox eintragen */
                        WinSendMsg (pwnd->hwndListbox1,
                                    LM_INSERTITEM,
                                    MPFROMSHORT (LIT_END),
                                    MPFROMP ((PSZ) pgrpinf->grpi0_name)) ;
                        }
                   /* Speicherplatz wieder freigeben */
                   free (pch) ;
                   return (MRESULT) FALSE ;
              /* User scannen */
              case IDM_SUSER:
                   /* Erst einmal pruefen wieviele User vorliegen */
                   pch = NULL ;
                   NetUserEnum ("",
                                ACCESS_NONE,
                                pch,
                                0,
                                &usEntriesRead,
                                &usTotalEntries) ;
                   /* Platz fuer diese Eintraeg allokieren */
                   pch = calloc (usTotalEntries,
                                 sizeof (struct user_info_0)) ;
                   /* Alle User lesen */
                   NetUserEnum ("",
                                ACCESS_NONE,
                                pch,
                                sizeof (struct user_info_0) * usTotalEntries,
                                &usEntriesRead,
                                &usTotalEntries) ;
                   /* In einer Schleife durch alle User */
                   pusrinf = (struct user_info_0*) pch ;
                   for  (ulI = 1; ulI <= usEntriesRead; ulI++, pusrinf++)
                        {
                        /* Name ist nicht NULL terminiert (mit Blank gepaddet) */
                        TCutString ((PSZ) pusrinf->usri0_name,
                                    ' ') ;
                        /* User in Listbox eintragen */
                        WinSendMsg (pwnd->hwndListbox2,
                                    LM_INSERTITEM,
                                    MPFROMSHORT (LIT_END),
                                    MPFROMP ((PSZ) pusrinf->usri0_name)) ;
                        }
                   /* Speicherplatz wieder freigeben */
                   free (pch) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
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
           CHAR  sz [128] ;
           PWND  pwnd ;
           SWP   swp ;
           ULONG ul ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
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
           /* Haelfte der Breite kalkulieren */
           ul = swp.cx / 2 ;
           /* 1. Listbox im Client positionieren */
           pwnd->hwndListbox1 = WinCreateWindow (hwndClient_g,
                                                 WC_LISTBOX,
                                                 "",
                                                 LS_HORZSCROLL | LS_NOADJUSTPOS | WS_GROUP | WS_VISIBLE,
                                                 0,
                                                 0,
                                                 ul,
                                                 swp.cy,
                                                 hwndFrame_g,
                                                 HWND_TOP,
                                                 IDL_GROUP,
                                                 NULL,
                                                 NULL) ;
           /* 2. Listbox im Client positionieren */
           pwnd->hwndListbox2 = WinCreateWindow (hwndClient_g,
                                                 WC_LISTBOX,
                                                 "",
                                                 LS_HORZSCROLL | LS_NOADJUSTPOS | WS_GROUP | WS_VISIBLE,
                                                 ul,
                                                 0,
                                                 swp.cx - ul,
                                                 swp.cy,
                                                 hwndFrame_g,
                                                 HWND_TOP,
                                                 IDL_USER,
                                                 NULL,
                                                 NULL) ;
           /* Gruppeninformationen im Menue aktivieren */
           WinPostMsg (hwnd,
                       WM_COMMAND,
                       MPFROMSHORT (IDM_SGROUP),
                       MPFROM2SHORT (CMDSRC_MENU, TRUE)) ;
           /* Userinformationen im Menue aktivieren */
           WinPostMsg (hwnd,
                       WM_COMMAND,
                       MPFROMSHORT (IDM_SUSER),
                       MPFROM2SHORT (CMDSRC_MENU, TRUE)) ;
           return (MRESULT) FALSE ;
           }
      /* Verzoegerte Groessenaenderung */
      case WMP_SIZE:
           {
           PWND  pwnd ;
           SWP   swp ;
           ULONG ul ;

           /* Instanzdaten des Fenster holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Groesse der Client Area ermitteln */
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                /* Haelfte der Breite kalkulieren */
                ul = swp.cx / 2 ;
                /* 1. Listbox im Client ausbreiten */
                WinSetWindowPos (pwnd->hwndListbox1,
                                 HWND_TOP,
                                 0,
                                 0,
                                 ul,
                                 swp.cy,
                                 SWP_MOVE | SWP_SHOW | SWP_SIZE | SWP_ACTIVATE) ;
                /* 2. Listbox im Client ausbreiten */
                WinSetWindowPos (pwnd->hwndListbox2,
                                 HWND_TOP,
                                 ul,
                                 0,
                                 swp.cx - ul,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standardverarbeitung */
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
                                     pszUsrGrp_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster oeffnen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszUsrGrp_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_USRGRP,
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

/* T-Helper Funktion zum Abschneiden von Zeichen an einem String */
BOOL TCutString (PSZ  pszString,
                 CHAR ch)
   {
   PSZ psz ;

   /* Ist Stringpointer ueberhaupt vorhanden? */
   if   (*pszString)
        {
        /* Ja */
        psz = pszString ;
        /* In einer Schleife an das Ende des Strings wandern */
        while (*pszString)
           {
           pszString++ ;
           }
        /* Vor den NULL Terminator stellen */
        pszString-- ;
        /* In einer Schleife von hinten */
        while (pszString != psz)
           {
           /* Wurde abzuschneidenes Zeichen gefunden? */
           if   (*pszString == ch)
                {
                /* Ja, also abschneiden */
                *pszString = '\0' ;
                }
           else
                {
                /* Das war's */
                break ;
                }
           /* Weiter zurueck wandern */
           pszString-- ;
           }
        /* Kein Fehler */
        return TNOERROR ;
        }
   else
        {
        /* Nein, Fehler */
        return TERROR ;
        }
   }


