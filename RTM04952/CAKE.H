/* $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKE.H 1.1 1995/05/25 18:46:05 HaWi Exp $ */

#ifndef _CAKE_H
   #define _CAKE_H

   /* Klassenname des neuen Kontrollelementes */
   #define CK_CLASSNAME "CAKE"

   /* ôffentliche private Messages */
   #define WMPC            (WM_USER + 1020)
   #define WMPC_INSERTCAKE (WMPC + 1)
   #define WMPC_QUERYCAKE  (WMPC + 2)
   #define WMPC_QUERYINFO  (WMPC + 3)
   #define WMPC_REMOVECAKE (WMPC + 4)
   #define WMPC_SETINFO    (WMPC + 5)
   #define WMPC_UPDATECAKE (WMPC + 6)

   /* Returncodes der Listenverwaltung */
   #define CKC_ERROR   FALSE
   #define CKC_NOERROR TRUE

   /* Konstanten fÅr die Listenverwaltung und Positionierungsflags fÅr die Messages */
   #define CK_FIRST      1
   #define CK_INDEX      2
   #define CK_INSERT     4
   #define CK_INVALIDATE 8
   #define CK_KEY        16
   #define CK_LAST       32
   #define CK_STRING     64
   #define CK_ULONG      128

   /* WM_CONTROL add-ons */
   #define CKN_ENTER  1
   #define CKN_SELECT 2

   /* Info Struktur des Controls */
   typedef struct
      {
      ULONG ulCount ;
      } CINFO, *PCINFO ;

   /* Info Struktur eines Kuchenteils */
   typedef struct
      {
      CHAR  szTitle [32] ;
      ULONG ulValue ;
      } CVALUE, *PCVALUE ;

   /* ôffentliche Funktionsdekalartionen */
   BOOL EXPENTRY CRegisterClass (HAB) ;

#endif /* _CAKE_H */
