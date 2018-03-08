/* $Header: D:/Projekte/Exploring WebExplorer/Source/rcs/WEBEX.C 1.3 1996/06/23 15:11:01 HaWi Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* C Kopfdateien */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* WEBExplorer Kopfdatei */
#define  WARPONLY
#include <webexwin.h>

/* Applikations Kopfdatei */
#include "WEBEX.H"

/* Moudlglobale Variablen */
BOOL fMinimized_g = FALSE ;
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszWEBEx_g = "WEBEx" ;

/* Fensterprozedur des Clients */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welcher Event ist von Interesse? */
   switch (msg)
      {
      /* DrgDragFiles wurde abgeschlossen */
      case DM_DRAGFILECOMPLETE:
           {
           BOOL  f ;
           CHAR  sz [CCHMAXPATH] ;
           CHAR  sz2 [CCHMAXPATH] ;
           PSZ   psz ;
           ULONG ul ;
           ULONG ul2 ;

           /* Ermittlung eines temporaeren Verzeichnissses */
           f = FALSE ;
           /* Erst TMP versuchen */
           if   (!DosScanEnv ("TMP",
                              &psz))
                {
                f = TRUE ;
                }
           else
                {
                /* Wenn kein Glueck dann TEMP versuchen */
                if   (!DosScanEnv ("TEMP",
                                   &psz))
                     {
                     f = TRUE ;
                     }
                }
           if   (!f)
                {
                /* Wenn immer noch kein Glueck dann das Root des Bootlaufwerks */
                DosQueryCurrentDisk (&ul,
                                     &ul2) ;
                sz [0] = (CHAR) ul + '@' ;
                sz [1] = ':' ;
                sz [2] = '\\' ;
                ul2 = sizeof (sz) - 3 ;
                DosQueryCurrentDir (ul,
                                    &sz [3],
                                    &ul2) ;
                }
           else
                {
                strcpy (sz,
                        psz) ;
                }
           /* Eventuell Backslash anhaengen */
           if   (sz [strlen (sz) - 1] != '\\')
                {
                strcat (sz,
                        "\\") ;
                }
           /* Umkopieren */
           strcpy (sz2,
                   sz) ;
           /* Dateiname besorgen */
           DrgQueryStrName (LONGFROMMP (mp1),
                            sizeof (sz),
                            sz) ;
           /* Dateiname anhaengen */
           strcat (sz2,
                   sz) ;
           /* Temporaere Datei wieder loeschen */
           remove (sz2) ;
           return (MRESULT) FALSE ;
           }
      /* Abschluss der Anwendung */
      case WM_CLOSE:
           /* MessageLoop beenden */
           WinPostMsg (hwnd,
                       WM_QUIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           CHAR    sz [CCHMAXPATH] ;
           FILEDLG fildlg ;
           PFN     pfn ;
           PUSRREC pusrrec ;
           PWND    pwnd ;
           ULONG   ul ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Welche Benutzeraktion? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel */
              case DID_CANCEL:
                   /* Applikationsende einleiten */
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Loeschen einer WEB Gruppe */
              case IDM_AREAEINFUEGEN:
              case IDM_ROOTEINFUEGEN:
                   /* Welcher Record ist selektiert? */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Einfuegen Area Dialog ausgeben */
                        WinDlgBox (HWND_DESKTOP,
                                   hwndFrame_g,
                                   EinfuegenAreaDlgWndProc,
                                   NULLHANDLE,
                                   IDD_EINFUEGENAREA,
                                   pwnd) ;
                        }
                   return (MRESULT) FALSE ;
              /* Loeschen einer WEB Gruppe */
              case IDM_AREALOESCHEN:
                   /* Welcher Record ist selektiert? */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Sicherheitsabfrage ausgeben */
                        if   (TWriteMessage (hwnd,
                                             NULLHANDLE,
                                             IDS_SICHERHEITSABFRAGE,
                                             IDS_WOLLENSIEDIESEWEBGRUPPEWIRKLICHLOESCHEN,
                                             MB_QUERY | MB_YESNO) == MBID_YES)
                             {
                             /* Selektierten Container Record loeschen */
                             WinSendMsg (pwnd->hwndContainer,
                                         CM_REMOVERECORD,
                                         MPFROMP (&pusrrec),
                                         MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* WEB Seite drucken */
              case IDM_DOKUMENTDRUCKEN:
                   /* Standard Queue Name ermitteln */
                   if   (TQueryDefaultQueueName (pwnd->wxprintinfo.szQueueName))
                        {
                        /* Standard Queue Driver ermitteln */
                        if   (TQueryQueueDriver (pwnd->wxprintinfo.szQueueName,
                                                 pwnd->wxprintinfo.szDriverName))
                             {
                             /* Groesse der Printer Driver Daten ermitteln */
                             if   ((ul = TQueryJobProperties (hab_g,
                                                              NULL,
                                                              pwnd->wxprintinfo.szDriverName,
                                                              pwnd->wxprintinfo.szQueueName,
                                                              sz,
                                                              FALSE)) != 0)
                                  {
                                  /* Speicher fuer die Printer Driver Daten allokieren */
                                  pwnd->wxprintinfo.pDriverData = calloc (1,
                                                                          ul) ;
                                  /* Printer Driver Daten auslesen */
                                  if   (TQueryJobProperties (hab_g,
                                                             pwnd->wxprintinfo.pDriverData,
                                                             pwnd->wxprintinfo.szDriverName,
                                                             pwnd->wxprintinfo.szQueueName,
                                                             sz,
                                                             FALSE))
                                       {
                                       /* Anzahl Kopien */
                                       pwnd->wxprintinfo.nCopies = 1 ;
                                       /* WEB Seite drucken */
                                       WXViewPrint (pwnd->hwndWEB,
                                                    &pwnd->wxprintinfo,
                                                    sizeof (WXPRINTINFO)) ;
                                       }
                                  }
                             }
                        }
                   return (MRESULT) FALSE ;
              /* WEB Seite in eine Datei laden */
              case IDM_DOKUMENTINDATEILADEN:
              case IDM_ITEMINDATEILADEN:
                   /* Nur wenn nicht gerade etwas anderes geladen wird */
                   if   (!pwnd->fViewActive)
                        {
                        /* Welcher Record ist selektiert? */
                        if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                  CM_QUERYRECORDEMPHASIS,
                                                                  MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                  MPFROMSHORT (CRA_SELECTED)))) != NULL)
                             {
                             /* Kommunikationsstruktur des Datei sichern Dialoges loeschen */
                             memset (&fildlg,
                                     0,
                                     sizeof (FILEDLG)) ;
                             /* Groesse der Kommunikationsstruktur */
                             fildlg.cbSize = sizeof (FILEDLG) ;
                             /* Einige Flags */
                             fildlg.fl = FDS_CENTER | FDS_SAVEAS_DIALOG ;
                             /* File sichern Dialog ausgeben */
                             if   (WinFileDlg (HWND_DESKTOP,
                                               hwndFrame_g,
                                               &fildlg) &&
                                   (fildlg.lReturn == DID_OK))
                                  {
                                  /* Da Compiler bei diesem Statement immer abstuerzt wird Prozedur dynamisch aus DLL geladen */
                                  if   (!DosQueryProcAddr (pwnd->hmodWEB,
                                                           0,
                                                           "WXViewLoadToFile",
                                                           &pfn))
                                       {
/*                                     WXViewLoadToFile (pwnd->hwndWEB,
                                                         pusrrec->szURL,
                                                         fildlg.szFullFile,
                                                         WXLOAD_FORCERELOAD,
                                                         NULL) ; */
                                       typedef APIRET (PFXN) (HWND, PSZ, PSZ, ULONG) ;
                                       (pfn) (pwnd->hwndWEB,
                                              pusrrec->szURL,
                                              fildlg.szFullFile,
                                              WXLOAD_FORCERELOAD,
                                              NULL) ;
                                       }
                                  }
                             }
                        }
                   return (MRESULT) FALSE ;
              /* WEB Seite laden */
              case IDM_DOKUMENTLADEN:
              case IDM_ITEMLADEN:
                   /* Nur wenn nicht gerade etwas anderes geladen wird */
                   if   (!pwnd->fViewActive)
                        {
                        /* Welcher Record ist selektiert? */
                        if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                  CM_QUERYRECORDEMPHASIS,
                                                                  MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                                  MPFROMSHORT (CRA_SELECTED)))) != NULL)
                             {
                             /* Da Compiler bei diesem Statement immer abstuerzt wird Prozedur dynamisch aus DLL geladen */
                             if   (!DosQueryProcAddr (pwnd->hmodWEB,
                                                      0,
                                                      "WXViewLoad",
                                                      &pfn))
                                  {
/*                                WXViewLoad (pwnd->hwndWEB,
                                              pusrrec->szURL,
                                              WXLOAD_FORCERELOAD,
                                              NULL) ; */
                                  typedef APIRET (PFXN) (HWND, PSZ, ULONG, PVOID) ;
                                  (pfn) (pwnd->hwndWEB,
                                         pusrrec->szURL,
                                         WXLOAD_FORCERELOAD,
                                         NULL) ;
                                  }
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Ladeprozess einer WEB Seite stoppen */
              case IDM_DOKUMENTLADENSTOPPEN:
                   /* Nur wenn gerade etwas anderes geladen wird */
                   if   (pwnd->fViewActive)
                        {
                        /*Laden abbrechen */
                        WXViewCancelLoad (pwnd->hwndWEB) ;
                        }
                   return (MRESULT) FALSE ;

              /* Container einklappen */
              case IDM_EDITIERENCOLLAPSE:
                   /* Einklappen veranlassen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_COLLAPSETREE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Conatiner ausklappen */
              case IDM_EDITIERENEXPAND:
                   /* Ausklappen veranlassen */
                   WinSendMsg (pwnd->hwndContainer,
                               CM_EXPANDTREE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* WEB Seite loeschen */
              case IDM_ITEMLOESCHEN:
                   /* Welcher Record ist selektiert? */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Sicherheitsabfrage ausgeben */
                        if   (TWriteMessage (hwnd,
                                             NULLHANDLE,
                                             IDS_SICHERHEITSABFRAGE,
                                             IDS_WOLLENSIEDIESEWEBSEITEWIRKLICHLOESCHEN,
                                             MB_QUERY | MB_YESNO) == MBID_YES)
                             {
                             /* Container Record loeschen */
                             WinSendMsg (pwnd->hwndContainer,
                                         CM_REMOVERECORD,
                                         MPFROMP (&pusrrec),
                                         MPFROM2SHORT (1, CMA_FREE | CMA_INVALIDATE)) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Optionen Cache */
              case IDM_OPTIONENCACHE:
                   /* Dialog ausgeben */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              OptionenCacheDlgWndProc,
                              NULLHANDLE,
                              IDD_OPTIONENCACHE,
                              pwnd) ;
                   return (MRESULT) FALSE ;
              /* Optionen Display */
              case IDM_OPTIONENDISPLAY:
                   /* Dialog ausgeben */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              OptionenDisplayDlgWndProc,
                              NULLHANDLE,
                              IDD_OPTIONENDISPLAY,
                              pwnd) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Control meldet einen Event */
      case WM_CONTROL:
           {
           BOOL               f ;
           CHAR               sz [CCHMAXPATH] ;
           CHAR               sz2 [CCHMAXPATH] ;
           CHAR               sz3 [CCHMAXPATH] ;
           EATDATA            aeatdata [2] ;
           FILE*              pfile ;
           HWND               hwndTemp ;
           PCNRDRAGINFO       pcdrginfo ;
           PCNRDRAGINIT       pcdrginit ;
           PCNREDITDATA       pcnredat ;
           PDRAGITEM          pditem ;
           PNOTIFYRECORDENTER pnotrecen ;
           POINTL             ptl ;
           PSZ                psz ;
           PUSRREC            pusrrec ;
           PUSRREC            pusrrec2 ;
           PWND               pwnd ;
           RECORDINSERT       recins ;
           SWP                swp ;
           ULONG              ul ;
           ULONG              ul2 ;

           /* Instanzdaten aus den Window Words holen */
           if   ((pwnd = WinQueryWindowPtr (hwndFrame_g,
                                            QWL_USER)) != NULL)
                {
                /* Welches Control? */
                switch (SHORT1FROMMP (mp1))
                   {
                   /* Der Container */
                   case IDCO_WEBSEITEN:
                        /* Welcher Event? */
                        switch (SHORT2FROMMP (mp1))
                           {
                           /* Contextmen soll ausgegeben werden */
                           case CN_CONTEXTMENU:
                                /* Nur wenn nicht gerade etwas anderes geladen wird */
                                if   (!pwnd->fViewActive)
                                     {
                                     /* Ist ein gltiger Record selektiert */
                                     if   ((pusrrec = PVOIDFROMMP (mp2)) != NULL)
                                          {
                                          /* Emphasis auf den Record setzen */
                                          WinSendMsg (pwnd->hwndContainer,
                                                      CM_SETRECORDEMPHASIS,
                                                      MPFROMP (pusrrec),
                                                      MPFROM2SHORT (TRUE, CRA_SELECTED)) ;
                                          /* Aktuelle Mausposition ermitteln */
                                          WinQueryPointerPos (HWND_DESKTOP,
                                                              &ptl) ;
                                          /* Frame Koordinaten ermitteln */
                                          WinQueryWindowPos (hwndFrame_g,
                                                             &swp) ;
                                          /* Frame von Maus abziehen */
                                          ptl.x -= swp.x ;
                                          ptl.y -= swp.y ;
                                          /* Welches Popupmenu und welcher Defaulteintrag? */
                                          if   (pusrrec->recordtype == K_RECORDROOT)
                                               {
                                               ul = IDM_ROOT ;
                                               ul2 = IDM_ROOTEINFUEGEN ;
                                               }
                                          if   (pusrrec->recordtype == K_RECORDAREA)
                                               {
                                               ul = IDM_AREA ;
                                               ul2 = IDM_AREAEINFUEGEN ;
                                               }
                                          if   (pusrrec->recordtype == K_RECORDITEM)
                                               {
                                               ul = IDM_ITEM ;
                                               ul2 = IDM_ITEMLADEN ;
                                               }
                                          /* Popup Menu aus Ressource laden */
                                          hwndTemp = WinLoadMenu (HWND_OBJECT,
                                                                  NULLHANDLE,
                                                                  ul) ;
                                          /* Popupmenue anzeigen */
                                          WinPopupMenu (pwnd->hwndContainer,
                                                        hwnd,
                                                        hwndTemp,
                                                        ptl.x,
                                                        ptl.y,
                                                        ul2,
                                                        PU_HCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE | PU_POSITIONONITEM | PU_VCONSTRAIN) ;
                                          }
                                     }
                                break ;
                           /* Ein Objekt wird ueber den Container gezogen */
                           case CN_DRAGOVER:
                                if   ((pcdrginfo = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Nur ueber einem Record ist von Interesse */
                                     if   ((pusrrec = (PUSRREC) pcdrginfo->pRecord) != NULL)
                                          {
                                          /* Und nur ueber WEB Gruppen Records */
                                          if   (pusrrec->recordtype == K_RECORDAREA)
                                               {
                                               /* Drag Infodaten besorgen */
                                               if   ((pditem = DrgQueryDragitemPtr (pcdrginfo->pDragInfo,
                                                                                    0)) != NULL)
                                                    {
                                                    /* Rendern */
                                                    if   (DrgVerifyRMF (pditem,
                                                                        "DRM_OS2FILE",
                                                                        "DRF_TEXT"))
                                                         {
                                                         /* Source Verzeichnis */
                                                         DrgQueryStrName (pditem->hstrContainerName,
                                                                          sizeof (sz),
                                                                          sz) ;
                                                         /* Source Dateiname */
                                                         DrgQueryStrName (pditem->hstrSourceName,
                                                                          sizeof (sz2),
                                                                          sz2) ;
                                                         /* Voll qualifizierter Dateiname */
                                                         strcat (sz,
                                                                 sz2) ;
                                                         /* EA Struktur vorbereiten */
                                                         memset (aeatdata,
                                                                 0,
                                                                 sizeof (aeatdata)) ;
                                                         aeatdata[0].ulValue = sizeof (sz2) ;
                                                         aeatdata[0].pbValue = sz2 ;
                                                         /* Multiple Typ multiple Value EAs .TYPE lesen */
                                                         if   (TEAReadMV (sz,
                                                                          ".TYPE",
                                                                          EAT_MVMT,
                                                                          aeatdata) == TNOERROR)
                                                              {
                                                              /* Ist der Typ richtig? */
                                                              if   (!stricmp (sz2,
                                                                              "WebExplorer_Url"))
                                                                   {
                                                                   /* Ja */
                                                                   return MRFROM2SHORT (DOR_DROP, DO_COPY) ;
                                                                   }
                                                              }
                                                         else
                                                              {
                                                              /* ansonsten */
                                                              memset (aeatdata,
                                                                      0,
                                                                      sizeof (aeatdata)) ;
                                                              aeatdata[0].ulValue = sizeof (sz2) ;
                                                              aeatdata[0].pbValue = sz2 ;
                                                              /* mit Single Typ multiple Value EAs .TYPE lesen */
                                                              if   (TEAReadMV (sz,
                                                                               ".TYPE",
                                                                               EAT_MVST,
                                                                               aeatdata) == TNOERROR)
                                                                   {
                                                                   /* Ist der Typ richtig? */
                                                                   if   (!stricmp (sz2,
                                                                                   "WebExplorer_Url"))
                                                                        {
                                                                        /* Ja */
                                                                        return MRFROM2SHORT (DOR_DROP, DO_COPY) ;
                                                                        }
                                                                   }
                                                              }
                                                         }
                                                    }
                                               }
                                          }
                                     }
                                break ;
                           /* Objekt wurde losgelassen */
                           case CN_DROP:
                                if   ((pcdrginfo = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Nur ueber einem Record ist von Interesse */
                                     if   ((pusrrec = (PUSRREC) pcdrginfo->pRecord) != NULL)
                                          {
                                          /* Und nur ueber WEB Gruppen Records */
                                          if   (pusrrec->recordtype == K_RECORDAREA)
                                               {
                                               /* Drag Infodaten besorgen */
                                               if   ((pditem = DrgQueryDragitemPtr (pcdrginfo->pDragInfo,
                                                                                    0)) != NULL)
                                                    {
                                                    /* Source Verzeichnis */
                                                    DrgQueryStrName (pditem->hstrContainerName,
                                                                     sizeof (sz),
                                                                     sz) ;
                                                    /* Source Dateiname */
                                                    DrgQueryStrName (pditem->hstrSourceName,
                                                                     sizeof (sz2),
                                                                     sz2) ;
                                                    /* Voll qualifizierter Dateiname */
                                                    strcat (sz,
                                                            sz2) ;
                                                    /* Diese Datei oeffnen */
                                                    if   ((pfile = fopen (sz,
                                                                          "r")) != NULL)
                                                         {
                                                         /* Neuen Record allokieren */
                                                         pusrrec2 = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                                             CM_ALLOCRECORD,
                                                                                             MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                                                             MPFROMSHORT (1))) ;
                                                         /* Name der neuen WEB Page */
                                                         DrgQueryStrName (pditem->hstrTargetName,
                                                                          sizeof (pusrrec2->szName),
                                                                          pusrrec2->szName) ;
                                                         /* URL aus Datei lesen */
                                                         fscanf (pfile,
                                                                 "%s",
                                                                 pusrrec2->szURL) ;
                                                         /* Pointer setzen */
                                                         pusrrec2->minirec.pszIcon = pusrrec2->szName ;
                                                         /* Record ist WEB Seite */
                                                         pusrrec2->recordtype = K_RECORDITEM ;
                                                         /* Record einfuegen */
                                                         memset (&recins,
                                                                 0,
                                                                 sizeof (RECORDINSERT)) ;
                                                         recins.cb = sizeof (RECORDINSERT) ;
                                                         recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                                                         /* WEB Gruppe ist Parent */
                                                         recins.pRecordParent = (PRECORDCORE) pusrrec ;
                                                         recins.zOrder = CMA_TOP ;
                                                         recins.cRecordsInsert = 1 ;
                                                         /* Direkt invalidieren */
                                                         recins.fInvalidateRecord = TRUE ;
                                                         /* Record nun drin */
                                                         WinSendMsg (pwnd->hwndContainer,
                                                                     CM_INSERTRECORD,
                                                                     MPFROMP (pusrrec2),
                                                                     MPFROMP (&recins)) ;
                                                         /* Datei wieder schliessen */
                                                         fclose (pfile) ;
                                                         }
                                                    }
                                               }
                                          }
                                     }
                                break ;
                           /* Doppelklick auf Container */
                           case CN_ENTER:
                                if   ((pnotrecen = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Nur ueber einem Record ist von Interesse */
                                     if   ((pusrrec = (PUSRREC) pnotrecen->pRecord) != NULL)
                                          {
                                          /* Und nur ueber WEB Seiten Records */
                                          if   (pusrrec->recordtype == K_RECORDITEM)
                                               {
                                               /* Laden einleiten */
                                               WinPostMsg (hwnd,
                                                           WM_COMMAND,
                                                           MPFROMSHORT (IDM_DOKUMENTLADEN),
                                                           MPFROM2SHORT (CMDSRC_PUSHBUTTON, TRUE)) ;
                                               }
                                          }
                                     }
                                break ;
                           /* Benutzer startet Drag and Drop vom Container */
                           case CN_INITDRAG:
                                if   ((pcdrginit = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Nur ueber einem Record ist von Interesse */
                                     if   ((pusrrec = (PUSRREC) pcdrginit->pRecord) != NULL)
                                          {
                                          /* Und nur ueber WEB Seiten Records */
                                          if   (pusrrec->recordtype == K_RECORDITEM)
                                               {
                                               /* Ermittlung eines temporaeren Verzeichnissses */
                                               f = FALSE ;
                                               /* Erst TMP versuchen */
                                               if   (!DosScanEnv ("TMP",
                                                                  &psz))
                                                    {
                                                    f = TRUE ;
                                                    }
                                               else
                                                    {
                                                    /* Wenn kein Glueck dann TEMP versuchen */
                                                    if   (!DosScanEnv ("TEMP",
                                                                       &psz))
                                                         {
                                                         f = TRUE ;
                                                         }
                                                    }
                                               if   (!f)
                                                    {
                                                    /* Wenn immer noch kein Glueck dann das Root des Bootlaufwerks */
                                                    DosQueryCurrentDisk (&ul,
                                                                         &ul2) ;
                                                    sz [0] = (CHAR) ul + '@' ;
                                                    sz [1] = ':' ;
                                                    sz [2] = '\\' ;
                                                    ul2 = sizeof (sz) - 3 ;
                                                    DosQueryCurrentDir (ul,
                                                                        &sz [3],
                                                                        &ul2) ;
                                                    }
                                               else
                                                    {
                                                    strcpy (sz,
                                                            psz) ;
                                                    }
                                               /* Eventuell Backslash anhaengen */
                                               if   (sz [strlen (sz) - 1] != '\\')
                                                    {
                                                    strcat (sz,
                                                            "\\") ;
                                                    }
                                               /* Umkopieren */
                                               strcpy (sz2,
                                                       sz) ;
                                               /* Dateiname aus Record anhaengen */
                                               strcat (sz2,
                                                       pusrrec->szName) ;
                                               /* Setup String fuer WEBExplorer Objekt aufbauen */
                                               sprintf (sz3,
                                                        "NORENAME=YES;LOCATOR=%s",
                                                        pusrrec->szURL) ;
                                               /* WebExplorer Objekt anlegen */
                                               if   ((WinCreateObject ("WebExplorer_Url",
                                                                       pusrrec->szName,
                                                                       sz3,
                                                                       sz,
                                                                       CO_REPLACEIFEXISTS)) != NULLHANDLE)
                                                    {
                                                    /* Wenn erfolgreich dann den Drag zulassen */
                                                    psz = sz2 ;
                                                    DrgDragFiles (hwnd,
                                                                  &psz,
                                                                  NULL,
                                                                  NULL,
                                                                  1,
                                                                  WinQuerySysPointer (HWND_DESKTOP,
                                                                                      SPTR_FILE,
                                                                                      FALSE),
                                                                  VK_ENDDRAG,
                                                                  FALSE,
                                                                  0) ;
                                                    }
                                               }
                                          }
                                     }
                                break ;
                           /* Direktes Editieren eines Record wurde abgeschlossen */
                           case CN_REALLOCPSZ:
                                if   ((pcnredat = PVOIDFROMMP (mp2)) != NULL)
                                     {
                                     /* Ermitteln ob der neue Text in die Variable passt */
                                     return (pcnredat->cbText < sizeof (pusrrec->szName)) ? (MRESULT) TRUE : (MRESULT) FALSE ;
                                     }
                                break ;
                           }
                        break ;
                   }
                }
           break ;
           }
      /* Fenster wird eroeffnet */
      case WM_CREATE:
           /* Platz fuer Instanzdaten allokieren und in den Window Words ablegen */
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
      /* Fenster wird wieder geschlossen */
      case WM_DESTROY:
           {
           FILE* pfileG ;
           FILE* pfileP ;
           PWND  pwnd ;
           ULONG ul ;
           ULONG ul2 ;

           /* Instanzdaten aus Windowwords holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* WEB View loeschen */
           WinDestroyWindow (pwnd->hwndWEB) ;
           /* Module Handle der WEB API wieder freigeben */
           DosFreeModule (pwnd->hmodWEB) ;
           /* WEB Gruppen Datei oeffnen */
           if   ((pfileG = fopen ("WEBEXG.ASC",
                                  "w")) != NULL)
                {
                /* WEB Seiten Datei oeffnen */
                if   ((pfileP = fopen ("WEBEXP.ASC",
                                       "w")) != NULL)
                     {
                     ul = 0 ;
                     ul2 = 0 ;
                     /* Rekursiv beide Datei miit den Container Records beschreiben */
                     WriteRecord (pwnd->hwndContainer,
                                  NULL,
                                  pfileG,
                                  pfileP,
                                  &ul,
                                  &ul2) ;
                     /* Zum Abschluss noch ein LineFeed */
                     fprintf (pfileP,
                              "\n") ;
                     /* WEB Seiten Datei wieder schliessen */
                     fclose (pfileP) ;
                     }
                     /* Zum Abschluss noch ein LineFeed */
                  fprintf (pfileG,
                           "\n") ;
                /* WEB Gruppen Datei wieder schliessen */
                fclose (pfileG) ;
                }
           /* Instanzdaten wieder freigeben */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
           }
      /* Wer soll Hintergrund des Clients zeichnen? */
      case WM_ERASEBACKGROUND:
           /* PM */
           return (MRESULT) TRUE ;
      /* Ein Menue wurde geschlossen */
      case WM_MENUEND:
           /* Ist es eines der Popupmenues? */
           if   ((SHORT1FROMMP (mp1) == IDM_AREA) ||
                 (SHORT1FROMMP (mp1) == IDM_ITEM) ||
                 (SHORT1FROMMP (mp1) == IDM_ROOT))
                {
                /* Ja, dann Menue wieder freigeben */
                WinDestroyWindow (HWNDFROMMP (mp2)) ;
                }
           break ;
      /* Min oder Max wurde ausgeloest */
      case WM_MINMAXFRAME:
           /* Status merken */
           fMinimized_g = ((PSWP) PVOIDFROMMP (mp1))->fl & SWP_MINIMIZE ;
           break ;
      /* Size wurde veraendert */
      case WM_SIZE:
           /* Verzoegert durchfuehren lassen */
           WinPostMsg (hwnd,
                       WMP_SIZE,
                       mp1,
                       mp2) ;
           break ;
      /* Verzoegerter Programmstart hier */
      case WMP_MAININIT:
           {
           CHAR    sz [CCHMAXPATH] ;
           CNRINFO ccinfo ;
           PWND    pwnd ;
           SWP     swp ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Menue Handle besorgen */
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           /* Text fuer die Applikation holen */
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_PROGRAMM,
                          sizeof (sz),
                          sz) ;
           /* Text in Titelzeile ausgeben */
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz) ;
           /* Client Fenstergroesse ermitteln */
           WinQueryWindowPos (hwndClient_g,
                              &swp) ;
           /* Container im linken Viertel anlegen */
           pwnd->hwndContainer = WinCreateWindow (hwndClient_g,
                                                  WC_CONTAINER,
                                                  "",
                                                  CCS_AUTOPOSITION | CCS_MINIICONS | CCS_MINIRECORDCORE | CCS_SINGLESEL | WS_GROUP | WS_VISIBLE,
                                                  0,
                                                  0,
                                                  swp.cx / 3,
                                                  swp.cy,
                                                  hwndFrame_g,
                                                  HWND_TOP,
                                                  IDCO_WEBSEITEN,
                                                  NULL,
                                                  NULL) ;
           /* Font fuer den Container bestimmen */
           WinSetPresParam (pwnd->hwndContainer,
                            PP_FONTNAMESIZE,
                            sizeof ("8.Helv"),
                            "8.Helv") ;
           /* Container Informationsblock loeschen */
           memset (&ccinfo,
                   0,
                   sizeof (CNRINFO)) ;
           /* Einige Container Flags */
           ccinfo.flWindowAttr = CA_TITLEREADONLY | CA_TITLESEPARATOR | CA_TREELINE | CV_MINI | CV_TEXT | CV_TREE ;
           /* Container Informationsblock uebergeben */
           WinSendMsg (pwnd->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_CNRTITLE | CMA_FLWINDOWATTR)) ;
           /* WEBExplorer DLL laden */
           DosLoadModule (sz,
                          sizeof (sz),
                          "WEBEXWIN",
                          &pwnd->hmodWEB) ;
           /* Ein WEBExplorer Fenster anlegen */
           pwnd->hwndWEB = WinCreateWindow (hwndClient_g,
                                            WC_WXVIEW,
                                            "",
                                            WS_VISIBLE,
                                            swp.cx / 4,
                                            0,
                                            (swp.cx / 4) * 3,
                                            swp.cy,
                                            hwndFrame_g,
                                            HWND_TOP,
                                            IDWEB_SEITE,
                                            NULL,
                                            NULL) ;
           /* Dokukument drucken kann jetzt noch nicht moeglich sein */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTDRUCKEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           /* Containerinhalte lesen */
           WinPostMsg (hwnd,
                       WMP_REREAD,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* Container Inhalte lesen */
      case WMP_REREAD:
           {
           CHAR         sz [3 * CCHMAXPATH] ;
           FILE*        pfile ;
           PUSRREC      pusrrec ;
           PWND         pwnd ;
           RECORDINSERT recins ;
           ULONG        ulKey ;
           ULONG        ulKeyParent ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Alle alten Eintraege entfernen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_REMOVERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_FREE | CMA_INVALIDATE)) ;
           /* WEB Gruppen Datei oeffnen */
           if   ((pfile = fopen ("WEBEXG.ASC",
                                 "r")) != NULL)
                {
                /* Solange nicht EOF */
                while (!feof (pfile))
                   {
                   /* Eine Zeile lesen */
                   if   (fgets (sz,
                                sizeof (sz) - 1,
                                pfile))
                        {
                        /* Ist die Zeile gueltig? */
                        if   ((*sz != ' ') &&
                              (strlen (sz) > 4))
                             {
                             /* Container Record allokieren */
                             pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                CM_ALLOCRECORD,
                                                                MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                                MPFROMSHORT (1))) ;
                             /* Gelesene Zeile analysieren */
                             sscanf (sz,
                                     "%u;%u;%[^\n]",
                                     &pusrrec->ulKey,
                                     &ulKeyParent,
                                     pusrrec->szName) ;
                             /* Pointer des Namens festlegen */
                             pusrrec->minirec.pszIcon = pusrrec->szName ;
                             /* Wenn Parent = 0 */
                             if   (!ulKeyParent)
                                  {
                                  /* dann ist dies der Root Eintrag */
                                  pusrrec->recordtype = K_RECORDROOT ;
                                  }
                             else
                                  {
                                  /* sonst ist es eine WEB Gruppe */
                                  pusrrec->recordtype = K_RECORDAREA ;
                                  }
                             /* Container Record einfuegen */
                             memset (&recins,
                                     0,
                                     sizeof (RECORDINSERT)) ;
                             recins.cb = sizeof (RECORDINSERT) ;
                             recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                             /* Nach Parent Record suchen */
                             recins.pRecordParent = (PRECORDCORE) SearchRecord (pwnd->hwndContainer,
                                                                                NULL,
                                                                                K_RECORDAREA,
                                                                                ulKeyParent) ;
                             recins.zOrder = CMA_TOP ;
                             recins.cRecordsInsert = 1 ;
                             /* Hier erfolgt das eigentliche Einfuegen */
                             WinSendMsg (pwnd->hwndContainer,
                                         CM_INSERTRECORD,
                                         MPFROMP (pusrrec),
                                         MPFROMP (&recins)) ;
                             }
                        }
                   }
                /* Datei wieder schliessen */
                fclose (pfile) ;
                }
           /* WEB Seiten Datei oeffnen */
           if   ((pfile = fopen ("WEBEXP.ASC",
                                 "r")) != NULL)
                {
                /* Solange nicht EOF */
                while (!feof (pfile))
                   {
                   /* Eine Zeile lesen */
                   if   (fgets (sz,
                                sizeof (sz) - 1,
                                pfile))
                        {
                        /* Ist die Zeile gueltig? */
                        if   ((*sz != ' ') &&
                              (strlen (sz) > 6))
                             {
                             /* Container Record allokieren */
                             pusrrec = PVOIDFROMMR (WinSendMsg (pwnd->hwndContainer,
                                                                CM_ALLOCRECORD,
                                                                MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                                MPFROMSHORT (1))) ;
                             /* Gelesene Zeile analysieren */
                             sscanf (sz,
                                     "%u;%u;%[^;];%[^\n]",
                                     &ulKey,
                                     &ulKeyParent,
                                     pusrrec->szName,
                                     pusrrec->szURL) ;
                             /* Pointer des Namens festlegen */
                             pusrrec->minirec.pszIcon = pusrrec->szName ;
                             /* Dies ist eine WEB Seite */
                             pusrrec->recordtype = K_RECORDITEM ;
                             /* Container Record einfuegen */
                             memset (&recins,
                                     0,
                                     sizeof (RECORDINSERT)) ;
                             recins.cb = sizeof (RECORDINSERT) ;
                             recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                             /* Nach Parent Record suchen */
                             recins.pRecordParent = (PRECORDCORE) SearchRecord (pwnd->hwndContainer,
                                                                                NULL,
                                                                                K_RECORDAREA,
                                                                                ulKeyParent) ;
                             recins.zOrder = CMA_TOP ;
                             recins.cRecordsInsert = 1 ;
                             /* Hier erfolgt das eigentliche Einfuegen */
                             WinSendMsg (pwnd->hwndContainer,
                                         CM_INSERTRECORD,
                                         MPFROMP (pusrrec),
                                         MPFROMP (&recins)) ;
                             }
                        }
                   }
                /* Datei wieder schliessen */
                fclose (pfile) ;
                }
           /* Container jetzt neu zeichnen lassen */
           WinSendMsg (pwnd->hwndContainer,
                       CM_INVALIDATERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_TEXTCHANGED)) ;
           return (MRESULT) FALSE ;
           }
      /* Verzoegerte Groessenaenderung des Clients hier */
      case WMP_SIZE:
           {
           PWND pwnd ;
           SWP  swp ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Nur wenn Fenster nicht minimiert */
           if   (!fMinimized_g)
                {
                /* Client Masse ermitteln */
                WinQueryWindowPos (hwndClient_g,
                                   &swp) ;
                /* Container neu positionieren */
                WinSetWindowPos (pwnd->hwndContainer,
                                 HWND_TOP,
                                 0,
                                 0,
                                 swp.cx / 4,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                /* WEB Fenster neu positionieren */
                WinSetWindowPos (pwnd->hwndWEB,
                                 HWND_TOP,
                                 swp.cx / 4,
                                 0,
                                 (swp.cx / 4) * 3,
                                 swp.cy,
                                 SWP_MOVE | SWP_SIZE) ;
                }
           return (MRESULT) FALSE ;
           }
      /* WEB Fenster meldet einen Mausklick */
      case WX_BUTTONCLICK:
           {
           CHAR          sz [CCHMAXPATH] ;
           PFN           pfn ;
           PWND          pwnd ;
           PWXBUTTONDATA pwxbuttondata ;
           WXPOS         wxpos ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Buttonklick Struktur holen */
           pwxbuttondata = PVOIDFROMMP (mp2) ;
           /* Position des Mausklicks ermitteln */
           if   (!WXViewQueryPos (pwnd->hwndWEB,
                                  pwxbuttondata->x,
                                  pwxbuttondata->y,
                                  &wxpos))
                {
                /* Ist es eine Link? */
                if   (!WXViewQueryAnchor (pwnd->hwndWEB,
                                          &wxpos,
                                          sz,
                                          sizeof (sz)))
                     {
                     /* Ja */
                     /* Da Compiler bei diesem Statement immer abstuerzt wird Prozedur dynamisch aus DLL geladen */
                     if   (!DosQueryProcAddr (pwnd->hmodWEB,
                                              0,
                                              "WXViewLoad",
                                              &pfn))
                          {
/*                        WXViewLoad (pwnd->hwndWEB,
                                      sz,
                                      WXLOAD_FORCERELOAD,
                                      NULL) ; */
                          typedef APIRET (PFXN) (HWND, PSZ, ULONG, PVOID) ;
                          (pfn) (pwnd->hwndWEB,
                                 sz,
                                 WXLOAD_FORCERELOAD,
                                 NULL) ;
                          }
                     }
                }
           return (MRESULT) FALSE ;
           }
      /* WEB Fenster meldet das Ende eines Ladeprozesses */
      case WX_LOADRESULT:
           {
           PWND pwnd ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Flag freigeben */
           pwnd->fViewActive = FALSE ;
           /* Die Menueintraege jetzt wieder freigeben */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTLADEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTINDATEILADEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTLADENSTOPPEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTDRUCKEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           return (MRESULT) FALSE ;
           }
      /* WEB Fenster meldet einen aktiven Ladeprozess */
      case WX_LOADSTATUS:
           {
           PWND pwnd ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Flag sperren */
           pwnd->fViewActive = TRUE ;
           /* Manche Menueeintraege sperren */
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTLADEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTINDATEILADEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED)) ;
           WinSendMsg (pwnd->hwndMenu,
                       MM_SETITEMATTR,
                       MPFROM2SHORT (IDM_DOKUMENTLADENSTOPPEN, TRUE),
                       MPFROM2SHORT (MIA_DISABLED, FALSE)) ;
           return (MRESULT) FALSE ;
           }
      /* WEB Fenster meldet das Ende eines Druckes */
      case WX_PRINTRESULT:
           {
           PWND pwnd ;

           /* Instanzdaten aus den Window Words holen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Printer Driver Daten freigeben */
           free (pwnd->wxprintinfo.pDriverData) ;
           pwnd->wxprintinfo.pDriverData = NULL ;
           return (MRESULT) FALSE ;
           }
      }
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

   /* Anmeldung beim PM */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Applikationsklasse registrieren */
             if   (WinRegisterClass (hab_g,
                                     pszWEBEx_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszWEBEx_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_WEBEX,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Loop */
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
                       /* Fenster wieder loeschen */
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

/* Dialogfensterprozedur des Area Einfuegen Dialoges */
MRESULT EXPENTRY EinfuegenAreaDlgWndProc (HWND   hwndDlg,
                                          MSG    msg,
                                          MPARAM mp1,
                                          MPARAM mp2)
   {
   /* Welcher Event ist von Interesse? */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           PDLGEG       pdlg ;
           PUSRREC      pusrrec ;
           PUSRREC      pusrrec2 ;
           RECORDINSERT recins ;

           /* Instanzdaten aus den Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel gedrueckt */
              case DID_CANCEL:
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok gedrueckt */
              case DID_OK:
                   /* Welcher Record hat Emphasis? */
                   if   ((pusrrec = PVOIDFROMMR (WinSendMsg (pdlg->pwnd->hwndContainer,
                                                             CM_QUERYRECORDEMPHASIS,
                                                             MPFROMLONG ((PRECORDCORE) CMA_FIRST),
                                                             MPFROMSHORT (CRA_SELECTED)))) != NULL)
                        {
                        /* Neuen Record allokieren */
                        pusrrec2 = PVOIDFROMMR (WinSendMsg (pdlg->pwnd->hwndContainer,
                                                            CM_ALLOCRECORD,
                                                            MPFROMLONG (sizeof (USRREC) - sizeof (MINIRECORDCORE)),
                                                            MPFROMSHORT (1))) ;
                        /* Eingabefeld auslesen */
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_EG_1,
                                             sizeof (pusrrec2->szName),
                                             pusrrec2->szName) ;
                        /* Pointer setzen */
                        pusrrec2->minirec.pszIcon = pusrrec2->szName ;
                        /* Record ist WEB Gruppe */
                        pusrrec2->recordtype = K_RECORDAREA ;
                        /* Record einfuegen */
                        memset (&recins,
                                0,
                                sizeof (RECORDINSERT)) ;
                        recins.cb = sizeof (RECORDINSERT) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        /* WEB Gruppe ist Parent */
                        recins.pRecordParent = (PRECORDCORE) pusrrec ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        /* Direkt invalidieren */
                        recins.fInvalidateRecord = TRUE ;
                        /* Record nun drin */
                        WinSendMsg (pdlg->pwnd->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec2),
                                    MPFROMP (&recins)) ;
                        }
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Fenster wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten besorgen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGEG pdlg ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGEG)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Pointer auf Client Instanzdaten wurde uebergeben. In Dialog Instanzdaten ablegen */
           pdlg->pwnd = PVOIDFROMMP (mp2) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialogfensterprozedur des Cache Optionen Dialoges */
