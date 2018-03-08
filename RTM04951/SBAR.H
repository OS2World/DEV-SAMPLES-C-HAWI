/* $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBAR.H 1.1 1995/05/25 15:37:35 HaWi Exp $ */

#ifndef _SBAR_H
   #define _SBAR_H

   #define WMPSBAR          (WM_USER + 1040)
   #define WMPSBAR_SETDATA  (WMPSBAR + 0)
   #define WMPSBAR_SETSIZE  (WMPSBAR + 1)
   #define WMPSBAR_SETSTYLE (WMPSBAR + 2)
   #define WMPSBAR_SETLOOK  (WMPSBAR + 3)
   #define WMPSBAR_SETTEXT  (WMPSBAR + 4)
   #define WMPSBAR_SETVALUE (WMPSBAR + 5)

   #define SK_LOOKIGNORE 0
   #define SK_LOOKIN     1
   #define SK_LOOKOUT    2

   #define FID_SBAR 2

   typedef struct
      {
      CHAR  szText [128] ;
      HWND  hwnd ;
      LONG  lSize ;
      ULONG ulLook ;
      ULONG ulStyle ;
      } SBARITEM, *PSBARITEM ;

   BOOL EXPENTRY Deregister (HWND) ;
   BOOL EXPENTRY Register (HWND) ;

#endif /* _SBAR_H */
