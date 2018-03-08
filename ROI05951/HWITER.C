/* $Header: D:\projekte\som!wps inside\source\RCS\HWITER.C 1.1 1995/05/25 18:53:24 HaWi Exp $ */

#define HWITer_Class_Source
#include "HWITER.ih"
#include "HWITER.rh"

PSZ pszHWITer = "HWITer" ;

BOOL InitDateSpins (HWND   hwndDlg,
                    PULONG pulFormat,
                    PCDATE pcdate,
                    ULONG  ulDate1ID,
                    ULONG  ulDate2ID,
                    ULONG  ulDate3ID,
                    ULONG  ulDate1DelID,
                    ULONG  ulDate2DelID,
                    ULONG  ulDate3TxtID)
   {
   CHAR  szBuffer [12] ;
   CHAR  szFormat [12] ;
   PSZ   apsz [] = { "MM DD YYYY",
                     "DD MM YYYY",
                     "YYYY MM DD" } ;
   ULONG ulID1 ;
   ULONG ulID2 ;
   ULONG ulID3 ;
   
   if   (!*pulFormat)
        {
        *pulFormat = PrfQueryProfileInt (HINI_PROFILE,
                                         "PM_National",
                                         "iDate",
                                         1) ;
        }
   switch (*pulFormat)
      {
      case 0:
           ulID1 = ulDate2ID ;
           ulID2 = ulDate1ID ;
           ulID3 = ulDate3ID ;
           break ;
      case 1:
           ulID1 = ulDate1ID ;
           ulID2 = ulDate2ID ;
           ulID3 = ulDate3ID ;
           break ;
      case 2:
           ulID1 = ulDate3ID ;
           ulID2 = ulDate2ID ;
           ulID3 = ulDate1ID ;
           break ;
      }
   WinSendDlgItemMsg (hwndDlg,
                      ulDate1ID,
                      SPBM_SETMASTER,
                      MPFROMHWND (WinWindowFromID (hwndDlg,
                                                   ulDate3ID)),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulDate2ID,
                      SPBM_SETMASTER,
                      MPFROMHWND (WinWindowFromID (hwndDlg,
                                                   ulDate3ID)),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID1,
                      SPBM_SETLIMITS,
                      MPFROMLONG (31),
                      MPFROMLONG (0)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID2,
                      SPBM_SETLIMITS,
                      MPFROMLONG (12),
                      MPFROMLONG (0)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID3,
                      SPBM_SETLIMITS,
                      MPFROMLONG (2500),
                      MPFROMLONG (0)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID1,
                      SPBM_SETCURRENTVALUE,
                      MPFROMLONG (pcdate->day),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID2,
                      SPBM_SETCURRENTVALUE,
                      MPFROMLONG (pcdate->month),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID3,
                      SPBM_SETCURRENTVALUE,
                      MPFROMLONG (pcdate->year),
                      MPVOID) ;
   PrfQueryProfileString (HINI_PROFILE,
                          "PM_National",
                          "sDate",
                          ".",
                          szBuffer,
                          sizeof (szBuffer)) ;
   WinSetDlgItemText (hwndDlg,
                      ulDate1DelID,
                      szBuffer) ;
   WinSetDlgItemText (hwndDlg,
                      ulDate2DelID,
                      szBuffer) ;
   strcpy (szFormat,
           apsz [*pulFormat]) ;
   switch (*pulFormat)
      {
      case 0:
      case 1:
           szFormat [2] = *szBuffer ;
           szFormat [5] = *szBuffer ;
           break ;
      case 2:
           szFormat [4] = *szBuffer ;
           szFormat [7] = *szBuffer ;
           break ;
      }
   WinSetDlgItemText (hwndDlg,
                      ulDate3TxtID,
                      szFormat) ;
   return TRUE ;
   }

