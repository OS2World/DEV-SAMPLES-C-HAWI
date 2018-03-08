/* $Header: D:\projekte\versteckte informationen\source\RCS\IOS2P009.C 1.1 1995/05/25 19:01:19 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOS2P009.H"

HAB  hab_g = NULLHANDLE ;

typedef struct
   {
   HWND    ahwndDialogs [PAGES + 1] ;
   HWND    hwndNotebook ;
   ULONG   aulPageID [PAGES + 1] ;
   ULONG   aulStyle [PAGES + 1] ;
   } IOS2PDLG, *PIOS2PDLG ;

typedef struct
   {
   HWND  hwndContainer ;
   ULONG ulStyle ;
   } IOS2PDLG1, *PIOS2PDLG1 ;

/* *******************************************************************
   *** Function: main                                              ***
   ******************************************************************* */
INT main (VOID)
   {
   HMQ hmq ;
   
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             WinDlgBox (HWND_DESKTOP,
                        HWND_DESKTOP,
                        (PFNWP) IOS2P_DlgWndProc,
                        NULLHANDLE,
                        IDD_IOS2P,
                        NULL) ;
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

MRESULT EXPENTRY IOS2P_DlgWndProc (const HWND hwndDlg,
                                   const MSG msg,
                                   const MPARAM mp1,
                                   const MPARAM mp2)
   {
   PIOS2PDLG            pdlg ;
   ULONG                ulI ;
   static const PGEINFO apgeinfo [] = {{ "Settings",      "1 of 1", BKA_MAJOR, 0, BKA_LAST },
                                       { "Overview",      "1 of 1", BKA_MAJOR, 0, BKA_LAST },
                                       { "Modules",       "1 of 2", BKA_MAJOR, 0, BKA_LAST },
                                       { "References",    "2 of 2", BKA_MINOR, 2, BKA_NEXT },
                                       { "Semaphores",    "1 of 1", BKA_MAJOR, 0, BKA_LAST },
                                       { "Shared Memory", "1 of 1", BKA_MAJOR, 0, BKA_LAST },
                                       { "Processes",     "1 of 2", BKA_MAJOR, 0, BKA_LAST },
                                       { "Threads",       "2 of 2", BKA_MINOR, 6, BKA_NEXT },
                                       { "Hardware",      "1 of 1", BKA_MAJOR, 0, BKA_LAST },
                                       { "PM",            "1 of 1", BKA_MAJOR, 0, BKA_LAST }} ;
   
   pdlg = WinQueryWindowPtr (hwndDlg,
                             QWL_USER) ;
   switch (msg)
      {
      case WM_COMMAND:
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              }
           break ;
      case WM_CONTROL:
           switch (SHORT1FROMMP (mp1))
              {
              case IDNO_IOS2P_1:
                   switch (SHORT2FROMMP (mp1))
                      {
                      case BKN_PAGESELECTED:
                           for  (ulI = 0; ulI < PAGES; ulI++)
                                {
                                if   (((PPAGESELECTNOTIFY) mp2)->ulPageIdCur == pdlg->aulPageID [ulI])
                                     {
                                     WinPostMsg (pdlg->ahwndDialogs [ulI],
                                                 WMP_CLEAR,
                                                 MPVOID,
                                                 MPVOID) ;
                                     break ;
                                     }
                                }
                           for  (ulI = 0; ulI < PAGES; ulI++)
                                {
                                if   (((PPAGESELECTNOTIFY) mp2)->ulPageIdNew == pdlg->aulPageID [ulI])
                                     {
                                     WinPostMsg (pdlg->ahwndDialogs [ulI],
                                                 WMP_MAININIT,
                                                 MPVOID,
                                                 MPVOID) ;
                                     break ;
                                     }
                                }
                           break ;
                      }
                   break ;
              }
           break ;
      case WM_DESTROY:
           for  (ulI = 0; ulI < PAGES; ulI++)
                {
                WinDestroyWindow (pdlg->ahwndDialogs [ulI]) ;
                }
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           pdlg = calloc (1,
                          sizeof (IOS2PDLG)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           pdlg->hwndNotebook = WinWindowFromID (hwndDlg,
                                                 IDNO_IOS2P_1) ;
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETDIMENSIONS,
                       MPFROM2SHORT (136, 26),
                       MPFROMSHORT (BKA_MAJORTAB)) ;
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETDIMENSIONS,
                       MPFROM2SHORT (136, 26),
                       MPFROMSHORT (BKA_MINORTAB)) ;
           WinSendMsg (pdlg->hwndNotebook,
                       BKM_SETNOTEBOOKCOLORS,
                       MPFROMLONG (SYSCLR_FIELDBACKGROUND),
                       MPFROMSHORT (BKA_BACKGROUNDPAGECOLORINDEX)) ;
           for  (ulI = 0; ulI < PAGES; ulI++)
                {
                pdlg->aulStyle [ulI] = ulI + 1 ;
                pdlg->aulPageID [ulI] = (ULONG) WinSendMsg (pdlg->hwndNotebook,
                                                            BKM_INSERTPAGE,
                                                            MPFROMLONG (pdlg->aulPageID [apgeinfo[ulI].ulMajor]),
                                                            MPFROM2SHORT ((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | apgeinfo[ulI].ulFlag), apgeinfo[ulI].ulOrder)) ;
                WinSendMsg (pdlg->hwndNotebook,
                            BKM_SETTABTEXT,
                            MPFROMLONG (pdlg->aulPageID [ulI]),
                            MPFROMP (apgeinfo[ulI].pszTitle)) ;
                pdlg->ahwndDialogs [ulI] = WinLoadDlg (pdlg->hwndNotebook,
                                                       pdlg->hwndNotebook,
                                                       (PFNWP) IOS2P_DlgWndProc_1,
                                                       NULLHANDLE,
                                                       IDD_IOS2P_1,
                                                       &pdlg->aulStyle [ulI]) ;
                WinSendMsg (pdlg->hwndNotebook,
                            BKM_SETPAGEWINDOWHWND,
                            MPFROMLONG (pdlg->aulPageID [ulI]),
                            MPFROMHWND (pdlg->ahwndDialogs [ulI])) ;
                WinSendMsg (pdlg->hwndNotebook,
                            BKM_SETSTATUSLINETEXT,
                            MPFROMLONG (pdlg->aulPageID [ulI]),
                            MPFROMP (apgeinfo[ulI].pszSide)) ;
                }
           WinPostMsg (pdlg->ahwndDialogs [0],
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }

typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szTopic [256] ;
   PSZ            pszTopic ;
   ULONG          ulValue ;
   } USRREC1, *PUSRREC1 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szHandle [8] ;
   CHAR           szName [CCHMAXPATH] ;
   CHAR           szType [12] ;
   PSZ            pszHandle ;
   PSZ            pszName ;
   PSZ            pszType ;
   ULONG          ulRefCount ;
   ULONG          ulSegCount ;
   } USRREC3, *PUSRREC3 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szModule [CCHMAXPATH] ;
   CHAR           szReference [CCHMAXPATH] ;
   PSZ            pszModule ;
   PSZ            pszReference ;
   } USRREC4, *PUSRREC4 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szFlags [256] ;
   CHAR           szIndex [8] ;
   CHAR           szName [256] ;
   PSZ            pszFlags ;
   PSZ            pszIndex ;
   PSZ            pszName ;
   ULONG          ulRefCount ;
   ULONG          ulReqCount ;
   } USRREC5, *PUSRREC5 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szHandle [8] ;
   CHAR           szName [256] ;
   CHAR           szSelektor [8] ;
   PSZ            pszHandle ;
   PSZ            pszName ;
   PSZ            pszSelektor ;
   ULONG          ulRefCount ;
   } USRREC6, *PUSRREC6 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szHandle [8] ;
   CHAR           szName [CCHMAXPATH] ;
   PSZ            pszHandle ;
   PSZ            pszName ;
   ULONG          ulDLLs ;
   ULONG          ulID ;
   ULONG          ulParentID ;
   ULONG          ulSem16s ;
   ULONG          ulSessionID ;
   ULONG          ulShrMems ;
   ULONG          ulStatus ;
   ULONG          ulThreadCount ;
   ULONG          ulType ;
   } USRREC7, *PUSRREC7 ;
   
