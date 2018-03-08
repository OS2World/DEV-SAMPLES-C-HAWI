/* $Header: D:\projekte\versteckte informationen\source\RCS\IOS2P009.H 1.1 1995/05/25 19:01:27 HaWi Exp $ */

#define WCP_IOS2P          "WC.IOS2P"

#define WMP_MAININIT       (WM_USER + 1)
#define WMP_CLEAR          (WM_USER + 2)

#define PAGES              10

#define IDCO_IOS2P_1       10
#define IDD_IOS2P          11
#define IDD_IOS2P_1        12
#define IDNO_IOS2P_1       13

/* FÅr 32-Bit PM */
#ifndef _MSG_
   #define _MSG_
   typedef unsigned long MSG ;
#endif

#ifndef CCS_MINIICONS
   #define CCS_MINIICONS   0x800
#endif
   
#pragma linkage (DosQProcStatus, far16 pascal)

#define PROCESS_END_INDICATOR 3

#pragma pack (1)
typedef struct
   {
   ULONG ulThreadCount ;
   ULONG ulProcessCount ;
   ULONG ulModuleCount ;
   } SUMMARY, *PSUMMARY ;

typedef struct _MODINFO
   {
   struct _MODINFO *pNext ;
   USHORT          hMod ;
   USHORT          usModType ;
   ULONG           ulModRefCount ;
   ULONG           ulSegmentCount ;
   ULONG           ulDontKnow1 ;
   PSZ             szModName ;
   USHORT          usModRef [1] ;
   } MODINFO, *PMODINFO ;

typedef struct
   {
   ULONG  ulRecType ;
   USHORT tidWithinProcess ;
   USHORT usSlot ;
   ULONG  ulBlockId ;
   ULONG  ulPriority ;
   ULONG  ulSysTime ;
   ULONG  ulUserTime ;
   UCHAR  uchState ;
   UCHAR  uchPad ;
   USHORT usPad ;
   } THREADINFO, *PTHREADINFO ;

typedef struct
   {
   ULONG       ulEndIndicator ;
   PTHREADINFO ptiFirst ;
   USHORT      pid ;
   USHORT      pidParent ;
   ULONG       ulType ;
   ULONG       ulStatus ;
   ULONG       idSession ;
   USHORT      hModRef ;
   USHORT      usThreadCount ;
   ULONG       ulReserved ;
   PVOID       pvReserved ;
   USHORT      usSem16Count ;
   USHORT      usDllCount ;
   USHORT      usShrMemHandles ;
   USHORT      usReserved ;
   PUSHORT     pusSem16TableAddr ;
   PUSHORT     pusDllTableAddr ;
   PUSHORT     pusShrMemTableAddr ;
   } PROCESSINFO, *PPROCESSINFO ;

typedef struct _SEMINFO
   {
   struct _SEMINFO *pNext ;
   USHORT          usRequestCount ;
   UCHAR           uchFlags ;
   UCHAR           uchReferenceCount ;
   ULONG           ulReserved ;
   USHORT          usIndex ;
   CHAR            szSemName [1] ;
   } SEMINFO, *PSEMINFO ;

typedef struct _SHRMEMINFO
   {
   struct _SHRMEMINFO *pNext ;
   USHORT             usMemHandle ;
   SEL                selMem ;
   USHORT             usReferenceCount ;
   CHAR               szMemName [1] ;
   } SHRMEMINFO, *PSHRMEMINFO ;

typedef struct
   {
   PSUMMARY     psumm ;
   PPROCESSINFO ppi ;
   PSEMINFO     psi ;
   PVOID        pDontKnow1 ;
   PSHRMEMINFO  psmi ;
   PMODINFO     pmi ;
   PVOID        pDontKnow2 ;
   PVOID        pDontKnow3 ;
   } BUFFHEADER, *PBUFFHEADER ;
#pragma pack ()

typedef struct
   {
   PSZ   pszTitle ;
   ULONG ulFlData ;
   ULONG ulFlTitle ;
   ULONG ulOffStruct ;
   } CONTNRINIT, *PCONTNRINIT ;

typedef struct
   {
   PSZ   pszTitle ;
   PSZ   pszSide ;
   ULONG ulFlag ;
   ULONG ulMajor ;
   ULONG ulOrder ;
   } PGEINFO, *PPGEINFO ;

/* Funktionsdeklarationen */
USHORT           DosQProcStatus (PVOID  pvBuf,
                                 USHORT usBuf) ;
INT              main (VOID) ;
MRESULT EXPENTRY IOS2P_DlgWndProc (const HWND,
                                   const MSG,
                                   const MPARAM,
                                   const MPARAM) ;
MRESULT EXPENTRY IOS2P_DlgWndProc_1 (const HWND,
                                    const MSG,
                                    const MPARAM,
                                    const MPARAM) ;
                                      
extern HAB  hab_g ;