BOOL InitTimeSpins (HWND   hwndDlg,
                    PCTIME pctime,
                    ULONG  ulTime1ID,
                    ULONG  ulTime2ID,
                    ULONG  ulTime1DelID,
                    ULONG  ulTime2TxtID)
   {
   CHAR  szBuffer [8] ;
   CHAR  szFormat [8] ;
   PSZ   psz = "HH MM" ;
   
   PrfQueryProfileString (HINI_PROFILE,
                          "PM_National",
                          "sTime",
                          ":",
                          szBuffer,
                          sizeof (szBuffer)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime1ID,
                      SPBM_SETMASTER,
                      MPFROMHWND (WinWindowFromID (hwndDlg,
                                                   ulTime2ID)),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime1ID,
                      SPBM_SETLIMITS,
                      MPFROMLONG (23),
                      MPFROMLONG (0)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime2ID,
                      SPBM_SETLIMITS,
                      MPFROMLONG (59),
                      MPFROMLONG (0)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime1ID,
                      SPBM_SETCURRENTVALUE,
                      MPFROMLONG (pctime->hours),
                      MPVOID) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime2ID,
                      SPBM_SETCURRENTVALUE,
                      MPFROMLONG (pctime->minutes),
                      MPVOID) ;
   WinSetDlgItemText (hwndDlg,
                      ulTime1DelID,
                      szBuffer) ;
   strcpy (szFormat,
           psz) ;
   szFormat [2] = *szBuffer ;
   WinSetDlgItemText (hwndDlg,
                      ulTime2TxtID,
                      szFormat) ;
   return TRUE ;
   }

