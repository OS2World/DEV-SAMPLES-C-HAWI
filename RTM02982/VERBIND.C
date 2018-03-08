/* $Header: D:\Projekte\Redaktion Toolbox\Verbindungen\Source\RCS\VERBIND.C 1.2 1997/11/26 21:40:38 HAWI Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VERBIND.H"

INT main (VOID)
   {
   HAB hab ;
   HMQ hmq ;

   /* Beim PM anmelden */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Message Queue mit Standardgrî·e anlegen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* Ein normaler Dialog reicht fÅr dieses Programm */
             WinDlgBox (HWND_DESKTOP,
                        HWND_DESKTOP,
                        DlgWndProc,
                        NULLHANDLE,
                        IDD_VERBIND,
                        NULL) ;
             /* Message Queue wieder wegwerfen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM wieder abmelden */
        WinTerminate (hab) ;
        }
   /* und tschÅ· */
   return 0 ;
   }

/* Fensterprozedur des Dialoges */
MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   switch (msg)
      {
      /* Der Benutzer startet eine Aktion */
      case WM_COMMAND:
           {
           CHAR    sz [CCHMAXPATH] ;
           HOBJECT hobj ;
           PDLG    pdlg ;
           ULONG   ul ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Was will er? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Der Benutzer hat ESC gedrÅckt, Anwendung verlassen */
              case DID_CANCEL:
                   /* Dialog wegwerfen */
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              /* Der Benutzer drÅckt den linken Pushbutton HinzufÅgen.
                 Er will also einen Dateifilter oder einen Dateityp hinzufÅgen */
              case IDP_V1:
                   /* Text aus dem unteren Eingabefeld lesen */
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_V1,
                                        sizeof (sz),
                                        sz) ;
                   /* Ist Åberhaupt was drin? */
                   if   (*sz)
                        {
                        /* Diesen Text der entsprechenden Liste in der INI Datei hinzufÅgen */
                        PrfWriteProfileString (HINI_USERPROFILE,
                                               (pdlg->vstyle == VSTYLE_FILTER) ? "PMWP_ASSOC_FILTER" : "PMWP_ASSOC_TYPE",
                                               sz,
                                               "") ;
                        /* Diesen Text nun auch in der Listbox hinzufÅgen */
                        ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                       LM_INSERTITEM,
                                                       MPFROMSHORT (LIT_SORTASCENDING),
                                                       MPFROMP (sz))) ;
                        /* Diesen neuen Eintrag auch selektieren */
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_SELECTITEM,
                                    MPFROMSHORT (ul),
                                    MPFROMSHORT (TRUE)) ;
                        }
                   return (MRESULT) FALSE ;
              /* Der Benutzer drÅckt den linken Pushbutton Lîschen.
                 Er will also einen Dateifilter oder einen Dateityp lîschen */
              case IDP_V2:
                   /* Welcher Eintrag ist in der linken Listbox selektiert? */
                   if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                         LM_QUERYSELECTION,
                                                         MPFROMSHORT (LIT_FIRST),
                                                         MPVOID))) != (ULONG) LIT_NONE)
                        {
                        /* Den Text dieses Eintrags ermitteln */
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (ul, sizeof (sz)),
                                    MPFROMP (sz)) ;
                        /* Diesen Eintrag in der INI mit einem NULL Åberschreiben. Dies lîscht den Eintrag */
                        PrfWriteProfileString (HINI_USERPROFILE,
                                               (pdlg->vstyle == VSTYLE_FILTER) ? "PMWP_ASSOC_FILTER" : "PMWP_ASSOC_TYPE",
                                               sz,
                                               NULL) ;
                        /* Nun noch den Eintrag aus der Listbox lîschen */
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_DELETEITEM,
                                    MPFROMSHORT (ul),
                                    MPVOID) ;
                        /* Den obersten Eintrag in der Listbox selektieren */
                        WinSendMsg (pdlg->hwndListbox1,
                                    LM_SELECTITEM,
                                    MPFROMSHORT (0),
                                    MPFROMSHORT (TRUE)) ;
                        }
                   return (MRESULT) FALSE ;
              /* Der Benutzer drÅckt den rechten Pushbutton HinzufÅgen.
                 Er will also eine Zuordnung eines Dateifilters oder eines Dateityps hinzufÅgen */
              case IDP_V3:
                   /* Welche Art der Zuordnung soll es geben? */
                   if   (SHORT1FROMMR (WinSendMsg (WinWindowFromID (hwndDlg,
                                                                    IDR_V3),
                                                   BM_QUERYCHECK,
                                                   MPVOID,
                                                   MPVOID)))
                        {
                        /* Eine Objektzuordnung
                           also in der Combobox den selektierten Eintrag suchen */
                        if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndCombobox,
                                                              LM_QUERYSELECTION,
                                                              MPFROMSHORT (LIT_FIRST),
                                                              MPVOID))) != (ULONG) LIT_NONE)
                             {
                             /* Den Text dieses Eintrags ermitteln */
                             WinSendMsg (pdlg->hwndCombobox,
                                         LM_QUERYITEMTEXT,
                                         MPFROM2SHORT (ul, sizeof (sz)),
                                         MPFROMP (sz)) ;
                             /* Den hinterlegten Object-Handle dieses Eintrags ermitteln */
                             hobj = LONGFROMMR (WinSendMsg (pdlg->hwndCombobox,
                                                            LM_QUERYITEMHANDLE,
                                                            MPFROMSHORT (ul),
                                                            MPVOID)) ;
                             /* Eintrag einfÅgen */
                             ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                            LM_INSERTITEM,
                                                            MPFROMSHORT (LIT_END),
                                                            MPFROMP (sz))) ;
                             /* Den Object-Handle dieses Eintrags ebenfalls merken */
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_SETITEMHANDLE,
                                         MPFROMSHORT (ul),
                                         MPFROMLONG (hobj)) ;
                             /* Diesen neuen Eintrag auch selektieren */
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_SELECTITEM,
                                         MPFROMSHORT (0),
                                         MPFROMSHORT (TRUE)) ;
                             /* Das Schreiben der neuen INI Daten veranlassen */
                             WinPostMsg (hwndDlg,
                                         WMP_REFRESHRIGHT,
                                         MPVOID,
                                         MPVOID) ;
                             }
                        }
                   else
                        {
                        /* Eine Programmzuordnung
                           also im Eingabefeld der Combobox den Text lesen */
                        WinQueryWindowText (pdlg->hwndCombobox,
                                            sizeof (sz),
                                            sz) ;
                        /* Den Objekt Handle des Programmes ermitteln */
                        if   ((hobj = WinQueryObject (sz)) != NULLHANDLE)
                             {
                             /* Diesen neuen Eintrag in der rechten Listbox eintragen */
                             ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                            LM_INSERTITEM,
                                                            MPFROMSHORT (LIT_END),
                                                            MPFROMP (sz))) ;
                             /* Den Object-Handle dieses Eintrags ebenfalls merken */
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_SETITEMHANDLE,
                                         MPFROMSHORT (ul),
                                         MPFROMLONG (hobj)) ;
                             /* Diesen neuen Eintrag auch selektieren */
                             WinSendMsg (pdlg->hwndListbox2,
                                         LM_SELECTITEM,
                                         MPFROMSHORT (0),
                                         MPFROMSHORT (TRUE)) ;
                             /* Das Schreiben der neuen INI Daten veranlassen */
                             WinPostMsg (hwndDlg,
                                         WMP_REFRESHRIGHT,
                                         MPVOID,
                                         MPVOID) ;
                             }
                        }
                   return (MRESULT) FALSE ;
              /* Der Benutzer drÅckt den rechten Pushbutton Lîschen.
                 Er will also eine Zuordnung eines Dateifilters oder eines Dateityps lîschen */
              case IDP_V4:
                   /* Welcher Eintrag ist in der rechten Listbox selektiert? */
                   if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                         LM_QUERYSELECTION,
                                                         MPFROMSHORT (LIT_FIRST),
                                                         MPVOID))) != (ULONG) LIT_NONE)
                        {
                        /* Eintrag aus der rechten Listbox lîschen */
                        WinSendMsg (pdlg->hwndListbox2,
                                    LM_DELETEITEM,
                                    MPFROMSHORT (ul),
                                    MPVOID) ;
                        /* Das Schreiben der neuen INI Daten veranlassen */
                        WinPostMsg (hwndDlg,
                                    WMP_REFRESHRIGHT,
                                    MPVOID,
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              /* Der Benutzer drÅckt den rechten Pushbutton Standard.
                 Er will also eine Zuordnung eines Dateifilters oder eines Dateityps bevorzugen */
              case IDP_V5:
                   /* Welcher Eintrag ist in der rechten Listbox selektiert? */
                   if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                         LM_QUERYSELECTION,
                                                         MPFROMSHORT (LIT_FIRST),
                                                         MPVOID))) != (ULONG) LIT_NONE)
                        {
                        /* Den Text dieses Eintrags ermitteln */
                        WinSendMsg (pdlg->hwndListbox2,
                                    LM_QUERYITEMTEXT,
                                    MPFROM2SHORT (ul, sizeof (sz)),
                                    MPFROMP (sz)) ;
                        /* Den hinterlegten Object-Handle dieses Eintrags ermitteln */
                        hobj = LONGFROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                       LM_QUERYITEMHANDLE,
                                                       MPFROMSHORT (ul),
                                                       MPVOID)) ;
                        /* Eintrag aus der rechten Listbox lîschen */
                        WinSendMsg (pdlg->hwndListbox2,
                                    LM_DELETEITEM,
                                    MPFROMSHORT (ul),
                                    MPVOID) ;
                        /* Diesen Eintrag an erster Stelle neu eintragen */
                        ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                       LM_INSERTITEM,
                                                       MPFROMSHORT (0),
                                                       MPFROMP (sz))) ;
                        /* Den Object-Handle dieses Eintrags ebenfalls merken */
                        WinSendMsg (pdlg->hwndListbox2,
                                    LM_SETITEMHANDLE,
                                    MPFROMSHORT (ul),
                                    MPFROMLONG (hobj)) ;
                        /* Diesen neuen Eintrag auch selektieren */
                        WinSendMsg (pdlg->hwndListbox2,
                                    LM_SELECTITEM,
                                    MPFROMSHORT (0),
                                    MPFROMSHORT (TRUE)) ;
                        /* Das Schreiben der neuen INI Daten veranlassen */
                        WinPostMsg (hwndDlg,
                                    WMP_REFRESHRIGHT,
                                    MPVOID,
                                    MPVOID) ;
                        }
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      /* Ein Event eines Kontrollelementes trifft ein */
      case WM_CONTROL:
           {
           CHAR    sz [CCHMAXPATH] ;
           CHAR    szBuffer [8192] ;
           HOBJECT hobj ;
           PDLG    pdlg ;
           PSZ     psz ;
           PSZ     pszBuffer ;
           PSZ     pszMax ;
           PSZ     pszMaxBuffer ;
           ULONG   ul ;
           USHORT  us ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Welches Kontrollelement will was? */
           switch (SHORT1FROMMP (mp1))
              {
              /* Die linke Listbox */
              case IDL_V1:
                   /* Was will die linke Listbox? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Ein neuer Eintrag wurde selektiert */
                      case LN_SELECT:
                           /* Welcher Eintrag ist in der linken Listbox selektiert? */
                           if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                                 LM_QUERYSELECTION,
                                                                 MPFROMSHORT (LIT_FIRST),
                                                                 MPVOID))) != (ULONG) LIT_NONE)
                                {
                                /* Den Text dieses Eintrags ermitteln */
                                WinSendMsg (pdlg->hwndListbox1,
                                            LM_QUERYITEMTEXT,
                                            MPFROM2SHORT (ul, sizeof (sz)),
                                            MPFROMP (sz)) ;
                                /* In der rechten Listbox alle EintrÑge lîschen */
                                WinSendMsg (pdlg->hwndListbox2,
                                            LM_DELETEALL,
                                            MPFROMSHORT (TRUE),
                                            MPVOID) ;
                                /* Alle Zuordnungen dieses Dateifilters bzw. dieses Dateitypes ermitteln */
                                ul = PrfQueryProfileString (HINI_USERPROFILE,
                                                            (pdlg->vstyle == VSTYLE_FILTER) ? "PMWP_ASSOC_FILTER" : "PMWP_ASSOC_TYPE",
                                                            sz,
                                                            NULL,
                                                            szBuffer,
                                                            sizeof (szBuffer)) ;
                                /* Speicherbereich begrenzen */
                                pszBuffer = szBuffer ;
                                pszMaxBuffer = szBuffer ;
                                pszMaxBuffer += ul ;
                                /* In diesem Speicher stehen alle Zuordnungen durch 0 getrennt hintereinander */
                                while (*pszBuffer &&
                                       pszBuffer < pszMaxBuffer)
                                   {
                                   /* Zuordnungsnummer wandeln */
                                   hobj = (HOBJECT) atol (pszBuffer) ;
                                   /* Das HIGHWORD gibt Auskunft ob es ein abstrakter Eintrag (1,2)
                                      oder eine Programmreferenz (3) ist */
                                   switch (*(((PUSHORT) &hobj) + 1))
                                      {
                                      case 1:
                                      case 2:
                                           /* Ein abstrakter Eintrag
                                              LOWWORD ist SchlÅssel */
                                           us = *((PUSHORT) &hobj) ;
                                           /* Nach Text wandeln */
                                           _ltoa (us,
                                                  sz,
                                                  16) ;
                                           /* Nach Upper Case */
                                           strupr (sz) ;
                                           /* Informationen zu diesem abstrakten Eintrag lesen */
                                           ul = PrfQueryProfileString (HINI_USERPROFILE,
                                                                       "PM_Abstract:Objects",
                                                                       sz,
                                                                       NULL,
                                                                       sz,
                                                                       sizeof (sz)) ;
                                           /* Speicherbereich begrenzen */
                                           psz = sz ;
                                           pszMax = sz ;
                                           pszMax += ul - strlen ("WPAbstract") ;
                                           /* In diesem Speicher suchen */
                                           while (psz < pszMax)
                                              {
                                              /* Steht hier irgendwo "WPAbstract"? */
                                              if   (memcmp (psz,
                                                            "WPAbstract",
                                                            strlen ("WPAbstract")) == 0)
                                                   {
                                                   /* Dann steht der Titel 7 Bytes dahinter (Achtung, keine Garantie) */
                                                   psz += strlen ("WPAbstract") + 7 ;
                                                   /* Den gefunden Text ein der rechten Listbox eintragen */
                                                   ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                                                  LM_INSERTITEM,
                                                                                  MPFROMSHORT (LIT_END),
                                                                                  MPFROMP (psz))) ;
                                                   /* Den Object-Handle ebenfalls merken */
                                                   WinSendMsg (pdlg->hwndListbox2,
                                                               LM_SETITEMHANDLE,
                                                               MPFROMSHORT (ul),
                                                               MPFROMLONG (hobj)) ;
                                                   break ;
                                                   }
                                              /* Im Speicher weitersuchen */
                                              psz++ ;
                                              }
                                           break ;
                                      case 3:
                                           /* Eine Programmreferenz
                                              mit diesem API ermitteln */
                                           WinQueryObjectPath (hobj,
                                                               sz,
                                                               sizeof (sz)) ;
                                           /* Den gefunden Text ein der rechten Listbox eintragen */
                                           ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                                          LM_INSERTITEM,
                                                                          MPFROMSHORT (LIT_END),
                                                                          MPFROMP (sz))) ;
                                           /* Den Object-Handle ebenfalls merken */
                                           WinSendMsg (pdlg->hwndListbox2,
                                                       LM_SETITEMHANDLE,
                                                       MPFROMSHORT (ul),
                                                       MPFROMLONG (hobj)) ;
                                           break ;
                                      }
                                   /* Zur nÑchsten Zuordnungsnummer */
                                   pszBuffer += strlen (pszBuffer) + 1 ;
                                   }
                                /* Den ersten Eintrag in der rechten Listbox selektieren */
                                WinSendMsg (pdlg->hwndListbox2,
                                            LM_SELECTITEM,
                                            MPFROMSHORT (0),
                                            MPFROMSHORT (TRUE)) ;
                                }
                           break ;
                      }
                   break ;
              /* Einer der beiden linken Radiobuttons */
              case IDR_V1:
              case IDR_V2:
                   /* Was will der linke Radiobutton? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Es wurde drauf geklickt */
                      case BN_CLICKED:
                           /* Merken welcher Radiobutton es war */
                           pdlg->vstyle = (SHORT1FROMMP (mp1) == IDR_V1) ? VSTYLE_FILTER : VSTYLE_TYPE ;
                           /* Den Inhalt der rechten Listbox lîschen */
                           WinSendMsg (pdlg->hwndListbox1,
                                       LM_DELETEALL,
                                       MPFROMSHORT (TRUE),
                                       MPVOID) ;
                           /* Alle EintrÑge zu den Dateifiltern oder Dateitypen aus der INI lesen */
                           PrfQueryProfileString (HINI_USERPROFILE,
                                                  (pdlg->vstyle == VSTYLE_FILTER) ? "PMWP_ASSOC_FILTER" : "PMWP_ASSOC_TYPE",
                                                  NULL,
                                                  NULL,
                                                  szBuffer,
                                                  sizeof (szBuffer)) ;
                           /* Zum Pointer machen */
                           pszBuffer = szBuffer ;
                           /* Solange noch EintrÑge vorhanden */
                           while (*pszBuffer)
                              {
                              /* Neuen Eintrag in der linken Listbox vornehmen */
                              WinSendMsg (pdlg->hwndListbox1,
                                          LM_INSERTITEM,
                                          MPFROMSHORT (LIT_SORTASCENDING),
                                          MPFROMP (pszBuffer)) ;
                              /* Zum nÑchsten Eintrag im Speicherbereich */
                              pszBuffer += strlen (pszBuffer) + 1 ;
                              }
                           /* Am Schlu· den obersten Eintrag selektieren */
                           WinSendMsg (pdlg->hwndListbox1,
                                       LM_SELECTITEM,
                                       MPFROMSHORT (0),
                                       MPFROMSHORT (TRUE)) ;
                           break ;
                      }
                   break ;
              /* Der Objekt Radiobutton wurde gedrÅckt */
              case IDR_V3:
                   /* Was will der Objekt Radiobutton? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Es wurde drauf geklickt */
                      case BN_CLICKED:
                           /* Den kompletten INI Eintrag mit den Objekt Handles besorgen */
                           PrfQueryProfileString (HINI_USERPROFILE,
                                                  "PM_Abstract:Objects",
                                                  NULL,
                                                  NULL,
                                                  szBuffer,
                                                  sizeof (szBuffer)) ;
                           /* Zum Pointer machen */
                           pszBuffer = szBuffer ;
                           /* Solange noch EintrÑge vorhanden */
                           while (*pszBuffer)
                              {
                              /* Informationen zu diesem Objekt Handle abfragen */
                              ul = PrfQueryProfileString (HINI_USERPROFILE,
                                                          "PM_Abstract:Objects",
                                                          pszBuffer,
                                                          NULL,
                                                          sz,
                                                          sizeof (sz)) ;
                              /* Speicherbreich begrenzen */
                              psz = sz ;
                              pszMax = sz ;
                              pszMax += ul - strlen ("WPAbstract") ;
                              /* Solange im Speicherbereich */
                              while (psz < pszMax)
                                 {
                                 /* Den Text suchen */
                                 if   (memcmp (psz,
                                               "WPAbstract",
                                               strlen ("WPAbstract")) == 0)
                                      {
                                      /* Wenn gefunden den Hex Objekt Handle nach Zahl wandeln */
                                      sscanf (pszBuffer,
                                              "%x",
                                              &hobj) ;
                                      /* Das HIGHWORD 2 setzen fÅr abstraktes Objekt */
                                      *(((PUSHORT) &hobj) + 1) = 2 ;
                                      /* Sieben Bytes weiter steht der Objektname */
                                      psz += strlen ("WPAbstract") + 7 ;
                                      /* Diesen in der Combobox eintragen */
                                      ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndCombobox,
                                                                     LM_INSERTITEM,
                                                                     MPFROMSHORT (LIT_SORTASCENDING),
                                                                     MPFROMP (psz))) ;
                                      /* Den Objekt Handle ebenfalls merken */
                                      WinSendMsg (pdlg->hwndCombobox,
                                                  LM_SETITEMHANDLE,
                                                  MPFROMSHORT (ul),
                                                  MPFROMLONG (hobj)) ;
                                      break ;
                                      }
                                 /* Weitersuchen im BinÑr Puffer */
                                 psz++ ;
                                 }
                              /* Zum nÑchsten Objekt */
                              pszBuffer += strlen (pszBuffer) + 1 ;
                              }
                           /* Zum Schlu· den ersten Eintrag in der Combobox selektieren */
                           WinSendMsg (pdlg->hwndCombobox,
                                       LM_SELECTITEM,
                                       MPFROMSHORT (0),
                                       MPFROMSHORT (TRUE)) ;
                           /* Den Text dieses Eintrags ermitteln */
                           WinSendMsg (pdlg->hwndCombobox,
                                       LM_QUERYITEMTEXT,
                                       MPFROM2SHORT (0, sizeof (sz)),
                                       MPFROMP (sz)) ;
                           /* Und diesen Text im Eingabefeld der Combobox prÑsentieren */
                           WinSetWindowText (pdlg->hwndCombobox,
                                             sz) ;
                           break ;
                      }
                   break ;
              /* Der Programm Radiobutton wurde gedrÅckt */
              case IDR_V4:
                   /* Was will der Programm Radiobutton? */
                   switch (SHORT2FROMMP (mp1))
                      {
                      /* Es wurde drauf geklickt */
                      case BN_CLICKED:
                           /* Den Inhalt der Listbox in der Combobox lîschen */
                           WinSendMsg (pdlg->hwndCombobox,
                                       LM_DELETEALL,
                                       MPVOID,
                                       MPVOID) ;
                           /* ZusÑtzlich das Eingabefeld der Combobox leeren */
                           WinSetWindowText (pdlg->hwndCombobox,
                                             "") ;
                           break ;
                      }
                   break ;
              }
           break ;
           }
      /* Der Dialog wird beendet */
      case WM_DESTROY:
           {
           PDLG pdlg ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* Instanzdaten lîschen */
           free (pdlg) ;
           break ;
           }
      /* Der Dialog wird gestartet */
      case WM_INITDLG:
           {
           PDLG pdlg ;

           /* Speicherbereich fÅr die Instanzdaten allokieren */
           pdlg = calloc (1,
                          sizeof (DLG)) ;
           /* Instanzpointer in WindowWords speichern */
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           /* Einige Handles fÅr den schnelleren Zugriff vorab ermitteln */
           pdlg->hwndCombobox = WinWindowFromID (hwndDlg,
                                                 IDC_V1) ;
           pdlg->hwndListbox1 = WinWindowFromID (hwndDlg,
                                                 IDL_V1) ;
           pdlg->hwndListbox2 = WinWindowFromID (hwndDlg,
                                                 IDL_V2) ;
           /* Den Dateifilter Radiobutton anklicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_V1,
                              BM_CLICK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* Den Dateifilter Radiobutton setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_V1,
                              BM_SETCHECK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* Den Objekt Radiobutton anklicken */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_V3,
                              BM_CLICK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           /* Den Objekt Radiobutton setzen */
           WinSendDlgItemMsg (hwndDlg,
                              IDR_V3,
                              BM_SETCHECK,
                              MPFROMSHORT (TRUE),
                              MPVOID) ;
           return (MRESULT) FALSE ;
           }
      /* VerÑnderte Zuordnungen mÅssen in die INI weggeschrieben werden */
      case WMP_REFRESHRIGHT:
           {
           CHAR    sz [CCHMAXPATH] ;
           CHAR    szBuffer [8192] ;
           HOBJECT hobj ;
           PDLG    pdlg ;
           PSZ     pszBuffer ;
           ULONG   ul ;
           ULONG   ulI ;

           /* Pointer auf Instanzdaten holen */
           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           /* ZunÑchst den selektierten Eintrag aus der linken Listbox ermitteln */
           if   ((ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox1,
                                                 LM_QUERYSELECTION,
                                                 MPFROMSHORT (LIT_FIRST),
                                                 MPVOID))) != (ULONG) LIT_NONE)
                {
                /* Dann den Text zu diesem selektierten Eintrag lesen */
                WinSendMsg (pdlg->hwndListbox1,
                            LM_QUERYITEMTEXT,
                            MPFROM2SHORT (ul, sizeof (sz)),
                            MPFROMP (sz)) ;
                /* Die Anzahl der EintrÑge in der rechten Listbox ermitteln */
                ul = SHORT1FROMMR (WinSendMsg (pdlg->hwndListbox2,
                                               LM_QUERYITEMCOUNT,
                                               MPVOID,
                                               MPVOID)) ;
                /* In einer Schleife durch alle EintrÑge in der rechten Listbox laufen */
                for  (ulI = 0, pszBuffer = szBuffer, *pszBuffer = 0; ulI < ul; ulI++)
                     {
                     /* Den hinterlegten Object-Handle dieses Eintrags ermitteln */
                     hobj = LONGFROMMR (WinSendMsg (pdlg->hwndListbox2,
                                                    LM_QUERYITEMHANDLE,
                                                    MPFROMSHORT (ulI),
                                                    MPVOID)) ;
                     /* Den Objekt Handle zu einer ASCII Nummer machen */
                     _ltoa (hobj,
                            pszBuffer,
                            10) ;
                     /* Pointer weitersetzen */
                     pszBuffer += strlen (pszBuffer) ;
                     /* Eine 0 anhÑngen */
                     *pszBuffer = 0 ;
                     /* Hier kÑme der nÑchste Eintrag hin */
                     pszBuffer++ ;
                     }
                /* Die Daten wegschreiben */
                PrfWriteProfileData (HINI_USERPROFILE,
                                     (pdlg->vstyle == VSTYLE_FILTER) ? "PMWP_ASSOC_FILTER" : "PMWP_ASSOC_TYPE",
                                     sz,
                                     szBuffer,
                                     pszBuffer - szBuffer) ;
                }
           return (MRESULT) FALSE ;
           }
      }
   /* Standardverarbeitung fÅr alle anderen Events */
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