MRESULT EXPENTRY OptionenCacheDlgWndProc (HWND   hwndDlg,
                                          MSG    msg,
                                          MPARAM mp1,
                                          MPARAM mp2)
   {
   /* Welcher Event ist von Interesse? */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           PDLGOC pdlg ;
           SHORT  s ;

           /* Instanzdaten aus den Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel gedrueckt */
              case DID_CANCEL:
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok gedrueckt */
              case DID_OK:
                   /* Generischer Cache An/Aus */
                   pdlg->wxcacheopts.bEnabled = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                                 IDH_OC_1,
                                                                                 BM_QUERYCHECK,
                                                                                 MPVOID,
                                                                                 MPVOID)) ;
                   /* Image Caching An/Aus */
                   pdlg->wxcacheopts.bMemoryImageCacheing = SHORT1FROMMR (WinSendDlgItemMsg (hwndDlg,
                                                                                             IDH_OC_2,
                                                                                             BM_QUERYCHECK,
                                                                                             MPVOID,
                                                                                             MPVOID)) ;
                   /* Anzahl Docs im Cache */
                   WinQueryDlgItemShort (hwndDlg,
                                         IDE_OC_1,
                                         &s,
                                         FALSE) ;
                   pdlg->wxcacheopts.lDocLimit = s ;
                   /* Anzahl Images im Cache */
                   WinQueryDlgItemShort (hwndDlg,
                                         IDE_OC_2,
                                         &s,
                                         FALSE) ;
                   pdlg->wxcacheopts.lImageLimit = s ;
                   /* Cache Optionen des WEBExplorer setzen */
                   WXViewSetCacheOpts (pdlg->pwnd->hwndWEB,
                                       &pdlg->wxcacheopts,
                                       sizeof (WXCACHEOPTS)) ;
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Fenster wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten besorgen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           BOOL   f ;
           PDLGOC pdlg ;
           PSZ    psz ;
           ULONG  ul ;
           ULONG  ul2 ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGOC)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Pointer auf Client Instanzdaten wurde uebergeben. In Dialog Instanzdaten ablegen */
           pdlg->pwnd = PVOIDFROMMP (mp2) ;
           /* WEBExplorer Cache Optionen lesen */
           if   (!WXViewQueryCacheOpts (pdlg->pwnd->hwndWEB,
                                        &pdlg->wxcacheopts,
                                        sizeof (WXCACHEOPTS)))
                {
                /* Generische Cache Optionen anzeigen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDH_OC_1,
                                   BM_SETCHECK,
                                   MPFROMSHORT (pdlg->wxcacheopts.bEnabled),
                                   MPVOID) ;
                /* Image Cache Optionen anzeigen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDH_OC_2,
                                   BM_SETCHECK,
                                   MPFROMSHORT (pdlg->wxcacheopts.bMemoryImageCacheing),
                                   MPVOID) ;
                /* Anzahl Docs im Cache anzeigen */
                WinSetDlgItemShort (hwndDlg,
                                    IDE_OC_1,
                                    pdlg->wxcacheopts.lDocLimit,
                                    FALSE) ;
                /* Anzahl Images im Cache anzeigen */
                WinSetDlgItemShort (hwndDlg,
                                    IDE_OC_2,
                                    pdlg->wxcacheopts.lImageLimit,
                                    FALSE) ;
                }
           /* Ermittlung eines temporaeren Verzeichnissses */
           f = FALSE ;
           /* Erst TMP versuchen */
           if   (!DosScanEnv ("TMP",
                              &psz))
                {
                f = TRUE ;
                }
           else
                {
                /* Wenn kein Glueck dann TEMP versuchen */
                if   (!DosScanEnv ("TEMP",
                                   &psz))
                     {
                     f = TRUE ;
                     }
                }
           if   (!f)
                {
                /* Wenn immer noch kein Glueck dann das Root des Bootlaufwerks */
                DosQueryCurrentDisk (&ul,
                                     &ul2) ;
                pdlg->wxcacheopts.szCacheDir [0] = (CHAR) ul + '@' ;
                pdlg->wxcacheopts.szCacheDir [1] = ':' ;
                pdlg->wxcacheopts.szCacheDir [2] = '\\' ;
                ul2 = sizeof (pdlg->wxcacheopts.szCacheDir) - 3 ;
                DosQueryCurrentDir (ul,
                                    &pdlg->wxcacheopts.szCacheDir [3],
                                    &ul2) ;
                }
           else
                {
                /* Ergebnis umkopieren */
                strcpy (pdlg->wxcacheopts.szCacheDir,
                        psz) ;
                }
           /* Temporaeres Verzeichnis ausgeben */
           WinSetDlgItemText (hwndDlg,
                              IDE_OC_3,
                              pdlg->wxcacheopts.szCacheDir) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialogfensterprozedur des Display Optionen Dialoges */
