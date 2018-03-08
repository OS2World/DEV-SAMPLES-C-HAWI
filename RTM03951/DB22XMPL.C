/* $Header: D:\projekte\gib mir die daten.1\source\RCS\DB22XMPL.C 1.1 1995/05/25 15:53:36 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DB22.H"
#include "DB22XMPL.H"

HAB   hab_g = NULLHANDLE ;
HWND  hwndClient_g = NULLHANDLE ;
HWND  hwndDatabase_g = NULLHANDLE ;
HWND  hwndFrame_g = NULLHANDLE ;
ULONG ulWait_g = 0 ;

/* Die Fensterprozedur des Clients */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   CHAR    szText [256] ;
   CHAR    szTitel [128] ;
   CNRINFO ccinfo ;
   HWND    hwndTemp ;
   PDBINFO pdbinfoSend ;
   PUSRREC pusrrec ;
   PWND    pwnd ;
   SWP     swp ;
   
   /* Instanzdaten des Client Fenster holen */
   pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,
                                             QW_PARENT),
                             QWL_USER) ;
   switch (msg)
      {
      /* Anwendung wird verlassen */
      case WM_CLOSE:
           /* An Datenbank Objektfenster einen QUIT schicken */
           WinPostMsg (hwndDatabase_g,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           /* Warten bis DB Engine Thread die Datenbank geschlossen hat und beendet wurde */
           DosWaitThread (&pwnd->ulThread,
                          DCWW_WAIT) ;
           /* QUIT an eigene Anwendung schicken */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              /* Datenbank anlegen gedrckt */
              case IDM_DANLEGEN:
                   /* Datenbank Anlegen aufrufen (in unserem Fall auf Laufwerk C) */
                   CreateDatabase ('C') ;
                   /* Wait Pointer anschalten */
                   WinSendMsg (hwnd,
                               WMP_WAIT,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Datenbank l”schen gedrckt */
              case IDM_DLOESCHEN:
                   /* Datenbank L”schen aufrufen */
                   DropDatabase () ;
                   /* Meneintr„ge anpassen */
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_DANLEGEN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_DLOESCHEN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_SEINFUEGEN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_SAENDERN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   WinSendMsg (pwnd->hwndMenu,
                               MM_SETITEMATTR,
                               MPFROM2SHORT (IDM_SLOESCHEN, TRUE),
                               MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                   return (MRESULT) FALSE ;
              /* Satz einfgen gedrckt */
              case IDM_SEINFUEGEN:
                   /* Dialog 'Satz einfgen' aufrufen */
                   if   (WinDlgBox (HWND_DESKTOP,
                                    hwndFrame_g,
                                    SEDlgProc,
                                    NULLHANDLE,
                                    IDD_SEINFUEGEN,
                                    NULL) == DID_OK)
                        {
                        /* Wenn Ok gedrckt wurde Daten neu lesen */
                        WinPostMsg (hwnd,
                                    WMP_REREAD,
                                    MPVOID,
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              /* Satz „ndern gedrckt */
              case IDM_SAENDERN:
                   /* Selektierten Record im Container ermitteln */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMP ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Dialog 'Satz „ndern' laden */
                        hwndTemp = WinLoadDlg (HWND_DESKTOP,
                                               hwndFrame_g,
                                               SADlgProc,
                                               NULLHANDLE,
                                               IDD_SAENDERN,
                                               NULL) ;
                        /* Schlssel des Records hinterherschicken */
                        WinPostMsg (hwndTemp,
                                    WMP_MAININIT,
                                    MPFROMLONG (pusrrec->ulKey),
                                    MPVOID) ;
                        /* Dialogverarbeitung aufrufen */
                        if   (WinProcessDlg (hwndTemp) == DID_OK)
                             {
                             /* Wenn Ok gedrckt wurde Daten neu lesen */
                             WinPostMsg (hwnd,
                                         WMP_REREAD,
                                         MPVOID,
                                         MPVOID) ;
                             }
                        WinDestroyWindow (hwndTemp) ;
                        }
                   return (MRESULT) FALSE ;
              /* Satz „ndern gedrckt */
              case IDM_SLOESCHEN:
                   /* Selektierten Record im Container ermitteln */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMP ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Wait Pointer anschalten */
                        WinSendMsg (hwnd,
                                    WMP_WAIT,
                                    MPFROMLONG (TRUE),
                                    MPVOID) ;
                        /* Kommunikationsstruktur allokieren, fllen und mit Schlssel abschicken */
                        pdbinfoSend = AllocInfo () ;
                        pdbinfoSend->hwnd = hwnd ;
                        pdbinfoSend->ulEbene = DBK_ADR ;
                        pdbinfoSend->dbadr.ulKey = pusrrec->ulKey ;
                        WinPostMsg (hwndDatabase_g,
                                    WMPDB_DELETE,
                                    MPFROMLONG (DBK_DELETEPART),
                                    MPFROMP (pdbinfoSend)) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Maus ist ber Kontrollelement */
      case WM_CONTROLPOINTER:
           if   (ulWait_g)
                {
                /* Wenn Wait Pointer angeschaltet, dann entsprechenden Pointer ausgeben */
                return (MRESULT) pwnd->hptrWait ;
                }
           break ;
      /* Clientfenster wird angelegt */
      case WM_CREATE:
           /* Instanzdaten fr Clientfenster allokieren und in Windowwords ablegen */
           pwnd = calloc (1,
                          sizeof (WND)) ;
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            pwnd) ;
           /* Eigentlichen Start aufrufen damit PM Zeit hat alles anzulegen */
           WinPostMsg (hwnd,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      case WM_DESTROY:
           /* Instanzdaten des Client Fensters zu l”schen */
           free (pwnd) ;
           break ;
      case WM_MINMAXFRAME:
           /* Speichern ob Fenster minimiert ist oder nicht */
           pwnd->fMinimized = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      case WM_PAINT:
           {
           HPS   hps ;
           RECTL rcl ;

           /* Client Hintergrund zeichnen */
           hps = WinBeginPaint (hwnd,
                                NULLHANDLE,
                                &rcl) ;
           WinFillRect (hps,
                        &rcl,
                        SYSCLR_FIELDBACKGROUND) ;
           WinEndPaint (hps) ;
           return (MRESULT) FALSE ;
           }
      /* Fenstergr”áe wurde ver„ndert */
      case WM_SIZE:
           /* Eigene Elemente positionieren lassen, aber dem PM Zeit geben, damit er vorher alles eigene zeichnen kann */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Eigentlicher Start */
      case WMP_MAININIT:
           {
           /* Struktur mit Container Column Definitionen */
           static CONTNRINIT acoin [] = {{{ "" }, CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_NAME,    offsetof (USRREC, pszName)    },
                                         {{ "" }, CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_VORNAME, offsetof (USRREC, pszVorname) },
                                         {{ "" }, CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR, CFA_CENTER, IDS_TELEFON, offsetof (USRREC, pszTelefon) }} ;
           FIELDINFOINSERT   fiins ;
           PFIELDINFO        pfldinfo ;
           PFIELDINFO        pfldinfoFirst ;
           ULONG             ulI ;

           /* Menuhandle ermitteln */
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           /* Arrow Pointer Handle ermitteln */
           pwnd->hptrArrow = WinQuerySysPointer (HWND_DESKTOP,
                                                 SPTR_ARROW,
                                                 FALSE) ;
           /* Wait Pointer Handle ermitteln */
           pwnd->hptrWait = WinQuerySysPointer (HWND_DESKTOP,
                                                SPTR_WAIT,
                                                FALSE) ;
           /* Clientgr”áe ermitteln */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* Container reinsetzen */
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
                                                  IDCO_CONTAINER,
                                                  NULL,
                                                  NULL) ;
           /* Spaltendefinitionen des Containers allokieren */
           pfldinfo = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                               CM_ALLOCDETAILFIELDINFO,
                                               MPFROMLONG (sizeof (acoin) / sizeof (acoin [0])),
                                               MPVOID)) ;
           pfldinfoFirst = pfldinfo ;
           /* In einer Schleife die Containerspalten beschreiben */
           for  (ulI = 0; ulI < (sizeof (acoin) / sizeof (acoin [0])); ulI++)
                {
                pfldinfo->flData = acoin[ulI].ulFlData ;
                pfldinfo->flTitle = acoin[ulI].ulFlTitle ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               acoin[ulI].ulIdTitle,
                               sizeof (acoin[ulI].szTitle),
                               acoin[ulI].szTitle) ;
                pfldinfo->pTitleData = acoin[ulI].szTitle ;
                pfldinfo->offStruct = acoin[ulI].ulOffStruct ;
                pfldinfo = pfldinfo->pNextFieldInfo ;
                }
           /* Containerspalten einfgen */
           memset (&fiins,
                   0,
                   sizeof (FIELDINFOINSERT)) ;
           fiins.cb = sizeof (FIELDINFOINSERT) ;
           fiins.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST ;
           fiins.cFieldInfoInsert = sizeof (acoin) / sizeof (acoin [0]) ;
           fiins.fInvalidateFieldInfo = TRUE ;
           WinSendMsg (pwnd->hwndContainer,
                       CM_INSERTDETAILFIELDINFO,
                       MPFROMP (pfldinfoFirst),
                       MPFROMP (&fiins)) ;
           /* Containerberschrift lesen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_TITEL,
                          sizeof (pwnd->szTitel),
                          pwnd->szTitel) ;
           /* Einige globale Einstellungen zum Container vornehmen */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
           ccinfo.pszCnrTitle = pwnd->szTitel ;
           WinSendMsg (pwnd->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           /* Menueintr„ge anpassen */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DANLEGEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DLOESCHEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_SEINFUEGEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_SAENDERN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_SLOESCHEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           /* Wait Pointer anschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           /* DB Engine starten */
           pwnd->ulThread = Register (hwnd) ;
           return (MRESULT) FALSE ;
           }
      /* Container neu aufbauen */
      case WMP_REREAD:
           /* Alle S„tze l”schen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_REMOVERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_FREE)) ;
           /* Wait Pointer anschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           /* Alle Records lesen */
           pdbinfoSend = AllocInfo () ;
           pdbinfoSend->hwnd = hwnd ;
           pdbinfoSend->ulEbene = DBK_ADR ;
           WinPostMsg (hwndDatabase_g,
                       WMPDB_REQUEST,
                       MPFROMLONG (DBK_REQUESTALL),
                       MPFROMP (pdbinfoSend)) ;
           return (MRESULT) FALSE ;
      /* Eigene Fenstergr”áen Verarbeitung */
      case WMP_SIZE:
           /* Nur wenn Fenster nicht minimiert */
           if   (!pwnd->fMinimized)
                {
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                WinSetWindowPos (pwnd->hwndContainer,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE) ;
                }
           return (MRESULT) FALSE ;
      /* Wait Pointer Verarbeitung */
      case WMP_WAIT:
           /* Wenn Wait Eventz„hler bereits auf 0 nicht weiter runterz„hlen <g> */
           if   (!LONGFROMMP (mp1) && !ulWait_g)
                {
                return (MRESULT) FALSE ;
                }
           /* Wait Eventz„hler rauf oder runter z„hlen je nach Anforderung */
           (LONGFROMMP (mp1)) ? ulWait_g++ : ulWait_g-- ;
           if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                 (!LONGFROMMP (mp1) && !ulWait_g))
                {
                WinSetPointer (HWND_DESKTOP,
                               (LONGFROMMP (mp1)) ? pwnd->hptrWait : pwnd->hptrArrow) ;
                WinEnableWindow (hwndFrame_g,
                                 !(LONGFROMMP (mp1))) ;
                }
           return (MRESULT) FALSE ;
      /* Datenbank meldet Ergebnis des Anlegens der Datenbank */
      case WMPDB_CREATE:
           /* Wait Pointer abschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (FALSE),
                       MPVOID) ;
           if   (LONGFROMMP (mp1) == DBC_NOERROR)
                {
                /* Wenn kein Fehler Menueintr„ge anpassen */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_DANLEGEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_DLOESCHEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_SEINFUEGEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                }
           else
                {
                /* Wenn Fehler dann Fehlermeldung ausgeben */
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_CREATEERROR,
                               sizeof (szTitel),
                               szTitel) ;
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_CREATEERRORT,
                               sizeof (szText),
                               szText) ;
                WinMessageBox (HWND_DESKTOP,
                               hwnd,
                               szText,
                               szTitel,
                               0,
                               MB_OK | MB_ERROR | MB_APPLMODAL) ;
                }
           return (MRESULT) FALSE ;
      /* Datenbank meldet Ergebnis des ™ffnens der Datenbank */
      case WMPDB_OPEN:
           /* Wait Pointer abschalten */
           WinSendMsg (hwnd,
                       WMP_WAIT,
                       MPFROMLONG (FALSE),
                       MPVOID) ;
           if   (LONGFROMMP (mp1) == DBC_NOERROR)
                {
                /* Wenn kein Fehler dann Menueintr„ge anpassen */
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_DLOESCHEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_SEINFUEGEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                /* Containerinhalt neu lesen lassen */
                WinPostMsg (hwnd,
                            WMP_REREAD,
                            MPVOID,
                            MPVOID) ;
                }
           else
                {
                WinSendMsg (pwnd->hwndMenu,
                            MM_SETITEMATTR,
                            MPFROM2SHORT (IDM_DANLEGEN, TRUE),
                            MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                }
           /* Fenster Handle der DB Engine merken */
           hwndDatabase_g = HWNDFROMMP (mp2) ;
           return (MRESULT) FALSE ;
      /* Ergebnis von Datenbank kommt rein */
      case WMPDB_SEND:
           {
           PDBINFO      pdbinfoReceive ;
           RECORDINSERT recins ;

           pdbinfoReceive = PVOIDFROMMP (mp2) ;
           /* Was war es? */
           switch (LONGFROMMP (mp1))
              {
              /* L”schen */
              case DBK_DELETEPART:
                   /* Welche Tabelle */
                   switch (pdbinfoReceive->ulEbene)
                      {
                      /* Adresse */
                      case DBK_ADR:
                           /* Wait Pointer abschalten */
                           WinSendMsg (hwnd,
                                       WMP_WAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           if   (pdbinfoReceive->ulRC == DBC_NOERROR)
                                {
                                /* Wenn kein fehler Containerinhalt neu lesen lassen */
                                WinPostMsg (hwnd,
                                            WMP_REREAD,
                                            MPVOID,
                                            MPVOID) ;
                                }
                           else
                                {
                                /* Wenn Fehler dann Fehlermeldung */
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_DELETEERROR,
                                               sizeof (szTitel),
                                               szTitel) ;
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_DELETEERRORT,
                                               sizeof (szText),
                                               szText) ;
                                WinMessageBox (HWND_DESKTOP,
                                               hwnd,
                                               szText,
                                               szTitel,
                                               0,
                                               MB_OK | MB_ERROR | MB_APPLMODAL) ;
                                }
                           break ;
                      }
                   break ;
              /* Lesen */
              case DBK_REQUESTALL:
                   /* Welche Tabelle */
                   switch (pdbinfoReceive->ulEbene)
                      {
                      /* Adresse */
                      case DBK_ADR:
                           switch (pdbinfoReceive->ulRC)
                              {
                              /* Alle S„tze wurden gelesen oder es trat ein Fehler auf */
                              case DBC_ERROR:
                              case DBC_END:
                              case DBC_NOTFOUND:
                                   /* Wait Pointer abschalten */
                                   WinSendMsg (hwnd,
                                               WMP_WAIT,
                                               MPFROMLONG (FALSE),
                                               MPVOID) ;
                                   /* Container zeichnen lassen */
                                   WinSendMsg (pwnd->hwndContainer,
                                               CM_INVALIDATERECORD,
                                               MPVOID,
                                               MPFROM2SHORT (0, CMA_REPOSITION)) ;
                                   /* Containerinfo lesen */
                                   WinSendMsg (pwnd->hwndContainer,
                                               CM_QUERYCNRINFO,
                                               MPFROMP (&ccinfo),
                                               MPFROMSHORT (sizeof (CNRINFO))) ;
                                   if   (ccinfo.cRecords)
                                        {
                                        /* Es sind Records drin */
                                        WinSendMsg (pwnd->hwndMenu,
                                                    MM_SETITEMATTR,
                                                    MPFROM2SHORT (IDM_SAENDERN, TRUE),
                                                    MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                                        WinSendMsg (pwnd->hwndMenu,
                                                    MM_SETITEMATTR,
                                                    MPFROM2SHORT (IDM_SLOESCHEN, TRUE),
                                                    MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
                                        }
                                   else
                                        {
                                        /* Es sind keine Records drin */
                                        WinSendMsg (pwnd->hwndMenu,
                                                    MM_SETITEMATTR,
                                                    MPFROM2SHORT (IDM_SAENDERN, TRUE),
                                                    MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                                        WinSendMsg (pwnd->hwndMenu,
                                                    MM_SETITEMATTR,
                                                    MPFROM2SHORT (IDM_SLOESCHEN, TRUE),
                                                    MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
                                        }
                                   break ;
                              /* Einer von vielen S„tzen trifft ein */
                              case DBC_NOERROR:
                                   /* Plaz im Container fr einen Record allokieren */
                                   pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                      CM_ALLOCRECORD,
                                                                      MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                                      MPFROMSHORT (1))) ;
                                   /* Daten in Container Record kopieren */
                                   strcpy (pusrrec->szName,
                                           pdbinfoReceive->dbadr.szName) ;
                                   strcpy (pusrrec->szTelefon,
                                           pdbinfoReceive->dbadr.szTelefon) ;
                                   strcpy (pusrrec->szVorname,
                                           pdbinfoReceive->dbadr.szVorname) ;
                                   pusrrec->ulKey = pdbinfoReceive->dbadr.ulKey ;
                                   /* PSZ Pointer werden fr Strings ben”tigt */
                                   pusrrec->pszName = pusrrec->szName ;
                                   pusrrec->pszTelefon = pusrrec->szTelefon ;
                                   pusrrec->pszVorname = pusrrec->szVorname ;
                                   /* Record einfgen */
                                   memset (&recins,
                                           0,
                                           sizeof (RECORDINSERT)) ;
                                   recins.cb = sizeof (RECORDINSERT) ;
                                   recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                   recins.zOrder = CMA_TOP ;
                                   recins.cRecordsInsert = 1 ;
                                   WinSendMsg (pwnd->hwndContainer,
                                               CM_INSERTRECORD,
                                               MPFROMP (pusrrec),
                                               MPFROMP (&recins)) ;
                                   break ;
                              }
                           break ;
                      }
                   break ;
              }
           /* Kommunikationsstruktur wieder freigeben */
           FreeInfo (pdbinfoReceive) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Anwendungsstart */
INT main (VOID)
   {
   HMQ          hmq ;
   QMSG         qmsg ;
   static ULONG ulCreateFlags = FCF_MINMAX | FCF_SIZEBORDER | FCF_SHELLPOSITION | FCF_TITLEBAR | FCF_SYSMENU | FCF_TASKLIST | FCF_MENU ;

   /* PM Anmeldung */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Messageque anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     WCPDB22XMPL,
                                     (PFNWP) ClientWndProc,
                                     CS_SIZEREDRAW | CS_SAVEBITS,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           WCPDB22XMPL,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_DB22XMPL,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Messageloop */
                       while (WinGetMsg (hab_g,
                                         &qmsg,
                                         0,
                                         0,
                                         0))
                          {
                          WinDispatchMsg (hab_g,
                                          &qmsg) ;
                          }
                       /* Standardfenster l”schen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Messagequeue l”schen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* PM Abmeldung */
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

/* Dialogprozedur Satz einfgen */
MRESULT EXPENTRY SEDlgProc (HWND   hwndDlg,
                            MSG    msg,
                            MPARAM mp1,
                            MPARAM mp2)
   {
   CHAR    szText [256] ;
   CHAR    szTitel [128] ;
   PDBINFO pdbinfoSend ;

   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              /* Abbrechen gedrckt */
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Einfgen gedrckt */
              case DID_OK:
                   /* Wait Pointer anschalten */
                   WinSendMsg (hwndDlg,
                               WMP_WAIT,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   /* Kommunikationsstruktur allokieren, fllen und abschicken */
                   pdbinfoSend = AllocInfo () ;
                   pdbinfoSend->hwnd = hwndDlg ;
                   pdbinfoSend->ulEbene = DBK_ADR ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SENAME,
                                        sizeof (pdbinfoSend->dbadr.szName),
                                        pdbinfoSend->dbadr.szName) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SEVORNAME,
                                        sizeof (pdbinfoSend->dbadr.szVorname),
                                        pdbinfoSend->dbadr.szVorname) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SETELEFON,
                                        sizeof (pdbinfoSend->dbadr.szTelefon),
                                        pdbinfoSend->dbadr.szTelefon) ;
                   WinPostMsg (hwndDatabase_g,
                               WMPDB_INSERT,
                               MPFROMLONG (DBK_INSERTPART),
                               MPFROMP (pdbinfoSend)) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Maus ist ber Kontrollelement */
      case WM_CONTROLPOINTER:
           if   (ulWait_g)
                {
                /* Wenn Wait Pointer angeschaltet, dann entsprechenden Pointer ausgeben */
                return (MRESULT) WinQuerySysPointer (HWND_DESKTOP,
                                                     SPTR_WAIT,
                                                     FALSE) ;
                }
           break ;
      case WM_INITDLG:
           /* Gr”áe der Eingabefelder setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SENAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SEVORNAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SETELEFON,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           return (MRESULT) FALSE ;
      /* Wait Pointer Verarbeitung fr einen Dialog */
      case WMP_WAIT:
           if   (!LONGFROMMP (mp1) && !ulWait_g)
                {
                return (MRESULT) FALSE ;
                }
           (LONGFROMMP (mp1)) ? ulWait_g++ : ulWait_g-- ;
           if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                 (!LONGFROMMP (mp1) && !ulWait_g))
                {
                WinSetPointer (HWND_DESKTOP,
                               WinQuerySysPointer (HWND_DESKTOP,
                                                   (LONGFROMMP (mp1)) ? SPTR_WAIT : SPTR_ARROW,
                                                   FALSE)) ;
                WinEnableWindow (hwndFrame_g,
                                 !LONGFROMMP (mp1)) ;
                WinEnableWindow (hwndDlg,
                                 !LONGFROMMP (mp1)) ;
                }
           return (MRESULT) FALSE ;
      /* Ergebnis von Datenbank kommt rein */
      case WMPDB_SEND:
           {
           PDBINFO pdbinfoReceive ;

           pdbinfoReceive = PVOIDFROMMP (mp2) ;
           /* Was war es? */
           switch (LONGFROMMP (mp1))
              {
              /* Einfgen */
              case DBK_INSERTPART:
                   /* Welche Tabelle */
                   switch (pdbinfoReceive->ulEbene)
                      {
                      /* Adresse */
                      case DBK_ADR:
                           /* Wait Pointer abschalten */
                           WinSendMsg (hwndDlg,
                                       WMP_WAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           if   (pdbinfoReceive->ulRC == DBC_NOERROR)
                                {
                                /* Wenn kein Fehler, dann Dialog verlassen */
                                WinDismissDlg (hwndDlg,
                                               DID_OK) ;
                                }
                           else
                                {
                                /* Wenn Fehler dann Fehlermeldung ausgeben */
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_INSERTERROR,
                                               sizeof (szTitel),
                                               szTitel) ;
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_INSERTERRORT,
                                               sizeof (szText),
                                               szText) ;
                                WinMessageBox (HWND_DESKTOP,
                                               hwndDlg,
                                               szText,
                                               szTitel,
                                               0,
                                               MB_OK | MB_ERROR | MB_APPLMODAL) ;
                                }
                           break ;
                      }
                   break ;
              }
           /* Kommunikationsstruktur wieder freigeben */
           FreeInfo (pdbinfoReceive) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialogprozedur Satz „ndern */
MRESULT EXPENTRY SADlgProc (HWND   hwndDlg,
                            MSG    msg,
                            MPARAM mp1,
                            MPARAM mp2)
   {
   CHAR    szText [256] ;
   CHAR    szTitel [128] ;
   PDBINFO pdbinfoSend ;
   PDLGSA  pdlg ;

   /* Instanzdaten des Dialoges holen */
   pdlg = WinQueryWindowPtr (hwndDlg,
                             QWL_USER) ;
   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              /* Abbrechen gedrckt */
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Žndern gedrckt */
              case DID_OK:
                   /* Wait Pointer anschalten */
                   WinSendMsg (hwndDlg,
                               WMP_WAIT,
                               MPFROMLONG (TRUE),
                               MPVOID) ;
                   /* Kommunikationsstruktur allokieren, fllen und abschicken */
                   pdbinfoSend = AllocInfo () ;
                   pdbinfoSend->hwnd = hwndDlg ;
                   pdbinfoSend->ulEbene = DBK_ADR ;
                   pdbinfoSend->dbadr.ulKey = pdlg->ulKey ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SANAME,
                                        sizeof (pdbinfoSend->dbadr.szName),
                                        pdbinfoSend->dbadr.szName) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SAVORNAME,
                                        sizeof (pdbinfoSend->dbadr.szVorname),
                                        pdbinfoSend->dbadr.szVorname) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_SATELEFON,
                                        sizeof (pdbinfoSend->dbadr.szTelefon),
                                        pdbinfoSend->dbadr.szTelefon) ;
                   WinPostMsg (hwndDatabase_g,
                               WMPDB_UPDATE,
                               MPFROMLONG (DBK_UPDATEPART),
                               MPFROMP (pdbinfoSend)) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      /* Maus ist ber Kontrollelement */
      case WM_CONTROLPOINTER:
           if   (ulWait_g)
                {
                /* Wenn Wait Pointer angeschaltet, dann entsprechenden Pointer ausgeben */
                return (MRESULT) WinQuerySysPointer (HWND_DESKTOP,
                                                     SPTR_WAIT,
                                                     FALSE) ;
                }
           break ;
      case WM_DESTROY:
           /* Instanzdaten des Dialoges wegwerfen */
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           /* Instanzdaten des Dialoges allokieren und in den Windowwords ablegen */
           pdlg = calloc (1,
                          sizeof (DLGSA)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Gr”áe der Eingabefelder setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SANAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SAVORNAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_SATELEFON,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           return (MRESULT) FALSE ;
      /* Das Client schickt noch Daten hinterher */
      case WMP_MAININIT:
           /* Key des Satzes in Instanzdaten des Dialoges speichern */
           pdlg->ulKey = LONGFROMMP (mp1) ;
           /* Wait Pointer anschalten */
           WinSendMsg (hwndDlg,
                       WMP_WAIT,
                       MPFROMLONG (TRUE),
                       MPVOID) ;
           /* Kommunikationsstruktur allokieren und Daten anfordern */
           pdbinfoSend = AllocInfo () ;
           pdbinfoSend->hwnd = hwndDlg ;
           pdbinfoSend->ulEbene = DBK_ADR ;
           pdbinfoSend->dbadr.ulKey = pdlg->ulKey ;
           WinPostMsg (hwndDatabase_g,
                       WMPDB_REQUEST,
                       MPFROMLONG (DBK_REQUESTPART),
                       MPFROMP (pdbinfoSend)) ;
           return (MRESULT) FALSE ;
      /* Wait Pointer Verarbeitung fr einen Dialog */
      case WMP_WAIT:
           if   (!LONGFROMMP (mp1) && !ulWait_g)
                {
                return (MRESULT) FALSE ;
                }
           (LONGFROMMP (mp1)) ? ulWait_g++ : ulWait_g-- ;
           if   ((LONGFROMMP (mp1) && (ulWait_g == 1)) ||
                 (!LONGFROMMP (mp1) && !ulWait_g))
                {
                WinSetPointer (HWND_DESKTOP,
                               WinQuerySysPointer (HWND_DESKTOP,
                                                   (LONGFROMMP (mp1)) ? SPTR_WAIT : SPTR_ARROW,
                                                   FALSE)) ;
                WinEnableWindow (hwndFrame_g,
                                 !LONGFROMMP (mp1)) ;
                WinEnableWindow (hwndDlg,
                                 !LONGFROMMP (mp1)) ;
                }
           return (MRESULT) FALSE ;
      /* Ergebnis von Datenbank kommt rein */
      case WMPDB_SEND:
           {
           PDBINFO pdbinfoReceive ;

           pdbinfoReceive = PVOIDFROMMP (mp2) ;
           /* Was war es? */
           switch (LONGFROMMP (mp1))
              {
              /* Lesen */
              case DBK_REQUESTPART:
                   /* Welche Tabelle */
                   switch (pdbinfoReceive->ulEbene)
                      {
                      /* Adresse */
                      case DBK_ADR:
                           /* Wait Pointer abschalten */
                           WinSendMsg (hwndDlg,
                                       WMP_WAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           if   (pdbinfoReceive->ulRC == DBC_NOERROR)
                                {
                                /* Wenn kein Fehler, dann Dialogfelder fllen */
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_SANAME,
                                                   pdbinfoReceive->dbadr.szName) ;
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_SAVORNAME,
                                                   pdbinfoReceive->dbadr.szVorname) ;
                                WinSetDlgItemText (hwndDlg,
                                                   IDE_SATELEFON,
                                                   pdbinfoReceive->dbadr.szTelefon) ;
                                }
                           else
                                {
                                /* Wenn Fehler dann Fehlermeldung ausgeben */
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_READERROR,
                                               sizeof (szTitel),
                                               szTitel) ;
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_READERRORT,
                                               sizeof (szText),
                                               szText) ;
                                WinMessageBox (HWND_DESKTOP,
                                               hwndDlg,
                                               szText,
                                               szTitel,
                                               0,
                                               MB_OK | MB_ERROR | MB_APPLMODAL) ;
                                }
                           break ;
                      }
                   break ;
              /* Ersetzen */
              case DBK_UPDATEPART:
                   /* Welche Tabelle */
                   switch (pdbinfoReceive->ulEbene)
                      {
                      /* Adresse */
                      case DBK_ADR:
                           /* Wait Pointer abschalten */
                           WinSendMsg (hwndDlg,
                                       WMP_WAIT,
                                       MPFROMLONG (FALSE),
                                       MPVOID) ;
                           if   (pdbinfoReceive->ulRC == DBC_NOERROR)
                                {
                                /* Wenn kein Fehler, dann Dialog verlassen */
                                WinDismissDlg (hwndDlg,
                                               DID_OK) ;
                                }
                           else
                                {
                                /* Wenn Fehler dann Fehlermeldung ausgeben */
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_UPDATEERROR,
                                               sizeof (szTitel),
                                               szTitel) ;
                                WinLoadString (hab_g,
                                               NULLHANDLE,
                                               IDS_UPDATEERRORT,
                                               sizeof (szText),
                                               szText) ;
                                WinMessageBox (HWND_DESKTOP,
                                               hwndDlg,
                                               szText,
                                               szTitel,
                                               0,
                                               MB_OK | MB_ERROR | MB_APPLMODAL) ;
                                }
                           break ;
                      }
                   break ;
              }
           /* Kommunikationsstruktur wieder freigeben */
           FreeInfo (pdbinfoReceive) ;
           return (MRESULT) FALSE ;
           }
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }
