/* $Header: D:/Projekte/Attraktive Attribute/Source/rcs/EAEDITOR.C 1.1 1996/06/01 10:41:27 HaWi Exp $ */

/* OS/2 Header */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C Header */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Applikationsheader */
#include "EAEDITOR.H"

/* Einige globale Variablen */
BOOL  fMinimized_g = FALSE ;
HAB   hab_g = NULLHANDLE ;
HWND  hwndClient_g = NULLHANDLE ;
HWND  hwndFrame_g = NULLHANDLE ;
PSZ   pszEAEditor_g = "EAEditor" ;
ULONG ulWait_g = 0 ;

/* Fensterprozedur des Clients */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Message ist von Interesse */
   switch (msg)
      {
      /* Der Benutzer will was */
      case WM_COMMAND:
           {
           CHAR        sz [CCHMAXPATH] ;
           HWND        hwndTemp ;
           PUSRRECDIR  pusrrecdir ;
           PUSRRECTREE pusrrectree ;
           PWND        pwnd ;

           /* Instanzvariablen aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Was will der Benutzer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Eine Datei loeschen */
              case IDM_DIRDELETE:
                   /* Selektiertes Verzeichnis holen */
                   if   ((pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                                 CM_QUERYRECORDEMPHASIS,
                                                                 MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                 MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Selektierte Datei holen */
                        if   ((pusrrecdir = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerDir,
                                                                     CM_QUERYRECORDEMPHASIS,
                                                                     MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                     MPFROMSHORT (CRA_SELECTED)))) != NULL)
                             {
                             /* Verzeichnisname und Dateiname konkatinieren */
                             strcpy (sz,
                                     pusrrectree->szDir) ;
                             if   (sz [strlen (sz) - 1] != '\\')
                                  {
                                  strcat (sz,
                                          "\\") ;
                                  }
                             strcat (sz,
                                     pusrrecdir->szFilename) ;
                             /* Datei loeschen */
                             if   (!remove (sz))
                                  {
                                  /* Wenn erfolgreich Eintrag aus im Container loeschen */
                                  WinSendMsg (pwnd->hwndContainerDir,
                                              CM_REMOVERECORD,
                                              MPFROMP (&pusrrecdir),
                                              MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
                                  }
                             }
                        }
                   return (MRESULT) FALSE ;
              /* EA soll editiert werden */
              case IDM_DIREAEDIT:
                   /* Selektiertes Verzeichnis holen */
                   if   ((pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                                 CM_QUERYRECORDEMPHASIS,
                                                                 MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                 MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Selektierte Datei holen */
                        if   ((pusrrecdir = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerDir,
                                                                     CM_QUERYRECORDEMPHASIS,
                                                                     MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                     MPFROMSHORT (CRA_SELECTED)))) != NULL)
                             {
                             /* Verzeichnisname und Dateiname konkatinieren */
                             strcpy (sz,
                                     pusrrectree->szDir) ;
                             if   (sz [strlen (sz) - 1] != '\\')
                                  {
                                  strcat (sz,
                                          "\\") ;
                                  }
                             strcat (sz,
                                     pusrrecdir->szFilename) ;
                             /* EA Dialog laden */
                             hwndTemp = WinLoadDlg (HWND_DESKTOP,
                                                    hwndFrame_g,
                                                    EADlgWndProc,
                                                    NULLHANDLE,
                                                    IDD_EA,
                                                    NULL) ;
                             /* Dateiname an Dialog schicken */
                             WinSendMsg (hwndTemp,
                                         WMP_MAININIT,
                                         MPFROMP (sz),
                                         MPVOID) ;
                             /* Dialogverarbeitung starten */
                             WinProcessDlg (hwndTemp) ;
                             /* Dialog wieder loeschen */
                             WinDestroyWindow (hwndTemp) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Container schrumpfen */
              case IDM_EDITCOLLAPSE:
                   /* Schrumpfen veranlassen */
                   WinSendMsg (pwnd->hwndContainerTree,
                               CM_COLLAPSETREE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Container ausklappen */
              case IDM_EDITEXPAND:
                   /* Ausklappen veranlassen */
                   WinSendMsg (pwnd->hwndContainerTree,
                               CM_EXPANDTREE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              case IDM_TREEDELETE:
                   /* Selektiertes Verzeichnis holen */
                   if   ((pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                                 CM_QUERYRECORDEMPHASIS,
                                                                 MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                 MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Nur Verzeichnisse koennen geloescht werden */
                        if   (pusrrectree->recordtype == K_RECORDITEM)
                             {
                             /* Loesch Funktion aufrufen */
                             if   (TDeleteDir2 (pusrrectree->szDir,
                                                TRUE,
                                                TRUE) == TNOERROR)
                                  {
                                  /* Wenn erfolgreich Container Eintrag entfernen */
                                  WinSendMsg (pwnd->hwndContainerTree,
                                              CM_REMOVERECORD,
                                              MPFROMP (&pusrrectree),
                                              MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
                                  }
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Lesen eines Verzeichnis */
              case IDM_TREEOPEN:
                   /* Selektiertes Verzeichnis holen */
                   if   ((pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                                 CM_QUERYRECORDEMPHASIS,
                                                                 MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                 MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Hier erfolgt das eigentliche Lesen */
                        WinPostMsg (hwnd,
                                    WMP_REREAD,
                                    MPFROMLONG (K_REREADDIR),
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Kontrollelement meldet etwas */
      case WM_CONTROL:
           {
           HWND        hwndTemp ;
           POINTL      ptl ;
           PUSRRECDIR  pusrrecdir ;
           PUSRRECTREE pusrrectree ;
           PWND        pwnd ;
           SWP         swp ;

           /* Instanzvariablen aus Window Words holen */
           if   ((pwnd = WinQueryWindowPtr (hwndFrame_g,
                                            QWL_USER)) != NULL)
                {
                /* Welches Kontrollelement? */
                switch (SHORT1FROMMP (mp1))
                   {
                   /* Dateicontainer */
                   case IDCO_DIR:
                        /* Was will es? */
                        switch (SHORT2FROMMP (mp1))
                           {
                           /* Benutzer fordert Kontextmenue an */
                           case CN_CONTEXTMENU:
                                /* Nur wenn auf einem Container Eintrag */
                                if   ((pusrrecdir = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Emphasis auf diesen Eintrag setzen */
                                     WinSendMsg (pwnd->hwndContainerDir,
                                                 CM_SETRECORDEMPHASIS,
                                                 MPFROMP (pusrrecdir),
                                                 MPFROM2SHORT (TRUE, CRA_SELECTED)) ;
                                     /* Wo steht Maus */
                                     WinQueryPointerPos (HWND_DESKTOP,
                                                         &ptl) ;
                                     /* Fensterposition des Frames ermitteln */
                                     WinQueryWindowPos (hwndFrame_g,
                                                        &swp) ;
                                     /* Position des Kontextmenues kalkulieren */
                                     ptl.x -= swp.x ;
                                     ptl.y -= swp.y ;
                                     /* Fensterposition des Containers ermitteln */
                                     WinQueryWindowPos (pwnd->hwndContainerDir,
                                                        &swp) ;
                                     /* Position des Kontextmenues weiter kalkulieren */
                                     ptl.x -= swp.x ;
                                     ptl.y -= swp.y ;
                                     /* Kontextmenue laden */
                                     hwndTemp = WinLoadMenu (HWND_OBJECT,
                                                             NULLHANDLE,
                                                             IDM_DIR) ;
                                     /* Kontextmenue anzeigen */
                                     WinPopupMenu (pwnd->hwndContainerDir,
                                                   hwnd,
                                                   hwndTemp,
                                                   ptl.x,
                                                   ptl.y,
                                                   IDM_DIREAEDIT,
                                                   PU_HCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE | PU_POSITIONONITEM | PU_VCONSTRAIN) ;
                                     }
                                break ;
                           /* Doppelklick auf einen Containereintrag */
                           case CN_ENTER:
                                /* Gueltigen Record holen */
                                if   ((pusrrecdir = (PUSRRECDIR) ((PNOTIFYRECORDENTER) PVOIDFROMMP (mp2))->pRecord) != NULL)
                                     {
                                     /* Anforderung simulieren */
                                     WinPostMsg (hwnd,
                                                 WM_COMMAND,
                                                 MPFROMSHORT (IDM_DIREAEDIT),
                                                 MPFROM2SHORT (CMDSRC_MENU, TRUE)) ;
                                     }
                                break ;
                           }
                        break ;
                   /* Verzeichniscontainer */
                   case IDCO_TREE:
                        /* Was will es? */
                        switch (SHORT2FROMMP (mp1))
                           {
                           /* Benutzer fordert Kontextmenue an */
                           case CN_CONTEXTMENU:
                                /* Nur wenn auf einem Container Eintrag */
                                if   ((pusrrectree = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Emphasis auf diesen Eintrag setzen */
                                     WinSendMsg (pwnd->hwndContainerTree,
                                                 CM_SETRECORDEMPHASIS,
                                                 MPFROMP (pusrrectree),
                                                 MPFROM2SHORT (TRUE, CRA_SELECTED)) ;
                                     /* Nur Verzeichnisse koennen geoeffnet werden */
                                     if   (pusrrectree->recordtype == K_RECORDAREA ||
                                           pusrrectree->recordtype == K_RECORDITEM)
                                          {
                                          /* Wo steht Maus */
                                          WinQueryPointerPos (HWND_DESKTOP,
                                                              &ptl) ;
                                          /* Fensterposition des Frames ermitteln */
                                          WinQueryWindowPos (hwndFrame_g,
                                                             &swp) ;
                                          /* Position des Kontextmenues kalkulieren */
                                          ptl.x -= swp.x ;
                                          ptl.y -= swp.y ;
                                          /* Fensterposition des Containers ermitteln */
                                          WinQueryWindowPos (pwnd->hwndContainerTree,
                                                             &swp) ;
                                          /* Position des Kontextmenues weiter kalkulieren */
                                          ptl.x -= swp.x ;
                                          ptl.y -= swp.y ;
                                          /* Kontextmenue laden */
                                          hwndTemp = WinLoadMenu (HWND_OBJECT,
                                                                  NULLHANDLE,
                                                                  IDM_TREE) ;
                                          /* Rootverzeichnisse koennen nicht geloescht werden */
                                          if   (pusrrectree->recordtype == K_RECORDAREA)
                                               {
                                               /* Also diesen Menueeintrag entfernen */
                                               WinSendMsg (hwndTemp,
                                                           MM_DELETEITEM,
                                                           MPFROM2SHORT (IDM_TREEDELETE, TRUE),
                                                           MPVOID) ;
                                               }
                                          /* Kontextmenue anzeigen */
                                          WinPopupMenu (pwnd->hwndContainerTree,
                                                        hwnd,
                                                        hwndTemp,
                                                        ptl.x,
                                                        ptl.y,
                                                        IDM_TREEOPEN,
                                                        PU_HCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE | PU_POSITIONONITEM | PU_VCONSTRAIN) ;
                                          }
                                     }
                                break ;
                           /* Doppelklick auf einen Containereintrag */
                           case CN_ENTER:
                                /* Gueltigen Record holen */
                                if   ((pusrrectree = (PUSRRECTREE) ((PNOTIFYRECORDENTER) PVOIDFROMMP (mp2))->pRecord) != NULL)
                                     {
                                     /* Nur Verzeichnisse koennen geoeffnet werden */
                                     if   (pusrrectree->recordtype == K_RECORDAREA ||
                                           pusrrectree->recordtype == K_RECORDITEM)
                                          {
                                          /* Anforderung simulieren */
                                          WinPostMsg (hwnd,
                                                      WMP_REREAD,
                                                      MPFROMLONG (K_REREADDIR),
                                                      MPVOID) ;
                                          }
                                     }
                                break ;
                           }
                        break ;
                   }
                }
           break ;
           }
      /* Die Maus ist ueber einem Kontrollelement */
      case WM_CONTROLPOINTER:
           /* Wenn pending WAIT */
           if   (ulWait_g)
                {
                /* WAIT Pointer ausgeben */
                return (MRESULT) WinQuerySysPointer (HWND_DESKTOP,
                                                     SPTR_WAIT,
                                                     FALSE) ;
                }
           break ;
      /* Das Fenster wird angelegt */
      case WM_CREATE:
           /* Platz fuer Instanzvariablen allokieren und in Window Words eintragen */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Verzoegerter Programmstart */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Fenster wird wieder geloescht */
      case WM_DESTROY:
           /* Instanzvariablen aus Window Words holen und loeschen */
           free (WinQueryWindowPtr (hwndFrame_g,
                                    QWL_USER)) ;
           break ;
      /* Hintergrundzeichnen des Frames steht an */
      case WM_ERASEBACKGROUND:
           /* PM soll selbst zeichnen */
           return (MRESULT) TRUE ;
      /* Ein Menu wurde beendet */
      case WM_MENUEND:
           /* Nur die Popup Menus */
           if   ((SHORT1FROMMP (mp1) == IDM_DIR) ||
                 (SHORT1FROMMP (mp1) == IDM_TREE))
                {
                /* Muessen explizit entfernt werden */
                WinDestroyWindow (HWNDFROMMP (mp2)) ;
                }
           break ;
      /* Min oder Max Button wurde gedrueckt */
      case WM_MINMAXFRAME:
           /* Neuer Zustand merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Groesse des Frames wurde veraendert */
      case WM_SIZE:
           /* Verzoegert durchfuehren */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       mp1,
                       mp2) ;
           break ;
      /* Verzoegerter Programmstart hier */
      case WMP_MAININIT:
           {
           CHAR                 sz [2048] ;
           CNRINFO              ccinfo ;
           static CONTAINERINIT acnrinitDir [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_FILENAME, offsetof (USRRECDIR, minirec.pszIcon) },
                                                  { "", CFA_DATE | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_DATE,     offsetof (USRRECDIR, cdateTemp)       },
                                                  { "", CFA_TIME | CFA_HORZSEPARATOR | CFA_CENTER | CFA_SEPARATOR, CFA_CENTER, IDS_TIME,     offsetof (USRRECDIR, ctimeTemp)       },
                                                  { "", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR, CFA_CENTER, IDS_SIZE,     offsetof (USRRECDIR, ulSize)          },
                                                  { "", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR, CFA_CENTER, IDS_EASIZE,   offsetof (USRRECDIR, ulEASize)        }} ;
           PWND                 pwnd ;
           SWP                  swp ;

           /* Instanzvariablen aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Programmtitel aus Ressource laden */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_PROGRAMM,
                          sizeof (sz),
                          sz) ;
           /* Programmtitel in Titelzeile ausgeben */
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz) ;
           /* Groesse des Clients berechnen */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* Ein Rahmen um den Verzeichniscontainer anlegen */
           pwnd->hwndStaticTree = WinCreateWindow (hwndClient_g,
                                                   WC_STATIC,
                                                   "",
                                                   SS_FGNDFRAME | WS_VISIBLE,
                                                   0,
                                                   0,
                                                   swp.cx / 3,
                                                   swp.cy,
                                                   hwndFrame_g,
                                                   HWND_TOP,
                                                   IDT_TREE,
                                                   NULL,
                                                   NULL) ;
           /* Verzeichniscontainer anlegen */
           pwnd->hwndContainerTree = WinCreateWindow (hwndClient_g,
                                                      WC_CONTAINER,
                                                      "",
                                                      CCS_AUTOPOSITION | CCS_MINIICONS | CCS_MINIRECORDCORE | CCS_READONLY | CCS_SINGLESEL | WS_GROUP | WS_VISIBLE,
                                                      1,
                                                      1,
                                                      swp.cx / 3 - 2,
                                                      swp.cy - 2,
                                                      hwndFrame_g,
                                                      HWND_TOP,
                                                      IDCO_TREE,
                                                      NULL,
                                                      NULL) ;
           /* Container Kommunikationsstruktur leeren */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Flags fuer Container */
           ccinfo.flWindowAttr = CA_TITLESEPARATOR | CA_TREELINE | CV_MINI | CV_TEXT | CV_TREE ;
           /* Flags setzen */
           WinSendMsg (pwnd->hwndContainerTree,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_FLWINDOWATTR)) ;
           /* Ein Rahmen um den Dateicontainer anlegen */
           pwnd->hwndStaticDir = WinCreateWindow (hwndClient_g,
                                                  WC_STATIC,
                                                  "",
                                                  SS_FGNDFRAME | WS_VISIBLE,
                                                  swp.cx / 3,
                                                  0,
                                                  swp.cx / 3 * 2,
                                                  swp.cy,
                                                  hwndFrame_g,
                                                  HWND_TOP,
                                                  IDT_DIR,
                                                  NULL,
                                                  NULL) ;
           /* Dateicontainer anlegen */
           pwnd->hwndContainerDir = WinCreateWindow (hwndClient_g,
                                                     WC_CONTAINER,
                                                     "",
                                                     CCS_AUTOPOSITION | CCS_MINIICONS | CCS_MINIRECORDCORE | CCS_READONLY | CCS_SINGLESEL | WS_GROUP | WS_VISIBLE,
                                                     swp.cx / 3 + 1,
                                                     1,
                                                     swp.cx / 3 * 2 - 2,
                                                     swp.cy - 2,
                                                     hwndFrame_g,
                                                     HWND_TOP,
                                                     IDCO_DIR,
                                                     NULL,
                                                     NULL) ;
           /* Spalten des Containers anlegen */
           TInitContainer (pwnd->hwndContainerDir,
                           NULLHANDLE,
                           &acnrinitDir [0],
                           sizeof (acnrinitDir) / sizeof (acnrinitDir [0])) ;
           /* Container Kommunikationsstruktur leeren */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Flags fuer Container */
           ccinfo.flWindowAttr = CA_CONTAINERTITLE | CA_DETAILSVIEWTITLES | CA_TITLESEPARATOR | CV_DETAIL | CV_MINI ;
           /* Adresse des Containertitels */
           ccinfo.pszCnrTitle = pwnd->szHeaderDir ;
           /* Flags und Adresse fuer Containertitel setzen */
           WinSendMsg (pwnd->hwndContainerDir,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           /* Verzeichnisbaumlesen anstossen */
           WinPostMsg (hwnd,
                       WMP_REREAD,
                       MPFROMLONG (K_REREADTREE),
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Nachlesen von Verzeichnisconatiner und Dateicontainer wird hier aktiviert */
      case WMP_REREAD:
           {
           PWND pwnd ;

           /* Instanzvariablen aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Welcher Container ist dran? */
           switch (LONGFROMMP (mp1))
              {
              /* Dateicontainer */
              case K_REREADDIR:
                   /* Dateicontainerlesen in einem Thread */
                   _beginthread (ThreadDir,
                                 NULL,
                                 80000,
                                 pwnd) ;
                   break ;
              /* Verzeichniscontainer */
              case K_REREADTREE:
                   /* Verzeichniscontainerlesen in einem Thread */
                   _beginthread (ThreadTree,
                                 NULL,
                                 80000,
                                 pwnd) ;
                   break ;
              }
           return (MRESULT) FALSE ;
           }
      /* Verzoegte Groessenaenderung hier */
      case WMP_SIZE:
           {
           PWND pwnd ;
           SWP  swp ;

           /* Instanzvariablen aus Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn Fenster nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Clientgroesse ermitteln */
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                /* Neue Position des Verzeichniscontainer Rahmen setzen */
                WinSetWindowPos (pwnd->hwndStaticTree,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx / 3,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                /* Neue Position des Verzeichniscontainer setzen */
                WinSetWindowPos (pwnd->hwndContainerTree,
                                 HWND_TOP,
                                 1,
                                 1,
                                 swp.cx / 3 - 2,
                                 swp.cy - 2,
                                 SWP_MOVE | SWP_SIZE) ;
                /* Neue Position des Dateicontainer Rahmen setzen */
                WinSetWindowPos (pwnd->hwndStaticDir,
                                 HWND_TOP,
                                 swp.cx / 3,
                                 0,
                                 swp.cx / 3 * 2,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                /* Neue Position des Dateicontainer setzen */
                WinSetWindowPos (pwnd->hwndContainerDir,
                                 HWND_TOP,
                                 swp.cx / 3 + 1,
                                 1,
                                 swp.cx / 3 * 2 - 2,
                                 swp.cy - 2,
                                 SWP_MOVE | SWP_SIZE) ;
                }
           return (MRESULT) FALSE ;
           }
      /* WAIT processing */
      case WMP_WAIT:
           /* FALSE wenn schon 0 ignorieren */
           if   (LONGFROMMP (mp1) || ulWait_g)
                {
                /* Anzahl der WAIT Events rauf oder runtersetzen, je nach Wunsch */
                (LONGFROMMP (mp1)) ? (ulWait_g)++ : (ulWait_g)-- ;
                /* Nur wenn Status wechselt */
                if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                      (!LONGFROMMP (mp1) && !ulWait_g))
                     {
                     /* Neuen Pointer fuer Maus setzen */
                     WinSetPointer (HWND_DESKTOP,
                                    WinQuerySysPointer (HWND_DESKTOP,
                                                        (LONGFROMMP (mp1)) ? SPTR_WAIT : SPTR_ARROW,
                                                        FALSE)) ;
                     /* Fenster disablen oder enablen */
                     WinEnableWindow (WinQueryWindow (hwnd,
                                                      QW_PARENT),
                                      !LONGFROMMP (mp1)) ;
                     }
                }
           return (MRESULT) FALSE ;
      }
   /* Aufruf der Standardfenster Prozedur */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Fensterprozedur EA Dialog */
MRESULT EXPENTRY EADlgWndProc (HWND   hwndDlg,
                               MSG    msg,
                               MPARAM mp1,
                               MPARAM mp2)
   {
   /* Welche Message ist von Interesse */
   switch (msg)
      {
      /* Benutzer will was */
      case WM_COMMAND:
           {
           CHAR    sz [CCHMAXPATH] ;
           CHAR    sz2 [CCHMAXPATH] ;
           EATDATA aeatdata [2] ;
           HWND    hwndTemp ;
           PBYTE   pb ;
           PDLGEA  pdlg ;
           SHORT   s ;
           ULONG   ul ;

           /* Instanzvariablen aus Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Was will Benutzer */
           switch (SHORT1FROMMP (mp1))
              {
              /* OK Button gedrueckt */
              case DID_OK:
                   /* Dialog wegwerfen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              /* Neues EA Button */
              case IDP_EA_1:
                   /* Dialog neues EA laden */
                   hwndTemp = WinLoadDlg (HWND_DESKTOP,
                                          hwndDlg,
                                          EANewDlgWndProc,
                                          NULLHANDLE,
                                          IDD_EANEW,
                                          NULL) ;
                   /* Dateiname hinterherschicken */
                   WinSendMsg (hwndTemp,
                               WMP_MAININIT,
                               MPFROMP (pdlg->szFilename),
                               MPVOID) ;
                   /* Dialog aktivieren */
                   WinProcessDlg (hwndTemp) ;
                   /* Dialog wieder entfernen */
                   WinDestroyWindow (hwndTemp) ;
                   /* Nachlesen der EAs zu der Datei */
                   WinPostMsg (hwndDlg,
                               WMP_REREAD,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* EA aendern Button */
              case IDP_EA_2:
                   /* Welcher Eintrag ist in Listbox selektiert */
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        /* Text des selektierten Eintrags holen */
                        WinSendMsg (pdlg->hwndListbox,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (sz)),
                                    MPFROMP (sz)) ;
                        /* Im Handle des selektierten Eintrags steht die Laenge der EAs */
                        ul = LONGFROMMR (WinSendMsg (pdlg->hwndListbox,
                                                     LM_QUERYITEMHANDLE,
                                                     MPFROMSHORT (s),
                                                     MPVOID)) ;
                        /* Platz hierfuer allokieren */
                        pb = calloc (1,
                                     ul) ;
                        /* Text aus Eingabefeld lesen */
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_EA_1,
                                             sizeof (sz2),
                                             sz2) ;
                        /* War es ein ASCII EA? */
                        if   (TEAReadASCII (pdlg->szFilename,
                                            sz,
                                            pb,
                                            &ul) == TNOERROR)
                             {
                             /* Dann als ASCII aendern */
                             TEAWriteASCII (pdlg->szFilename,
                                            sz,
                                            sz2) ;
                             }
                        /* Wenn kein ASCII EA */
                        else
                             {
                             /* EA Struktur leeren */
                             memset (aeatdata,
                                     0,
                                     sizeof (aeatdata)) ;
                             /* Laenge der alten EA Textes */
                             aeatdata[0].ulValue = ul ;
                             /* Platz fuer alten EA Text */
                             aeatdata[0].pbValue = pb ;
                             /* Alten EA Text lesen */
                             if   (TEARead (pdlg->szFilename,
                                            sz,
                                            aeatdata) == TNOERROR)
                                  {
                                  /* War es ASCII */
                                  if   (aeatdata[0].ulEAType == EAT_ASCII)
                                       {
                                       /* Ja, Kommunikationsstruktur wieder leeren fuer neuen EA Text */
                                       memset (aeatdata,
                                               0,
                                               sizeof (aeatdata)) ;
                                       /* ASCII Typ vermerken */
                                       aeatdata[0].ulEAType = EAT_ASCII ;
                                       /* Laenge des neuen EA Textes */
                                       aeatdata[0].ulValue = strlen (sz2) ;
                                       /* Adresse des neuen EA Textes */
                                       aeatdata[0].pbValue = sz2 ;
                                       /* EA setzen */
                                       TEAWriteMV (pdlg->szFilename,
                                                   sz,
                                                   EAT_MVST,
                                                   aeatdata) ;
                                       }
                                  }
                             }
                        /* Platz fuer alten EA Text loeschen */
                        free (pb) ;
                        /* Nachlesen der EAs */
                        WinPostMsg (hwndDlg,
                                    WMP_REREAD,
                                    MPVOID,
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              /* EA loeschen Button */
              case IDP_EA_3:
                   /* Welcher Eintrag ist in Listbox selektiert */
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox,
                                                        LM_QUERYSELECTION,
                                                        MPFROMSHORT (LIT_FIRST),
                                                        MPVOID))) != LIT_NONE)
                        {
                        /* Text des selektierten Eintrags holen */
                        WinSendMsg (pdlg->hwndListbox,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (s, sizeof (sz)),
                                    MPFROMP (sz)) ;
                        /* Leeren EA Text uebergeben */
                        TEAWrite (pdlg->szFilename,
                                  sz,
                                  NULL) ;
                        /* Nachlesen der EAs */
                        WinPostMsg (hwndDlg,
                                    WMP_REREAD,
                                    MPVOID,
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Kontrollelement meldet sich */
      case WM_CONTROL:
           {
           BOOL    f ;
           CHAR    sz [CCHMAXPATH] ;
           EATDATA aeatdata [2] ;
           LONG    al [9] [2] = {{ EAT_ASCII,    IDS_ASCII                },
                                 { EAT_ASN1,     IDS_UNKNOWN              },
                                 { EAT_BINARY,   IDS_BINARY               },
                                 { EAT_BITMAP,   IDS_BITMAP               },
                                 { EAT_EA,       IDS_EA                   },
                                 { EAT_ICON,     IDS_ICON                 },
                                 { EAT_METAFILE, IDS_METAFILE             },
                                 { EAT_MVMT,     IDS_MULTIVALUEMULTITYPE  },
                                 { EAT_MVST,     IDS_MULTIVALUESINGLETYPE }} ;
           PBYTE   pb ;
           PDLGEA  pdlg ;
           SHORT   s ;
           ULONG   ul ;
           ULONG   ulI ;

           /* Instanzvariablen aus Window Words holen */
           if   ((pdlg = WinQueryWindowPtr (hwndDlg,
                                            QWL_USER)) != NULL)
                {
                /* Welches Control? */
                switch (SHORT1FROMMP (mp1))
                   {
                   /* Die Listbox */
                   case IDL_EA_1:
                        /* Was will sie? */
                        switch (SHORT2FROMMP (mp1))
                           {
                           /* Benutzer hat eine Zeile selektiert */
                           case LN_SELECT:
                                /* Welche Zeile? */
                                if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox,
                                                                     LM_QUERYSELECTION,
                                                                     MPFROMSHORT (LIT_FIRST),
                                                                     MPVOID))) != LIT_NONE)
                                     {
                                     /* Text der Zeile holen */
                                     WinSendMsg (pdlg->hwndListbox,
                                                 LM_QUERYITEMTEXT,
                                                 MPFROM2SHORT (s, sizeof (sz)),
                                                 MPFROMP (sz)) ;
                                     /* Laenge des EAs dieser Zeile steht im Handle */
                                     ul = LONGFROMMR (WinSendMsg (pdlg->hwndListbox,
                                                                  LM_QUERYITEMHANDLE,
                                                                  MPFROMSHORT (s),
                                                                  MPVOID)) ;
                                     /* Platz fuer EA allokieren */
                                     pb = calloc (1,
                                                  ul) ;
                                     /* Ausgabefeld loeschen */
                                     WinSetDlgItemText (hwndDlg,
                                                        IDT_EA_5,
                                                        "") ;
                                     /* Eingabefeld loeschen */
                                     WinSetDlgItemText (hwndDlg,
                                                        IDE_EA_1,
                                                        "") ;
                                     /* Ist es ein ASCII EA? */
                                     if   (TEAReadASCII (pdlg->szFilename,
                                                         sz,
                                                         pb,
                                                         &ul) == TNOERROR)
                                          {
                                          /* Ja, dann entsprechenden Text aus Ressource laden */
                                          WinLoadString (hab_g,
                                                         NULLHANDLE,
                                                         IDS_ASCII,
                                                         sizeof (sz),
                                                         sz) ;
                                          /* und ausgeben */
                                          WinSetDlgItemText (hwndDlg,
                                                             IDT_EA_5,
                                                             sz) ;
                                          /* EA Inhalt ausgeben */
                                          WinSetDlgItemText (hwndDlg,
                                                             IDE_EA_1,
                                                             pb) ;
                                          /* Text kan editiert werden */
                                          WinEnableControl (hwndDlg,
                                                            IDE_EA_1,
                                                            TRUE) ;
                                          /* Aendern ist erlaubt */
                                          WinEnableControl (hwndDlg,
                                                            IDP_EA_2,
                                                            TRUE) ;
                                          }
                                     /* Kein ASCII EA */
                                     else
                                          {
                                          /* EA Struktur loeschen */
                                          memset (aeatdata,
                                                  0,
                                                  sizeof (aeatdata)) ;
                                          /* Laenge des alten EA Textes */
                                          aeatdata[0].ulValue = ul ;
                                          /* Platz fuer alten EA Text */
                                          aeatdata[0].pbValue = pb ;
                                          /* EA lesen */
                                          if   (TEARead (pdlg->szFilename,
                                                         sz,
                                                         aeatdata) == TNOERROR)
                                               {
                                               f = FALSE ;
                                               /* In Tabelle nach EA Typ suchen */
                                               for  (ulI = 0; ulI < sizeof (al) / sizeof (al [0]); ulI++)
                                                    {
                                                    /* Wenn gefunden */
                                                    if   (aeatdata[0].ulEAType == al [ulI] [0])
                                                         {
                                                         /* Text hierzu aus Ressource laden */
                                                         WinLoadString (hab_g,
                                                                        NULLHANDLE,
                                                                        al [ulI] [1],
                                                                        sizeof (sz),
                                                                        sz) ;
                                                         f = TRUE ;
                                                         break ;
                                                         }
                                                    }
                                               if   (!f)
                                                    {
                                                    /* Wenn EA Typ nicht gefunden UNKNOWN laden */
                                                    WinLoadString (hab_g,
                                                                   NULLHANDLE,
                                                                   IDS_UNKNOWN,
                                                                   sizeof (sz),
                                                                   sz) ;
                                                    }
                                               /* Typ des EAs ausgeben */
                                               WinSetDlgItemText (hwndDlg,
                                                                  IDT_EA_5,
                                                                  sz) ;
                                               /* EA Text ausgeben wenn ASCII */
                                               WinSetDlgItemText (hwndDlg,
                                                                  IDE_EA_1,
                                                                  (aeatdata[0].ulEAType == EAT_ASCII) ? pb : "") ;
                                               /* Eingabefeld zum Aendern nur bei ASCII freigeben */
                                               WinEnableControl (hwndDlg,
                                                                 IDE_EA_1,
                                                                 (aeatdata[0].ulEAType == EAT_ASCII)) ;
                                               /* Pushbutton Aendern nur bei ASCII freigeben */
                                               WinEnableControl (hwndDlg,
                                                                 IDP_EA_2,
                                                                 (aeatdata[0].ulEAType == EAT_ASCII)) ;
                                               }
                                          }
                                     /* Platz fuer alten EA Text wieder freigeben */
                                     free (pb) ;
                                     }
                                break ;
                           }
                        break ;
                   }
                }
           break ;
           }
      /* Dialog wird geschlossen */
      case WM_DESTROY:
           /* Instanzvariablen aus Window Words holen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGEA pdlg ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGEA)) ;
           /* pointer auf Instanzvariablen in Window Words eintragen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Handle der Listbox merken */
           pdlg->hwndListbox = WinWindowFromID (hwndDlg,
                                                IDL_EA_1) ;
           /* Eingabefeld laenge setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_EA_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Client schickt weitere Informationen nach dem Start des Dialoges hinterher */
      case WMP_MAININIT:
           {
           PDLGEA pdlg ;

           /* Instanzvariablen aus Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Der Dateiname */
           strcpy (pdlg->szFilename,
                   PVOIDFROMMP (mp1)) ;
           /* Dateiname ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDT_EA_2,
                              pdlg->szFilename) ;
           /* Lesen der EAs fuer diese Datei anwerfen */
           WinPostMsg (hwndDlg,
                       WMP_REREAD,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Nachlesen der EAs */
      case WMP_REREAD:
           {
           APIRET rc ;
           BYTE   b [CCHMAXPATH] ;
           PDLGEA pdlg ;
           PFEA2  pfea2 ;
           SHORT  s ;
           ULONG  ulCount ;
           ULONG  ulIndex ;

           /* Instanzvariablen aus Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Listbox leeren */
           WinSendMsg (pdlg->hwndListbox,
                       LM_DELETEALL,
                       MPVOID,
                       MPVOID) ;
           pfea2 = (PFEA2) b ;
           rc = NO_ERROR ;
           ulCount = 1 ;
           ulIndex = 0 ;
           /* In einer Schleife */
           do
              {
              /* Die EAs der Datei besorgen */
              rc = DosEnumAttribute (ENUMEA_REFTYPE_PATH,
                                     pdlg->szFilename,
                                     ++ulIndex,
                                     b,
                                     sizeof (b),
                                     &ulCount,
                                     ENUMEA_LEVEL_NO_VALUE) ;
              /* Wenn kein Fehler und EA gefunden */
              if   (ulCount &&
                    rc == NO_ERROR)
                   {
                   /* EA in Listbox eintragen */
                   if   ((s = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox,
                                                        LM_INSERTITEM,
                                                        MPFROMSHORT (LIT_END),
                                                        MPFROMP (pfea2->szName)))) != LIT_MEMERROR)
                        {
                        /* EA Laenge in Handle der Listboxzeile eintragen */
                        WinSendMsg (pdlg->hwndListbox,
                                    LM_SETITEMHANDLE,
                                    MPFROMSHORT (s),
                                    MPFROMLONG (pfea2->cbValue)) ;
                        }
                   }
              /* Weiter in Schleife */
              } while (ulCount &&
                       rc == NO_ERROR) ;
           /* Ersten Eintrag immer selektieren */
           WinSendMsg (pdlg->hwndListbox,
                       LM_SELECTITEM,
                       MPFROMSHORT (0),
                       MPFROMSHORT (TRUE)) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standarddialog Verarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialogprozedur EA Neu */
MRESULT EXPENTRY EANewDlgWndProc (HWND   hwndDlg,
                                  MSG    msg,
                                  MPARAM mp1,
                                  MPARAM mp2)
   {
   /* Welche Messages sind von Interesse */
   switch (msg)
      {
      /* Benutzer will was */
      case WM_COMMAND:
           {
           BOOL      f ;
           CHAR      sz [CCHMAXPATH] ;
           CHAR      sz2 [CCHMAXPATH] ;
           EATDATA   aeatdata [2] ;
           PDLGEANEW pdlg ;

           /* Instanzvariablen aus Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Was will er */
           switch (SHORT1FROMMP (mp1))
              {
              /* Ok Button */
              case DID_OK:
                   /* EA Name lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_EANEW_1,
                                        sizeof (sz),
                                        sz) ;
                   /* Lerer Text als Datum */
                   strcpy (sz2,
                           " ") ;
                   /* Ist ASCII Radiobutton aktiviert? */
                   if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                          IDR_EANEW_1,
                                                          BM_QUERYCHECK,
                                                          MPVOID,
                                                          MPVOID)))
                        {
                        /* Ja, ein ASCII EA schreiben */
                        TEAWriteASCII (pdlg->szFilename,
                                       sz,
                                       sz2) ;
                        }
                   else
                        {
                        /* Nein, feststellen ob Multiple-Value Single or Multiple Type */
                        if   (SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                               IDR_EANEW_2,
                                                               BM_QUERYCHECK,
                                                               MPVOID,
                                                               MPVOID)))
                             {
                             f = TRUE ;
                             }
                        else
                             {
                             f = FALSE ;
                             }
                        /* EA Struktur leeren */
                        memset (aeatdata,
                                0,
                                sizeof (aeatdata)) ;
                        /* Neuer Typ ASCII */
                        aeatdata[0].ulEAType = EAT_ASCII ;
                        /* EA Textlaenge */
                        aeatdata[0].ulValue = strlen (sz2) ;
                        /* EA Text */
                        aeatdata[0].pbValue = sz2 ;
                        /* EA schreiben */
                        TEAWriteMV (pdlg->szFilename,
                                    sz,
                                    (f) ? EAT_MVST : EAT_MVMT,
                                    aeatdata) ;
                        }
                   /* Dialog wegwerfen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Dialog wird geschloessen */
      case WM_DESTROY:
           /* Instanzvariablen aus Window Words holen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGEANEW pdlg ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGEANEW)) ;
           /* Instanzvariablen in Window Words eintragen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Eingabefeldlaenge festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_EANEW_1,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Client schickt noch Daten hinterher */
      case WMP_MAININIT:
           {
           PDLGEANEW pdlg ;

           /* Instanzvariablen aus Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Dateiname merken */
           strcpy (pdlg->szFilename,
                   PVOIDFROMMP (mp1)) ;
           /* Dateiname audgeben */
           WinSetDlgItemText (hwndDlg,
                              IDT_EANEW_2,
                              pdlg->szFilename) ;
           /* Erster Radiobutton ist standardmaessig aktiviert */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_EANEW_1,
                              BM_SETCHECK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Dialog Standardprozedur */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Keine Kommandozeilenparameter Analyse benoetigt */
VOID _setuparg (VOID)
   {
   }

/* Programmstart */
INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_AUTOICON | FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Anmelden beim PM */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse definieren */
             if   (WinRegisterClass (hab_g,
                                     pszEAEditor_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszEAEditor_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_EAEDITOR,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Loop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Fenster loeschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM abmelden */
        WinTerminate (hab_g) ;
        }
   /* Programmende */
   return 0 ;
   }

/* Rekursive Funktion zum Lesen eines Verzeichnisastes */
BOOL ScanTree (PSZ         psz,
               PWND        pwnd,
               PUSRRECTREE pusrrectree)
   {
   APIRET       rc ;
   CHAR         sz [CCHMAXPATH] ;
   FILEFINDBUF3 findbuf3 ;
   HDIR         hdir ;
   PUSRRECTREE  pusrrectree2 ;
   RECORDINSERT recins ;
   ULONG        ul ;

   /* Mit diesem Verzeichnis starten */
   strcpy (sz,
           psz) ;
   /* Wildcard anhaengen */
   strcat (sz,
           "\\*.*") ;
   hdir = HDIR_CREATE ;
   ul = 1 ;
   /* Suche starten */
   rc = DosFindFirst (sz,
                      &hdir,
                      FILE_DIRECTORY,
                      &findbuf3,
                      sizeof (findbuf3),
                      &ul,
                      FIL_STANDARD) ;
   /* Solange OK */
   while (rc == NO_ERROR &&
          ul == 1)
      {
      /* Nur Verzeichnisse werden gesucht */
      if   (findbuf3.attrFile & FILE_DIRECTORY)
           {
           /* Die beiden Rootwechsler werden ignoriert */
           if   (!(((findbuf3.achName [0] == '.') &&
                   (findbuf3.cchName == 1)) ||
                  ((findbuf3.achName [0] == '.') &&
                   (findbuf3.achName [1] == '.') &&
                   (findbuf3.cchName == 2))))
                {
                /* Neuen Verzeichnisnamen konstruieren */
                strcpy (sz,
                        psz) ;
                strcat (sz,
                        "\\") ;
                strcat (sz,
                        findbuf3.achName) ;
                /* Verzeichniscontainer Eintrag allokieren */
                pusrrectree2 = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                        CM_ALLOCRECORD,
                                                        MPFROMLONG (sizeof (USRRECTREE) - sizeof (MINIRECORDCORE)),
                                                        MPFROMSHORT (1))) ;
                strcpy (pusrrectree2->szDir,
                        sz) ;
                strcpy (pusrrectree2->szFilename,
                        findbuf3.achName) ;
                pusrrectree2->minirec.pszIcon = pusrrectree2->szFilename ;
                pusrrectree2->recordtype = K_RECORDITEM ;
                /* Verzeichnis einfuegen */
                memset (&recins,
                        0,
                        sizeof (RECORDINSERT)) ;
                recins.cb = sizeof (RECORDINSERT) ;
                recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                /* Unter diesem Eintrag einsortieren */
                recins.pRecordParent = (PRECORDCORE) pusrrectree ;
                recins.zOrder = CMA_TOP ;
                recins.cRecordsInsert = 1 ;
                WinSendMsg (pwnd->hwndContainerTree,
                            CM_INSERTRECORD,
                            MPFROMP (pusrrectree2),
                            MPFROMP (&recins)) ;
                /* Diesen Ast weiterverfolgen */
                if   (!ScanTree (sz,
                                 pwnd,
                                 pusrrectree2))
                     {
                     return FALSE ;
                     }
                }
           }
      /* Weitersuchen */
      rc = DosFindNext (hdir,
                        &findbuf3,
                        sizeof (findbuf3),
                        &ul) ;
      }
   /* Suche beenden */
   DosFindClose (hdir) ;
   return TRUE ;
   }

/* Loeschen eines Verzeichnisbaums */
BOOL TDeleteDir2 (PSZ  psz,
                  BOOL fDeleteContents,
                  BOOL fIgnoreAttributes)
   {
   APIRET       rc ;
   BOOL         f ;
   CHAR         sz [CCHMAXPATH] ;
   FILEFINDBUF3 findbuf3 ;
   FILESTATUS3  fsts3 ;
   HDIR         hdir ;
   ULONG        ul ;

   f = TNOERROR ;
   /* Mit diesem Verzeichnis starten */
   strcpy (sz,
           psz) ;
   /* Wildcard anhaengen */
   strcat (sz,
           "\\*.*") ;
   hdir = HDIR_CREATE ;
   ul = 1 ;
   /* Suche beginnen */
   rc = DosFindFirst (sz,
                      &hdir,
                      FILE_ARCHIVED | FILE_DIRECTORY | FILE_NORMAL | FILE_READONLY,
                      &findbuf3,
                      sizeof (findbuf3),
                      &ul,
                      FIL_STANDARD) ;
   /* Solange OK */
   while (rc == NO_ERROR &&
          ul == 1)
      {
      /* Meues Verzeichnis konstuieren */
      strcpy (sz,
              psz) ;
      strcat (sz,
              "\\") ;
      strcat (sz,
              findbuf3.achName) ;
      /* Nur Verzeichnisse beruecksichtigen */
      if   (findbuf3.attrFile & FILE_DIRECTORY)
           {
           /* Rootwechsel ignorieren */
           if   (!(((findbuf3.achName [0] == '.') &&
                   (findbuf3.cchName == 1)) ||
                  ((findbuf3.achName [0] == '.') &&
                   (findbuf3.achName [1] == '.') &&
                   (findbuf3.cchName == 2))))
                {
                /* Im Ast weiterloeschen */
                if   ((f = TDeleteDir2 (sz,
                                        fDeleteContents,
                                        fIgnoreAttributes)) == TERROR)
                     {
                     /* Es ist noch etwas drin was nicht geloescht werden soll oder
                        es ist etwas mit Schreibschutz versehen und der soll auch nicht entfernt werden */
                     return TERROR ;
                     }
                else
                     {
                     /* Ansonsten diesen Verzeichnisast loeschen */
                     if   (DosDeleteDir (sz))
                          {
                          return TERROR ;
                          }
                     }
                }
           }
      /* Kein Verzeichnis */
      else
           {
           /* Sollen Inhalte geloescht werden */
           if   (fDeleteContents)
                {
                /* Ist Schreibschutz drauf */
                if   (findbuf3.attrFile & FILE_READONLY)
                     {
                     /* Soll Schreibschutz ignoriert werden */
                     if   (fIgnoreAttributes)
                          {
                          /* Alte Attribute lesen */
                          if   (!DosQueryPathInfo (sz,
                                                   FIL_STANDARD,
                                                   &fsts3,
                                                   sizeof (FILESTATUS3)))
                               {
                               /* READONLY wegnehmen */
                               fsts3.attrFile &= ~FILE_READONLY ;
                               /* Attribute wieder setzen */
                               DosSetPathInfo (sz,
                                               FIL_STANDARD,
                                               &fsts3,
                                               sizeof (FILESTATUS3),
                                               0) ;
                               /* Datei loeschen */
                               if   (DosDelete (sz))
                                    {
                                    return TERROR ;
                                    }
                               }
                          }
                     }
                /* Kein Schreibschutz */
                else
                     {
                     /* Also direkt loeschen */
                     if   (DosDelete (sz))
                          {
                          return TERROR ;
                          }
                     }
                }
           }
      /* Weitersuchen */
      rc = DosFindNext (hdir,
                        &findbuf3,
                        sizeof (findbuf3),
                        &ul) ;
      }
   /* Suche beenden */
   DosFindClose (hdir) ;
   /* Alles OK bis hierhin? */
   if   (f == TNOERROR)
        {
        /* Dann Starverzeichnis auch loeschen */
        if   (DosDeleteDir (psz))
             {
             f = TERROR ;
             }
        }
   /* Fehler zurueckgeben */
   return f ;
   }

/* Hilfsfunktion zum Allokieren eines EAOP2 Puffers */
PEAOP2 TEACreateEAOP2Read (ULONG     ul,
                           PGEA2LIST pgea2l)
   {
   PEAOP2 peaop2 ;

   /* Speicherplatz fuer EA Puffer allokieren */
   peaop2 = calloc (1,
                    ul) ;
   /* Pointer auf Suchliste eintragen */
   peaop2->fpGEA2List = pgea2l ;
   peaop2->fpFEA2List = (FEA2LIST *) (peaop2 + 1) ;
   peaop2->fpFEA2List->cbList = ul - sizeof (EAOP2) ;
   return peaop2 ;
   }

/* Hilfsfunktion zum Allokieren eines EAOP2 Buffers */
PEAOP2 TEACreateEAOP2Write (PFEA2LIST pfea2l)
   {
   PEAOP2 peaop2 ;

   peaop2 = calloc (1,
                    sizeof (PEAOP2)) ;
   peaop2->fpFEA2List = pfea2l ;
   return peaop2 ;
   }

/* Hilfsfunktion zum Allokieren einer FEA2 Liste */
PFEA2LIST TEACreateFEA2List (PSZ   pszEAName,
                             PBYTE pbValue,
                             ULONG ulValue)
   {
   PFEA2LIST pfea2l ;

   /* Speicherplatz fuer Suchliste allokieren */
   pfea2l = calloc (1,
                    sizeof (FEA2LIST) + strlen (pszEAName) + ulValue) ;
   pfea2l->cbList = sizeof (FEA2LIST) + strlen (pszEAName) + ulValue ;
   pfea2l->list->cbName = strlen (pszEAName) ;
   pfea2l->list->cbValue = ulValue ;
   strcpy (pfea2l->list->szName, pszEAName) ;
   if   (pbValue && ulValue)
        {
        memcpy (pfea2l->list->szName + strlen (pszEAName) + 1,
                pbValue,
                ulValue) ;
        }
   return pfea2l ;
   }

/* Hilfsfunktion zum Allokieren einer GEA2 Liste */
PGEA2LIST TEACreateGEA2List (PSZ psz)
   {
   PGEA2LIST pgea2l ;

   /* Speicherplatz fuer Suchliste allokieren */
   pgea2l = calloc (1,
                    sizeof (GEA2LIST) + strlen (psz)) ;
   pgea2l->cbList = sizeof (GEA2LIST) + strlen (psz) ;
   pgea2l->list->cbName = strlen (psz) ;
   strcpy (pgea2l->list->szName,
           psz) ;
   return pgea2l ;
   }

/* Read EA */
BOOL TEARead (PSZ      pszFilename,
              PSZ      pszEAName,
              PEATDATA peatdata)
   {
   BOOL        f ;
   FILESTATUS4 fsts4 ;
   union
      {
      PBYTE    pb ;
      PFEA2    pfea2 ;
      PUSHORT  pus ;
      }        pea ;
   PEAOP2      peaop2 ;
   PGEA2LIST   pgea2l ;
   ULONG       ul ;

   f = TERROR ;
   if   (!DosQueryPathInfo (pszFilename,
                            FIL_QUERYEASIZE,
                            &fsts4,
                            sizeof (FILESTATUS4)))
        {
        pgea2l = TEACreateGEA2List (pszEAName) ;
        peaop2 = TEACreateEAOP2Read (sizeof (EAOP2) + fsts4.cbList,
                                     pgea2l) ;
        if   (!DosQueryPathInfo (pszFilename,
                                 FIL_QUERYEASFROMLIST,
                                 peaop2,
                                 sizeof (EAOP2)))
             {
             pea.pfea2 = peaop2->fpFEA2List->list ;
             if   (pea.pfea2->cbValue)
                  {
                  pea.pb = (PBYTE) &(pea.pfea2->szName) + pea.pfea2->cbName + 1 ;
                  peatdata->ulEAType = *pea.pus++ ;
                  ul = min (*pea.pus, peatdata->ulValue) ;
                  if   (ul)
                       {
                       memcpy (peatdata->pbValue,
                               pea.pus + 1,
                               ul) ;
                       if   (peatdata->ulEAType == EAT_ASCII)
                            {
                            ul = min (*pea.pus, peatdata->ulValue - 1) ;
                            peatdata->pbValue [ul] = '\0' ;
                            }
                       }
                  peatdata->ulValue = *pea.pus ;
                  f = TNOERROR ;
                  }
             else
                  {
                  peatdata->ulValue = 0 ;
                  f = TNOERROR ;
                  }
             }
        free (peaop2) ;
        free (pgea2l) ;
        }
   if   (!f)
        {
        peatdata->ulValue = 0 ;
        }
   /* Fehler zurueckgeben */
   return f ;
   }

/* Read ASCII EA */
BOOL TEAReadASCII (PSZ    pszFilename,
                   PSZ    pszEAName,
                   PSZ    pszValue,
                   PULONG pulValue)
   {
   BOOL        f ;
   FILESTATUS4 fsts4 ;
   union
      {
      PBYTE    pb ;
      PFEA2    pfea2 ;
      PUSHORT  pus ;
      }        pea ;
   PEAOP2      peaop2 ;
   PGEA2LIST   pgea2l ;
   ULONG       ul ;

   if   (*pulValue)
        {
        *pszValue = '\0' ;
        }
   f = TERROR ;
   if   (!DosQueryPathInfo (pszFilename,
                            FIL_QUERYEASIZE,
                            &fsts4,
                            sizeof (FILESTATUS4)))
        {
        pgea2l = TEACreateGEA2List (pszEAName) ;
        peaop2 = TEACreateEAOP2Read (sizeof (EAOP2) + fsts4.cbList,
                                     pgea2l) ;
        if   (!DosQueryPathInfo (pszFilename,
                                 FIL_QUERYEASFROMLIST,
                                 peaop2,
                                 sizeof (EAOP2)))
             {
             pea.pfea2 = peaop2->fpFEA2List->list ;
             if   (pea.pfea2->cbValue)
                  {
                  pea.pb = (PBYTE) &(pea.pfea2->szName) + pea.pfea2->cbName + 1 ;
                  if   (*pea.pus++ == EAT_ASCII)
                       {
                       ul = min (*pea.pus, *pulValue - 1) ;
                       if   (ul)
                            {
                            memcpy (pszValue,
                                    pea.pus + 1,
                                    ul) ;
                            pszValue [ul] = '\0' ;
                            }
                       *pulValue = *pea.pus ;
                       f = TNOERROR ;
                       }
                  }
             else
                  {
                  f = TNOERROR ;
                  }
             }
        free (peaop2) ;
        free (pgea2l) ;
        }
   if   (!f)
        {
        *pulValue = 0 ;
        }
   /* Fehler zurueckgeben */
   return f ;
   }

/* Multiple Value EA Lesefunktion */
BOOL TEAReadMV (PSZ     pszFilename,
                PSZ     pszEAName,
                ULONG   ulEAType,
                EATDATA aeatdata [])
   {
   BOOL        f ;
   FILESTATUS4 fsts4 ;
   union
       {
       PBYTE   pb ;
       PEATMV  peatmv ;
       PFEA2   pfea2 ;
       PMVMT   pmvmt ;
       PMVST   pmvst ;
       PUSHORT pWord;
       }       pea ;
   PEAOP2      peaop2 ;
   PGEA2LIST   pgea2l ;
   ULONG       ulBytes ;
   ULONG       ulEAType2 ;
   ULONG       ulI ;
   ULONG       ulMaxEA ;
   ULONG       ulValue ;

   /* Anzahl Elemente in EA zaehlen */
   for  (ulValue = 0; aeatdata[ulValue].pbValue != NULL; ulValue++) ;
   /* Ist da ueberhaupt etwas */
   if   (!ulValue)
        {
        return TNOERROR ;
        }
   f = FALSE ;
   /* Groesse der EAs ermitteln */
   if   (!DosQueryPathInfo (pszFilename,
                            FIL_QUERYEASIZE,
                            &fsts4,
                            sizeof (FILESTATUS4)))
        {
        /* Hierfuer Platz allokieren */
        pgea2l = TEACreateGEA2List (pszEAName) ;
        peaop2 = TEACreateEAOP2Read (sizeof (EAOP2) + fsts4.cbList,
                                     pgea2l) ;
        /* EAs lesen */
        if   (!DosQueryPathInfo (pszFilename,
                                 FIL_QUERYEASFROMLIST,
                                 peaop2,
                                 sizeof (EAOP2)))
             {
             pea.pfea2 = peaop2->fpFEA2List->list ;
             if   (pea.pfea2->cbValue)
                  {
                  pea.pb = (PBYTE) &(pea.pfea2->szName) + pea.pfea2->cbName + 1 ;
                  if   (pea.peatmv->usEAType == ulEAType)
                       {
                       ulMaxEA = pea.peatmv->usEA ;
                       pea.pmvmt = pea.peatmv->mvmt ;
                       if   (ulEAType == EAT_MVST)
                            {
                            aeatdata[0].ulEAType = pea.pmvmt->usEAType ;
                            ulEAType2 = pea.pmvmt->usEAType ;
                            pea.pmvmt = (PMVMT) &(pea.pmvmt->usValue) ;
                            }
                       for  (ulI = 0; ulI < ulValue; ulI++)
                            {
                            if   (ulI < ulMaxEA)
                                 {
                                 if   (ulEAType == EAT_MVMT)
                                      {
                                      aeatdata[ulI].ulEAType = pea.pmvmt->usEAType ;
                                      pea.pmvst = (PMVST) &(pea.pmvmt->usValue) ;
                                      }
                                 else
                                      {
                                      aeatdata[ulI].ulEAType = ulEAType2 ;
                                      }
                                 ulBytes = min (pea.pmvst->usValue, aeatdata[ulI].ulValue) ;
                                 if   (aeatdata[ulI].ulEAType == EAT_ASCII)
                                      {
                                      ulBytes = min (ulBytes, aeatdata[ulI].ulValue - 1) ;
                                      aeatdata[ulI].pbValue [ulBytes] = '\0' ;
                                      }
                                 aeatdata[ulI].ulValue = ulBytes ;
                                 memcpy (aeatdata[ulI].pbValue,
                                         pea.pmvst->bValue,
                                         ulBytes) ;
                                 }
                            else
                                 {
                                 aeatdata[ulI].ulValue = 0 ;
                                 }
                            pea.pmvst = (PMVST) (pea.pmvst->bValue + pea.pmvst->usValue) ;
                            }
                       aeatdata[ulValue].ulValue = 0 ;
                       f = TNOERROR ;
                       }
                  }
             else
                  {
                  f = TNOERROR ;
                  for  (ulI = 0; ulI < ulValue; ulI++)
                       {
                       aeatdata[ulI].ulValue = 0 ;
                       aeatdata[ulI].ulEAType = 0 ;
                       }
                  }
             }
        /* EA Strukuren wieder freigeben */
        free (peaop2) ;
        free (pgea2l) ;
        }
   /* Im Fehlerfall */
   if   (!f)
        {
        /* Alles loeschen */
        for  (ulI = 0; ulI < ulValue; ulI++)
             {
             aeatdata[ulI].ulValue = 0 ;
             }
        }
   /* Fehler zurueckgeben */
   return f ;
   }

/* Write EA */
BOOL TEAWrite (PSZ      pszFilename,
               PSZ      pszEAName,
               PEATDATA peatdata)
   {
   BOOL      f ;
   PEAOP2    peaop2 ;
   PEATSV    peatsv ;
   PFEA2LIST pfea2l ;

   if   (peatdata)
        {
        peatsv = calloc (1,
                         sizeof (EATSV) + peatdata->ulValue - 1) ;
        peatsv->usEAType = peatdata->ulEAType ;
        peatsv->usValue = peatdata->ulValue ;
        memcpy (peatsv->bValue,
                peatdata->pbValue,
                peatdata->ulValue) ;
        pfea2l = TEACreateFEA2List (pszEAName,
                                    (PBYTE) peatsv,
                                    sizeof (EATSV) + peatdata->ulValue - 1) ;
        }
   else
        {
        pfea2l = TEACreateFEA2List (pszEAName,
                                    NULL,
                                    0) ;
        }
   peaop2 = TEACreateEAOP2Write (pfea2l);
   f = (DosSetPathInfo (pszFilename,
                        FIL_QUERYEASIZE,
                        peaop2,
                        sizeof (EAOP2),
                        DSPI_WRTTHRU)) ? TERROR : TNOERROR ;
   free (peaop2) ;
   free (pfea2l) ;
   if   (peatdata)
        {
        free (peatsv) ;
        }
   /* Fehler zurueckgeben */
   return f ;
   }

/* Write ASCII EA */
BOOL TEAWriteASCII (PSZ pszFilename,
                    PSZ pszEAName,
                    PSZ pszValue)
   {
   BOOL      f ;
   PEAOP2    peaop2 ;
   PEATSV    peatsv ;
   PFEA2LIST pfea2l ;

   peatsv = calloc (1,
                    sizeof (EATSV) + strlen (pszValue) - 1) ;
   peatsv->usEAType = EAT_ASCII ;
   peatsv->usValue = strlen (pszValue) ;
   memcpy (peatsv->bValue,
           pszValue,
           strlen (pszValue)) ;
   pfea2l = TEACreateFEA2List (pszEAName,
                               (PBYTE) peatsv,
                               sizeof (EATSV) + strlen (pszValue) - 1) ;
   peaop2 = TEACreateEAOP2Write (pfea2l) ;
   f = (DosSetPathInfo (pszFilename,
                        FIL_QUERYEASIZE,
                        peaop2,
                        sizeof (EAOP2),
                        DSPI_WRTTHRU)) ? TERROR : TNOERROR ;
   free (peaop2) ;
   free (pfea2l) ;
   free (peatsv) ;
   /* Fehler zurueckgeben */
   return f ;
   }

/* Write Multiple-Value EA */
BOOL TEAWriteMV (PSZ     pszFilename,
                 PSZ     pszEAName,
                 ULONG   ulEAType,
                 EATDATA aeatdata [])
   {
   BOOL      f ;
   union
      {
      PMVMT  pmvmt ;
      PMVST  pmvst ;
      }      pea ;
   PEAOP2    peaop2 ;
   PEATMV    peatmv ;
   PFEA2LIST pfea2l ;
   ULONG     ulBytes ;
   ULONG     ulI ;
   ULONG     ulValue ;

   for  (ulValue = 0; aeatdata[ulValue].pbValue != NULL; ulValue++) ;
   if   (!ulValue)
        {
        return TNOERROR ;
        }
   ulBytes = sizeof (EATMV) ;
   switch (ulEAType)
      {
      case EAT_MVST:
           ulBytes += (ulValue - 1) * sizeof (MVST) ;
           break ;
      case EAT_MVMT:
           ulBytes += (ulValue - 1) * sizeof (MVMT) ;
           break ;
      default:
           return TERROR ;
      }
   for  (ulI = 0; ulI < ulValue; ulI++)
        {
        ulBytes += aeatdata[ulI].ulValue - 1 ;
        }
   peatmv = calloc (1,
                    ulBytes) ;
   peatmv->usEAType = ulEAType ;
   peatmv->usCodepage = 0 ;
   peatmv->usEA = ulValue ;
   pea.pmvmt = peatmv->mvmt ;
   if   (ulEAType == EAT_MVST)
        {
        pea.pmvmt->usEAType = aeatdata[0].ulEAType ;
        pea.pmvmt = (PMVMT) &(peatmv->mvmt[0].usValue) ;
        }
   for  (ulI = 0; ulI < ulValue; ulI++)
        {
        if   (ulEAType == EAT_MVMT)
             {
             pea.pmvmt->usEAType = aeatdata[ulI].ulEAType ;
             pea.pmvst = (PMVST) &(pea.pmvmt->usValue) ;
             }
        pea.pmvst->usValue = aeatdata[ulI].ulValue ;
        memcpy (pea.pmvst->bValue,
                aeatdata[ulI].pbValue,
                pea.pmvst->usValue) ;
        pea.pmvst = (PMVST) (pea.pmvst->bValue + pea.pmvst->usValue) ;
        }
   pfea2l = TEACreateFEA2List (pszEAName,
                               (PBYTE) peatmv,
                               ulBytes) ;
   peaop2 = TEACreateEAOP2Write (pfea2l) ;
   f = (DosSetPathInfo (pszFilename,
                        FIL_QUERYEASIZE,
                        peaop2,
                        sizeof (EAOP2),
                        DSPI_WRTTHRU)) ? TERROR : TNOERROR ;
   free (peaop2) ;
   free (pfea2l) ;
   free (peatmv) ;
   /* Fehler zurueckgeben */
   return f ;
   }

/* Thread zum Lesen des Verzeichnisses */
VOID ThreadDir (VOID* pv)
   {
   APIRET       rc ;
   CHAR         sz [CCHMAXPATH] ;
   CNRINFO      ccinfo ;
   FILEFINDBUF3 findbuf3 ;
   FILESTATUS4  fsts4 ;
   HAB          hab ;
   HDIR         hdir ;
   HMQ          hmq ;
   PUSRRECDIR   pusrrecdir ;
   PUSRRECTREE  pusrrectree ;
   PWND         pwnd ;
   RECORDINSERT recins ;
   ULONG        ul ;

   /* Clientinstanzdaten holen */
   pwnd = pv ;
   /* Thread beim PM anmelden */
   hab = WinInitialize (0) ;
   /* Message Queue fuer Thread anlegen */
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   /* Abbruch der Anwendung wird nicht durch Thread blockiert */
   WinCancelShutdown (hmq,
                      TRUE) ;
   /* WAIT Pointer an */
   WinSendMsg (hwndClient_g,
               WMP_WAIT,
               MPFROMLONG (TRUE),
               MPVOID) ;
   /* Verzeichniscontainer leeren */
   WinSendMsg (pwnd->hwndContainerDir,
               CM_REMOVERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
   /* Selektieren Verzeichniseintrag lesen */
   if   ((pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                 CM_QUERYRECORDEMPHASIS,
                                                 MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                 MPFROMSHORT (CRA_SELECTED)))) != NULL)
        {
        /* Verzeichnis im Dateicontainer Titel ausgeben */
        strcpy (pwnd->szHeaderDir,
                pusrrectree->szDir) ;
        /* Container Kommunikationsstruktur leeren */
        memset (&ccinfo,
                0,
                sizeof (CNRINFO)) ;
        /* Titel des Containers */
        ccinfo.pszCnrTitle = pwnd->szHeaderDir ;
        /* Containerinformationen setzen */
        WinSendMsg (pwnd->hwndContainerDir,
                    CM_SETCNRINFO,
                    MPFROMP (&ccinfo),
                    MPFROMLONG (CMA_CNRTITLE)) ;
        /* Verzeichnis zusammenbauen */
        strcpy (sz,
                pusrrectree->szDir) ;
        if   (strlen (sz) > 3)
             {
             strcat (sz,
                     "\\") ;
             }
        strcat (sz,
                "*.*") ;
        /* Jeweils ein Verzeichnis lesen */
        hdir = HDIR_CREATE ;
        ul = 1 ;
        /* Suche starten */
        rc = DosFindFirst (sz,
                           &hdir,
                           FILE_ARCHIVED | FILE_NORMAL | FILE_READONLY,
                           &findbuf3,
                           sizeof (findbuf3),
                           &ul,
                           FIL_STANDARD) ;
        /* Solange etwas gefunden */
        while (rc == NO_ERROR &&
               ul == 1)
           {
           /* Neuen Dateicontainer Eintrag allokieren */
           pusrrecdir = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerDir,
                                                 CM_ALLOCRECORD,
                                                 MPFROMLONG (sizeof (USRRECDIR) - sizeof (MINIRECORDCORE)),
                                                 MPFROMSHORT (1))) ;
           /* Dateiname */
           strcpy (pusrrecdir->szFilename,
                   findbuf3.achName) ;
           pusrrecdir->minirec.pszIcon = pusrrecdir->szFilename ;
           strcpy (sz,
                   pusrrectree->szDir) ;
           if   (strlen (sz) > 3)
                {
                strcat (sz,
                        "\\") ;
                }
           strcat (sz,
                   pusrrecdir->szFilename) ;
           /* Dateinformationen auslesen */
           if   (!DosQueryPathInfo (sz,
                                    FIL_QUERYEASIZE,
                                    &fsts4,
                                    sizeof (FILESTATUS4)))
                {
                /* Datum */
                pusrrecdir->cdateTemp.day = fsts4.fdateLastWrite.day ;
                pusrrecdir->cdateTemp.month = fsts4.fdateLastWrite.month ;
                pusrrecdir->cdateTemp.year = fsts4.fdateLastWrite.year + 1980 ;
                /* Uhrzeit */
                pusrrecdir->ctimeTemp.hours = fsts4.ftimeLastWrite.hours ;
                pusrrecdir->ctimeTemp.minutes = fsts4.ftimeLastWrite.minutes ;
                pusrrecdir->ctimeTemp.seconds = fsts4.ftimeLastWrite.twosecs * 2 ;
                /* EA Groesse */
                pusrrecdir->ulEASize = fsts4.cbList - 4 ;
                /* Dateigroesse */
                pusrrecdir->ulSize = fsts4.cbFile ;
                }
           /* Dateicontainer Eintrag einfuegen */
           memset (&recins,
                   0,
                   sizeof (RECORDINSERT)) ;
           recins.cb = sizeof (RECORDINSERT) ;
           recins.pRecordOrder = (PRECORDCORE) CMA_END ;
           recins.zOrder = CMA_TOP ;
           recins.cRecordsInsert = 1 ;
           WinSendMsg (pwnd->hwndContainerDir,
                       CM_INSERTRECORD,
                       MPFROMP (pusrrecdir),
                       MPFROMP (&recins)) ;
           /* Weitersuchen */
           rc = DosFindNext (hdir,
                             &findbuf3,
                             sizeof (findbuf3),
                             &ul) ;
           }
        /* Suche abschliessen */
        DosFindClose (hdir) ;
        }
   /* Container invalidieren */
   WinSendMsg (pwnd->hwndContainerDir,
               CM_INVALIDATERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
   /* WAIT aus */
   WinSendMsg (hwndClient_g,
               WMP_WAIT,
               MPFROMLONG (FALSE),
               MPVOID) ;
   /* Message Queue loeschen */
   WinDestroyMsgQueue (hmq) ;
   /* Beim PM abmelden */
   WinTerminate (hab) ;
   /* Threadende */
   _endthread () ;
   }

/* Thread zum Lesen des Verzeichnisbaumes */
VOID ThreadTree (VOID* pv)
   {
   CHAR         sz [CCHMAXPATH] ;
   HAB          hab ;
   HMQ          hmq ;
   PFSQBUFFER2  pfsqbuf2 ;
   PSZ          psz ;
   PUSRRECTREE  pusrrectree ;
   PWND         pwnd ;
   RECORDINSERT recins ;
   ULONG        ulI ;
   ULONG        ulJ ;

   /* Clientinstanzdaten holen */
   pwnd = pv ;
   /* Thread beim PM anmelden */
   hab = WinInitialize (0) ;
   /* Message Queue fuer Thread anlegen */
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   /* Abbruch der Anwendung wird nicht durch Thread blockiert */
   WinCancelShutdown (hmq,
                      TRUE) ;
   /* WAIT Pointer an */
   WinSendMsg (hwndClient_g,
               WMP_WAIT,
               MPFROMLONG (TRUE),
               MPVOID) ;
   /* Verzeichniscontainer leeren */
   WinSendMsg (pwnd->hwndContainerTree,
               CM_REMOVERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
   /* Einen Record allokieren */
   pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                          CM_ALLOCRECORD,
                                          MPFROMLONG (sizeof (USRRECTREE) - sizeof (MINIRECORDCORE)),
                                          MPFROMSHORT (1))) ;
   /* Text fuer Root Eintrag aus Ressource lesen */
   WinLoadString (hab_g,
                  NULLHANDLE,
                  IDS_LOCALMAP,
                  sizeof (pusrrectree->szFilename),
                  pusrrectree->szFilename) ;
   pusrrectree->minirec.pszIcon = pusrrectree->szFilename ;
   /* Typ vermerken */
   pusrrectree->recordtype = K_RECORDROOT ;
   /* Record einfuegen */
   memset (&recins,
           0,
           sizeof (RECORDINSERT)) ;
   recins.cb = sizeof (RECORDINSERT) ;
   recins.pRecordOrder = (PRECORDCORE) CMA_END ;
   recins.zOrder = CMA_TOP ;
   recins.cRecordsInsert = 1 ;
   recins.fInvalidateRecord = TRUE ;
   WinSendMsg (pwnd->hwndContainerTree,
               CM_INSERTRECORD,
               MPFROMP (pusrrectree),
               MPFROMP (&recins)) ;
   pfsqbuf2 = malloc (8192) ;
   /* Fehlermeldungen ausschalten */
   DosError (FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION) ;
   /* In einer Schleife durch die moeglichen Drives */
   for  (ulI = 3; ulI <= 26; ulI++)
        {
        sz [0] = ulI + '@' ;
        sz [1] = ':' ;
        sz [2] = '\0' ;
        ulJ = 8192 ;
        memset (pfsqbuf2,
                0,
                ulJ) ;
        /* Device lesen */
        if   (!DosQueryFSAttach (sz,
                                 0,
                                 FSAIL_QUERYNAME,
                                 pfsqbuf2,
                                 &ulJ))
             {
             psz = pfsqbuf2->szName + pfsqbuf2->cbName + 1 ;
             /* CDs ignorieren */
             if   (stricmp (psz,
                            "CDFS"))
                  {
                  /* Record allokieren */
                  pusrrectree = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                         CM_ALLOCRECORD,
                                                         MPFROMLONG (sizeof (USRRECTREE) - sizeof (MINIRECORDCORE)),
                                                         MPFROMSHORT (1))) ;
                  /* Rootverzeichnis eintragen */
                  strcpy (pusrrectree->szDir,
                          sz) ;
                  strcat (pusrrectree->szDir,
                          "\\") ;
                  strcpy (pusrrectree->szFilename,
                          sz) ;
                  pusrrectree->minirec.pszIcon = pusrrectree->szFilename ;
                  pusrrectree->recordtype = K_RECORDAREA ;
                  /* Record einfuegen */
                  memset (&recins,
                          0,
                          sizeof (RECORDINSERT)) ;
                  recins.cb = sizeof (RECORDINSERT) ;
                  recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                  recins.pRecordParent = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainerTree,
                                                                  CM_QUERYRECORD,
                                                                  MPVOID,
                                                                  MPFROM2SHORT (CMA_FIRST, CMA_ITEMORDER))) ;
                  recins.zOrder = CMA_TOP ;
                  recins.cRecordsInsert = 1 ;
                  WinSendMsg (pwnd->hwndContainerTree,
                              CM_INSERTRECORD,
                              MPFROMP (pusrrectree),
                              MPFROMP (&recins)) ;
                  /* Verzeichnisbaum des Drives lesen */
                  ScanTree (sz,
                            pwnd,
                            pusrrectree) ;
                  }
             }
        }
   /* Fehlermeldungen wieder einschalten */
   DosError (FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION) ;
   free (pfsqbuf2) ;
   /* Container invalidieren */
   WinSendMsg (pwnd->hwndContainerTree,
               CM_INVALIDATERECORD,
               MPVOID,
               MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
   /* WAIT aus */
   WinSendMsg (hwndClient_g,
               WMP_WAIT,
               MPFROMLONG (FALSE),
               MPVOID) ;
   /* Message Queue loeschen */
   WinDestroyMsgQueue (hmq) ;
   /* Beim PM abmelden */
   WinTerminate (hab) ;
   /* Threadende */
   _endthread () ;
   }

/* Hilfsfunktion zum Initialisieren von Containern */
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
   WinSendMsg (hwnd,
               CM_INSERTDETAILFIELDINFO,
               MPFROMP (pfldinfoFirst),
               MPFROMP (&fiins)) ;
   return TNOERROR ;
   }


