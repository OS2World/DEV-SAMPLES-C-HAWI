/* $Header: D:\Projekte\Redaktion Toolbox\An der langen Leine\Source\RCS\NSDDE.C 1.0 1997/08/29 16:05:38 HAWI Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikations Kopfdatei */
#include "NSDDE.H"

/* Globale Variablen */
CHAR szApplication_g [] = "NETSCAPE" ;   /* Netscape DDE Applikation */
HAB  hab_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszNSDde_g = "NSDde" ;

/* Client Fenster Prozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   /* Welcher Event? */
   switch (msg)
      {
      /* Benutzerkommando */
      case WM_COMMAND:
           {
           CONVCONTEXT cctxt ;
           PWND        pwnd ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                     QW_PARENT),
                                     QWL_USER) ;
           /* Welches Benutzerkommando? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Fenster schlie·en */
              case DID_CANCEL:
                   /* Anwendung beenden lassen */
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* Netscape soll abgeschossen werden */
              case IDM_NSDDEEXIT:
                   /* DDE Kommando merken */
                   pwnd->ulType = IDM_NSDDEEXIT ;
                   /* DDE Kommunikationsbereich leeren */
                   memset (&cctxt,
                           0,
                           sizeof (CONVCONTEXT)) ;
                   /* LÑnge des DDE Kommunikationsbereichs */
                   cctxt.cb = sizeof (CONVCONTEXT) ;
                   cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                   cctxt.idCountry = 49 ;
                   cctxt.usCodepage = 850 ;
                   /* DDE Kommunikation initiieren */
                   WinDdeInitiate (hwndClient_g,
                                   szApplication_g,
                                   "WWW_Exit",
                                   &cctxt) ;
                   return (MRESULT) FALSE ;
              /* Fensterliste von Netscape abfragen */
              case IDM_NSDDELISTWINDOWS:
                   /* Dialog hierfÅr starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              LISTWINDOWSWndProc,
                              NULLHANDLE,
                              IDD_LISTWINDOWS,
                              NULL) ;
                   return (MRESULT) FALSE ;
              /* Eine URL îffnen lassen */
              case IDM_NSDDEOPENURL:
                   /* Dialog hierfÅr starten */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              OPENURLWndProc,
                              NULLHANDLE,
                              IDD_OPENURL,
                              NULL) ;
                   return (MRESULT) FALSE ;
              /* Netscape DDE Version abfragen */
              case IDM_NSDDEVERSION:
                   /* DDE Kommando merken */
                   pwnd->ulType = IDM_NSDDEVERSION ;
                   /* DDE Kommunikationsbereich leeren */
                   memset (&cctxt,
                           0,
                           sizeof (CONVCONTEXT)) ;
                   /* LÑnge des DDE Kommunikationsbereichs */
                   cctxt.cb = sizeof (CONVCONTEXT) ;
                   cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                   cctxt.idCountry = 49 ;
                   cctxt.usCodepage = 850 ;
                   /* DDE Kommunikation initiieren */
                   WinDdeInitiate (hwndClient_g,
                                   szApplication_g,
                                   "WWW_Version",
                                   &cctxt) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Client Fenster wird geffnet */
      case WM_CREATE:
           /* Speicherbereich fÅr Instanzdaten allokieren und in den WindowWords zwischenlagern */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           break ;
      /* Netscapes DDE bestÑtigt etwas oder meldet Fehler */
      case WM_DDE_ACK:
           {
           BOOL       f ;
           PDDESTRUCT pdde ;

           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Diese Struktur hÑlt dann die Daten */
           pdde = PVOIDFROMMP (mp2) ;
           /* Keine BestÑtigung schicken */
           f = FALSE ;
           /* Server busy? */
           if   (pdde->fsStatus & DDE_FBUSY)
                {
                /* Nachricht ausgeben */
                TWriteMessage (HWND_DESKTOP,
                               NULLHANDLE,
                               IDS_DDENOTIFICATION,
                               IDS_DDESERVERBUSY,
                               MB_OK) ;
                }
           else
                {
                /* DDE Kommando konnte nicht ausgefÅhrt werden? */
                if   (pdde->fsStatus & DDE_NOTPROCESSED)
                     {
                     /* Nachricht ausgeben */
                     TWriteMessage (HWND_DESKTOP,
                                    NULLHANDLE,
                                    IDS_DDENOTIFICATION,
                                    IDS_UNSUPPORTEDDDETRANSACTIONDATA,
                                    MB_OK) ;
                     }
                else
                     {
                     /* Will Server BestÑtigung fÅr diese Nachricht */
                     if   (!(pdde->fsStatus & DDE_FACK))
                          {
                          /* Ja */
                          f = TRUE ;
                          }
                     }
                }
           /* Wenn keine BestÑtigung schicken */
           if   (!f)
                {
                /* Verbindung terminieren */
                WinDdePostMsg (HWNDFROMMP (mp1),
                               hwndClient_g,
                               WM_DDE_TERMINATE,
                               NULL,
                               DDEPM_RETRY) ;
                }
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
           }
      /* Es treffen DDE Daten ein */
      case WM_DDE_DATA:
           {
           CHAR       szText [256] ;
           PDDESTRUCT pdde ;
           PSZ        pszVersion ;
           PWND       pwnd ;
           ULONG      ulMajor ;
           ULONG      ulMinor ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                     QW_PARENT),
                                     QWL_USER) ;
           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Diese Struktur hÑlt dann die Daten */
           pdde = PVOIDFROMMP (mp2) ;
           /* Welches Kommando wird ausgefÅhrt? */
           switch (pwnd->ulType)
              {
              /* Ach so, Version */
              case IDM_NSDDEVERSION:
                   /* Pointer auf Daten im DDE Datenbereich */
                   pszVersion = DDES_PABDATA (pdde) ;
                   /* Minor Version berechnen */
                   ulMinor = *((PUSHORT) (pszVersion)) ;
                   /* Major Version berechnen */
                   ulMajor = *((PUSHORT) (pszVersion + 2)) ;
                   /* Text aufbereiten */
                   sprintf (szText,
                            "Netscape Version: Major=%u Minor=%u",
                            ulMajor,
                            ulMinor) ;
                   /* Text ausgeben */
                   WinMessageBox (HWND_DESKTOP,
                                  NULLHANDLE,
                                  szText,
                                  "",
                                  0,
                                  MB_OK) ;
                   break ;
              }
           /* Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndClient_g,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           /* DDE Kommunikationsbereich freigeben */
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
           }
      /* Netscape bestÑtigt DDE Verbindungsaufbau */
      case WM_DDE_INITIATEACK:
           {
           CHAR       szData [8192] ;
           CHAR       szItem [8192] ;
           PDDESTRUCT pdde ;
           PWND       pwnd ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pwnd = WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                     QW_PARENT),
                                     QWL_USER) ;

           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Welches DDE Kommando wird bearbeitet? */
           switch (pwnd->ulType)
                {
                /* Ach so, Netscape abschiessen */
                case IDM_NSDDEEXIT:
                     /*Es werden keine Zusatzinformationen benîtigt */
                     *szData = 0 ;
                     *szItem = 0 ;
                     /* Datenbereich fÅr diese Daten allokieren */
                     pdde = TMakeDDESeg (HWNDFROMMP (mp1),
                                         szItem,
                                         0,
                                         CF_TEXT,
                                         szData,
                                         strlen (szData) + 1) ;
                     /* Daten rÅberschicken */
                     WinDdePostMsg (HWNDFROMMP (mp1),
                                    hwndClient_g,
                                    WM_DDE_POKE,
                                    pdde,
                                    DDEPM_RETRY) ;
                     break ;
                /* Acho so, Version abfragen */
                case IDM_NSDDEVERSION:
                     *szData = 0 ;
                     *szItem = 0 ;
                     /* Datenbereich fÅr diese Daten allokieren */
                     pdde = TMakeDDESeg (HWNDFROMMP (mp1),
                                         szItem,
                                         0,
                                         CF_TEXT,
                                         szData,
                                         strlen (szData) + 1) ;
                     /* Daten rÅberschicken */
                     WinDdePostMsg (HWNDFROMMP (mp1),
                                    hwndClient_g,
                                    WM_DDE_REQUEST,
                                    pdde,
                                    DDEPM_RETRY) ;
                     break ;
                }
           return (MRESULT) FALSE ;
           }
      /* DDE Server terminiert Verbindung */
      case WM_DDE_TERMINATE:
           /* BestÑtigung Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndClient_g,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           return (MRESULT) FALSE ;
      /* Client Fenster wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten aus WindowWords holen und freigeben */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      /* Wer soll Fensterhintergrund zeichnen? */
      case WM_ERASEBACKGROUND:
           /* Presentation Manager */
           return (MRESULT) TRUE ;
      }
   /* Standard Fensterverarbeitung */
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Dialog fuer Netscape Fensterliste */
MRESULT EXPENTRY LISTWINDOWSWndProc (HWND   hwndDlg,
                                     MSG    msg,
                                     MPARAM mp1,
                                     MPARAM mp2)
   {
   /* Welcher Event? */
   switch (msg)
      {
      /* Benutzerkommando */
      case WM_COMMAND:
           {
           /* Welches Benutzerkommando? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Dialog schlie·en */
              case DID_CANCEL:
                   /* Dialog beenden lassen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Es treffen DDE Daten ein */
      case WM_DDE_DATA:
           {
           PDDESTRUCT      pdde ;
           PDLGLISTWINDOWS pdlg ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Diese Struktur hÑlt dann die Daten */
           pdde = PVOIDFROMMP (mp2) ;
           /* Wenn erster Schritt */
           if   (!pdlg->ulType)
                {
                /* Liste der Fensterhandles sichern */
                strcpy (pdlg->szWindows,
                        DDES_PABDATA (pdde)) ;
                pdlg->pszWindows = pdlg->szWindows ;
                }
           else
                {
                /* Im zweiten Schritt den Namen eines Fensters in der Listbox eintragen */
                WinSendDlgItemMsg (hwndDlg,
                                   IDL_LISTWINDOWS,
                                   LM_INSERTITEM,
                                   MPFROMSHORT (LIT_END),
                                   MPFROMP (DDES_PABDATA (pdde))) ;
                /* Zum nÑchsten Fensterhandle */
                pdlg->pszWindows += sizeof (ULONG) ;
                }
           /* Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndDlg,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           /* DDE Kommunikationsbereich freigeben */
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
           }
      /* Netscape bestÑtigt DDE Verbindungsaufbau */
      case WM_DDE_INITIATEACK:
           {
           CHAR            szData [8192] ;
           CHAR            szItem [8192] ;
           PDDESTRUCT      pdde ;
           PDLGLISTWINDOWS pdlg ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Welches Kommando wird ausgefÅhrt? */
           if   (!pdlg->ulType)
                {
                /*Es werden keine Zusatzinformationen benîtigt */
                *szData = 0 ;
                *szItem = 0 ;
                /* Datenbereich fÅr diese Daten allokieren */
                pdde = TMakeDDESeg (HWNDFROMMP (mp1),
                                    szItem,
                                    0,
                                    CF_TEXT,
                                    szData,
                                    strlen (szData) + 1) ;
                /* Daten rÅberschicken */
                WinDdePostMsg (HWNDFROMMP (mp1),
                               hwndDlg,
                               WM_DDE_REQUEST,
                               pdde,
                               DDEPM_RETRY) ;
                }
           else
                {
                /* Fensterhandle in Itembereich eintragen */
                sprintf (szItem,
                         "%u",
                         *((PULONG) (pdlg->pszWindows))) ;
                /* Nescapes DDE ignoriert den Datenbereich */
                *szData = 0 ;
                /* Datenbereich fÅr diese Daten allokieren */
                pdde = TMakeDDESeg (HWNDFROMMP (mp1),
                                    szItem,
                                    0,
                                    CF_TEXT,
                                    szData,
                                    strlen (szData) + 1) ;
                /* Daten rÅberschicken */
                WinDdePostMsg (HWNDFROMMP (mp1),
                               hwndDlg,
                               WM_DDE_REQUEST,
                               pdde,
                               DDEPM_RETRY) ;
                }
           return (MRESULT) FALSE ;
           }
      /* DDE Server terminiert Verbindung */
      case WM_DDE_TERMINATE:
           {
           CONVCONTEXT     cctxt ;
           PDLGLISTWINDOWS pdlg ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;

           /* Wenn erster Schritt */
           if   (!pdlg->ulType)
                {
                /* Dann Uebergang zum zweiten Schritt */
                pdlg->ulType = 1 ;
                }
           /* Sind Fensterhandles enthalten */
           if   (*((PULONG) pdlg->pszWindows))
                {
                /* DDE Kommunikationsbereich leeren */
                memset (&cctxt,
                        0,
                        sizeof (CONVCONTEXT)) ;
                /* LÑnge des DDE Kommunikationsbereichs */
                cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                cctxt.idCountry = 49 ;
                cctxt.usCodepage = 850 ;
                /* DDE Kommunikation initiieren */
                WinDdeInitiate (hwndDlg,
                                szApplication_g,
                                "WWW_GetWindowInfo",
                                &cctxt) ;
                }
           /* Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndDlg,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           return (MRESULT) FALSE ;
           }
      /* Dialog wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten aus WindowWords holen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           CONVCONTEXT     cctxt ;
           PDLGLISTWINDOWS pdlg ;

           /* Speicherbereich fÅr Instanzdaten allokieren und in den WindowWords zwischenlagern */
           pdlg = calloc (1,
                          sizeof (DLGLISTWINDOWS)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* DDE Kommunikationsbereich leeren */
           memset (&cctxt,
                   0,
                   sizeof (CONVCONTEXT)) ;
           /* LÑnge des DDE Kommunikationsbereichs */
           cctxt.cb = sizeof (CONVCONTEXT) ;
           cctxt.fsContext = DDECTXT_CASESENSITIVE ;
           cctxt.idCountry = 49 ;
           cctxt.usCodepage = 850 ;
           /* DDE Kommunikation initiieren */
           WinDdeInitiate (hwndDlg,
                           szApplication_g,
                           "WWW_ListWindows",
                           &cctxt) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Es gibt keine Programmstart Parameter */
VOID _setuparg (VOID)
   {
   }

/* Programmstart */
INT main (VOID)
   {
   HMQ   hmq ;
   QMSG  qmsg ;
   ULONG ulCreateFlags = FCF_MENU | FCF_MINMAX | FCF_SHELLPOSITION | FCF_SIZEBORDER | FCF_SYSMENU | FCF_TASKLIST | FCF_TITLEBAR ;

   /* Beim PM anmelden */
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             /* Fensterklasse regitrieren */
             if   (WinRegisterClass (hab_g,
                                     pszNSDde_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Standardfenster anlegen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszNSDde_g,
                                                           "DDE Test",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_NSDDE,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Message Loop */
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
                       /* Standardfenser schliessen */
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* Message Queue loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM abmelden */
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

/* Dialog zum ôffnen eines URL */
MRESULT EXPENTRY OPENURLWndProc (HWND   hwndDlg,
                                 MSG    msg,
                                 MPARAM mp1,
                                 MPARAM mp2)
   {
   /* Welcher Event? */
   switch (msg)
      {
      /* Benutzerkommando */
      case WM_COMMAND:
           {
           CONVCONTEXT cctxt ;

           /* Welches Benutzerkommando? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Dialog schlie·en */
              case DID_CANCEL:
                   /* Dialog beenden lassen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* URL soll geffne werden */
              case DID_OK:
                   /* DDE Kommunikationsbereich leeren */
                   memset (&cctxt,
                           0,
                           sizeof (CONVCONTEXT)) ;
                   /* LÑnge des DDE Kommunikationsbereichs */
                   cctxt.cb = sizeof (CONVCONTEXT) ;
                   cctxt.fsContext = DDECTXT_CASESENSITIVE ;
                   cctxt.idCountry = 49 ;
                   cctxt.usCodepage = 850 ;
                   /* DDE Kommunikation initiieren */
                   WinDdeInitiate (hwndDlg,
                                   szApplication_g,
                                   "WWW_OpenURL",
                                   &cctxt) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Netscapes DDE bestÑtigt etwas oder meldet Fehler */
      case WM_DDE_ACK:
           {
           BOOL       f ;
           PDDESTRUCT pdde ;

           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Diese Struktur hÑlt dann die Daten */
           pdde = PVOIDFROMMP (mp2) ;
           /* Keine BestÑtigung schicken */
           f = FALSE ;
           /* Server busy? */
           if   (pdde->fsStatus & DDE_FBUSY)
                {
                /* Nachricht ausgeben */
                TWriteMessage (HWND_DESKTOP,
                               NULLHANDLE,
                               IDS_DDENOTIFICATION,
                               IDS_DDESERVERBUSY,
                               MB_OK) ;
                }
           else
                {
                /* DDE Kommando konnte nicht ausgefÅhrt werden? */
                if   (pdde->fsStatus & DDE_NOTPROCESSED)
                     {
                     /* Nachricht ausgeben */
                     TWriteMessage (HWND_DESKTOP,
                                    NULLHANDLE,
                                    IDS_DDENOTIFICATION,
                                    IDS_UNSUPPORTEDDDETRANSACTIONDATA,
                                    MB_OK) ;
                     }
                else
                     {
                     /* Will Server BestÑtigung fÅr diese Nachricht */
                     if   (!(pdde->fsStatus & DDE_FACK))
                          {
                          /* Ja */
                          f = TRUE ;
                          }
                     }
                }
           /* Wenn keine BestÑtigung schicken */
           if   (!f)
                {
                /* Verbindung terminieren */
                WinDdePostMsg (HWNDFROMMP (mp1),
                               hwndDlg,
                               WM_DDE_TERMINATE,
                               NULL,
                               DDEPM_RETRY) ;
                }
           /* DDE Kommunikationsbereich freigeben */
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
           }
      /* Es treffen DDE Daten ein */
      case WM_DDE_DATA:
           {
           PDDESTRUCT pdde ;

           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* Diese Struktur hÑlt dann die Daten */
           pdde = PVOIDFROMMP (mp2) ;
           /* Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndDlg,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           /* DDE Kommunikationsbereich freigeben */
           DosFreeMem (pdde) ;
           return (MRESULT) FALSE ;
           }
      /* Netscape bestÑtigt DDE Verbindungsaufbau */
      case WM_DDE_INITIATEACK:
           {
           CHAR        szData [8192] ;
           CHAR        szItem [8192] ;
           PDDESTRUCT  pdde ;
           PDLGOPENURL pdlg ;

           /* Instanzdaten dieses Fensters aus WindowWords holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Pointer auf DDE Kommunikationsstruktur holen */
           if   (!PVOIDFROMMP (mp2))
                {
                /* Wenn leer dann Abbruch */
                return (MRESULT) FALSE ;
                }
           /* URL aus Eingabefeld auslesen */
           WinQueryDlgItemText (hwndDlg,
                                IDE_OPENURL,
                                sizeof (pdlg->szURL),
                                pdlg->szURL) ;
           /* Benîtigte Parameter */
           pdlg->ulFlags = 0x1 ;
           pdlg->ulWindow = 0xFFFFFFFF ;
           /* Itembereich aufbereten */
           sprintf (szItem,
                    "%s,,%u,%u,,,",
                    pdlg->szURL,
                    pdlg->ulWindow,
                    pdlg->ulFlags) ;
           /* Datenbereich fÅr diese Daten allokieren */
           pdde = TMakeDDESeg (HWNDFROMMP (mp1),
                               szItem,
                               0,
                               CF_TEXT,
                               szData,
                               strlen (szData) + 1) ;
           /* Daten rÅberschicken */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndDlg,
                          WM_DDE_REQUEST,
                          pdde,
                          DDEPM_RETRY) ;
           return (MRESULT) FALSE ;
           }
      /* DDE Server terminiert Verbindung */
      case WM_DDE_TERMINATE:
           /* BestÑtigung Verbindung terminieren */
           WinDdePostMsg (HWNDFROMMP (mp1),
                          hwndDlg,
                          WM_DDE_TERMINATE,
                          NULL,
                          DDEPM_RETRY) ;
           return (MRESULT) FALSE ;
      /* Dialog wird geschlossen */
      case WM_DESTROY:
           /* Instanzdaten aus WindowWords holen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLGOPENURL pdlg ;

           /* Speicherbereich fÅr Instanzdaten allokieren und in den WindowWords zwischenlagern */
           pdlg = calloc (1,
                          sizeof (DLGOPENURL)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* TextlÑnge Eingabefeld festlegen */
           WinSendDlgItemMsg (hwndDlg,
                              IDE_OPENURL,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (CCHMAXPATH - 1),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      }
   /* Standard Dialogverarbeitung */
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* DDE Datenbereich allokieren und Rechte an EmpfÑnger weitergeben */
PDDESTRUCT TMakeDDESeg (HWND  hwndDest,
                        PSZ   pszItemName,
                        ULONG ulStatus,
                        ULONG ulFormat,
                        PVOID pvData,
                        ULONG ulDataLen)
   {
   PDDESTRUCT pdde ;
   PID        pid ;
   TID        tid ;
   ULONG      ul ;
   ULONG      ulTotal ;

   /* LÑnge DDE Item */
   ul = strlen (pszItemName) + 1 ;
   /* Komplette LÑnge Item und Daten */
   ulTotal = sizeof (DDESTRUCT) + ul + ulDataLen ;
   /* Speicher allokieren */
   if   (!DosAllocSharedMem ((PPVOID) &pdde,
                             NULL,
                             ulTotal,
                             PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_GIVEABLE))
        {
        /* Welcher Prozess steckt hinter diesem Fenster */
        WinQueryWindowProcess (hwndDest,
                               &pid,
                               &tid) ;
        /* Lese und Schreibrechte an diesen geben */
        DosGiveSharedMem (&pdde,
                          pid,
                          PAG_READ | PAG_WRITE) ;
        /* DDE Daten in diesen Bereich umkopieren */
        pdde->cbData = ulDataLen ;
        pdde->fsStatus = ulStatus ;
        pdde->usFormat = ulFormat ;
        pdde->offszItemName = sizeof (DDESTRUCT) ;
        pdde->offabData = (ulDataLen && pvData) ? sizeof (DDESTRUCT) + ul : 0 ;
        /* Itemdaten reinkopieren */
        memcpy (DDES_PSZITEMNAME (pdde),
                pszItemName,
                strlen (pszItemName) + 1) ;
        /* Allgemeine Daten reinkopieren */
        memcpy (DDES_PABDATA (pdde),
                pvData,
                ulDataLen) ;
        /* Pointer retournieren */
        return pdde ;
        }
   else
        {
        /* Im Fehlerfall 0 rtournieren */
        return NULL ;
        }
   }

/* Nachricht ausgeben */
USHORT TWriteMessage (HWND    hwnd,
                      HMODULE hmod,
                      ULONG   ulTitle,
                      ULONG   ulText,
                      ULONG   ulStyle)
   {
   CHAR szText [256] ;
   CHAR szTitle [128] ;
   HAB hab ;

   /* Handle Anchor Block besorgen */
   hab = WinQueryAnchorBlock (hwnd) ;
   /* Text fÅr Titelzeile aus Resource laden */
   WinLoadString (hab,
                  hmod,
                  ulTitle,
                  sizeof (szTitle),
                  szTitle) ;
   /* Text fÅr Body aus Resource laden */
   WinLoadString (hab,
                  hmod,
                  ulText,
                  sizeof (szText),
                  szText) ;
   /* Text ausgeben lassen */
   return WinMessageBox (HWND_DESKTOP,
                         hwnd,
                         szText,
                         szTitle,
                         0,
                         ulStyle) ;
   }

