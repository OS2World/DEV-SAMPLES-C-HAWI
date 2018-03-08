/* $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKETEST.H 1.1 1995/05/25 18:46:28 HaWi Exp $ */

/* Control IDs fÅr den Dialog */
#define IDD   11
#define IDE_1 12
#define IDE_2 13
#define IDE_3 14
#define IDP_1 15
#define IDT_1 16
#define IDT_2 17

/* Instanzdaten des Dialoges */
typedef struct
   {
   HWND hwndCake ;
   } DLG, *PDLG ;

/* Helferstruktur zum öbergeben von Initialdaten */
typedef struct
   {
   PSZ   pszTitle ;
   ULONG ulFlags ;
   ULONG ulValue ;
   } VALUES ;

/* 32Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

/* Funktionsdekalartionen */
INT              main (VOID) ;
MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