typedef struct
   {
   MINIRECORDCORE minirec ;
   CHAR           szPID [16] ;
   CHAR           szState [12] ;
   PSZ            pszPID ;
   PSZ            pszState ;
   ULONG          ulBlockID ;
   ULONG          ulPriority ;
   ULONG          ulSlot ;
   ULONG          ulSysTime ;
   ULONG          ulTID ;
   ULONG          ulType ;
   ULONG          ulUserTime ;
   } USRREC8, *PUSRREC8 ;
   
MRESULT EXPENTRY IOS2P_DlgWndProc_1 (const HWND hwndDlg,
                                     const MSG msg,
                                     const MPARAM mp1,
                                     const MPARAM mp2)
   {
   BOOL              f ;
   CHAR              szBuffer [4] ;
   CNRINFO           ccinfo ;
   static CONTNRINIT acoin1 [] = {{ "Topic",      CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC1, pszTopic)      },
                                  { "Value",      CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC1, ulValue)       }} ;
   static CONTNRINIT acoin3 [] = {{ "Name",       CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC3, pszName)       },
                                  { "Handle",     CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC3, pszHandle)     },
                                  { "Type",       CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC3, pszType)       },
                                  { "References", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC3, ulRefCount)    },
                                  { "Segments",   CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC3, ulSegCount)    }} ;
   static CONTNRINIT acoin4 [] = {{ "Module",     CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC4, pszModule)     },
                                  { "Reference",  CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC4, pszReference)  }} ;
   static CONTNRINIT acoin5 [] = {{ "Name",       CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC5, pszName)       },
                                  { "Index",      CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC5, pszIndex)      },
                                  { "Flags",      CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC5, pszFlags)      },
                                  { "References", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC5, ulRefCount)    },
                                  { "Requests",   CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC5, ulReqCount)    }} ;
   static CONTNRINIT acoin6 [] = {{ "Name",       CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC6, pszName)       },
                                  { "Handle",     CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC6, pszHandle)     },
                                  { "Selektor",   CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC6, pszSelektor)   },
                                  { "References", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC6, ulRefCount)    }} ;
   static CONTNRINIT acoin7 [] = {{ "Name",       CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, pszName)       },
                                  { "ID",         CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulID)          },
                                  { "Parent ID",  CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulParentID)    },
                                  { "Type",       CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulType)        },
                                  { "Status",     CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulStatus)      },
                                  { "Session ID", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulSessionID)   },
                                  { "Handle",     CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, pszHandle)     },
                                  { "Threads",    CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulThreadCount) },
                                  { "16-Bit Sem", CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulSem16s)      },
                                  { "DLLs",       CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulDLLs)        },
                                  { "ShareMems",  CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC7, ulShrMems)     }} ;
   static CONTNRINIT acoin8 [] = {{ "PID",        CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, pszPID)        },
                                  { "Type",       CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulType)        },
                                  { "TID",        CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulTID)         },
                                  { "Slot",       CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulSlot)        },
                                  { "Block ID",   CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulBlockID)     },
                                  { "Priority",   CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulPriority)    },
                                  { "SysTime",    CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulSysTime)     },
                                  { "UserTime",   CFA_ULONG | CFA_HORZSEPARATOR | CFA_RIGHT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, ulUserTime)    },
                                  { "State",      CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR,  CFA_CENTER, offsetof (USRREC8, pszState)      }} ;
   FIELDINFOINSERT   fiins ;
   LONG              l ;
   PBUFFHEADER       pbh ;
   PCONTNRINIT       pcoin ;
   PFIELDINFO        pfldinfo ;
   PFIELDINFO        pfldinfoFirst ;
   PIOS2PDLG1        pdlg ;
   PMODINFO          pmi ;
   PMODINFO          pmiSearch ;
   PPROCESSINFO      ppi ;
   PSEMINFO          psi ;
   PSHRMEMINFO       psmi ;
   PSZ               psz ;
   static const PSZ  pszHeader1 = "Settings" ;
   static const PSZ  pszHeader2 = "Overview" ;
   static const PSZ  pszHeader3 = "Modules" ;
   static const PSZ  pszHeader4 = "References" ;
   static const PSZ  pszHeader5 = "Semaphores" ;
   static const PSZ  pszHeader6 = "Shared Memory" ;
   static const PSZ  pszHeader7 = "Processes" ;
   static const PSZ  pszHeader8 = "Threads" ;
   static const PSZ  pszHeader9 = "Hardware" ;
   static const PSZ  pszHeader10 = "PM" ;
   static const PSZ  pszTopics1 [] = { "Maximum length, in bytes, of a path name",
                                       "Maximun number of text sessions",
                                       "Maximum number of PM sessions",
                                       "Maximum number of DOS sessions",
                                       "Number of boot drive",
                                       "Dynamic priority variation flag (0=ABSOLUTE, 1=DYNAMIC)",
                                       "Maximum wait in seconds",
                                       "Minimum time slice in milliseconds",
                                       "Maximum timeslice in milliseconds",
                                       "Memory page size in bytes",
                                       "Major version number",
                                       "Minor version number",
                                       "Revision letter",
                                       "Free-running millisecond counter",
                                       "Low order of the time in seconds since 01.01.1970",
                                       "High order of the time in seconds since 01.01.1970",
                                       "Total number of pages of physical memory",
                                       "Total number of pages of resident memory",
                                       "Maximum number of pages of memory that can be allocated by all processes",
                                       "Maximum number of bytes of memory that this process can allocate in its private arena",
                                       "Maximum number of bytes of memory that a process can allocate in the shared arena",
                                       "Timer intervall in 10th of a millisecond",
                                       "Maximum length, in bytes, of one component in a path name" } ;
   static const PSZ  pszTopics2 [] = { "Threads",
                                       "Processes",
                                       "Modules" } ;
   static const PSZ  pszTopics9 [] = { "Number of attached printers",
                                       "Number of RS232 ports",
                                       "Number of diskette drives",
                                       "Presence of math coprocessor (0=NO, 1= YES)",
                                       "PC submodel type",
                                       "PC model type",
                                       "Type of primary display adapter (0=MONO, 1=OTHER)" } ;
   static const PSZ  pszTopics10A [] = { "1 indicates that pointing device buttons are swapped",
                                         "Pointing device doubleclick time in milliseconds",
                                         "Width of the pointing device doubleclick sensitive area",
                                         "Height of the pointing device doubleclick sensitive area",
                                         "Width of the sizing border",
                                         "Height of the sizing border",
                                         "1 indicates that alarm sound generated by WinAlarm is enables" } ;
   static const PSZ  pszTopics10B [] = { "Cursor blink rate in milliseconds",
                                         "The delay in milliseconds before autoscrolling starts, when using a scrollbar",
                                         "The delay in milliseconds between scroll operations, when using a scrollbar",
                                         "SV_NUMBEREDLISTS",
                                         "Frequency for warning alarms generated by WinAlarm",
                                         "Frequency for note alarms generated by WinAlarm",
                                         "Frequency for error alarms generated by WinAlarm",
                                         "Duration for warning alarms generated by WinAlarm",
                                         "Duration for note alarms generated by WinAlarm",
                                         "Duration for error alarms generated by WinAlarm" } ;
   static const PSZ  pszTopics10C [] = { "Width of the screen",
                                         "Height of the screen",
                                         "Width of the vertical scrollbar",
                                         "Height of the horizontal scrollbar",
                                         "Height of the vertical scrollbar arrow bitmaps",
                                         "Width of the horizontal scrollbar arrow bitmaps",
                                         "Width of the nominal-width border",
                                         "Height of the nominal-width border",
                                         "Width of the dialog-frame border",
                                         "Height of the dialog-frame border",
                                         "Height of the caption",
                                         "Height of the vertical scrollbar thumb",
                                         "Width of the horizontal scrollbar thumb",
                                         "Width of the minimize/maximize buttons",
                                         "Height of the minimize/maximize buttons",
                                         "Height of the single-line menu height",
                                         "Width of the client area when the window is fullscreen",
                                         "Height of the client area when the window is fullscreen (excluding menu height)",
                                         "Icon width",
                                         "Icon height",
                                         "Pointer width",
                                         "Pointer height",
                                         "0 indicates this is not a debug system",
                                         "The number of buttons on the pointing device (0=no pointing device)",
                                         "Pointer hide level (0=Pointer visible, >0=Pointer not visible)",
                                         "Cursor hide level",
                                         "Tracking rectangle hide level",
                                         "Count of available timers",
                                         "0 indicates a mouse pointing device attached to the system",
                                         "Horizontal count of pels for alignment",
                                         "Vertical count of pels for alignment" } ;
   static const PSZ  pszTopics10D [] = { "The delay in milliseconds before displaying a pulldown refered to from a submenu item",
                                         "The delay in milliseconds before hiding a pulldown refered to from a submenu item",
                                         "SV_ALTMNEMONIC",
                                         "SV_TASKLISTMOUSEACCESS",
                                         "SV_CXICONTEXTWIDTH",
                                         "SV_CICONTEXTLINES",
                                         "SV_CHORDTIME",
                                         "SV_CXCHORD",
                                         "SV_CYCHORD",
                                         "SV_CXMOTIONSTART",
                                         "SV_CYMOTIONSTART",
                                         "Mouse begin drag",
                                         "Mouse end drag",
                                         "SV_SINGLESELECT",
                                         "Mouse open",
                                         "Mouse request popup menu",
                                         "SV_CONTEXTHELP",
                                         "Mouse begin direct name edit",
                                         "Mouse begin swipe select",
                                         "Mouse select or end swipe select",
                                         "SV_BEGINDRAGKB",
                                         "SV_ENDDRAGKB",
                                         "SV_SELECTKB",
                                         "SV_OPENKB",
                                         "Keyboard request popup menu",
                                         "SV_CONTEXTHELPKB",
                                         "SV_TEXTEDITKB",
                                         "SV_BEGINSELECTKB",
                                         "SV_ENDSELECTKB",
                                         "SV_ANIMATION",
                                         "SV_ANIMATIONSPEED",
                                         "SV_MONOICONS",
                                         "Keyboard ID of the newly attached keyboard",
                                         "1 indicates Print Screen function is enabled",
                                         "SV_CSYSVALUES" } ;
   PTHREADINFO       pti ;
   PULONG            paul ;
   PUSRREC1          pusrrec1 ;
   PUSRREC3          pusrrec3 ;
   PUSRREC4          pusrrec4 ;
   PUSRREC5          pusrrec5 ;
   PUSRREC6          pusrrec6 ;
   PUSRREC7          pusrrec7 ;
   PUSRREC8          pusrrec8 ;
   RECORDINSERT      recins ;
   ULONG             ulI ;
   ULONG             ulSizeCoin ;
   ULONG             ulSizeCoin0 ;
   
   pdlg = WinQueryWindowPtr (hwndDlg,
                             QWL_USER) ;
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
           free (pdlg) ;
           break ;
      case WM_INITDLG:
           pdlg = calloc (1,
                          sizeof (IOS2PDLG1)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           pdlg->hwndContainer = WinWindowFromID (hwndDlg,
                                                  IDCO_IOS2P_1) ;
           pdlg->ulStyle = *((PULONG) PVOIDFROMMP (mp2)) ;
           switch (pdlg->ulStyle)
              {
              case 1:
                   pcoin = acoin1 ;
                   ulSizeCoin = sizeof (acoin1) ;
                   ulSizeCoin0 = sizeof (acoin1 [0]) ;
                   psz = pszHeader1 ;
                   break ;
              case 2:
                   pcoin = acoin1 ;
                   ulSizeCoin = sizeof (acoin1) ;
                   ulSizeCoin0 = sizeof (acoin1 [0]) ;
                   psz = pszHeader2 ;
                   break ;
              case 3:
                   pcoin = acoin3 ;
                   ulSizeCoin = sizeof (acoin3) ;
                   ulSizeCoin0 = sizeof (acoin3 [0]) ;
                   psz = pszHeader3 ;
                   break ;
              case 4:
                   pcoin = acoin4 ;
                   ulSizeCoin = sizeof (acoin4) ;
                   ulSizeCoin0 = sizeof (acoin4 [0]) ;
                   psz = pszHeader4 ;
                   break ;
              case 5:
                   pcoin = acoin5 ;
                   ulSizeCoin = sizeof (acoin5) ;
                   ulSizeCoin0 = sizeof (acoin5 [0]) ;
                   psz = pszHeader5 ;
                   break ;
              case 6:
                   pcoin = acoin6 ;
                   ulSizeCoin = sizeof (acoin6) ;
                   ulSizeCoin0 = sizeof (acoin6 [0]) ;
                   psz = pszHeader6 ;
                   break ;
              case 7:
                   pcoin = acoin7 ;
                   ulSizeCoin = sizeof (acoin7) ;
                   ulSizeCoin0 = sizeof (acoin7 [0]) ;
                   psz = pszHeader7 ;
                   break ;
              case 8:
                   pcoin = acoin8 ;
                   ulSizeCoin = sizeof (acoin8) ;
                   ulSizeCoin0 = sizeof (acoin8 [0]) ;
                   psz = pszHeader8 ;
                   break ;
              case 9:
                   pcoin = acoin1 ;
                   ulSizeCoin = sizeof (acoin1) ;
                   ulSizeCoin0 = sizeof (acoin1 [0]) ;
                   psz = pszHeader9 ;
                   break ;
              case 10:
                   pcoin = acoin1 ;
                   ulSizeCoin = sizeof (acoin1) ;
                   ulSizeCoin0 = sizeof (acoin1 [0]) ;
                   psz = pszHeader10 ;
                   break ;
              }
           pfldinfo = WinSendMsg (pdlg->hwndContainer,
                                  CM_ALLOCDETAILFIELDINFO,
                                  MPFROMLONG (ulSizeCoin / ulSizeCoin0),
                                  MPVOID) ;
           pfldinfoFirst = pfldinfo ;
           for  (ulI = 0; ulI < (ulSizeCoin / ulSizeCoin0); ulI++)
                {
                pfldinfo->cb = sizeof (*pfldinfo) ;
                pfldinfo->flData = pcoin[ulI].ulFlData ;
                pfldinfo->flTitle = pcoin[ulI].ulFlTitle ;
                pfldinfo->pTitleData = pcoin[ulI].pszTitle ;
                pfldinfo->offStruct = pcoin[ulI].ulOffStruct ;
                pfldinfo = pfldinfo->pNextFieldInfo ;
                }
           fiins.cb = sizeof (fiins) ;
           fiins.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST ;
           fiins.cFieldInfoInsert = ulSizeCoin / ulSizeCoin0 ;
           fiins.fInvalidateFieldInfo = TRUE ;
           WinSendMsg (pdlg->hwndContainer,
                       CM_INSERTDETAILFIELDINFO,
                       MPFROMP (pfldinfoFirst),
                       MPFROMP (&fiins)) ;
           memset (&ccinfo,
                   0,
                   sizeof (ccinfo)) ;
           ccinfo.flWindowAttr = CV_DETAIL | CV_MINI | CA_CONTAINERTITLE | CA_TITLECENTER | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES ;
           ccinfo.pszCnrTitle = psz ;
           WinSendMsg (pdlg->hwndContainer,
                       CM_SETCNRINFO,
                       MPFROMP (&ccinfo),
                       MPFROMLONG (CMA_FLWINDOWATTR | CMA_CNRTITLE)) ;
           return (MRESULT) FALSE ;
      case WMP_CLEAR:
           WinSendMsg (pdlg->hwndContainer,
                       CM_REMOVERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_FREE)) ;
           return (MRESULT) FALSE ;
      case WMP_MAININIT:
           switch (pdlg->ulStyle)
              {
              case 1:
                   paul = calloc (1,
                                  QSV_MAX * sizeof (ULONG)) ;
                   DosQuerySysInfo (QSV_MAX_PATH_LENGTH,
                                    QSV_MAX_COMP_LENGTH,
                                    paul,
                                    QSV_MAX * sizeof (ULONG)) ;
                   for  (ulI = QSV_MAX_PATH_LENGTH; ulI <= QSV_MAX_COMP_LENGTH; ulI++)
                        {
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics1 [ulI - QSV_MAX_PATH_LENGTH]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = paul [ulI - QSV_MAX_PATH_LENGTH] ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   free (paul) ;
                   break ;
              case 2:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   paul = (PULONG) pbh->psumm ;
                   for  (ulI = 0; ulI < 3; ulI++)
                        {
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics2 [ulI]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = paul [ulI] ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   free (pbh) ;
                   break ;
              case 3:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (pmi = pbh->pmi; pmi; pmi = pmi->pNext)
                        {
                        pusrrec3 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC3) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        sprintf (pusrrec3->szHandle,
                                 "%04X",
                                 (ULONG) pmi->hMod) ;
                        pusrrec3->pszHandle = pusrrec3->szHandle ;
                        switch (pmi->usModType)
                           {
                           case 0:
                                strcpy (pusrrec3->szType,
                                        "16 Bit") ;
                                break ;
                           case 1:
                                strcpy (pusrrec3->szType,
                                        "32 Bit") ;
                                break ;
                           }
                        pusrrec3->pszType = pusrrec3->szType ;
                        pusrrec3->ulRefCount = pmi->ulModRefCount ;
                        pusrrec3->ulSegCount = pmi->ulSegmentCount ;
                        strcpy (pusrrec3->szName,
                                pmi->szModName) ;
                        pusrrec3->pszName = pusrrec3->szName ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec3),
                                    MPFROMP (&recins)) ;
                       }
                   free (pbh) ;
                   break ;
              case 4:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (pmi = pbh->pmi; pmi; pmi = pmi->pNext)
                        {
                        f = FALSE ;
                        for  (ulI = 0; ulI < pmi->ulModRefCount; ulI++)
                             {
                             pusrrec4 = WinSendMsg (pdlg->hwndContainer,
                                                    CM_ALLOCRECORD,
                                                    MPFROMLONG (sizeof (USRREC4) - sizeof (MINIRECORDCORE)),
                                                    MPFROMSHORT (1)) ;
                             if   (!f)
                                  {
                                  strcpy (pusrrec4->szModule,
                                          pmi->szModName) ;
                                  f = TRUE ;
                                  }
                             else
                                  {
                                  *pusrrec4->szModule = '\0' ;
                                  }
                             pusrrec4->pszModule = pusrrec4->szModule ;
                             pmiSearch = pbh->pmi ;
                             while (pmiSearch)
                                {
                                if   (pmiSearch->hMod == pmi->usModRef [ulI])
                                     {
                                     strcpy (pusrrec4->szReference,
                                             pmiSearch->szModName) ;
                                     pusrrec4->pszReference = pusrrec4->szReference ;
                                     break ;
                                     }
                                pmiSearch = pmiSearch->pNext ;
                                }
                             recins.cb = sizeof (recins) ;
                             recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                             recins.pRecordParent = NULL ;
                             recins.zOrder = CMA_TOP ;
                             recins.cRecordsInsert = 1 ;
                             recins.fInvalidateRecord = FALSE ;
                             WinSendMsg (pdlg->hwndContainer,
                                         CM_INSERTRECORD,
                                         MPFROMP (pusrrec4),
                                         MPFROMP (&recins)) ;
                             }
                        }
                   free (pbh) ;
                   break ;
              case 5:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (psi = (PSEMINFO) ((PBYTE) pbh->psi + 16); psi && *psi->szSemName; psi = psi->pNext)
                        {
                        pusrrec5 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC5) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        sprintf (pusrrec5->szFlags,
                                 "%04X",
                                 (ULONG) psi->uchFlags) ;
                        pusrrec5->pszFlags = pusrrec5->szFlags ;
                        pusrrec5->ulRefCount = psi->uchReferenceCount ;
                        pusrrec5->ulReqCount = psi->usRequestCount ;
                        sprintf (pusrrec5->szIndex,
                                 "%04X",
                                 (ULONG) psi->usIndex) ;
                        pusrrec5->pszIndex = pusrrec5->szIndex ;
                        strcpy (pusrrec5->szName,
                                "\\S") ;
                        strcat (pusrrec5->szName,
                                psi->szSemName) ;
                        pusrrec5->pszName = pusrrec5->szName ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec5),
                                    MPFROMP (&recins)) ;
                        }
                   free (pbh) ;
                   break ;
              case 6:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (psmi = pbh->psmi; psmi && psmi->selMem != (SEL) 0; psmi = psmi->pNext)
                        {
                        pusrrec6 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC6) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        sprintf (pusrrec6->szHandle,
                                 "%04X",
                                 (ULONG) psmi->usMemHandle) ;
                        pusrrec6->pszHandle = pusrrec6->szHandle ;
                        sprintf (pusrrec6->szSelektor,
                                 "%04X",
                                 (ULONG) psmi->selMem) ;
                        pusrrec6->pszSelektor = pusrrec6->szSelektor ;
                        pusrrec6->ulRefCount = psmi->usReferenceCount ;
                        strcpy (pusrrec6->szName,
                                psmi->szMemName) ;
                        pusrrec6->pszName = pusrrec6->szName ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec6),
                                    MPFROMP (&recins)) ;
                        }
                   free (pbh) ;
                   break ;
              case 7:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (ppi = pbh->ppi; ppi->ulEndIndicator != PROCESS_END_INDICATOR; ppi = (PPROCESSINFO) (ppi->ptiFirst + ppi->usThreadCount))
                        {
                        pusrrec7 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC7) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        pusrrec7->ulID = ppi->pid ;
                        pusrrec7->ulParentID = ppi->pidParent ;
                        pusrrec7->ulType = ppi->ulType ;
                        pusrrec7->ulStatus = ppi->ulStatus ;
                        pusrrec7->ulSessionID = ppi->idSession ;
                        sprintf (pusrrec7->szHandle,
                                 "%04X",
                                 (ULONG) ppi->hModRef) ;
                        pusrrec7->pszHandle = pusrrec7->szHandle ;
                        pusrrec7->ulThreadCount = ppi->usThreadCount ;
                        pusrrec7->ulSem16s = ppi->usSem16Count ;
                        pusrrec7->ulDLLs = ppi->usDllCount ;
                        pusrrec7->ulShrMems = ppi->usShrMemHandles ;
                        pmiSearch = pbh->pmi ;
                        while (pmiSearch)
                           {
                           if   (pmiSearch->hMod == ppi->hModRef)
                                {
                                strcpy (pusrrec7->szName,
                                        pmiSearch->szModName) ;
                                pusrrec7->pszName = pusrrec7->szName ;
                                break ;
                                }
                           pmiSearch = pmiSearch->pNext ;
                           }
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec7),
                                    MPFROMP (&recins)) ;
                        }
                   free (pbh) ;
                   break ;
              case 8:
                   pbh = calloc (1,
                                 65535) ;
                   DosQProcStatus (pbh,
                                   65535) ;
                   for  (ppi = pbh->ppi; ppi->ulEndIndicator != PROCESS_END_INDICATOR; ppi = (PPROCESSINFO) (ppi->ptiFirst + ppi->usThreadCount))
                        {
                        f = FALSE ;
                        pti = ppi->ptiFirst ;
                        for  (ulI = 0; ulI < ppi->usThreadCount; ulI++, pti++)
                             {
                             pusrrec8 = WinSendMsg (pdlg->hwndContainer,
                                                    CM_ALLOCRECORD,
                                                    MPFROMLONG (sizeof (USRREC8) - sizeof (MINIRECORDCORE)),
                                                    MPFROMSHORT (1)) ;
                             if   (!f)
                                  {
                                  sprintf (pusrrec8->szPID,
                                           "%u",
                                           ppi->pid) ;
                                  f = TRUE ;
                                  }
                             else
                                  {
                                  *pusrrec8->szPID = '\0' ;
                                  }
                             pusrrec8->pszPID = pusrrec8->szPID ;
                             pusrrec8->ulType = pti->ulRecType ;
                             pusrrec8->ulTID = pti->tidWithinProcess ;
                             pusrrec8->ulSlot = pti->usSlot ;
                             pusrrec8->ulBlockID = pti->ulBlockId ;
                             pusrrec8->ulPriority = pti->ulPriority ;
                             pusrrec8->ulSysTime = pti->ulSysTime ;
                             pusrrec8->ulUserTime = pti->ulUserTime ;
                             switch (pti->uchState)
                                {
                                case 1:
                                     strcpy (pusrrec8->szState,
                                             "ready") ;
                                     break ;
                                case 2:
                                     strcpy (pusrrec8->szState,
                                             "blocked") ;
                                     break ;
                                case 3:
                                     strcpy (pusrrec8->szState,
                                             "running") ;
                                     break ;
                                }
                             pusrrec8->pszState = pusrrec8->szState ;
                             recins.cb = sizeof (recins) ;
                             recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                             recins.pRecordParent = NULL ;
                             recins.zOrder = CMA_TOP ;
                             recins.cRecordsInsert = 1 ;
                             recins.fInvalidateRecord = FALSE ;
                             WinSendMsg (pdlg->hwndContainer,
                                         CM_INSERTRECORD,
                                         MPFROMP (pusrrec8),
                                         MPFROMP (&recins)) ;
                             }
                        }
                   free (pbh) ;
                   break ;
              case 9:
                   for  (ulI = DEVINFO_PRINTER; ulI <= DEVINFO_ADAPTER; ulI++)
                        {
                        DosDevConfig (szBuffer,
                                      ulI) ;
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics9 [ulI - DEVINFO_PRINTER]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = szBuffer [0] ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   break ;
              case 10:
                   for  (ulI = SV_SWAPBUTTON; ulI <= SV_ALARM; ulI++)
                        {
                        l = WinQuerySysValue (HWND_DESKTOP,
                                              ulI) ;
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics10A [ulI - SV_SWAPBUTTON]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = l ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   for  (ulI = SV_CURSORRATE; ulI <= SV_ERRORDURATION; ulI++)
                        {
                        l = WinQuerySysValue (HWND_DESKTOP,
                                              ulI) ;
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics10B [ulI - SV_CURSORRATE]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = l ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   for  (ulI = SV_CXSCREEN; ulI <= SV_CYALIGN; ulI++)
                        {
                        l = WinQuerySysValue (HWND_DESKTOP,
                                              ulI) ;
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics10C [ulI - SV_CXSCREEN]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = l ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   for  (ulI = SV_MENUROLLDOWNDELAY; ulI <= SV_CSYSVALUES; ulI++)
                        {
                        l = WinQuerySysValue (HWND_DESKTOP,
                                              ulI) ;
                        pusrrec1 = WinSendMsg (pdlg->hwndContainer,
                                               CM_ALLOCRECORD,
                                               MPFROMLONG (sizeof (USRREC1) - sizeof (MINIRECORDCORE)),
                                               MPFROMSHORT (1)) ;
                        strcpy (pusrrec1->szTopic,
                                pszTopics10D [ulI - SV_MENUROLLDOWNDELAY]) ;
                        pusrrec1->pszTopic = pusrrec1->szTopic ;
                        pusrrec1->ulValue = l ;
                        recins.cb = sizeof (recins) ;
                        recins.pRecordOrder = (PRECORDCORE) CMA_END ;
                        recins.pRecordParent = NULL ;
                        recins.zOrder = CMA_TOP ;
                        recins.cRecordsInsert = 1 ;
                        recins.fInvalidateRecord = FALSE ;
                        WinSendMsg (pdlg->hwndContainer,
                                    CM_INSERTRECORD,
                                    MPFROMP (pusrrec1),
                                    MPFROMP (&recins)) ;
                        }
                   break ;
              }
           WinSendMsg (pdlg->hwndContainer,
                       CM_INVALIDATERECORD,
                       MPVOID,
                       MPFROM2SHORT (0, CMA_REPOSITION)) ;
           return (MRESULT) FALSE ;
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }
