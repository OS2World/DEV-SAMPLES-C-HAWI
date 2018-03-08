/* $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREIDTST.H 1.1 1995/10/04 13:08:00 HaWi Exp $ */

/* Control IDs fÅr den Dialog */
#define IDD   11
#define IDT_1 12
#define IDT_2 13

/* Instanzdaten des Dialoges */
typedef struct
   {
   ULONG ulDummy ;
   } DLG, *PDLG ;

/* 32Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsdekalartionen */
INT              main (VOID) ;
MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;


