/* $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKE.PH 1.1 1995/05/25 18:46:13 HaWi Exp $ */

#ifndef _CAKE_PH
   #define _CAKE_PH

   /* Private PAINT Message */
   #define WMPC_PAINT (WMPC + 0)

   /* Konstanten fÅr die Listenverwaltung */
   #define CK_HEADERPREV 0
   #define CK_HEADERNEXT 4
   #define CK_HEADERDATA 8
   #define CK_PTR2DATAINCURR pb + CK_HEADERDATA
   #define CK_PTR2PREVINCURR *((PBYTE *) (pb + CK_HEADERPREV))
   #define CK_PTR2NEXTINCURR *((PBYTE *) (pb + CK_HEADERNEXT))
   #define CK_PTR2NEXTINPREV *((PBYTE *) ((*((PBYTE *) (pb + CK_HEADERPREV))) + CK_HEADERNEXT))
   #define CK_PTR2PREVINNEXT *((PBYTE *) ((*((PBYTE *) (pb + CK_HEADERNEXT))) + CK_HEADERPREV))

   /* Root einer Liste */
   typedef struct
      {
      PVOID pvFirst ;
      PVOID pvLast ;
      ULONG ulCount ;
      ULONG ulDataSize ;
      ULONG ulKeyOffset ;
      ULONG ulKeyStyle ;
      } LISTROOT, *PLISTROOT ;

   /* Instanzdaten des neuen Kontrollelementes */
   typedef struct
      {
      CINFO     cinfo ;
      PFNWP     pfnwp ;
      PLISTROOT plistroot ;
      } WND, *PWND ;

   /* 32Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Private Funktionsdekalartionen des neuen Kontrollelementes */
   ULONG            CListCount (PVOID) ;
   BOOL             CListDelete (PVOID, ULONG, PVOID) ;
   BOOL             CListDestroy (PVOID) ;
   PVOID            CListInit (ULONG, ULONG, ULONG) ;
   BOOL             CListInsert (PVOID, ULONG, PVOID) ;
   BOOL             CListQuery (PVOID, ULONG, PVOID, PVOID) ;
   BOOL             CListUpdate (PVOID, ULONG, PVOID, PVOID) ;
   MRESULT EXPENTRY CWndProc (HWND, MSG, MPARAM, MPARAM) ;

#endif /* _CAKE_PH */
