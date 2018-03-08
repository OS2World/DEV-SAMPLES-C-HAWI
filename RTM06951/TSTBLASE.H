/* $Header: D:/projekte/blasen\040im\040os!2/source/RCS/TSTBLASE.H 1.1 1995/07/02 13:27:13 HaWi Exp $ */

/* Ressource IDs */
#define IDD   11
#define IDB_1 12
#define IDB_2 13
#define IDE_1 14
#define IDE_2 15
#define IDE_3 16
#define IDP_1 17
#define IDP_2 18
#define IDP_3 19
#define IDR_1 10
#define IDR_2 20
#define IDR_3 21
#define IDT_1 22

/* FÅr den 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsprototypen */
INT              main (VOID) ;
MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;

