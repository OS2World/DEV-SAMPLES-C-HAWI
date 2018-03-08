/* $Header: D:\projekte\dde - das dicke ende\source\RCS\IOS2P008.H 1.1 1995/05/25 15:25:09 HaWi Exp $ */

/* Classnames */
#define WCP_IOS2P "WC.IOS2P"
#define WCP_DDE   "WC.DDE"

/* Additional Messages */
#define WMP_CREATE            (WM_USER + 20)
#define WMP_DDECOUNT          (WM_USER + 21)
#define WMP_DDECREATE         (WM_USER + 22)
#define WMP_DDEDESTROY        (WM_USER + 23)
#define WMP_DDEUPDATE         (WM_USER + 24)
#define WMP_DELETELISTBOXITEM (WM_USER + 25)
#define WMP_INSERTLISTBOXITEM (WM_USER + 26)
#define WMP_QUERYLISTBOXITEM  (WM_USER + 27)
#define WMP_SIZE              (WM_USER + 28)
#define WMP_STARTUP           (WM_USER + 29)

/* Resource IDs */
#define IDW_IOS2P   100
#define IDL_IOS2P_1 101
#define IDL_IOS2P_2 102

/* DDE Constants */
#define DDEP_MAXDDELINKS 26
#define DDEP_APPLICATION "IOS2P"
#define DDEP_TOPIC1      "QueryListbox1Item"
#define DDEP_TOPIC2      "QueryListbox2Item"
#define DDEP_TOPIC3      "InsertListbox1Item"
#define DDEP_TOPIC4      "InsertListbox2Item"
#define DDEP_TOPIC5      "DeleteListbox1Item"
#define DDEP_TOPIC6      "DeleteListbox2Item"

/* For 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* used structs */
typedef struct
   {
   HAB  hab ;
   HWND hwndClient ;
   HWND hwndFrame ;
   } STARTUP, *PSTARTUP ;

typedef struct
   {
   BOOL fMinimized ;
   HAB  hab ;
   HWND ahwndDDE [DDEP_MAXDDELINKS] ;
   HWND hwndClient ;
   HWND hwndFrame ;
   HWND hwndListbox1 ;
   HWND hwndListbox2 ;
   } WND, *PWND ;

typedef struct
   {
   CHAR szTopic [256] ;
   HWND hwndDDEClient ;
   HWND hwndParent ;
   } DDECONV, *PDDECONV ;

typedef struct
   {
   BOOL  fImmediate ;
   CHAR  szItemName [512] ;
   CHAR  szTopic [256] ;
   HWND  hwndDDEClient ;
   HWND  hwndParent ;
   ULONG ulRequestType ;
   } WNDDDE, *PWNDDDE ;

/* function declarations */
MRESULT EXPENTRY ClientWndProc (HWND, MSG, MPARAM, MPARAM) ;
MRESULT EXPENTRY DDEWndProc (HWND, MSG, MPARAM, MPARAM) ;
INT              main (VOID) ;
PDDESTRUCT       MakeDDESeg (HWND, PSZ, ULONG, ULONG, PVOID, ULONG) ;
