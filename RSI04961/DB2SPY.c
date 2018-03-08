static char sqla_program_id[40] = 
{111,65,65,66,65,71,65,68,85,83,69,82,73,68,32,32,68,66,50,83,
80,89,32,32,65,65,81,114,80,76,77,76,48,49,49,49,49,32,32,32};


#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};

#line 1 "DB2SPY.SQC"
/* $Header: D:/PROJEKTE/Die Programmierung des DB2!2/Source/rcs/DB2SPY.SQC 1.2 1995/12/11 15:43:02 HaWi Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DB2/2 Kopfdateien */
#include <sql.h>
#include <sqlcodes.h>
#include <sqlenv.h>

/* Applikations Kopfdatei */
#include "DB2SPY.H"

/* Einlesen der SQL Kommunikationsstruktur */

/*
EXEC SQL INCLUDE SQLCA ;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 22 "DB2SPY.SQC"


/* Deklarationen der SQL Hostvariablen */

/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 25 "DB2SPY.SQC"

   char  SZDBNAME [9] ;
   char  SZCREATOR [9] ;
   char  SZNAME [19] ;

/*
EXEC SQL END DECLARE SECTION ;
*/

#line 29 "DB2SPY.SQC"


/* Modulglobale Variablen */
BOOL fMinimized_g = FALSE ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszDB2SPY_g = "DB2SPY" ;

