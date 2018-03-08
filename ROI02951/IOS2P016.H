/* $Header: D:\projekte\dde - die zweite\source\RCS\IOS2P016.H 1.1 1995/05/25 15:27:23 HaWi Exp $ */

/* Classnames */
#define WCP_DDESPY  "WCP.DDESPY"

/* Additional Messages */
#define WMP_CLEAR   (WM_USER + 0)

/* Resource IDs */
#define IDD_DDESPY  100
#define IDE_DDESPY1 101
#define IDF_DDESPY1 102
#define IDL_DDESPY1 103
#define IDL_DDESPY2 104
#define IDL_DDESPY3 105
#define IDP_DDESPY1 106
#define IDP_DDESPY2 107
#define IDP_DDESPY3 108
#define IDP_DDESPY4 109
#define IDP_DDESPY5 110
#define IDP_DDESPY6 111
#define IDP_DDESPY7 112

/* For 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* used structs */
#define DDESPY_REQTYPAPP 1
#define DDESPY_REQTYPTOP 2
#define DDESPY_REQTYPITM 3
#define DDESPY_REQTYPREQ 4
#define DDESPY_REQTYPPKE 5
#define DDESPY_REQTYPADV 6

typedef struct
   {
   CHAR  szApplication [CCHMAXPATH] ;
   CHAR  szData [CCHMAXPATH] ;
   CHAR  szItem [CCHMAXPATH] ;
   CHAR  szTopic [CCHMAXPATH] ;
   HWND  hwndListbox1 ;
   HWND  hwndListbox2 ;
   HWND  hwndListbox3 ;
   HWND  hwndServer ;
   ULONG ulRequesttype ;
   } DLG, *PDLG ;


/* function declarations */
INT              main (VOID) ;
MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
PDDESTRUCT       MakeDDESeg (HWND, PSZ, ULONG, ULONG, PVOID, ULONG) ;