BOOL QueryDateSpins (HWND   hwndDlg,
                     ULONG  ulFormat,
                     PCDATE pcdate,
                     ULONG  ulDate1ID,
                     ULONG  ulDate2ID,
                     ULONG  ulDate3ID)
   {
   BOOL  f ;
   ULONG ulDay ;
   ULONG ulID1 ;
   ULONG ulID2 ;
   ULONG ulID3 ;
   ULONG ulMonth ;
   ULONG ulYear ;
   
   switch (ulFormat)
      {
      case 0:
           ulID1 = ulDate2ID ;
           ulID2 = ulDate1ID ;
           ulID3 = ulDate3ID ;
           break ;
      case 1:
           ulID1 = ulDate1ID ;
           ulID2 = ulDate2ID ;
           ulID3 = ulDate3ID ;
           break ;
      case 2:
           ulID1 = ulDate3ID ;
           ulID2 = ulDate2ID ;
           ulID3 = ulDate1ID ;
           break ;
      }
   WinSendDlgItemMsg (hwndDlg,
                      ulID1,
                      SPBM_QUERYVALUE,
                      MPFROMP (&ulDay),
                      MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID2,
                      SPBM_QUERYVALUE,
                      MPFROMP (&ulMonth),
                      MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulID3,
                      SPBM_QUERYVALUE,
                      MPFROMP (&ulYear),
                      MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
   f = TRUE ;
   if   ((ulDay == 0) ||
         (ulMonth == 0) ||
         (ulYear == 0))
        {
        f = FALSE ;
        }
   else
        {
        if   ((ulDay == 31) &&
              ((ulMonth == 2) ||
               (ulMonth == 4) ||
               (ulMonth == 6) ||
               (ulMonth == 9) ||
               (ulMonth == 11)))
             {
             f = FALSE ;
             }
        else
             {
             if   ((ulDay == 30) &&
                   (ulMonth == 2))
                  {
                  f = FALSE ;
                  }
             else
                  {
                  if   ((ulDay == 29) &&
                        (ulMonth == 2))
                       {
                       if   (ulYear)
                            {
                            if   ((ulYear % 4) != 0)
                                 {
                                 f = FALSE ;
                                 }
                            else
                                 {
                                 if   ((ulYear % 100) == 0)
                                      {
                                      if   ((ulYear % 400) != 0)
                                           {
                                           f = FALSE ;
                                           }
                                      }
                                 }
                            }
                       }
                  }
             }
        }
   pcdate->day = ulDay ;
   pcdate->month = ulMonth ;
   pcdate->year = ulYear ;
   return f ;
   }

BOOL QueryTimeSpins (HWND   hwndDlg,
                     PCTIME pctime,
                     ULONG  ulTime1ID,
                     ULONG  ulTime2ID)
   {
   WinSendDlgItemMsg (hwndDlg,
                      ulTime1ID,
                      SPBM_QUERYVALUE,
                      MPFROMP (&pctime->hours),
                      MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
   WinSendDlgItemMsg (hwndDlg,
                      ulTime2ID,
                      SPBM_QUERYVALUE,
                      MPFROMP (&pctime->minutes),
                      MPFROM2SHORT (0, SPBQ_DONOTUPDATE)) ;
   return TRUE ;
   }

VOID ProcessResult (HWITer** hwiterResult,
                    ULONG    ulCount,
                    PBOOL    pf)
   {
   ULONG ul ;
   
   for  (ul = 0; ul < ulCount; ul++)
        {
        if   (_checkDateTime (hwiterResult [ul]))
             {
             *pf = TRUE ;
             }
        _wpUnlockObject (hwiterResult [ul]) ;
        }
   }
   
VOID Thread (VOID* pv)
   {
   BOOL          f ;
   CLASS         aclass [2] ;
   HAB           hab ;
   HFIND         hfind ;
   HMQ           hmq ;
   HTIMER        htimer ;
   HWITer**      hwiterResult ;
   M_HWITer*     somSelf ;
   M_HWITerData* somThis ;
   SOMAny*       somAnyDesktop ;
   ULONG         aulBeeps [] = { 261,
                                 330,
                                 392,
                                 523,
                                 523,
                                 392,
                                 330,
                                 261 } ;
   ULONG         ul ;
   ULONG         ulCount ;
   ULONG         ulError ;

   somSelf = (M_HWITer*) pv ;
   somThis = M_HWITerGetData (somSelf) ;
   hab = WinInitialize (0) ;
   hmq = WinCreateMsgQueue (hab,
                            0) ;
   WinCancelShutdown (hmq,
                      TRUE) ;
   DosCreateEventSem (NULL,
                      &_hev,
                      0,
                      FALSE) ;
   DosStartTimer (300000,
                  (HSEM) _hev,
                  &htimer) ;
   aclass [0] = somSelf ;
   aclass [1] = NULL ;
   somAnyDesktop = _wpclsQueryFolder (somSelf,
                                      "<WP_DESKTOP>",
                                      FALSE) ;
   hwiterResult = SOMCalloc (10,
                             sizeof (HWITer*)) ;
   while (_f)
      {
      DosWaitEventSem (_hev,
                       300000) ;
      f = FALSE ;
      ulCount = 10 ;
      ulError = 0 ;
      _wpclsSetError (somSelf,
                      0) ;
      if   (!_wpclsFindObjectFirst (somSelf,
                                    aclass,
                                    &hfind,
                                    "*",
                                    somAnyDesktop,
                                    TRUE,
                                    NULL,
                                    hwiterResult,
                                    &ulCount))
           {
           ulError = _wpclsQueryError (somSelf) ;
           }
      if   (!ulError ||
            ulError == WPERR_BUFFER_OVERFLOW)
           {
           ProcessResult (hwiterResult,
                          ulCount,
                          &f) ;
           while (ulCount &&
                  ulError == WPERR_BUFFER_OVERFLOW)
              {
              ulCount = 10 ;
              ulError = 0 ;
              _wpclsSetError (somSelf,
                              0) ;
              if   (!_wpclsFindObjectNext (somSelf,
                                           hfind,
                                           hwiterResult,
                                           &ulCount))
                   {
                   ulError = _wpclsQueryError (somSelf) ;
                   }
              if   (!ulError ||
                    ulError == WPERR_BUFFER_OVERFLOW)
                   {
                   ProcessResult (hwiterResult,
                                  ulCount,
                                  &f) ;
                   }
              }
           }
      _wpclsFindObjectEnd (somSelf,
                           hfind) ;
      DosResetEventSem (_hev,
                        &ul) ;
      if   (f)
           {
           for  (ul = 0; ul < (sizeof (aulBeeps) / sizeof (aulBeeps [0])); ul++)
                {
                DosBeep (aulBeeps [ul],
                         100) ;
                }
           }
      }
   SOMFree (hwiterResult) ;
   DosStopTimer (htimer) ;
   DosCloseEventSem (_hev) ;
   WinDestroyMsgQueue (hmq) ;
   WinTerminate (hab) ;
   _endthread () ;
   }

MRESULT EXPENTRY TerminDlgWndProc (HWND   hwndDlg,
                                   ULONG  msg,
                                   MPARAM mp1,
                                   MPARAM mp2)
   {
   HWITer*     somSelf ;
   HWITerData* somThis ;

   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_DESTROY:
           somSelf = WinQueryWindowPtr (hwndDlg,
                                        QWL_USER) ;
           somThis = HWITerGetData (somSelf) ;
           QueryDateSpins (hwndDlg,
                           _ulFormat,
                           &_cdate,
                           IDSP_Tag,
                           IDSP_Monat,
                           IDSP_Jahr) ;
           QueryTimeSpins (hwndDlg,
                           &_ctime,
                          IDSP_Stunde,
                          IDSP_Minute) ;
           break ;
      case WM_INITDLG:
           somSelf = (HWITer*) mp2 ;
           somThis = HWITerGetData (somSelf) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            mp2) ;
           _ulFormat = 0 ;
           InitDateSpins (hwndDlg,
                          &_ulFormat,
                          &_cdate,
                          IDSP_Tag,
                          IDSP_Monat,
                          IDSP_Jahr,
                          IDT_DatumDelimiter1,
                          IDT_DatumDelimiter2,
                          IDT_DatumDelimiter3) ;
           InitTimeSpins (hwndDlg,
                          &_ctime,
                          IDSP_Stunde,
                          IDSP_Minute,
                          IDT_UhrzeitDelimiter1,
                          IDT_UhrzeitDelimiter2) ;
           return (MRESULT) FALSE ;
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

MRESULT EXPENTRY BemerkungDlgWndProc (HWND   hwndDlg,
                                      ULONG  msg,
                                      MPARAM mp1,
                                      MPARAM mp2)
   {
   HWITer*     somSelf ;
   HWITerData* somThis ;

   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_DESTROY:
           somSelf = WinQueryWindowPtr (hwndDlg,
                                        QWL_USER) ;
           somThis = HWITerGetData (somSelf) ;
           WinQueryDlgItemText (hwndDlg,
                                IDF_Bemerkung,
                                sizeof (_szBemerkung),
                                _szBemerkung) ;
           break ;
      case WM_INITDLG:
           somSelf = (HWITer*) mp2 ;
           somThis = HWITerGetData (somSelf) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            mp2) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDF_Bemerkung,
                              MLM_SETTEXTLIMIT,
                              MPFROMLONG (2047),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDF_Bemerkung,
                              MLM_FORMAT,
                              MPFROMSHORT (MLFIE_NOTRANS),
                              MPVOID) ;
           WinSetDlgItemText (hwndDlg,
                              IDF_Bemerkung,
                              _szBemerkung) ;
           return (MRESULT) FALSE ;
      }
   return WinDefDlgProc (hwndDlg,
                         msg,
                         mp1,
                         mp2) ;
   }

SOM_Scope HMODULE SOMLINK hwiter_clsQueryHmod (M_HWITer* somSelf)
   {
   M_HWITerData* somThis ;

   somThis = M_HWITerGetData (somSelf) ;
   return _hmod ;
   }

SOM_Scope void SOMLINK hwiter_wpclsInitData (M_HWITer* somSelf)
   {
   M_HWITerData* somThis ;
   PSZ           psz ;

   somThis = M_HWITerGetData (somSelf) ;
   M_HWITer_parent_M_WPAbstract_wpclsInitData (somSelf) ;
   psz = _somLocateClassFile (SOMClassMgrObject,
                              SOM_IdFromString (_somGetClassName (somSelf)),
                              1,
                              1) ;
   DosQueryModuleHandle (psz,
                         &_hmod) ;
   _f = TRUE ;
   _beginthread (Thread,
                 NULL,
                 32767,
                 somSelf) ;
   }

SOM_Scope ULONG SOMLINK hwiter_wpclsQueryDefaultView (M_HWITer* somSelf)
   {
   return OPEN_SETTINGS ;
   }

SOM_Scope ULONG SOMLINK hwiter_wpclsQueryIconData (M_HWITer* somSelf,
                                                   PICONINFO pIconInfo)
   {
   M_HWITerData* somThis ;

   somThis = M_HWITerGetData (somSelf) ;
   if   (pIconInfo)
        {
        pIconInfo->fFormat = ICON_RESOURCE ;
        pIconInfo->hmod = _hmod ;
        pIconInfo->resid = IDN_Termin ;
        }
   return sizeof (ICONINFO) ;
   }