/* Funktion Client Window Procedure */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welche Message lÑuft gerade ein? */
   switch (msg)
      {
      /* Anwendung schlie·en */
      case WM_CLOSE:
           /* Message Schleife durchbrechen */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Benutzaktion */
      case WM_COMMAND:
           {
           CNRINFO              ccinfo ;
           static CONTAINERINIT acnrinitconnectionservices [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,    offsetof (USRRECCONNECTIONSERVICES, pszName)    },
                                                                 { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_HOST,    offsetof (USRRECCONNECTIONSERVICES, pszHost)    },
                                                                 { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_COMMENT, offsetof (USRRECCONNECTIONSERVICES, pszComment) }} ;
           static CONTAINERINIT acnrinitdatabases [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_ALIAS,          offsetof (USRRECDATABASES, pszAlias)          },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,           offsetof (USRRECDATABASES, pszName)           },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DRIVE,          offsetof (USRRECDATABASES, pszDrive)          },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DIRECTORY,      offsetof (USRRECDATABASES, pszDirectory)      },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NODE,           offsetof (USRRECDATABASES, pszNode)           },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_TYPE,           offsetof (USRRECDATABASES, pszType)           },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DBTYPE,         offsetof (USRRECDATABASES, pszDBType)         },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_AUTHENTICATION, offsetof (USRRECDATABASES, pszAuthentication) },
                                                        { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_COMMENT,        offsetof (USRRECDATABASES, pszComment)        }} ;
           static CONTAINERINIT acnrinitpackages [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DBNAME,  offsetof (USRRECPACKAGES, pszDBName)  },
                                                       { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,    offsetof (USRRECPACKAGES, pszName)    },
                                                       { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_CREATOR, offsetof (USRRECPACKAGES, pszCreator) }} ;
           static CONTAINERINIT acnrinittables [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DBNAME,  offsetof (USRRECTABLES, pszDBName)  },
                                                     { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,    offsetof (USRRECTABLES, pszName)    },
                                                     { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_CREATOR, offsetof (USRRECTABLES, pszCreator) }} ;
           static CONTAINERINIT acnrinittablespaces [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DBNAME, offsetof (USRRECTABLESPACES, pszDBName) },
                                                          { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,   offsetof (USRRECTABLESPACES, pszName)   }} ;
           static CONTAINERINIT acnrinitviews [] = {{ "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_DBNAME,  offsetof (USRRECVIEWS, pszDBName)  },
                                                    { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,    offsetof (USRRECVIEWS, pszName)    },
                                                    { "", CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_CREATOR, offsetof (USRRECVIEWS, pszCreator) }} ;
           PWND                 pwnd ;

           /* Instanzdaten des Client Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Was will Benutzer? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Abbrechen */
              case DID_CANCEL:
                   /* Abbrechen aktivieren */
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will Informationen zu Connection Services */
              case IDM_SCONNECTIONSERVICES:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_SCONNECTIONSERVICES),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_CONNECTIONSERVICES,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinitconnectionservices [0],
                                   sizeof (acnrinitconnectionservices) / sizeof (acnrinitconnectionservices [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will Informationen zu Databases */
              case IDM_SDATABASES:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_SDATABASES),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_DATABASES,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinitdatabases [0],
                                   sizeof (acnrinitdatabases) / sizeof (acnrinitdatabases [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              /* benutzer will Informationen zu Packages */
              case IDM_SPACKAGES:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_SPACKAGES),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_PACKAGES,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinitpackages [0],
                                   sizeof (acnrinitpackages) / sizeof (acnrinitpackages [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will Informationen zu Tables */
              case IDM_STABLES:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_STABLES),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_TABLES,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinittables [0],
                                   sizeof (acnrinittables) / sizeof (acnrinittables [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will Informationen zu TableSpaces */
              case IDM_STABLESPACES:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_STABLESPACES),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_TABLESPACES,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinittablespaces [0],
                                   sizeof (acnrinittablespaces) / sizeof (acnrinittablespaces [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will Informationen zu Views */
              case IDM_SVIEWS:
                   /* MenÅeintrÑge selektieren */
                   WinSendMsg (hwnd,
                               WMP_RESETMENU,
                               MPFROMSHORT (IDM_SVIEWS),
                               MPVOID) ;
                   /* Alle alten Records im Container lîschen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVERECORD,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE)) ;
                   /* Alle alten Spalteninformationen im Container entfernen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_REMOVEDETAILFIELDINFO,
                               MPVOID,
                               MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
                   /* Neue öberschrift des Containers aus den Ressourcen lesen */
                   WinLoadString (hab_g,
                                  NULLHANDLE,
                                  IDS_VIEWS,
                                  sizeof (pwnd->szTitle),
                                  pwnd->szTitle) ;
                   /* Neue Conainer Spalten Informationen aufbauen */
                   TInitContainer (pwnd->hwndContainer,
                                   NULLHANDLE,
                                   &acnrinitviews [0],
                                   sizeof (acnrinitviews) / sizeof (acnrinitviews [0])) ;
                   /* Container Informationsstruktur leeren */
                   memset (&ccinfo,
                           0,
                           sizeof (CNRINFO)) ;
                   /* Container Attribute festlegen */
                   ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
                   /* Neue Container öberschrift */
                   ccinfo.pszCnrTitle = pwnd->szTitle ;
                   /* Neue Informationen dem Container bekanntgeben */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_SETCNRINFO,
                               MPFROMP (&ccinfo),
                               MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
                   /* GewÅnschte Aktion fÅr Thread speichern */
                   pwnd->ulAction = SHORT1FROMMP (mp1) ;
                   /* Thread starten */
                   _beginthread (Thread,
                                 NULL,
                                 131000,
                                 pwnd) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Das Fenster wird aufgebaut */
      case WM_CREATE:
           /* Platz fÅr Instanzdaten allokieren und in den Window Words ablegen */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Verzîgerten Start auslîsen */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Fenster wird gelîscht */
      case WM_DESTROY:
           /* Instanzdaten aus den Window Words lesen und deallokieren */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      /* Client Hintergrund mu· gezeichnet werden */
      case WM_ERASEBACKGROUND:
           /* PM soll selbst malen */
           return (MRESULT) TRUE ;
      /* Das Fenster wird minimiert oder maximiert */
      case WM_MINMAXFRAME:
           /* Neuen Zustand merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Die Fenstergrî·e wird verÑndert */
      case WM_SIZE:
           /* Verzîgert durchfÅhren */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       mp1,
                       mp2) ;
           break ;
      /* Verzîgerter Applikationsstart */
      case WMP_MAININIT:
           {
           PWND pwnd ;
           SWP  swp ;

           /* Instanzdaten des Client Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Fenster Handle des MenÅs merken */
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           /* Grî·e des Clients ermitteln */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* Container im Client anlegen */
           pwnd->hwndContainer = WinCreateWindow (hwndClient_g,
                                                  WC_CONTAINER,
                                                  "",
                                                  CCS_AUTOPOSITION | CCS_MINIICONS | CCS_MINIRECORDCORE | CCS_READONLY | CCS_SINGLESEL | WS_GROUP | WS_VISIBLE,
                                                  0,
                                                  0,
                                                  swp.cx,
                                                  swp.cy,
                                                  hwndFrame_g,
                                                  HWND_TOP,
                                                  IDCO_DB2,
                                                  NULL,
                                                  NULL) ;
           /* StandardmÑ·ig beim Applikationsstart diesen MenÅeintrag auswÑhlen */
           WinPostMsg (hwnd,
                       WM_COMMAND,
                       MPFROMSHORT (IDM_SDATABASES),
                       MPFROM2SHORT (CMDSRC_MENU, TRUE)) ;
           return (MRESULT) FALSE ;
           }
      /* MenÅeintrÑge selektieren / deselktieren */
      case WMP_RESETMENU:
           {
           PWND  pwnd ;
           ULONG aul [] = { IDM_SCONNECTIONSERVICES,
                            IDM_SDATABASES,
                            IDM_SPACKAGES,
                            IDM_STABLES,
                            IDM_STABLESPACES,
                            IDM_SVIEWS } ;
           ULONG ulI ;

           /* Instanzdaten des Client Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* In einer Schleife */
           for  (ulI = 0; ulI < sizeof (aul) / sizeof (aul [0]); ulI++)
                {
                /* Alle MenÅeintrÑge deselektieren */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (aul [ulI], TRUE),
                            MPFROM2SHORT (MIA_CHECKED, FALSE)) ;
                }
           /* Den neuen MenÅeintrag selektieren */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (SHORT1FROMMP (mp1), TRUE),
                       MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED)) ;
           return (MRESULT) FALSE ;
           }
      /* Verzîgerte Grî·enanpassung */
      case WMP_SIZE:
           {
           PWND pwnd ;
           SWP  swp ;

           /* Instanzdaten des Client Windows holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Grî·e des Clients ermitteln */
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                /* Container im Client ausbreiten */
                WinSetWindowPos (pwnd->hwndContainer,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx,
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

/* Applikationsstart */
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
             /* Applikationsklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszDB2SPY_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszDB2SPY_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_DB2SPY,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Loop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          /* Message verteilen */
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Standardfenster wieder lîschen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue wieder lîschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab_g) ;
        }
   /* ZurÅck */
   return 0 ;
   }

/* Hilfsfunktion zum eliminieren von Trailing Charcaters */
BOOL TCutString (PSZ  pszString,
                 CHAR ch)
   {
   PSZ psz ;

   /* Nur wenn String gÅltig */
   if   (*pszString)
        {
        /* Pointer auf String merken */
        psz = pszString ;
        /* In einer Schleife */
        while (*pszString)
           {
           /* An's Ende des Strings */
           pszString++ ;
           }
        /* Ein Position nach vorne */
        pszString-- ;
        /* War String leer? */
        while (pszString != psz)
           {
           /* Nein, Trailing Character? */
           if   (*pszString == ch)
                {
                /* Ja, abschneiden */
                *pszString = '\0' ;
                }
           else
                {
                /* Nein, abbrechen */
                break ;
                }
           /* Einen zurÅck */
           pszString-- ;
           }
        /* Kein Fehler */
        return TNOERROR ;
        }
   else
        {
        /* Fehler */
        return TERROR ;
        }
   }

/* Der Thread */
VOID Thread (PVOID pv)
   {
   BOOL                      f ;
   HAB                       hab ;
   HMQ                       hmq ;
   PUSRRECCONNECTIONSERVICES pusrrecconnectionservices ;
   PUSRRECDATABASES          pusrrecdatabases ;
   PUSRRECPACKAGES           pusrrecpackages ;
   PUSRRECTABLES             pusrrectables ;
   PUSRRECTABLESPACES        pusrrectablespaces ;
   PUSRRECVIEWS              pusrrecviews ;
   PWND                      pwnd ;
   RECORDINSERT              recins ;
   struct sql_dir_entry*     psqldirentry ;
   struct sql_dir_entry*     psqldirentrySave ;
   struct sqledinfo*         psqledinfo ;
   ULONG                     ulI ;
   USHORT                    usCount ;
   USHORT                    usHandle ;

   /* Beim PM anmelden fÅr eigene Error Informationen */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Eigene Message Queue da wir Messages versenden wollen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* WICHTIG! Beim Systemabbruch darf dieser Thread einfach entfernt werden */
             WinCancelShutdown (hmq,
                                TRUE) ;
             /* Instanzdaten des Client Windows holen */
             pwnd = pv ;
             /* DB2/2 Engine starten */
             sqlestar () ;
             /* Welche Aktion soll durchgefÅhrt werden */
             switch (pwnd->ulAction)
                {
                /* Benutzer will Informationen zu den Connection Services */
                case IDM_SCONNECTIONSERVICES:
                     /* Wieviele Daten gibt es zu diesen Informationen */
                     sqlegdsc ((PSHORT) &usCount,
                               &sqlca) ;
                     /* Platz dafÅr allokieren */
                     psqldirentry = calloc (usCount,
                                            sizeof (struct sql_dir_entry)) ;
                     /* Pointer merken */
                     psqldirentrySave = psqldirentry ;
                     /* Informationen einlesen */
                     sqlegdgt ((PSHORT) &usCount,
                               psqldirentry,
                               &sqlca) ;
                     /* In einer Schleife alle EintrÑge abarbeiten */
                     for  (ulI = 0; ulI < usCount; ulI++, psqldirentry++)
                          {
                          /* Eine Record Zeile anfordern */
                          pusrrecconnectionservices = WinSendMsg (pwnd->hwndContainer,
                                                                  CM_ALLOCRECORD,
                                                                  MPFROMLONG (sizeof (USRRECCONNECTIONSERVICES) - sizeof (MINIRECORDCORE)),
                                                                  MPFROMSHORT (1)) ;
                          /* Daten aus Struktur auslesen */
                          strncpy (pusrrecconnectionservices->szComment,
                                   psqldirentry->comment,
                                   SQL_CMT_SZ) ;
                          TCutString (pusrrecconnectionservices->szComment,
                                      ' ') ;
                          strncpy (pusrrecconnectionservices->szName,
                                   psqldirentry->ldb,
                                   SQL_DBNAME_SZ) ;
                          TCutString (pusrrecconnectionservices->szName,
                                      ' ') ;
                          strncpy (pusrrecconnectionservices->szHost,
                                   psqldirentry->tdb,
                                   SQL_LONG_NAME_SZ) ;
                          TCutString (pusrrecconnectionservices->szHost,
                                      ' ') ;
                          /* Pointer in Recordstruktur aufbauen */
                          pusrrecconnectionservices->pszComment = pusrrecconnectionservices->szComment ;
                          pusrrecconnectionservices->pszHost = pusrrecconnectionservices->szHost ;
                          pusrrecconnectionservices->pszName = pusrrecconnectionservices->szName ;
                          /* Struktur zum EinfÅgen von Records leeren */
                          memset (&recins,
                                  0,
                                  sizeof (RECORDINSERT)) ;
                          /* Grî·e der Struktur */
                          recins.cb = sizeof (recins) ;
                          /* Record hinten anhÑngen */
                          recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                          /* Oberste Ebene */
                          recins.zOrder = CMA_TOP ;
                          /* 1 Records werden eingefÅgt */
                          recins.cRecordsInsert = 1 ;
                          /* Record tatsÑchlich einfÅgen */
                          WinSendMsg (pwnd->hwndContainer,
                                      CM_INSERTRECORD,
                                      MPFROMP (pusrrecconnectionservices),
                                      MPFROMP (&recins)) ;
                          }
                     /* Allokierten Platz wieder freigeben */
                     free (psqldirentrySave) ;
                     /* Scan schlie·en */
                     sqlegdcl (&sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                /* Benutzer will Informationen zu den Databases */
                case IDM_SDATABASES:
                     /* Datenbankscan starten */
                     sqledosd (NULL,
                               &usHandle,
                               &usCount,
                               &sqlca) ;
                     /* In einer Schleife durch alle Datenbanken */
                     do
                        {
                        /* NÑchste Datenbank ermitteln */
                        sqledgne (usHandle,
                                  &psqledinfo,
                                  &sqlca) ;
                        /* Nur wenn kein Fehler */
                        if   (sqlca.sqlcode == SQL_RC_OK)
                             {
                             /* Eine Record Zeile anfordern */
                             pusrrecdatabases = WinSendMsg (pwnd->hwndContainer,
                                                            CM_ALLOCRECORD,
                                                            MPFROMLONG (sizeof (USRRECDATABASES) - sizeof (MINIRECORDCORE)),
                                                            MPFROMSHORT (1)) ;
                             /* SchlÅssel Informationen mit sprechenden Ressource Texten versehen */
                             if   (psqledinfo->authentication <= SQL_AUTHENTICATION_DCS)
                                  {
                                  WinLoadString (hab_g,
                                                 NULLHANDLE,
                                                 IDS_SQLEDINFOAUTHENTICATIONLOW + psqledinfo->authentication,
                                                 sizeof (pusrrecdatabases->szAuthentication),
                                                 pusrrecdatabases->szAuthentication) ;
                                  }
                             else
                                  {
                                  WinLoadString (hab_g,
                                                 NULLHANDLE,
                                                 IDS_SQLEDINFOAUTHENTICATIONHIGH,
                                                 sizeof (pusrrecdatabases->szAuthentication),
                                                 pusrrecdatabases->szAuthentication) ;
                                  }
                             /* Daten aus Struktur auslesen */
                             strncpy (pusrrecdatabases->szAlias,
                                      psqledinfo->alias,
                                      SQL_ALIAS_SZ) ;
                             TCutString (pusrrecdatabases->szAlias,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szComment,
                                      psqledinfo->comment,
                                      SQL_CMT_SZ) ;
                             TCutString (pusrrecdatabases->szComment,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szDBType,
                                      psqledinfo->dbtype,
                                      SQL_DBTYP_SZ) ;
                             TCutString (pusrrecdatabases->szDBType,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szDrive,
                                      psqledinfo->drive,
                                      SQL_DRIVE_SZ) ;
                             TCutString (pusrrecdatabases->szDrive,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szDirectory,
                                      psqledinfo->intname,
                                      SQL_INAME_SZ) ;
                             TCutString (pusrrecdatabases->szDirectory,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szName,
                                      psqledinfo->dbname,
                                      SQL_DBNAME_SZ) ;
                             TCutString (pusrrecdatabases->szName,
                                         ' ') ;
                             strncpy (pusrrecdatabases->szNode,
                                      psqledinfo->nodename,
                                      SQL_NNAME_SZ) ;
                             TCutString (pusrrecdatabases->szNode,
                                         ' ') ;
                             /* SchlÅssel Informationen mit sprechenden Ressource Texten versehen */
                             WinLoadString (hab_g,
                                            NULLHANDLE,
                                            IDS_SQLEDINFOTYPELOW + (psqledinfo->type - '0'),
                                            sizeof (pusrrecdatabases->szType),
                                            pusrrecdatabases->szType) ;
                             /* Pointer in Recordstruktur aufbauen */
                             pusrrecdatabases->pszAlias = pusrrecdatabases->szAlias ;
                             pusrrecdatabases->pszAuthentication = pusrrecdatabases->szAuthentication ;
                             pusrrecdatabases->pszComment = pusrrecdatabases->szComment ;
                             pusrrecdatabases->pszDBType = pusrrecdatabases->szDBType ;
                             pusrrecdatabases->pszDrive = pusrrecdatabases->szDrive ;
                             pusrrecdatabases->pszDirectory = pusrrecdatabases->szDirectory ;
                             pusrrecdatabases->pszName = pusrrecdatabases->szName ;
                             pusrrecdatabases->pszNode = pusrrecdatabases->szNode ;
                             pusrrecdatabases->pszType = pusrrecdatabases->szType ;
                             /* Struktur zum EinfÅgen von Records leeren */
                             memset (&recins,
                                     0,
                                     sizeof (RECORDINSERT)) ;
                             /* Grî·e der Struktur */
                             recins.cb = sizeof (recins) ;
                             /* Record hinten anhÑngen */
                             recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                             /* Oberste Ebene */
                             recins.zOrder = CMA_TOP ;
                             /* 1 Records werden eingefÅgt */
                             recins.cRecordsInsert = 1 ;
                             /* Record tatsÑchlich einfÅgen */
                             WinSendMsg (pwnd->hwndContainer,
                                         CM_INSERTRECORD,
                                         MPFROMP (pusrrecdatabases),
                                         MPFROMP (&recins)) ;
                             }
                        } while (sqlca.sqlcode == SQL_RC_OK) ;
                     /* Scan schlie·en */
                     sqledcls (usHandle,
                               &sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                /* Benutzer will Informationen zu den Packages */
                case IDM_SPACKAGES:
                     /* Datenbankscan starten */
                     sqledosd (NULL,
                               &usHandle,
                               &usCount,
                               &sqlca) ;
                     /* In einer Schleife durch alle Datenbanken */
                     do
                        {
                        /* NÑchste Datenbank ermitteln */
                        sqledgne (usHandle,
                                  &psqledinfo,
                                  &sqlca) ;
                        /* Nur wenn kein Fehler */
                        if   (sqlca.sqlcode == SQL_RC_OK)
                             {
                             /* Datenbankname umkopieren */
                             strncpy (SZDBNAME,
                                      psqledinfo->dbname,
                                      SQL_DBNAME_SZ) ;
                             TCutString (SZDBNAME,
                                         ' ') ;
                             /* Diese Datenbank îffnen */
                             
/*
EXEC SQL CONNECT TO :SZDBNAME ;
*/

{
#line 888 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 888 "DB2SPY.SQC"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 888 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 9;
#line 888 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = (void*)SZDBNAME;
#line 888 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 888 "DB2SPY.SQC"
      sqlasetd(2,0,1,sql_setdlist,0L);
    }
#line 888 "DB2SPY.SQC"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 888 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 888 "DB2SPY.SQC"

                             /* Versuchsballon zum Test ob Binden notwendig ist */
                             
/*
EXEC SQL SELECT NAME INTO :SZNAME FROM SYSIBM.SYSTABLES ;
*/

{
#line 890 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 890 "DB2SPY.SQC"
  sqlaaloc(3,1,2,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 890 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 890 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 890 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 890 "DB2SPY.SQC"
      sqlasetd(3,0,1,sql_setdlist,0L);
    }
#line 890 "DB2SPY.SQC"
  sqlacall((unsigned short)24,1,0,3,0L);
#line 890 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 890 "DB2SPY.SQC"

                             /* Binden notwendig? */
                             if   (sqlca.sqlcode == SQL_RC_E805)
                                  {
                                  /* Applikation an Datenbank binden */
                                  sqlabndx ("DB2SPY.BND",
                                            "DB2SPY.OUT",
                                            NULL,
                                            &sqlca) ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_E811)
                                  {
                                  sqlca.sqlcode = SQL_RC_OK ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_OK)
                                  {
                                  /* Einen Cursor fÅr diese Abfrage anlegen */
                                  
/*
EXEC SQL DECLARE cursorpackages CURSOR FOR
                                     SELECT PKGNAME, BOUNDBY FROM SYSCAT.PACKAGES ORDER BY PKGNAME ;
*/

#line 908 "DB2SPY.SQC"

                                  /* Cursor îffnen */
                                  
/*
EXEC SQL OPEN cursorpackages ;
*/

{
#line 910 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 910 "DB2SPY.SQC"
  sqlacall((unsigned short)26,2,0,0,0L);
#line 910 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 910 "DB2SPY.SQC"

                                  /* ZÑhler initialisieren */
                                  f = FALSE ;
                                  /* Solange kein Fehler */
                                  while (sqlca.sqlcode == SQL_RC_OK)
                                     {
                                     /* Eine Ergebniszeile holen */
                                     
/*
EXEC SQL FETCH cursorpackages INTO :SZNAME, :SZCREATOR ;
*/

{
#line 917 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 917 "DB2SPY.SQC"
  sqlaaloc(3,2,3,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 917 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 917 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 917 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 917 "DB2SPY.SQC"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 9;
#line 917 "DB2SPY.SQC"
      sql_setdlist[1].sqldata = SZCREATOR;
#line 917 "DB2SPY.SQC"
      sql_setdlist[1].sqlind = 0L;
#line 917 "DB2SPY.SQC"
      sqlasetd(3,0,2,sql_setdlist,0L);
    }
#line 917 "DB2SPY.SQC"
  sqlacall((unsigned short)25,2,0,3,0L);
#line 917 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 917 "DB2SPY.SQC"

                                     /* Eine Record Zeile anfordern */
                                     pusrrecpackages = WinSendMsg (pwnd->hwndContainer,
                                                                   CM_ALLOCRECORD,
                                                                   MPFROMLONG (sizeof (USRRECPACKAGES) - sizeof (MINIRECORDCORE)),
                                                                   MPFROMSHORT (1)) ;
                                     /* Nur bei erster Zeile pro Datenbank */
                                     if   (!f)
                                          {
                                          /* Datenbankname auch in Zeile aufnehmen */
                                          strcpy (pusrrecpackages->szDBName,
                                                  SZDBNAME) ;
                                          }
                                     /* Hostvariablen in Recordstruktur aufnehmen */
                                     strncpy (pusrrecpackages->szCreator,
                                              SZCREATOR,
                                              8) ;
                                     TCutString (pusrrecpackages->szCreator,
                                                 ' ') ;
                                     strncpy (pusrrecpackages->szName,
                                              SZNAME,
                                              8) ;
                                     TCutString (pusrrecpackages->szName,
                                                 ' ') ;
                                     /* Pointer in Recordstruktur aufbauen */
                                     pusrrecpackages->pszCreator = pusrrecpackages->szCreator ;
                                     pusrrecpackages->pszDBName = pusrrecpackages->szDBName ;
                                     pusrrecpackages->pszName = pusrrecpackages->szName ;
                                     /* Struktur zum EinfÅgen von Records leeren */
                                     memset (&recins,
                                             0,
                                             sizeof (RECORDINSERT)) ;
                                     /* Grî·e der Struktur */
                                     recins.cb = sizeof (recins) ;
                                     /* Record hinten anhÑngen */
                                     recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                     /* Oberste Ebene */
                                     recins.zOrder = CMA_TOP ;
                                     /* 1 Records werden eingefÅgt */
                                     recins.cRecordsInsert = 1 ;
                                     /* Record tatsÑchlich einfÅgen */
                                     WinSendMsg (pwnd->hwndContainer,
                                                 CM_INSERTRECORD,
                                                 MPFROMP (pusrrecpackages),
                                                 MPFROMP (&recins)) ;
                                     /* Merken das bereits eine Zeile verarbeitet wurde */
                                     f = TRUE ;
                                     }
                                  /* Cursor wieder schlie·en */
                                  
/*
EXEC SQL CLOSE cursorpackages ;
*/

{
#line 966 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 966 "DB2SPY.SQC"
  sqlacall((unsigned short)20,2,0,0,0L);
#line 966 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 966 "DB2SPY.SQC"

                                  }
                             /* Datenbank schlie·en */
                             
/*
EXEC SQL CONNECT RESET ;
*/

{
#line 969 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 969 "DB2SPY.SQC"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 969 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 969 "DB2SPY.SQC"

                             }
                        /* Zeile fÅr Zeile lesen bis keine mehr da */
                        } while (sqlca.sqlcode == SQL_RC_OK) ;
                     /* Scan schlie·en */
                     sqledcls (usHandle,
                               &sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                /* Benutzer will Informationen zu den Tables */
                case IDM_STABLES:
                     /* Datenbankscan starten */
                     sqledosd (NULL,
                               &usHandle,
                               &usCount,
                               &sqlca) ;
                     /* In einer Schleife durch alle Datenbanken */
                     do
                        {
                        /* NÑchste Datenbank ermitteln */
                        sqledgne (usHandle,
                                  &psqledinfo,
                                  &sqlca) ;
                        /* Nur wenn kein Fehler */
                        if   (sqlca.sqlcode == SQL_RC_OK)
                             {
                             /* Datenbankname umkopieren */
                             strncpy (SZDBNAME,
                                      psqledinfo->dbname,
                                      SQL_DBNAME_SZ) ;
                             TCutString (SZDBNAME,
                                         ' ') ;
                             /* Diese Datenbank îffnen */
                             
/*
EXEC SQL CONNECT TO :SZDBNAME ;
*/

{
#line 1006 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1006 "DB2SPY.SQC"
  sqlaaloc(2,1,4,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1006 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 9;
#line 1006 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = (void*)SZDBNAME;
#line 1006 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1006 "DB2SPY.SQC"
      sqlasetd(2,0,1,sql_setdlist,0L);
    }
#line 1006 "DB2SPY.SQC"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 1006 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1006 "DB2SPY.SQC"

                             /* Versuchsballon zum Test ob Binden notwendig ist */
                             
/*
EXEC SQL SELECT NAME INTO :SZNAME FROM SYSIBM.SYSTABLES ;
*/

{
#line 1008 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1008 "DB2SPY.SQC"
  sqlaaloc(3,1,5,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1008 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1008 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1008 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1008 "DB2SPY.SQC"
      sqlasetd(3,0,1,sql_setdlist,0L);
    }
#line 1008 "DB2SPY.SQC"
  sqlacall((unsigned short)24,3,0,3,0L);
#line 1008 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1008 "DB2SPY.SQC"

                             /* Binden notwendig? */
                             if   (sqlca.sqlcode == SQL_RC_E805)
                                  {
                                  /* Applikation an Datenbank binden */
                                  sqlabndx ("DB2SPY.BND",
                                            "DB2SPY.OUT",
                                            NULL,
                                            &sqlca) ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_E811)
                                  {
                                  sqlca.sqlcode = SQL_RC_OK ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_OK)
                                  {
                                  /* Einen Cursor fÅr diese Abfrage anlegen */
                                  
/*
EXEC SQL DECLARE cursortables CURSOR FOR
                                     SELECT TABNAME, DEFINER FROM SYSCAT.TABLES WHERE TYPE = 'T' ORDER BY TABNAME ;
*/

#line 1026 "DB2SPY.SQC"

                                  /* Cursor îffnen */
                                  
/*
EXEC SQL OPEN cursortables ;
*/

{
#line 1028 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1028 "DB2SPY.SQC"
  sqlacall((unsigned short)26,4,0,0,0L);
#line 1028 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1028 "DB2SPY.SQC"

                                  /* ZÑhler initialisieren */
                                  f = FALSE ;
                                  /* Solange kein Fehler */
                                  while (sqlca.sqlcode == SQL_RC_OK)
                                     {
                                     /* Eine Ergebniszeile holen */
                                     
/*
EXEC SQL FETCH cursortables INTO :SZNAME, :SZCREATOR ;
*/

{
#line 1035 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1035 "DB2SPY.SQC"
  sqlaaloc(3,2,6,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 1035 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1035 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1035 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1035 "DB2SPY.SQC"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 9;
#line 1035 "DB2SPY.SQC"
      sql_setdlist[1].sqldata = SZCREATOR;
#line 1035 "DB2SPY.SQC"
      sql_setdlist[1].sqlind = 0L;
#line 1035 "DB2SPY.SQC"
      sqlasetd(3,0,2,sql_setdlist,0L);
    }
#line 1035 "DB2SPY.SQC"
  sqlacall((unsigned short)25,4,0,3,0L);
#line 1035 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1035 "DB2SPY.SQC"

                                     /* Eine Record Zeile anfordern */
                                     pusrrectables = WinSendMsg (pwnd->hwndContainer,
                                                                 CM_ALLOCRECORD,
                                                                 MPFROMLONG (sizeof (USRRECTABLES) - sizeof (MINIRECORDCORE)),
                                                                 MPFROMSHORT (1)) ;
                                     /* Nur bei erster Zeile pro Datenbank */
                                     if   (!f)
                                          {
                                          /* Datenbankname auch in Zeile aufnehmen */
                                          strcpy (pusrrectables->szDBName,
                                                  SZDBNAME) ;
                                          }
                                     /* Hostvariablen in Recordstruktur aufnehmen */
                                     strncpy (pusrrectables->szCreator,
                                              SZCREATOR,
                                              8) ;
                                     TCutString (pusrrectables->szCreator,
                                                 ' ') ;
                                     strncpy (pusrrectables->szName,
                                              SZNAME,
                                              18) ;
                                     TCutString (pusrrectables->szName,
                                                 ' ') ;
                                     /* Pointer in Recordstruktur aufbauen */
                                     pusrrectables->pszCreator = pusrrectables->szCreator ;
                                     pusrrectables->pszDBName = pusrrectables->szDBName ;
                                     pusrrectables->pszName = pusrrectables->szName ;
                                     /* Struktur zum EinfÅgen von Records leeren */
                                     memset (&recins,
                                             0,
                                             sizeof (RECORDINSERT)) ;
                                     /* Grî·e der Struktur */
                                     recins.cb = sizeof (recins) ;
                                     /* Record hinten anhÑngen */
                                     recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                     /* Oberste Ebene */
                                     recins.zOrder = CMA_TOP ;
                                     /* 1 Records werden eingefÅgt */
                                     recins.cRecordsInsert = 1 ;
                                     /* Record tatsÑchlich einfÅgen */
                                     WinSendMsg (pwnd->hwndContainer,
                                                 CM_INSERTRECORD,
                                                 MPFROMP (pusrrectables),
                                                 MPFROMP (&recins)) ;
                                     /* Merken das bereits eine Zeile verarbeitet wurde */
                                     f = TRUE ;
                                     }
                                  /* Cursor wieder schlie·en */
                                  
/*
EXEC SQL CLOSE cursortables ;
*/

{
#line 1084 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1084 "DB2SPY.SQC"
  sqlacall((unsigned short)20,4,0,0,0L);
#line 1084 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1084 "DB2SPY.SQC"

                                  }
                             /* Datenbank schlie·en */
                             
/*
EXEC SQL CONNECT RESET ;
*/

{
#line 1087 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1087 "DB2SPY.SQC"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 1087 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1087 "DB2SPY.SQC"

                             }
                        /* Zeile fÅr Zeile lesen bis keine mehr da */
                        } while (sqlca.sqlcode == SQL_RC_OK) ;
                     /* Scan schlie·en */
                     sqledcls (usHandle,
                               &sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                /* Benutzer will Informationen zu den TableSpaces */
                case IDM_STABLESPACES:
                     /* Datenbankscan starten */
                     sqledosd (NULL,
                               &usHandle,
                               &usCount,
                               &sqlca) ;
                     /* In einer Schleife durch alle Datenbanken */
                     do
                        {
                        /* NÑchste Datenbank ermitteln */
                        sqledgne (usHandle,
                                  &psqledinfo,
                                  &sqlca) ;
                        /* Nur wenn kein Fehler */
                        if   (sqlca.sqlcode == SQL_RC_OK)
                             {
                             /* Datenbankname umkopieren */
                             strncpy (SZDBNAME,
                                      psqledinfo->dbname,
                                      SQL_DBNAME_SZ) ;
                             TCutString (SZDBNAME,
                                         ' ') ;
                             /* Diese Datenbank îffnen */
                             
/*
EXEC SQL CONNECT TO :SZDBNAME ;
*/

{
#line 1124 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1124 "DB2SPY.SQC"
  sqlaaloc(2,1,7,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1124 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 9;
#line 1124 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = (void*)SZDBNAME;
#line 1124 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1124 "DB2SPY.SQC"
      sqlasetd(2,0,1,sql_setdlist,0L);
    }
#line 1124 "DB2SPY.SQC"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 1124 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1124 "DB2SPY.SQC"

                             /* Versuchsballon zum Test ob Binden notwendig ist */
                             
/*
EXEC SQL SELECT NAME INTO :SZNAME FROM SYSIBM.SYSTABLES ;
*/

{
#line 1126 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1126 "DB2SPY.SQC"
  sqlaaloc(3,1,8,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1126 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1126 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1126 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1126 "DB2SPY.SQC"
      sqlasetd(3,0,1,sql_setdlist,0L);
    }
#line 1126 "DB2SPY.SQC"
  sqlacall((unsigned short)24,5,0,3,0L);
#line 1126 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1126 "DB2SPY.SQC"

                             /* Binden notwendig? */
                             if   (sqlca.sqlcode == SQL_RC_E805)
                                  {
                                  /* Applikation an Datenbank binden */
                                  sqlabndx ("DB2SPY.BND",
                                            "DB2SPY.OUT",
                                            NULL,
                                            &sqlca) ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_E811)
                                  {
                                  sqlca.sqlcode = SQL_RC_OK ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_OK)
                                  {
                                  /* Einen Cursor fÅr diese Abfrage anlegen */
                                  
/*
EXEC SQL DECLARE cursortablespaces CURSOR FOR
                                     SELECT TBSPACE FROM SYSCAT.TABLESPACES ORDER BY TBSPACE ;
*/

#line 1144 "DB2SPY.SQC"

                                  /* Cursor îffnen */
                                  
/*
EXEC SQL OPEN cursortablespaces ;
*/

{
#line 1146 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1146 "DB2SPY.SQC"
  sqlacall((unsigned short)26,6,0,0,0L);
#line 1146 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1146 "DB2SPY.SQC"

                                  /* ZÑhler initialisieren */
                                  f = FALSE ;
                                  /* Solange kein Fehler */
                                  while (sqlca.sqlcode == SQL_RC_OK)
                                     {
                                     /* Eine Ergebniszeile holen */
                                     
/*
EXEC SQL FETCH cursortablespaces INTO :SZNAME ;
*/

{
#line 1153 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1153 "DB2SPY.SQC"
  sqlaaloc(3,1,9,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1153 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1153 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1153 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1153 "DB2SPY.SQC"
      sqlasetd(3,0,1,sql_setdlist,0L);
    }
#line 1153 "DB2SPY.SQC"
  sqlacall((unsigned short)25,6,0,3,0L);
#line 1153 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1153 "DB2SPY.SQC"

                                     /* Eine Record Zeile anfordern */
                                     pusrrectablespaces = WinSendMsg (pwnd->hwndContainer,
                                                                      CM_ALLOCRECORD,
                                                                      MPFROMLONG (sizeof (USRRECTABLESPACES) - sizeof (MINIRECORDCORE)),
                                                                      MPFROMSHORT (1)) ;
                                     /* Nur bei erster Zeile pro Datenbank */
                                     if   (!f)
                                          {
                                          /* Datenbankname auch in Zeile aufnehmen */
                                          strcpy (pusrrectablespaces->szDBName,
                                                  SZDBNAME) ;
                                          }
                                     /* Hostvariablen in Recordstruktur aufnehmen */
                                     strncpy (pusrrectablespaces->szName,
                                              SZNAME,
                                              18) ;
                                     TCutString (pusrrectablespaces->szName,
                                                 ' ') ;
                                     /* Pointer in Recordstruktur aufbauen */
                                     pusrrectablespaces->pszDBName = pusrrectablespaces->szDBName ;
                                     pusrrectablespaces->pszName = pusrrectablespaces->szName ;
                                     /* Struktur zum EinfÅgen von Records leeren */
                                     memset (&recins,
                                             0,
                                             sizeof (RECORDINSERT)) ;
                                     /* Grî·e der Struktur */
                                     recins.cb = sizeof (recins) ;
                                     /* Record hinten anhÑngen */
                                     recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                     /* Oberste Ebene */
                                     recins.zOrder = CMA_TOP ;
                                     /* 1 Records werden eingefÅgt */
                                     recins.cRecordsInsert = 1 ;
                                     /* Record tatsÑchlich einfÅgen */
                                     WinSendMsg (pwnd->hwndContainer,
                                                 CM_INSERTRECORD,
                                                 MPFROMP (pusrrectablespaces),
                                                 MPFROMP (&recins)) ;
                                     /* Merken das bereits eine Zeile verarbeitet wurde */
                                     f = TRUE ;
                                     }
                                  /* Cursor wieder schlie·en */
                                  
/*
EXEC SQL CLOSE cursortablespaces ;
*/

{
#line 1196 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1196 "DB2SPY.SQC"
  sqlacall((unsigned short)20,6,0,0,0L);
#line 1196 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1196 "DB2SPY.SQC"

                                  }
                             /* Datenbank schlie·en */
                             
/*
EXEC SQL CONNECT RESET ;
*/

{
#line 1199 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1199 "DB2SPY.SQC"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 1199 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1199 "DB2SPY.SQC"

                             }
                        /* Zeile fÅr Zeile lesen bis keine mehr da */
                        } while (sqlca.sqlcode == SQL_RC_OK) ;
                     /* Scan schlie·en */
                     sqledcls (usHandle,
                               &sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                /* Benutzer will Informationen zu den Views */
                case IDM_SVIEWS:
                     /* Datenbankscan starten */
                     sqledosd (NULL,
                               &usHandle,
                               &usCount,
                               &sqlca) ;
                     /* In einer Schleife durch alle Datenbanken */
                     do
                        {
                        /* NÑchste Datenbank ermitteln */
                        sqledgne (usHandle,
                                  &psqledinfo,
                                  &sqlca) ;
                        /* Nur wenn kein Fehler */
                        if   (sqlca.sqlcode == SQL_RC_OK)
                             {
                             /* Datenbankname umkopieren */
                             strncpy (SZDBNAME,
                                      psqledinfo->dbname,
                                      SQL_DBNAME_SZ) ;
                             TCutString (SZDBNAME,
                                         ' ') ;
                             /* Diese Datenbank îffnen */
                             
/*
EXEC SQL CONNECT TO :SZDBNAME ;
*/

{
#line 1236 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1236 "DB2SPY.SQC"
  sqlaaloc(2,1,10,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1236 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 9;
#line 1236 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = (void*)SZDBNAME;
#line 1236 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1236 "DB2SPY.SQC"
      sqlasetd(2,0,1,sql_setdlist,0L);
    }
#line 1236 "DB2SPY.SQC"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 1236 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1236 "DB2SPY.SQC"

                             /* Versuchsballon zum Test ob Binden notwendig ist */
                             
/*
EXEC SQL SELECT NAME INTO :SZNAME FROM SYSIBM.SYSTABLES ;
*/

{
#line 1238 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1238 "DB2SPY.SQC"
  sqlaaloc(3,1,11,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 1238 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1238 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1238 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1238 "DB2SPY.SQC"
      sqlasetd(3,0,1,sql_setdlist,0L);
    }
#line 1238 "DB2SPY.SQC"
  sqlacall((unsigned short)24,7,0,3,0L);
#line 1238 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1238 "DB2SPY.SQC"

                             /* Binden notwendig? */
                             if   (sqlca.sqlcode == SQL_RC_E805)
                                  {
                                  /* Applikation an Datenbank binden */
                                  sqlabndx ("DB2SPY.BND",
                                            "DB2SPY.OUT",
                                            NULL,
                                            &sqlca) ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_E811)
                                  {
                                  sqlca.sqlcode = SQL_RC_OK ;
                                  }
                             if   (sqlca.sqlcode == SQL_RC_OK)
                                  {
                                  /* Einen Cursor fÅr diese Abfrage anlegen */
                                  
/*
EXEC SQL DECLARE cursorviews CURSOR FOR
                                     SELECT VIEWNAME, DEFINER FROM SYSCAT.VIEWS ORDER BY VIEWNAME ;
*/

#line 1256 "DB2SPY.SQC"

                                  /* Cursor îffnen */
                                  
/*
EXEC SQL OPEN cursorviews ;
*/

{
#line 1258 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1258 "DB2SPY.SQC"
  sqlacall((unsigned short)26,8,0,0,0L);
#line 1258 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1258 "DB2SPY.SQC"

                                  /* ZÑhler initialisieren */
                                  f = FALSE ;
                                  /* Solange kein Fehler */
                                  while (sqlca.sqlcode == SQL_RC_OK)
                                     {
                                     /* Eine Ergebniszeile holen */
                                     
/*
EXEC SQL FETCH cursorviews INTO :SZNAME, :SZCREATOR ;
*/

{
#line 1265 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1265 "DB2SPY.SQC"
  sqlaaloc(3,2,12,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 1265 "DB2SPY.SQC"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 19;
#line 1265 "DB2SPY.SQC"
      sql_setdlist[0].sqldata = SZNAME;
#line 1265 "DB2SPY.SQC"
      sql_setdlist[0].sqlind = 0L;
#line 1265 "DB2SPY.SQC"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 9;
#line 1265 "DB2SPY.SQC"
      sql_setdlist[1].sqldata = SZCREATOR;
#line 1265 "DB2SPY.SQC"
      sql_setdlist[1].sqlind = 0L;
#line 1265 "DB2SPY.SQC"
      sqlasetd(3,0,2,sql_setdlist,0L);
    }
#line 1265 "DB2SPY.SQC"
  sqlacall((unsigned short)25,8,0,3,0L);
#line 1265 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1265 "DB2SPY.SQC"

                                     /* Eine Record Zeile anfordern */
                                     pusrrecviews = WinSendMsg (pwnd->hwndContainer,
                                                                CM_ALLOCRECORD,
                                                                MPFROMLONG (sizeof (USRRECVIEWS) - sizeof (MINIRECORDCORE)),
                                                                MPFROMSHORT (1)) ;
                                     /* Nur bei erster Zeile pro Datenbank */
                                     if   (!f)
                                          {
                                          /* Datenbankname auch in Zeile aufnehmen */
                                          strcpy (pusrrecviews->szDBName,
                                                  SZDBNAME) ;
                                          }
                                     /* Hostvariablen in Recordstruktur aufnehmen */
                                     strncpy (pusrrecviews->szCreator,
                                              SZCREATOR,
                                              8) ;
                                     TCutString (pusrrecviews->szCreator,
                                                 ' ') ;
                                     strncpy (pusrrecviews->szName,
                                              SZNAME,
                                              18) ;
                                     TCutString (pusrrecviews->szName,
                                                 ' ') ;
                                     /* Pointer in Recordstruktur aufbauen */
                                     pusrrecviews->pszCreator = pusrrecviews->szCreator ;
                                     pusrrecviews->pszDBName = pusrrecviews->szDBName ;
                                     pusrrecviews->pszName = pusrrecviews->szName ;
                                     /* Struktur zum EinfÅgen von Records leeren */
                                     memset (&recins,
                                             0,
                                             sizeof (RECORDINSERT)) ;
                                     /* Grî·e der Struktur */
                                     recins.cb = sizeof (recins) ;
                                     /* Record hinten anhÑngen */
                                     recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                     /* Oberste Ebene */
                                     recins.zOrder = CMA_TOP ;
                                     /* 1 Records werden eingefÅgt */
                                     recins.cRecordsInsert = 1 ;
                                     /* Record tatsÑchlich einfÅgen */
                                     WinSendMsg (pwnd->hwndContainer,
                                                 CM_INSERTRECORD,
                                                 MPFROMP (pusrrecviews),
                                                 MPFROMP (&recins)) ;
                                     /* Merken das bereits eine Zeile verarbeitet wurde */
                                     f = TRUE ;
                                     }
                                  /* Cursor wieder schlie·en */
                                  
/*
EXEC SQL CLOSE cursorviews ;
*/

{
#line 1314 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1314 "DB2SPY.SQC"
  sqlacall((unsigned short)20,8,0,0,0L);
#line 1314 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1314 "DB2SPY.SQC"

                                  }
                             /* Datenbank schlie·en */
                             
/*
EXEC SQL CONNECT RESET ;
*/

{
#line 1317 "DB2SPY.SQC"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 1317 "DB2SPY.SQC"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 1317 "DB2SPY.SQC"
  sqlastop(0L);
}

#line 1317 "DB2SPY.SQC"

                             }
                        /* Zeile fÅr Zeile lesen bis keine mehr da */
                        } while (sqlca.sqlcode == SQL_RC_OK) ;
                     /* Scan schlie·en */
                     sqledcls (usHandle,
                               &sqlca) ;
                     /* Container invalidieren (zeichnen) */
                     WinSendMsg (pwnd->hwndContainer,
                                 CM_INVALIDATERECORD,
                                 MPVOID,
                                 MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
                     break ;
                }
             /* Message Queue wieder lîschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab) ;
        }
   /* Thread beenden */
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


