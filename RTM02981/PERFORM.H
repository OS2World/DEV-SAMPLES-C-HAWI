/* $Header: D:\Projekte\Redaktion Toolbox\Geschwindigkeitsrausch\Source\RCS\PERFORM.H 1.1 1997/11/30 14:58:49 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _PERFORM_H
   #define _PERFORM_H

   /*
      The following 17 lines are
      (c) Copyright International Business Machines Corporation 1996. All rights reserved.
   */
   #ifndef PERFCALL_INCLUDED
      #define PERFCALL_INCLUDED
      #define CMD_KI_RDCNT 0x63
      typedef struct
         {
         ULONG ulTimeLow ;  /* Low 32 bits of time stamp */
         ULONG ulTimeHigh ; /* High 32 bits of time stamp */
         ULONG ulIdleLow ;  /* Low 32 bits of idle time */
         ULONG ulIdleHigh ; /* High 32 bits of idle time */
         ULONG ulBusyLow ;  /* Low 32 bits of busy time */
         ULONG ulBusyHigh ; /* High 32 bits of busy time */
         ULONG ulIntrLow ;  /* Low 32 bits of interrupt time */
         ULONG ulIntrHigh ; /* High 32 bits of interrupt time */
         } CPUUTIL, *PCPUUTIL ;
      #define LL2F(high, low) (4294967296.0*(high)+(low))
      APIRET APIENTRY DosPerfSysCall (ULONG, ULONG, ULONG, ULONG) ;
   #endif /* PERFCALL_INCLUDED */

   /* Fuer 32 Bit PM */
   #ifndef MSG
      typedef unsigned long MSG ;
   #endif /* MSG */

   #ifndef DOUBLE
      typedef double DOUBLE ;
   #endif /* DOUBLE */

   #ifndef DB_RAISED
      #define DB_RAISED 0x0400
   #endif /* DB_RAISED */

   #ifndef DB_DEPRESSED
      #define DB_DEPRESSED 0x0800
   #endif /* DB_DEPRESSED */

   /* Private Events fÅr den Datenaustausch zwischen Anwendung
      und der neuen Fensterklasse */
   #define WMPPX        (WM_USER + 1000)
   #define WMPPX_SETCPU (WMPPX + 0)

   /* Fensterklassenname der neuen Fensterklasse */
   #define PXK_CLASSNAME "PERFORM.P"
   /* ID fÅr den Timer */
   #define PXK_TIMER     9999

   /* Struktur fÅr die nach DOUBLE gewandelten HIGH/LOW Performancedaten */
   typedef struct
      {
      DOUBLE dBusy ;
      DOUBLE dIdle ;
      DOUBLE dIntr ;
      DOUBLE dTime ;
      } MYCPUUTIL, *PMYCPUUTIL ;

   /* Instanzdaten des Dialoges */
   typedef struct
      {
      HWND hwndSpinbutton ;
      } DLG, *PDLG ;

   /* Instanzdaten der Fensterklasse */
   typedef struct
      {
      BOOL      fTimer ;        /* Flag ob Timer aktiv */
      HAB       hab ;           /* Gemerkter Handle Anchorblock */
      MYCPUUTIL mycpuutilPrev ; /* Jeweiligen Vergangenheitswerte des Performance APIs */
      PCPUUTIL  pcpuutil ;      /* Speicher fÅr CPU Daten */
      PFNWP     pfnwp ;         /* Gemerkte Adresse der Original Ausgabefeld Fensterklasse */
      PPOINTL   paptl ;         /* Speicher fÅr das Pixel Array */
      ULONG     ulColumns ;     /* Fortschrittsanzeiger fÅr die x Koordinate */
      ULONG     ulCPU ;         /* Zu "monitorende" CPU (1 basiert) */
      ULONG     ulCPUs ;        /* Gesamtanzahl CPUs */
      ULONG     ulMaxColumns ;  /* Maximal beschreibbare x Pixel */
      ULONG     ulMaxRows ;     /* Maximal beschreibbare y Pixel */
      } WND, *PWND ;

   /* Prozedurdeklarationen */
   INT              main (VOID) ;
   MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ;
   BOOL             PRegisterClass (HAB) ;
   MRESULT EXPENTRY PWndProc (HWND, MSG, MPARAM, MPARAM) ;

   /* Konstantdeklarationen */
   #define IDD_PERFORM 1000
   #define IDSP_P1     1001
   #define IDT_P1      1002
   #define IDT_P2      1003

#endif /* _PERFORM_H */

#ifdef __cplusplus
   }
#endif