SOM_Scope PSZ SOMLINK hwiter_wpclsQueryTitle (M_HWITer* somSelf)
   {
   return "Reminder" ;
   }

SOM_Scope void SOMLINK hwiter_wpclsUnInitData (M_HWITer* somSelf)
   {
   M_HWITerData* somThis ;

   somThis = M_HWITerGetData (somSelf) ;
   _f = FALSE ;
   M_HWITer_parent_M_WPAbstract_wpclsUnInitData (somSelf) ;
   }

SOM_Scope BOOL SOMLINK hwiter_checkDateTime (HWITer* somSelf)
   {
   BOOL        f ;
   CHAR        szCurrentDateTime [20] ;
   CHAR        szLastBeepDate [20] ;
   CHAR        szThisDateTime [20] ;
   DATETIME    date ;
   HWITerData* somThis ;
   
   somThis = HWITerGetData (somSelf) ;
   DosGetDateTime (&date) ;
   sprintf (szCurrentDateTime,
            "%04lu%02lu%02lu%02lu%02lu",
            (ULONG) date.year,
            (ULONG) date.month,
            (ULONG) date.day,
            (ULONG) date.hours,
            (ULONG) date.minutes) ;
   sprintf (szLastBeepDate,
            "%04lu%02lu%02lu",
            (ULONG) _cdateLastBeep.year,
            (ULONG) _cdateLastBeep.month,
            (ULONG) _cdateLastBeep.day) ;
   sprintf (szThisDateTime,
            "%04lu%02lu%02lu%02lu%02lu",
            (ULONG) _cdate.year,
            (ULONG) _cdate.month,
            (ULONG) _cdate.day,
            (ULONG) _ctime.hours,
            (ULONG) _ctime.minutes) ;
   f = FALSE ;
   if   (_cdate.year == 0)
        {
        if   (strcmp (&szCurrentDateTime [4],
                      &szThisDateTime [4]) >= 0)
             {
             f = TRUE ;
             }
        }
   else
        {
        if   (strcmp (szCurrentDateTime,
                      szThisDateTime) >= 0)
             {
             f = TRUE ;
             }
        }
   if   (f)
        {
        f = FALSE ;
        szCurrentDateTime [8] = '\0' ;
        if   (strcmp (szCurrentDateTime,
                      szLastBeepDate) > 0)
             {
             _cdateLastBeep.day = date.day ;
             _cdateLastBeep.month = date.month ;
             _cdateLastBeep.year = date.year ;
             }
        }
   return f ;
   }

SOM_Scope APIRET SOMLINK hwiter_DeskMan2QuerySetup (HWITer*   somSelf, 
                                                    WPObject* setupObject)
   {
   CHAR        sz [] = "ENTRY=YES;" ;
   CHAR        szBuffer [12] ;
   HWITerData* somThis ;
   
   somThis = HWITerGetData (somSelf) ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("fromString"),
                  NULL,
                  sz) ;
   sprintf (szBuffer,
            "%02lu.%02lu.%04lu",
            (ULONG) _cdate.day,
            (ULONG) _cdate.month,
            (ULONG) _cdate.year) ;
   szBuffer [2] = '\0' ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("addSetup"),
                  NULL,
                  "Day",
                  szBuffer) ;
   szBuffer [2] = '.' ;
   szBuffer [5] = '\0' ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("addSetup"),
                  NULL,
                  "Month",
                  &szBuffer [3]) ;
   szBuffer [5] = '.' ;
   szBuffer [11] = '\0' ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("addSetup"),
                  NULL,
                  "Year",
                  &szBuffer [6]) ;
   sprintf (szBuffer,
            "%02lu:%02lu",
            (ULONG) _ctime.hours,
            (ULONG) _ctime.minutes) ;
   szBuffer [2] = '\0' ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("addSetup"),
                  NULL,
                  "Hour",
                  szBuffer) ;
   szBuffer [2] = ':' ;
   szBuffer [5] = '\0' ;
   _somDispatchL (setupObject,
                  SOM_IdFromString ("addSetup"),
                  NULL,
                  "Minute",
                  &szBuffer [3]) ;
   if   (*_szBemerkung)
        {
        _somDispatchL (setupObject,
                       SOM_IdFromString ("addSetup"),
                       NULL,
                       "Comment",
                       _szBemerkung) ;
        }
   return 0 ;
   }