MRESULT EXPENTRY OptionenDisplayDlgWndProc (HWND   hwndDlg,
                                            MSG    msg,
                                            MPARAM mp1,
                                            MPARAM mp2)
   {
   /* Welcher Event ist von Interesse? */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           CHAR        sz [CCHMAXPATH] ;
           FONTDLG     fntdlg ;
           FONTMETRICS fm ;
           PDLGOD      pdlg ;

           /* Instanzdaten aus den Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel gedrueckt */
              case DID_CANCEL:
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Ok gedrueckt */
              case DID_OK:
                   /* Display Optionen des WEBExplorer setzen */
                   WXViewSetDisplayOpts (pdlg->pwnd->hwndWEB,
                                         &pdlg->wxdisplayopts,
                                         sizeof (WXDISPLAYOPTS)) ;
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  DID_OK) ;
                   return (MRESULT) FALSE ;
              /* Text Farbe soll ausgewaehlt werden */
              case IDP_OD_1:
                   /* Dialog zur Farbauswahl ausgeben und Ergebnis in Dialog Optionen Struktur merken */
                   if   ((pdlg->wxdisplayopts.lClrText = WinDlgBox (HWND_DESKTOP,
                                                                    hwndFrame_g,
                                                                    OptionenFarbeDlgWndProc,
                                                                    NULLHANDLE,
                                                                    IDD_OPTIONENFARBE,
                                                                    NULL)) != 9999)
                        {
                        /* Gewaehlte Farbe in Presentation Parameter setzen */
                        WinSetPresParam (WinWindowFromID (hwndDlg,
                                                          IDT_OD_2),
                                         PP_BACKGROUNDCOLORINDEX,
                                         sizeof (pdlg->wxdisplayopts.lClrText),
                                         &pdlg->wxdisplayopts.lClrText) ;
                        }
                   return (MRESULT) FALSE ;
              /* Link Farbe soll ausgewaehlt werden */
              case IDP_OD_2:
                   /* Dialog zur Farbauswahl ausgeben und Ergebnis in Dialog Optionen Struktur merken */
                   if   ((pdlg->wxdisplayopts.lClrAnchor = WinDlgBox (HWND_DESKTOP,
                                                                      hwndFrame_g,
                                                                      OptionenFarbeDlgWndProc,
                                                                      NULLHANDLE,
                                                                      IDD_OPTIONENFARBE,
                                                                      NULL)) != 9999)
                        {
                        /* Gewaehlte Farbe in Presentation Parameter setzen */
                        WinSetPresParam (WinWindowFromID (hwndDlg,
                                                          IDT_OD_4),
                                         PP_BACKGROUNDCOLORINDEX,
                                         sizeof (pdlg->wxdisplayopts.lClrAnchor),
                                         &pdlg->wxdisplayopts.lClrAnchor) ;
                        }
                   return (MRESULT) FALSE ;
              /* Farbe von gesichteten Links soll ausgewaehlt werden */
              case IDP_OD_3:
                   /* Dialog zur Farbauswahl ausgeben und Ergebnis in Dialog Optionen Struktur merken */
                   if   ((pdlg->wxdisplayopts.lClrVisitedAnchor = WinDlgBox (HWND_DESKTOP,
                                                                             hwndFrame_g,
                                                                             OptionenFarbeDlgWndProc,
                                                                             NULLHANDLE,
                                                                             IDD_OPTIONENFARBE,
                                                                             NULL)) != 9999)
                        {
                        /* Gewaehlte Farbe in Presentation Parameter setzen */
                        WinSetPresParam (WinWindowFromID (hwndDlg,
                                                          IDT_OD_6),
                                         PP_BACKGROUNDCOLORINDEX,
                                         sizeof (pdlg->wxdisplayopts.lClrVisitedAnchor),
                                         &pdlg->wxdisplayopts.lClrVisitedAnchor) ;
                        }
                   return (MRESULT) FALSE ;
              /* Hintergrund Farbe soll ausgewaehlt werden */
              case IDP_OD_4:
                   /* Dialog zur Farbauswahl ausgeben und Ergebnis in Dialog Optionen Struktur merken */
                   if   ((pdlg->wxdisplayopts.lClrBackground = WinDlgBox (HWND_DESKTOP,
                                                                          hwndFrame_g,
                                                                          OptionenFarbeDlgWndProc,
                                                                          NULLHANDLE,
                                                                          IDD_OPTIONENFARBE,
                                                                          NULL)) != 9999)
                        {
                        /* Gewaehlte Farbe in Presentation Parameter setzen */
                        WinSetPresParam (WinWindowFromID (hwndDlg,
                                                          IDT_OD_8),
                                         PP_BACKGROUNDCOLORINDEX,
                                         sizeof (pdlg->wxdisplayopts.lClrBackground),
                                         &pdlg->wxdisplayopts.lClrBackground) ;
                        }
                   return (MRESULT) FALSE ;
              /* Font soll ausgewaehlt werden */
              case IDP_OD_5:
                   /* Kommunikationsstruktur des Fontdialoges loeschen */
                   memset (&fntdlg,
                           0,
                           sizeof (FONTDLG)) ;
                   fntdlg.cbSize = sizeof (FONTDLG) ;
                   /* Aktuellen HPS besorgen */
                   fntdlg.hpsScreen = WinGetPS (hwndFrame_g) ;
                   /* Logischen Font abfragen */
                   GpiQueryLogicalFont (fntdlg.hpsScreen,
                                        0,
                                        (PSTR8) fntdlg.fAttrs.szFacename,
                                        &fntdlg.fAttrs,
                                        sizeof (fntdlg.fAttrs)) ;
                   /* Fontmetrics lesen */
                   GpiQueryFontMetrics (fntdlg.hpsScreen,
                                        sizeof (fm),
                                        &fm) ;
                   fntdlg.pszFamilyname = sz ;
                   fntdlg.fl = FNTS_BITMAPONLY | FNTS_CENTER ;
                   fntdlg.flType = fm.fsType ;
                   fntdlg.clrFore = CLR_BLACK ;
                   fntdlg.clrBack = CLR_WHITE ;
                   fntdlg.sNominalPointSize = fm.sNominalPointSize ;
                   fntdlg.usWeight = fm.usWeightClass ;
                   fntdlg.usWidth = fm.usWidthClass ;
                   fntdlg.usFamilyBufLen = sizeof (sz) ;
                   if (WinFontDlg (HWND_DESKTOP,
                                   hwndFrame_g,
                                   &fntdlg) &&
                       (fntdlg.lReturn == DID_OK))
                      {
                      /* Gewaehlten Font ausgeben */
                      WinSetDlgItemText (hwndDlg,
                                         IDE_OD_1,
                                         sz) ;
                      }
                   /* Presentation Space wieder freigeben */
                   WinReleasePS (fntdlg.hpsScreen) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Fenster wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten besorgen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGOD pdlg ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGOD)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Pointer auf Client Instanzdaten wurde uebergeben. In Dialog Instanzdaten ablegen */
           pdlg->pwnd = PVOIDFROMMP (mp2) ;
           /* WEBExplorer Display Optionen lesen */
           if   (!WXViewQueryDisplayOpts (pdlg->pwnd->hwndWEB,
                                          &pdlg->wxdisplayopts,
                                          sizeof (WXDISPLAYOPTS)))
                {
                /* Font ausgeben */
                WinSetDlgItemText (hwndDlg,
                                   IDE_OD_1,
                                   pdlg->wxdisplayopts.szFontName) ;
                /* Farbe der Texte im Presentation Parameter setzen */
                WinSetPresParam (WinWindowFromID (hwndDlg,
                                                  IDT_OD_2),
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (pdlg->wxdisplayopts.lClrText),
                                 &pdlg->wxdisplayopts.lClrText) ;
                /* Farbe der Links im Presentation Parameter setzen */
                WinSetPresParam (WinWindowFromID (hwndDlg,
                                                  IDT_OD_4),
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (pdlg->wxdisplayopts.lClrAnchor),
                                 &pdlg->wxdisplayopts.lClrAnchor) ;
                /* Farbe der gesichteten Links im Presentation Parameter setzen */
                WinSetPresParam (WinWindowFromID (hwndDlg,
                                                  IDT_OD_6),
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (pdlg->wxdisplayopts.lClrVisitedAnchor),
                                 &pdlg->wxdisplayopts.lClrVisitedAnchor) ;
                /* Farbe des Hintergrundes im Presentation Parameter setzen */
                WinSetPresParam (WinWindowFromID (hwndDlg,
                                                  IDT_OD_8),
                                 PP_BACKGROUNDCOLORINDEX,
                                 sizeof (pdlg->wxdisplayopts.lClrBackground),
                                 &pdlg->wxdisplayopts.lClrBackground) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Dialogfensterprozedur des Farb Optionen Dialoges */
MRESULT EXPENTRY OptionenFarbeDlgWndProc (HWND   hwndDlg,
                                          MSG    msg,
                                          MPARAM mp1,
                                          MPARAM mp2)
   {
   /* Welcher Event ist von Interesse? */
   switch (msg)
      {
      /* Benutzeraktion */
      case WM_COMMAND:
           {
           LONG   l ;
           PDLGOF pdlg ;

           /* Instanzdaten aus den Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welche Benutzeraktion? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel gedrueckt */
              case DID_CANCEL:
                   /* Dialog verlassen */
                   WinDismissDlg (hwndDlg,
                                  9999) ;
                   return (MRESULT) FALSE ;
              /* Ok gedrueckt */
              case DID_OK:
                   /* Gewaehltes Item im Valueset abfragen */
                   l = LONGFROMMR (WinSendMsg (pdlg->hwndValueset,
                                               VM_QUERYSELECTEDITEM,
                                               MPVOID,
                                               MPVOID)) ;
                   /* Wert des gewaehlten Eintrags im Valueset abfragen */
                   l = LONGFROMMR (WinSendMsg (pdlg->hwndValueset,
                                               VM_QUERYITEM,
                                               MPFROMLONG (l),
                                               MPVOID)) ;
                   /* Dialog verlassen und Wert zurueckliefern */
                   WinDismissDlg (hwndDlg,
                                  l) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Control meldet einen Event */
      case WM_CONTROL:
           /* Welches Control? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Das Valueset */
              case IDVS_OF_1:
                   /* Welchen Event */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Enter gedrueckt */
                      case VN_ENTER:
                           /* Verarbeitung durch simulierten WM_COMMAND ausloesen */
                           WinPostMsg (hwndDlg,
                                       WM_COMMAND,
                                       MPFROMSHORT (DID_OK),
                                       MPFROM2SHORT (CMDSRC_PUSHBUTTON, TRUE)) ;
                           break ;
                      }
                   break ;
              }
           break ;
      /* Fenster wird geschlossen */
      case WM_DESTROY:
           {
           PDLGOF pdlg ;

           /* Instanzdaten aus den Window Words holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Valueset Fenster schliessen */
           WinDestroyWindow (pdlg->hwndValueset) ;
           /* Instanzdaten freigeben */
           free (pdlg) ;
           break ;
           }
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           LONG    l ;
           PDLGOF  pdlg ;
           SWP     swp ;
           ULONG   ulI ;
           ULONG   ulJ ;
           VSCDATA vscd ;

           /* Platz fuer Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGOF)) ;
           /* Pointer auf Instanzdaten in WindowWords ablegen */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Kommunikationsstruktur zum Valueset loeschen */
           memset (&vscd,
                   0,
                   sizeof (VSCDATA)) ;
           vscd.cbSize = sizeof (vscd) ;
           /* Anzahl Reihen */
           vscd.usRowCount = 2 ;
           /* Anzahl Spalten */
           vscd.usColumnCount = 8 ;
           /* Dialogmasse ermitteln */
           WinQueryWindowPos (hwndDlg,
                              &swp) ;
           /* Valueset auf Dialog positionieren */
           pdlg->hwndValueset = WinCreateWindow (hwndDlg,
                                                 WC_VALUESET,
                                                 "",
                                                 VS_BORDER | VS_COLORINDEX | WS_VISIBLE,
                                                 18,
                                                 60,
                                                 swp.cx - 36,
                                                 swp.cy - 100,
                                                 hwndDlg,
                                                 HWND_TOP,
                                                 IDVS_OF_1,
                                                 &vscd,
                                                 NULL) ;
           /* In einer Schleife alle Reihen */
           for  (ulI = 1, l = CLR_BACKGROUND; ulI <= 2; ulI++)
                {
                /* In einer Schleife alle Spalten */
                for  (ulJ = 1; ulJ < 8; ulJ++, l++)
                     {
                     /* Wert des Valueset Eintrags bestimmen */
                     WinSendMsg (pdlg->hwndValueset,
                                 VM_SETITEM,
                                 MPFROM2SHORT (ulI, ulJ),
                                 MPFROMLONG (l)) ;
                     }
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Aufruf der Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Rekursive Funktion zum Suchen nach einem bestimmten Container Records */
PUSRREC SearchRecord (HWND       hwndContainer,
                      PUSRREC    pusrrec,
                      RECORDTYPE recordtype,
                      ULONG      ulKey)
   {
   PUSRREC pusrrecChild ;
   PUSRREC pusrrecResult ;
   PUSRREC pusrrecTemp ;
   USHORT  us ;

   /* Startecord von dem aus gesucht werden soll, kann auch NULL sein fuer ersten */
   pusrrecTemp = pusrrec ;
   /* Immer einen Ast pro Rekursion */
   us = CMA_FIRSTCHILD ;
   /* Endlosschleife */
   while (TRUE)
      {
      /* Erstes Element des Astes ermitteln, ist da ueberhaupt noch ein Element? */
      if   ((pusrrecTemp = PVOIDFROMMR (WinSendMsg (hwndContainer,
                                                    CM_QUERYRECORD,
                                                    MPFROMP (pusrrecTemp),
                                                    MPFROM2SHORT (us, CMA_ITEMORDER)))) == NULL)
           {
           /* Nein, Ast beenden */
           break ;
           }
      /* Wenn nach einer WEB Gruppe gesucht wird, muss gleichzeitig auch das Root eingeschlossen werden */
      if   (recordtype == K_RECORDAREA)
           {
           /* Ist gefundener Record vom Typ Root? */
           if   (pusrrecTemp->recordtype == K_RECORDROOT)
                {
                /* Ja, wird nach einem Schlssel gesucht? */
                if   (ulKey)
                     {
                     /* Ja, stimmt der Schluessel? */
                     if   (pusrrecTemp->ulKey == ulKey)
                          {
                          /* Ja, dann mit gefundenem Record die Rekursion abbrechen */
                          return pusrrecTemp ;
                          }
                     }
                }
           /* Ist gefundener Record vom Typ WEB Gruppe? */
           if   (pusrrecTemp->recordtype == K_RECORDAREA)
                {
                /* Ja, wird nach einem Schlssel gesucht? */
                if   (ulKey)
                     {
                     /* Ja, stimmt der Schluessel? */
                     if   (pusrrecTemp->ulKey == ulKey)
                          {
                          /* Ja, dann mit gefundenem Record die Rekursion abbrechen */
                          return pusrrecTemp ;
                          }
                     }
                }
           }
      else
           {
           /* Stimmt Record Typ der Suche mit dem Record ueberein? */
           if   (pusrrecTemp->recordtype == recordtype)
                {
                /* Ja, wird nach einem Schlssel gesucht? */
                if   (ulKey)
                     {
                     /* Ja, stimmt der Schluessel? */
                     if   (pusrrecTemp->ulKey == ulKey)
                          {
                          /* Ja, dann mit gefundenem Record die Rekursion abbrechen */
                          return pusrrecTemp ;
                          }
                     }
                }
           }
      /* Hat der gefundene Record weitere Kinder (neuer Ast)? */
      if   ((pusrrecChild = PVOIDFROMMR (WinSendMsg (hwndContainer,
                                                     CM_QUERYRECORD,
                                                     MPFROMP (pusrrecTemp),
                                                     MPFROM2SHORT (CMA_FIRSTCHILD, CMA_ITEMORDER)))) != NULL)
           {
           /* Ja, weitere Rekursion fuer diesen Ast starten */
           if   ((pusrrecResult = SearchRecord (hwndContainer,
                                                pusrrecTemp,
                                                recordtype,
                                                ulKey)) != NULL)
                {
                /* Wenn Rueckgabewert ein Record, dann die Rekursion mit gefundenem Record abbrechen */
                return pusrrecResult ;
                }
           }
      /* Naechstes Element des gleichen Astes suchen */
      us = CMA_NEXT ;
      }
   /* In diesem Ast kein Record gefunden, nur diese Rekursion abbrechen */
   return NULL ;
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

/* Hilfsfunktion zum Ermitteln des Default Queue Namen */
BOOL TQueryDefaultQueueName (PSZ psz)
   {
   BOOL  f ;
   ULONG ul ;

   f = TERROR ;
   /* INIs abfragen */
   if   ((ul = PrfQueryProfileString (HINI_PROFILE,
                                      "PM_SPOOLER",
                                      "QUEUE",
                                      NULL,
                                      psz,
                                      CCHMAXPATH)) != 0)
        {
        /* Queue Name extrahieren */
        psz [ul - 2] = 0 ;
        f = TNOERROR ;
        }
   return f ;
   }

/* Funktion zum Abfragen von Job Properties */
ULONG TQueryJobProperties (HAB   hab,
                           PVOID pvDriverData,
                           PSZ   pszPDriverName,
                           PSZ   pszPQueueName,
                           PSZ   pszPPrinterName,
                           BOOL  f)
   {
   BOOL  fFound ;
   CHAR  szDeviceName [CCHMAXPATH] ;
   CHAR  szDriverName [CCHMAXPATH] ;
   LONG  l ;
   PCHAR pch ;
   PSZ   pszDriver ;
   PSZ   pszDriverInfo ;
   PSZ   pszPrinterName ;
   PSZ   pszPrinterNames ;
   PSZ   pszPrinterInfo ;
   PSZ   pszQueue ;
   PSZ   pszQueueInfo ;
   ULONG ul ;
   ULONG ulOptions ;

   ul = 0 ;
   pszPrinterNames = NULL ;
   ulOptions = (f) ? DPDM_POSTJOBPROP : DPDM_QUERYJOBPROP ;
   if   ((pch = strchr (pszPDriverName,
                        '.')) != NULL)
        {
        strncpy (szDriverName,
                 pszPDriverName,
                 pch - pszPDriverName) ;
        szDriverName [pch - pszPDriverName] = '\0' ;
        pch++ ;
        strcpy (szDeviceName,
                pch) ;
        }
   else
        {
        strcpy (szDriverName,
                pszPDriverName) ;
        *szDeviceName = '\0' ;
        pch = pszPDriverName ;
        }
   if   (*pch)
        {
        /* INIs abfragen */
        PrfQueryProfileSize (HINI_PROFILE,
                             "PM_SPOOLER_PRINTER",
                             NULL,
                             &ul) ;
        pszPrinterNames = calloc (1,
                                  ul) ;
        /* INIs abfragen */
        PrfQueryProfileString (HINI_PROFILE,
                               "PM_SPOOLER_PRINTER",
                               NULL,
                               NULL,
                               pszPrinterNames,
                               ul) ;
        for  (pszPrinterName = pszPrinterNames, fFound = FALSE; *pszPrinterName; pszPrinterName = &pszPrinterName [strlen (pszPrinterName) + 1])
             {
             /* INIs abfragen */
             PrfQueryProfileSize (HINI_PROFILE,
                                  "PM_SPOOLER_PRINTER",
                                  pszPrinterName,
                                  &ul) ;
             pszPrinterInfo = calloc (1,
                                      ul) ;
             /* INIs abfragen */
             PrfQueryProfileString (HINI_PROFILE,
                                    "PM_SPOOLER_PRINTER",
                                    pszPrinterName,
                                    NULL,
                                    pszPrinterInfo,
                                    ul) ;
             pszDriverInfo = &pszPrinterInfo [strcspn (pszPrinterInfo,
                                                       ";") + 1] ;
             pszQueueInfo  = &pszDriverInfo [strcspn (pszDriverInfo,
                                                      ";") + 1] ;
             pszDriverInfo [strcspn (pszDriverInfo,
                                     ";")] = '\0' ;
             pszQueueInfo [strcspn (pszQueueInfo,
                                    ";")] = '\0' ;
             pszQueue = pszQueueInfo ;
             while (pszQueue)
                {
                if   (!strncmp (pszQueue,
                                pszPQueueName,
                                strlen (pszPQueueName)))
                     {
                     pszDriver = pszDriverInfo ;
                     while (pszDriver)
                        {
                        if   (!strncmp (pszDriver,
                                        pszPDriverName,
                                        strlen (pszPDriverName)))
                             {
                             fFound = TRUE ;
                             break ;
                             }
                        if   ((pszDriver = strchr (pszDriver,
                                                   ',')) != NULL)
                             {
                             pszDriver++ ;
                             }
                        }
                     break ;
                     }
                if   ((pszQueue = strchr (pszQueue,
                                          ',')) != NULL)
                     {
                     pszQueue++ ;
                     }
                }
             free (pszPrinterInfo) ;
             if   (fFound)
                  {
                  break ;
                  }
             }
        if   (!pvDriverData)
             {
             l = DevPostDeviceModes (hab,
                                     NULL,
                                     szDriverName,
                                     szDeviceName,
                                     pszPrinterName,
                                     ulOptions) ;
             ul = (l <= 0) ? 0 : l ;
             }
        else
             {
             ul = DevPostDeviceModes (hab,
                                      pvDriverData,
                                      szDriverName,
                                      szDeviceName,
                                      pszPrinterName,
                                      ulOptions);
             if   ((pszPPrinterName) &&
                   (pszPrinterName))
                  {
                  strcpy (pszPPrinterName,
                          pszPrinterName) ;
                  }
             }
        free (pszPrinterNames) ;
        }
   return ul ;
   }

/* Hilfsfunktion zum Ermitteln des Default Queue Drivers */
BOOL TQueryQueueDriver (PSZ pszName,
                        PSZ pszDriver)
   {
   BOOL f ;

   f = TERROR ;
   /* INIs abfragen */
   if   (PrfQueryProfileString (HINI_PROFILE,
                                "PM_SPOOLER_QUEUE_DD",
                                pszName,
                                NULL,
                                pszDriver,
                                CCHMAXPATH))
        {
        /* Driver String pruefen */
        if   ((*pszDriver != ';') &&
              (*pszDriver != '\0'))
             {
             /* Queue Driver extrahieren */
             pszDriver [strcspn (pszDriver,
                                 ";")] = '\0' ;
             f = TNOERROR ;
             }
        }
   return f ;
   }

/* Ausgabe einer Messagebox mit STRINGTABLE Ressource IDs */
USHORT TWriteMessage (HWND    hwnd,
                      HMODULE hmod,
                      ULONG   ulTitle,
                      ULONG   ulText,
                      ULONG   ulStyle)
   {
   CHAR szText [256] ;
   CHAR szTitle [128] ;
   HAB hab ;

   /* Handle Anchor Block ermitteln */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Titel mit Hilfe der Ressource ID aus Ressourcen lesen */
   WinLoadString (hab,
                  hmod,
                  ulTitle,
                  sizeof (szTitle),
                  szTitle) ;
   /* Text mit Hilfe der Ressource ID aus Ressourcen lesen */
   WinLoadString (hab,
                  hmod,
                  ulText,
                  sizeof (szText),
                  szText) ;
   /* Jetzt erst die normale Message Box ausgeben */
   return WinMessageBox (HWND_DESKTOP,
                         hwnd,
                         szText,
                         szTitle,
                         0,
                         ulStyle) ;
   }

/* Rekursive Funktion zum Schreiben der beiden WEB Gruppen und WEB Seiten Dateien */
VOID WriteRecord (HWND    hwndContainer,
                  PUSRREC pusrrec,
                  FILE*   pfileG,
                  FILE*   pfileP,
                  PULONG  pulKey,
                  PULONG  pulKey2)
   {
   PUSRREC pusrrecChild ;
   PUSRREC pusrrecTemp ;
   USHORT  us ;

   /* Startecord von dem aus gesucht werden soll, kann auch NULL sein fuer ersten */
   pusrrecTemp = pusrrec ;
   /* Immer einen Ast pro Rekursion */
   us = CMA_FIRSTCHILD ;
   /* Endlosschleife */
   while (TRUE)
      {
      /* Erstes Element des Astes ermitteln, ist da ueberhaupt noch ein Element? */
      if   ((pusrrecTemp = PVOIDFROMMR (WinSendMsg (hwndContainer,
                                                    CM_QUERYRECORD,
                                                    MPFROMP (pusrrecTemp),
                                                    MPFROM2SHORT (us, CMA_ITEMORDER)))) == NULL)
           {
           /* Nein, Ast beenden */
           break ;
           }
      /* Ist Record vom Typ Root? */
      if   (pusrrecTemp->recordtype == K_RECORDROOT)
           {
           /* laufender Schluessel der Gruppen erhoehen */
           (*pulKey)++ ;
           /* Zeile ausgeben */
           fprintf (pfileG,
                    "%u;%u;%s\n",
                    *pulKey,
                    0,
                    pusrrecTemp->szName) ;
           /* Parent ID im Record hinterlegen */
           pusrrecTemp->ulKeyParent = *pulKey ;
           }
      /* Ist Record vom Typ WEB Gruppe? */
      if   (pusrrecTemp->recordtype == K_RECORDAREA)
           {
           /* laufender Schluessel der Gruppen erhoehen */
           (*pulKey)++ ;
           /* Zeile ausgeben */
           fprintf (pfileG,
                    "%u;%u;%s\n",
                    *pulKey,
                    pusrrec->ulKeyParent,
                    pusrrecTemp->szName) ;
           /* Parent ID im Record hinterlegen */
           pusrrecTemp->ulKeyParent = *pulKey ;
           }
      /* Ist Record vom Typ WEB Seite? */
      if   (pusrrecTemp->recordtype == K_RECORDITEM)
           {
           /* laufender Schluessel der Seiten erhoehen */
           (*pulKey2)++ ;
           /* Zeile ausgeben */
           fprintf (pfileP,
                    "%u;%u;%s;%s\n",
                    *pulKey2,
                    pusrrec->ulKeyParent,
                    pusrrecTemp->szName,
                    pusrrecTemp->szURL) ;
           }
      /* Hat der gefundene Record weitere Kinder (neuer Ast)? */
      if   ((pusrrecChild = PVOIDFROMMR (WinSendMsg (hwndContainer,
                                                     CM_QUERYRECORD,
                                                     MPFROMP (pusrrecTemp),
                                                     MPFROM2SHORT (CMA_FIRSTCHILD, CMA_ITEMORDER)))) != NULL)
           {
           /* Ja, weitere Rekursion fuer diesen Ast starten */
           WriteRecord (hwndContainer,
                        pusrrecTemp,
                        pfileG,
                        pfileP,
                        pulKey,
                        pulKey2) ;
           }
      /* Naechstes Element des gleichen Astes suchen */
      us = CMA_NEXT ;
      }
   }

