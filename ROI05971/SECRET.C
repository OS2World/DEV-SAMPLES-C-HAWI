/* $Header: y:/projekte/redaktion os!2 inside/Ganz geheim/Source/rcs/SECRET.C 1.1 1997/03/07 16:00:01 HAWI Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_GPI
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Applikations Kopfdatei */
#include "SECRET.H"

/* Einige globale Variablen */
BOOL fRegistriert_g = FALSE ;
HAB  hab_g = NULLHANDLE ;
HINI hini_g = NULLHANDLE ;
HWND hwndClient_g = NULLHANDLE ;
HWND hwndFrame_g = NULLHANDLE ;
PSZ  pszSecret_g = "Secret" ; /* ApplikationsschlÅssel */

/* About Dialog Fensterprozedur */
MRESULT EXPENTRY AWndProc (HWND   hwndDlg,
                           MSG    msg,
                           MPARAM mp1,
                           MPARAM mp2)
   {
   switch (msg)
      {
      case WM_INITDLG:
           {
           CHAR    sz [250] ;
           CHAR    sz2 [250] ;
           MRESULT mr ;
           
           /* Erst die Standardverarbeitung aufrufen */
           mr = WinDefDlgProc (hwndDlg,
                               msg,
                               mp1,
                               mp2) ;
           /* Ist die Anwendung registriert? */                    
           if   (fRegistriert_g)
                {
                /* Ja, entsprechenden Text aus Ressource lesen */
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_REGISTRIERT,
                               sizeof (sz),
                               sz) ;
                /* und den Namen des Benutzers aus der INI anhÑngen */               
                PrfQueryProfileString (hini_g,
                                       pszSecret_g,
                                       "RegisterName",
                                       "",
                                       sz2,
                                       sizeof (sz2)) ;
                strcat (sz,
                        sz2) ;
                }
           else
                {
                /* Ansonsten Text fÅr Unregistriert aus Ressource lesen */
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_UNREGISTRIER,
                               sizeof (sz),
                               sz) ;
                }
           /* Text ausgeben */     
           WinSetDlgItemText (hwndDlg,
                              IDT_HABOUT3,
                              sz) ;
           return mr ;
           }
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Client Fensterprozedur */
MRESULT EXPENTRY ClientWndProc (HWND   hwnd,
                                MSG    msg,
                                MPARAM mp1,
                                MPARAM mp2)
   {
   switch (msg)
      {
      /* Ein Kommando trudelt ein */
      case WM_COMMAND:
           {
           CHAR sz [CCHMAXPATH * 6] ;
           PWND pwnd ;

           /* Instanzdaten aus Window Words auslesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Wer will was? */                          
           switch (SHORT1FROMMP (mp1))
              {
              /* Cancel auf SysMenu */
              case DID_CANCEL:
                   /* Anwendung zum Beenden bringen */
                   WinPostMsg (hwnd,
                               WM_CLOSE,
                               MPVOID,
                               MPVOID) ;
                   return (MRESULT) FALSE ;
              /* ABout Dialog soll angezeigt werden */     
              case IDM_HABOUT:
                   /* Ausgeben */
                   WinDlgBox (HWND_DESKTOP,
                              hwndFrame_g,
                              AWndProc,
                              NULLHANDLE,
                              IDD_HABOUT,
                              NULL) ;
                   return (MRESULT) FALSE ;
              /* Benutzer will registrieren */     
              case IDM_HREGISTER:
                   /* Dialog ausgeben */
                   if   (WinDlgBox (HWND_DESKTOP,
                                    hwndFrame_g,
                                    HRWndProc,
                                    NULLHANDLE,
                                    IDD_HR,
                                    NULL) == DID_OK)
                        {
                        /* Ist jetzt registriert? */
                        if   (fRegistriert_g)
                             {
                             /* Ja, dann Eintrag aus MenÅ entfernen */
                             WinSendMsg (pwnd->hwndMenu,
                                         MM_DELETEITEM,
                                         MPFROM2SHORT (IDM_HREGISTER, TRUE),
                                         MPVOID) ;
                             /* Programmname aus Ressource lesen */            
                             WinLoadString (hab_g,
                                            NULLHANDLE,
                                            IDS_PROGRAMM,
                                            sizeof (sz),
                                            sz) ;
                             /* und in Titelzeile ausgeben */               
                             WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                                                FID_TITLEBAR),
                                               sz) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Fenster wird erzeugt */     
      case WM_CREATE:
           /* Instanzdaten allokieren und in Window Words ablegen */
           WinSetWindowPtr (WinQueryWindow (hwnd,
                                            QW_PARENT),
                            QWL_USER,
                            calloc (1,
                                    sizeof (WND))) ;
           /* Verzîgerte Startverarbeitung */                         
           WinPostMsg (hwnd,
                       WMPTX_MAININIT,
                       MPVOID,
                       MPVOID) ;
           break ;
      /* Fenster wird entfernt */     
      case WM_DESTROY:
           /* Instanzdaten aus Window Words lesen und wieder freigeben */
           free (WinQueryWindowPtr (WinQueryWindow (hwnd,
                                                    QW_PARENT),
                                    QWL_USER)) ;
           break ;
      /* Text ausgeben */
      case WM_PAINT:
           {
           CHAR   sz [CCHMAXPATH] ;
           HPS    hps ;
           POINTL ptl ;
           
           ptl.x = 100 ;
           ptl.y = 100 ;
           hps = WinBeginPaint (hwnd,
                                0,
                                NULL) ;
           GpiErase (hps) ;
           WinLoadString (hab_g,
                          NULLHANDLE,
                          IDS_WICHTIG,
                          sizeof (sz),
                          sz) ;
           GpiCharStringAt (hps,
                            &ptl,
                            strlen (sz),
                            sz) ;
           WinEndPaint (hps) ;
           return (MRESULT) FALSE ;
           }
      /* Verzîgerter Start */     
      case WMPTX_MAININIT:
           {
           CHAR sz [CCHMAXPATH] ;
           PWND pwnd ;

           /* Instanzdaten aus Window Words lesen */
           pwnd = WinQueryWindowPtr (hwndFrame_g,
                                     QWL_USER) ;
           /* Fensterhandle des Menus besorgen */                          
           pwnd->hwndMenu = WinWindowFromID (hwndFrame_g,
                                             FID_MENU) ;
           /* Ist registriert? */
           if   (fRegistriert_g)
                {
                /* Ja, enstprechenden Menueintrag lîschen */
                WinSendMsg (pwnd->hwndMenu,
                            MM_DELETEITEM,
                            MPFROM2SHORT (IDM_HREGISTER, TRUE),
                            MPVOID) ;
                /* Programmname aus Ressource lesen */            
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_PROGRAMM,
                               sizeof (sz),
                               sz) ;
                }
           else
                {
                /* Unregistriert aus Ressource lesen */
                WinLoadString (hab_g,
                               NULLHANDLE,
                               IDS_UNREGISTRIERT,
                               sizeof (sz),
                               sz) ;
                }
           /* Text in Titelzeile ausgeben */     
           WinSetWindowText (WinWindowFromID (hwndFrame_g,
                                              FID_TITLEBAR),
                             sz) ;
           break ;
           }
      }
   /* Standardverarbeitung */   
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* Registrierdialog */
MRESULT EXPENTRY HRWndProc (HWND   hwndDlg,
                            MSG    msg,
                            MPARAM mp1,
                            MPARAM mp2)
   {
   switch (msg)
      {
      case WM_COMMAND:
           {
           CHAR  sz [33] ;
           CHAR  sz2 [33] ;
           ULONG ul ;

           switch (SHORT1FROMMP (mp1))
              {
              /* Benutzer hat OK gedrÅckt */
              case DID_OK:
                   /* Name lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_HR1,
                                        sizeof (sz),
                                        sz) ;
                   /* Code lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_HR2,
                                        sizeof (sz2),
                                        sz2) ;
                   ul = atol (sz2) ;
                   /* Registrierfunktion aufrufen und neuen Status ermitteln */
                   fRegistriert_g = TRegister (hini_g,
                                               pszSecret_g,
                                               sz,
                                               &ul) ;
                   break ;
              }
           break ;
           }
      }
   /* Standardverarbeitung */   
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Programmstart */
INT main (VOID)
   {
   CHAR  sz [128] ;
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
             /* INI suchen bzw. erzeugen */
             hini_g = TLocateIni (hab_g,
                                  "SECRET.INI") ;
             /* Fenterklasse fÅr Anwendung erzeugen */
             if   (WinRegisterClass (hab_g,
                                     pszSecret_g,
                                     ClientWndProc,
                                     CS_SIZEREDRAW,
                                     0))
                  {
                  /* Standardfenster îffnen */
                  if   ((hwndFrame_g = WinCreateStdWindow (HWND_DESKTOP,
                                                           WS_ANIMATE | WS_VISIBLE,
                                                           &ulCreateFlags,
                                                           pszSecret_g,
                                                           "",
                                                           0,
                                                           NULLHANDLE,
                                                           IDM_SECRET,
                                                           &hwndClient_g)) != NULLHANDLE)
                       {
                       /* Programmname aus Ressource lesen */
                       WinLoadString (hab_g,
                                      NULLHANDLE,
                                      IDS_PROGRAMM,
                                      sizeof (sz),
                                      sz) ;
                       /* Test ob registriert ist */               
                       fRegistriert_g = TRegister (hini_g,
                                                   pszSecret_g,
                                                   NULL,
                                                   NULL) ;
                       /* Wenn nicht, dann Sticky Window ausgeben */                            
                       if   (!fRegistriert_g)
                            {
                            TStickyWindowCreate (HWND_DESKTOP,
                                                 hwndFrame_g) ;
                            }
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
                       /* Wurde zwischenzeitlich registriert? */
                       if   (!fRegistriert_g)
                            {
                            TStickyWindowCreate (HWND_DESKTOP,
                                                 hwndFrame_g) ;
                            }
                       /* Applikationsfenster entfernen */     
                       WinDestroyWindow (hwndFrame_g) ;
                       }
                  }
             /* INI wieder schliessen */     
             PrfCloseProfile (hini_g) ;
             /* Message Queue loeschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM abmelden */     
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

/* Pruefen ob Laufwerk readonly */
BOOL TCheckForReadOnlyDrive (CHAR ch)
   {
   BOOL  f ;
   CHAR  szBuffer [] = "C:\\~TMPTMP.TM~" ;
   FILE* pfile ;

   f = TXK_NOERROR ;
   /* Laufwerksbuchstabe in Konstante eintragen */
   *szBuffer = ch ;
   /* Zum schreiben îffnen */
   if   ((pfile = fopen (szBuffer,
                        "w")) != NULL)
        {
        /* Medium ist beschreibbar, Datei schliessen und entfernen */
        fclose (pfile) ;
        remove (szBuffer) ;
        f = TXK_ERROR ;
        }
   return f ;
   }

/* INI suchen und îffnen oder geeignete Lokation finden und anlegen */
HINI TLocateIni (HAB hab,
                 PSZ pszIni)
   {
   CHAR  sz [CCHMAXPATH] ;
   FILE* pfile ;
   PPIB  ppib ;
   PSZ   psz ;
   PTIB  ptib ;
   ULONG ul ;

   strcpy (sz,
           pszIni) ;
   /* Ist sie im aktuellen Verzeichnis oder im DPATH? */
   if   ((pfile = fopen (pszIni,
                         "rb")) != NULL)
        {
        /* Ja wieder schliessen */
        fclose (pfile) ;
        }
   else
        {
        /* Ansonsten Boot Laufwerk ermitteln */
        DosGetInfoBlocks (&ptib,
                          &ppib) ;
        /* Programmname ermitteln */                  
        DosQueryModuleName (ppib->pib_hmte,
                            sizeof (sz),
                            sz) ;
        /* Liegt Programm auf beschreibbaren Medium? */                    
        if   (TCheckForReadOnlyDrive (*sz) == TXK_NOERROR)
             {
             /* Nein, dann erst in OS2 Verzeichnis ablegen */
             strcpy (sz,
                     "C:\\OS2\\") ;
             strcat (sz,
                     pszIni) ;
             /* Hierzu Bootlaufwerk ermittteln */        
             DosQuerySysInfo (QSV_BOOT_DRIVE,
                              QSV_BOOT_DRIVE,
                              &ul,
                              sizeof (ul)) ;
             *sz = ul + '@' ;
             }
        else
             {
             if   ((psz = strrchr (sz,
                                   '\\')) != NULL)
                  {
                  strcpy ((psz + 1),
                          pszIni) ;
                  }
             }
        }
   /* INI îffnen */     
   return PrfOpenProfile (hab,
                          sz) ;
   }

/* Diese Funktion muss in jeder Sharewareanwendungen enthalten sein und berechnet einen Registriercode */
BOOL TRegister (HINI   hini,
                PSZ    pszApplication,
                PSZ    pszName,
                PULONG pul)
   {
   CHAR  sz [128] ;
   CHAR  sz2 [128] ;
   ULONG ulCode ;
   ULONG ulI ;
   ULONG ulL ;
   ULONG ulN ;

   /* Wenn HINI Åbergeben dann Daten aus INI auslesen, ansonsten nur Berechnung */
   if   (hini)
        {
        /* Wurde Name Åbergeben? */
        if   (pszName)
             {
             /* Ja, in INI eintragen */
             PrfWriteProfileString (hini,
                                    pszApplication,
                                    "RegisterName",
                                    pszName) ;
             }
        /* Wurde Code Åbergeben? */
        if   (pul)
             {
             /* Ja */
             _ltoa (*pul,
                    sz2,
                    10) ;
             /* In INI eintragen */
             PrfWriteProfileString (hini,
                                    pszApplication,
                                    "RegisterCode",
                                    sz2) ;
             }
        /* Programmname */
        strcpy (sz,
                pszApplication) ;
        /* Name wieder auslesen */
        PrfQueryProfileString (hini,
                               pszApplication,
                               "RegisterName",
                               " ",
                               sz2,
                               sizeof (sz2)) ;
        /* Programmname und Benutzername konkatinieren */                       
        strcat (sz,
                sz2) ;
        /* Code wieder auslesen */        
        PrfQueryProfileString (hini,
                               pszApplication,
                               "RegisterCode",
                               "0",
                               sz2,
                               sizeof (sz2)) ;
        ulCode = atol (sz2) ;
        }
   else
        {
        strcpy (sz,
                pszApplication) ;
        strcat (sz,
                pszName) ;
        ulCode = *pul ;
        }
   /* LÑnge des Applikationsnamens */
   ulL = strlen (sz) ;
   if   (ulL >= 5)
        {
        /* Startwert */
        ulN = 123456 ;
        /* FÅr jedes Zeichen (Programmname und Benutzername */
        for  (ulI = 0; ulI < ulL; ulI++)
             {
             /* Multiplizieren */
             ulN *= 421 ;
             /* Ein bisschen Zeichenarithmetik */
             ulN += 54773 + sz [ulI] ;
             ulN %= 259200 ;
             }
        }
   else
        {
        return TXK_ERROR ;
        }
   if   (hini)
        {
        if   (ulCode != ulN)
             {
             return TXK_ERROR ;
             }
        }
   if   (pul)
        {
        *pul = ulN ;
        }
   return TXK_NOERROR ;
   }

/* Anlegen des Sticky Windows */
HWND TStickyWindowCreate (HWND hwndParent,
                          HWND hwndOwner)
   {
   WinDlgBox (hwndParent,
              hwndOwner,
              TStickyWindowProc,
              NULLHANDLE,
              IDD_SW,
              NULL) ;
   return TXK_NOERROR ;
   }

/* Fensterprozedur des Sticky Windows */
MRESULT EXPENTRY TStickyWindowProc (HWND   hwndDlg,
                                    MSG    msg,
                                    MPARAM mp1,
                                    MPARAM mp2)
   {
   switch (msg)
      {
      /* Abbrechen nicht mîglich */
      case WM_COMMAND:
           return (MRESULT) FALSE ;
      /* Beim Fensterabschlu· */     
      case WM_DESTROY:
           /* Instanzdaten aus Windows Words lesen und freigeben */
           free (WinQueryWindowPtr (hwndDlg,
                                    QWL_USER)) ;
           break ;
      /* Fensterstart */     
      case WM_INITDLG:
           {
           PDLGSW pdlg ;

           /* Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLGSW)) ;
           /* Und Pointer in Windows Words hinterlegen */               
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Applikations Anchor Block merken */                 
           pdlg->hab = WinQueryAnchorBlock (hwndDlg) ;
           /* Fenster zentrieren */
           TWindowCenter (hwndDlg,
                          WinQueryWindow (hwndDlg,
                                          QW_OWNER)) ;
           /* 12 Sekunden Verzîgerung */                               
           pdlg->us = 12 ;
           /* Zahl ausgeben */
           WinSetDlgItemShort (hwndDlg,
                               IDT_SW_2,
                               pdlg->us,
                               FALSE) ;
           /* Timer starten */                    
           WinStartTimer (pdlg->hab,
                          hwndDlg,
                          TXK_TIMER,
                          1000) ;
           break ;
           }
      /* Wieder eine Sekunde um */     
      case WM_TIMER:
           {
           PDLGSW pdlg ;

           /* Instanzdaten besorgen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           switch (SHORT1FROMMP (mp1))
              {
              /* Ist es auch mein Timer? */
              case TXK_TIMER:
                   pdlg->us-- ;
                   /* Neuen Wert ausgeben */
                   WinSetDlgItemShort (hwndDlg,
                                       IDT_SW_2,
                                       pdlg->us,
                                       FALSE) ;
                   /* Wenn auf 0 */                    
                   if   (!pdlg->us)
                        {
                        /* Timer stoppen */
                        WinStopTimer (pdlg->hab,
                                      hwndDlg,
                                      TXK_TIMER) ;
                        /* Fenster entfernen */              
                        WinDismissDlg (hwndDlg,
                                       DID_OK) ;
                        }
                   break ;
              }
           break ;
           }
      }
   /* Standardverarbeitung */   
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

/* Zentrieren eines Fensters auf einem anderen */
BOOL TWindowCenter (HWND hwnd,
                    HWND hwndParent)
   {
   SWP swp ;
   SWP swpParent ;
   SWP swpDesktop ;

   WinQueryWindowPos (hwnd,
                      &swp) ;
   WinQueryWindowPos (hwndParent,
                      &swpParent) ;
   WinQueryWindowPos (HWND_DESKTOP,
                      &swpDesktop) ;
   if   (((swpParent.cx - swp.cx) / 2 + swpParent.x) < 0 ||
         ((swpParent.cy - swp.cy) / 2 + swpParent.y) < 0 ||
         ((swpParent.cx - swp.cx) / 2 + swpParent.x + swp.cx) > swpDesktop.cx ||
         ((swpParent.cy - swp.cy) / 2 + swpParent.y + swp.cy) > swpDesktop.cy )
        {
        swpParent = swpDesktop ;
        }
   return WinSetWindowPos (hwnd,
                           HWND_TOP,
                           (swpParent.cx - swp.cx) / 2 + swpParent.x,
                           (swpParent.cy - swp.cy) / 2 + swpParent.y,
                           0,
                           0,
                           SWP_MOVE) ;
   }