SOM_Scope BOOL SOMLINK hwiter_wpAddSettingsPages (HWITer* somSelf,
                                                  HWND    hwndNotebook)
   {
   BOOL     f ;
   PAGEINFO pginf ;

   f = HWITer_parent_WPAbstract_wpAddSettingsPages (somSelf,
                                                    hwndNotebook) ;
   memset (&pginf,
           0,
           sizeof (PAGEINFO)) ;
   pginf.cb = sizeof (PAGEINFO) ;
   pginf.pfnwp = BemerkungDlgWndProc ;
   pginf.resid = hwiter_clsQueryHmod (_somGetClass (somSelf)) ;
   pginf.pCreateParams = somSelf ;
   pginf.dlgid = IDD_Bemerkung ;
   pginf.usPageStyleFlags = BKA_MAJOR ;
   pginf.usPageInsertFlags = BKA_FIRST ;
   pginf.pszName = "Kommentar" ;
   pginf.idDefaultHelpPanel = IDD_Bemerkung ;
   pginf.pszHelpLibraryName = "HWITER.HLP" ;
   DosGetResource (pginf.resid,
                   RT_HELPSUBTABLE,
                   IDD_Bemerkung,
                   &pginf.pHelpSubtable) ;
   _wpInsertSettingsPage (somSelf,
                          hwndNotebook,
                          &pginf) ;
   DosFreeResource (pginf.pHelpSubtable) ;
   memset (&pginf,
           0,
           sizeof (PAGEINFO)) ;
   pginf.cb = sizeof (PAGEINFO) ;
   pginf.pfnwp = TerminDlgWndProc ;
   pginf.resid = hwiter_clsQueryHmod (_somGetClass (somSelf)) ;
   pginf.pCreateParams = somSelf ;
   pginf.dlgid = IDD_Termin ;
   pginf.usPageStyleFlags = BKA_MAJOR ;
   pginf.usPageInsertFlags = BKA_FIRST ;
   pginf.pszName = "Termin" ;
   pginf.idDefaultHelpPanel = IDD_Termin ;
   pginf.pszHelpLibraryName = "HWITER.HLP" ;
   DosGetResource (pginf.resid,
                   RT_HELPSUBTABLE,
                   IDD_Termin,
                   &pginf.pHelpSubtable) ;
   _wpInsertSettingsPage (somSelf,
                          hwndNotebook,
                          &pginf) ;
   DosFreeResource (pginf.pHelpSubtable) ;
   return f ;
   }

SOM_Scope BOOL SOMLINK hwiter_wpFormatDragItem (HWITer*   somSelf,
                                                PDRAGITEM pdrgItem)
   {
   PSZ psz = "DRT_UNKNOWN, DRT_TERMIN" ;
   
   DrgDeleteStrHandle (pdrgItem->hstrType) ;
   pdrgItem->hstrType = DrgAddStrHandle (psz) ;
   return HWITer_parent_WPAbstract_wpFormatDragItem (somSelf, 
                                                     pdrgItem) ;
   }

SOM_Scope void SOMLINK hwiter_wpInitData (HWITer* somSelf)
   {
   DATETIME    date ;
   HWITerData* somThis ;
   
   somThis = HWITerGetData (somSelf) ;
   HWITer_parent_WPAbstract_wpInitData (somSelf) ;
   memset (&_cdateLastBeep,
           0,
           sizeof (_cdateLastBeep)) ;
   DosGetDateTime (&date) ;
   _cdate.day = date.day ;
   _cdate.month = date.month ;
   _cdate.year = date.year ;
   _ctime.hours = date.hours ;
   _ctime.minutes = date.minutes ;
   }

SOM_Scope BOOL SOMLINK hwiter_wpRestoreState (HWITer* somSelf,
                                              ULONG   ulReserved)
   {
   HWITerData* somThis ;
   ULONG       ul ;

   somThis = HWITerGetData (somSelf) ;
   ul = sizeof (_cdate) ;
   _wpRestoreData (somSelf,
                   pszHWITer,
                   1,
                   (PBYTE) &_cdate,
                   &ul) ;
   ul = sizeof (_cdateLastBeep) ;
   _wpRestoreData (somSelf,
                   pszHWITer,
                   2,
                   (PBYTE) &_cdateLastBeep,
                   &ul) ;
   ul = sizeof (_ctime) ;
   _wpRestoreData (somSelf,
                   pszHWITer,
                   3,
                   (PBYTE) &_ctime,
                   &ul) ;
   ul = sizeof (_szBemerkung) ;
   _wpRestoreString (somSelf,
                     pszHWITer,
                     4,
                     _szBemerkung,
                     &ul) ;
   return HWITer_parent_WPAbstract_wpRestoreState (somSelf,
                                                   ulReserved) ;
   }

SOM_Scope BOOL SOMLINK hwiter_wpSaveState (HWITer* somSelf)
   {
   HWITerData* somThis ;

   somThis = HWITerGetData (somSelf) ;
   _wpSaveData (somSelf,
                pszHWITer,
                1,
                (PBYTE) &_cdate,
                sizeof (_cdate)) ;
   _wpSaveData (somSelf,
                pszHWITer,
                2,
                (PBYTE) &_cdateLastBeep,
                sizeof (_cdateLastBeep)) ;
   _wpSaveData (somSelf,
                pszHWITer,
                3,
                (PBYTE) &_ctime,
                sizeof (_ctime)) ;
   _wpSaveString (somSelf,
                  pszHWITer,
                  4,
                  _szBemerkung) ;
   return HWITer_parent_WPAbstract_wpSaveState (somSelf) ;
   }

SOM_Scope BOOL SOMLINK hwiter_wpSetup (HWITer* somSelf,
                                       PSZ     pszSetupString)
   {
   CHAR        szBuffer [12] ;
   HWITerData* somThis ;
   ULONG       ul ;
   
   somThis = HWITerGetData (somSelf) ;
   ul = sizeof (szBuffer) ;
   if   (_wpScanSetupString (somSelf,
                             pszSetupString,
                             "Day",
                             szBuffer,
                             &ul))
        {
        _cdate.day = atol (szBuffer) ;
        }
   ul = sizeof (szBuffer) ;
   if   (_wpScanSetupString (somSelf,
                             pszSetupString,
                             "Month",
                             szBuffer,
                             &ul))
        {
        _cdate.month = atol (szBuffer) ;
        }
   ul = sizeof (szBuffer) ;
   if   (_wpScanSetupString (somSelf,
                             pszSetupString,
                             "Year",
                             szBuffer,
                             &ul))
        {
        _cdate.year = atol (szBuffer) ;
        }
   ul = sizeof (szBuffer) ;
   if   (_wpScanSetupString (somSelf,
                             pszSetupString,
                             "Hour",
                             szBuffer,
                             &ul))
        {
        _ctime.hours = atol (szBuffer) ;
        }
   ul = sizeof (szBuffer) ;
   if   (_wpScanSetupString (somSelf,
                             pszSetupString,
                             "Minute",
                             szBuffer,
                             &ul))
        {
        _ctime.minutes = atol (szBuffer) ;
        }
   ul = sizeof (_szBemerkung) ;
   _wpScanSetupString (somSelf,
                       pszSetupString,
                       "Comment",
                       _szBemerkung,
                       &ul) ;
   return HWITer_parent_WPAbstract_wpSetup (somSelf,
                                            pszSetupString) ;
   }
