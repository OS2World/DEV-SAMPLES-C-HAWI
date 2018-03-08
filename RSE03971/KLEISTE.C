/* $Header: Y:/Projekte/Redaktion SE/Knopfleiste/Source/rcs/KLEISTE.C 1.1 1996/12/29 13:49:40 HaWi Exp $ */

#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "KLEISTE.H"

HBITMAP   ahbm_g [IDKLEISTE_HIGH - IDKLEISTE_LOW + 1] ;
HDC       hdc_g = NULLHANDLE ;
HMODULE   hmodSelf_g = NULLHANDLE ;
HPS       hps_g = NULLHANDLE ;
PSUBCLASS psubclass_g = NULL ;
ULONG     ulCount_g = 0 ;

BOOL EXPENTRY KLeisteDeregister (HWND hwnd)
   {
   PSUBCLASS psubclass ;
   PSUBCLASS psubclassPrev ;
   ULONG     ulI ;
   ULONG     ulJ ;

   if   (ulCount_g)
        {
        for  (ulI = 0, psubclassPrev = NULL, psubclass = psubclass_g; ulI < ulCount_g; ulI++, psubclassPrev = psubclass, psubclass = psubclass->psubclassNext)
             {
             if   (psubclass->hwndSubclass == hwnd)
                  {
                  WinSubclassWindow (psubclass->hwndSubclass,
                                     psubclass->pfnwp) ;
                  WinDestroyWindow (psubclass->hwndToolbar) ;
                  if   (psubclassPrev)
                       {
                       psubclassPrev->psubclassNext = psubclass->psubclassNext ;
                       }
                  else
                       {
                       psubclass_g = psubclass->psubclassNext ;
                       }
                  free (psubclass) ;
                  ulCount_g-- ;
                  if   (!ulCount_g)
                       {
                       for  (ulJ = 0; ulJ < sizeof (ahbm_g) / sizeof (ahbm_g [0]); ulJ++)
                            {
                            GpiDeleteBitmap (ahbm_g [ulJ]) ;
                            }
                       DosFreeModule (hmodSelf_g) ;
                       GpiDestroyPS (hps_g) ;
                       DevCloseDC (hdc_g) ;
                       }
                  break ;
                  }
             }
        }
   return TRUE ;
   }

BOOL EXPENTRY KLeisteRegister (HWND hwnd)
   {
   HAB       hab ;
   PSUBCLASS psubclass ;
   PSUBCLASS psubclassPrev ;
   SIZEL     sizl ;
   ULONG     ulI ;

   hab = WinQueryAnchorBlock (hwnd) ;
   if   (!ulCount_g)
        {
        hdc_g = DevOpenDC (hab,
                           OD_MEMORY,
                           "*",
                           0,
                           NULL,
                           NULLHANDLE) ;
        sizl.cx = 0 ;
        sizl.cy = 0 ;
        hps_g = GpiCreatePS (hab,
                             hdc_g,
                             &sizl,
                             PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC) ;
        DosQueryModuleHandle ("KLEISTE",
                              &hmodSelf_g) ;
        for  (ulI = 0; ulI < sizeof (ahbm_g) / sizeof (ahbm_g [0]); ulI++)
             {
             ahbm_g [ulI] = GpiLoadBitmap (hps_g,
                                           hmodSelf_g,
                                           IDKLEISTE_LOW + ulI,
                                           0,
                                           0) ;
             }
        psubclass_g = calloc (1,
                              sizeof (SUBCLASS)) ;
        psubclass = psubclass_g ;
        }
   else
        {
        for  (ulI = 0, psubclass = psubclass_g; ulI < ulCount_g; ulI++)
             {
             psubclassPrev = psubclass ;
             psubclass = psubclass->psubclassNext ;
             }
        psubclassPrev->psubclassNext = calloc (1,
                                               sizeof (SUBCLASS)) ;
        psubclass = psubclassPrev->psubclassNext ;
        }
   ulCount_g++ ;
   psubclass->ulHeight = 24 ;
   WinRegisterClass (WinQueryAnchorBlock (hwnd),
                     KLEISTE_CLASSNAME,
                     ToolbarWndProc,
                     0,
                     0) ;
   psubclass->hwndToolbar = WinCreateWindow (hwnd,
                                             KLEISTE_CLASSNAME,
                                             "",
                                             0,
                                             0,
                                             0,
                                             0,
                                             0,
                                             hwnd,
                                             HWND_TOP,
                                             FID_KLEISTE,
                                             NULL,
                                             NULL) ;
   psubclass->hab = hab ;
   psubclass->hwndSubclass = hwnd ;
   psubclass->pfnwp = WinSubclassWindow (psubclass->hwndSubclass,
                                         SubclassWndProc) ;
   return TRUE ;
   }

INT KLeisteSort (const void* pv1,
                 const void* pv2)
   {
   INT   i ;
   ULONG ul1 ;
   ULONG ul2 ;

   i = 0 ;
   ul1 = ((PKLEISTEITEM) pv1)->ulOrder ;
   ul2 = ((PKLEISTEITEM) pv2)->ulOrder ;
   if   (ul1 < ul2)
        {
        i = -1 ;
        }
   else
        {
        if   (ul1 > ul2)
             {
             i = 1 ;
             }
        }
   return i ;
   }

MRESULT EXPENTRY SubclassWndProc (HWND   hwnd,
                                  MSG    msg,
                                  MPARAM mp1,
                                  MPARAM mp2)
   {
   PSUBCLASS psubclass ;

   {
   ULONG ulI ;

   for  (ulI = 0, psubclass = psubclass_g; ulI < ulCount_g; ulI++, psubclass = psubclass->psubclassNext)
        {
        if   (psubclass->hwndSubclass == hwnd)
             {
             break ;
             }
        }
   }
   switch (msg)
      {
      case WM_ACTIVATE:
           WinInvalidateRect (psubclass->hwndToolbar,
                              NULL,
                              FALSE) ;
           break ;
      case WM_CALCFRAMERECT:
           {
           BOOL   f ;
           PRECTL prcl ;

           if   ((f = LONGFROMMR (psubclass->pfnwp (hwnd,
                                                    msg,
                                                    mp1,
                                                    mp2))) == TRUE)
                {
                prcl = PVOIDFROMMP (mp1) ;
                if   (SHORT1FROMMP (mp2))
                     {
                     prcl->yTop -= psubclass->ulHeight + 2 ;
                     }
                }
           return MRFROMLONG (f) ;
           }
      case WM_FORMATFRAME:
           {
           PRECTL prcl ;
           PSWP   pswp ;
           ULONG  ul ;
           ULONG  ulI ;

           ul = LONGFROMMR (psubclass->pfnwp (hwnd,
                                              msg,
                                              mp1,
                                              mp2)) ;
           pswp = PVOIDFROMMP (mp1) ;
           for  (ulI = 0; ulI < ul; ulI++)
                {
                if   (WinQueryWindowUShort (pswp[ulI].hwnd,
                                            QWS_ID) == FID_VERTSCROLL)
                     {
                     pswp[ulI].fl |= SWP_SIZE ;
                     pswp[ulI].cy -= psubclass->ulHeight + 2 ;
                     break ;
                     }
                }
           for  (ulI = 0; ulI < ul; ulI++)
                {
                if   (WinQueryWindowUShort (pswp[ulI].hwnd,
                                            QWS_ID) == FID_CLIENT)
                     {
                     pswp[ulI].fl |= SWP_SIZE ;
                     pswp[ulI].cy -= psubclass->ulHeight + 2 ;
                     break ;
                     }
                }
           pswp[ul].fl = pswp[ulI].fl ;
           pswp[ul].cy = psubclass->ulHeight + 2 ;
           pswp[ul].cx = pswp[ulI].cx ;
           pswp[ul].y = pswp[ulI].y + pswp[ulI].cy ;
           pswp[ul].x = pswp[ulI].x ;
           pswp[ul].hwndInsertBehind = 0 ;
           pswp[ul].hwnd = psubclass->hwndToolbar ;
           if   ((prcl = PVOIDFROMMP (mp2)) != NULL)
                {
                prcl->yTop -= psubclass->ulHeight + 2 ;
                }
           return MRFROMLONG (ul + 1) ;
           }
      case WM_QUERYFRAMECTLCOUNT:
           return MRFROMSHORT (SHORT1FROMMR (psubclass->pfnwp (hwnd,
                                                               msg,
                                                               mp1,
                                                               mp2)) + 1) ;
      case WMPKLEISTE_ACTIVATEBUTTON:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->aitem[ulI].ulCommand == LONGFROMMP (mp1))
                     {
                     psubclass->aitem[ulI].fActive = LONGFROMMP (mp2) ;
                     WinInvalidateRect (psubclass->hwndToolbar,
                                        NULL,
                                        FALSE) ;
                     break ;
                     }
                }
           return (MRESULT) FALSE ;
           }
      case WMPKLEISTE_ENABLEBUTTON:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->aitem[ulI].ulCommand == LONGFROMMP (mp1))
                     {
                     psubclass->aitem[ulI].fEnable = LONGFROMMP (mp2) ;
                     WinEnableWindow (psubclass->ahwnd [ulI],
                                      psubclass->aitem[ulI].fEnable) ;
                     break ;
                     }
                }
           return (MRESULT) FALSE ;
           }
      case WMPKLEISTE_INSERTDATA:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->ahwnd [ulI])
                     {
                     WinDestroyWindow (psubclass->ahwnd [ulI]) ;
                     psubclass->ahwnd [ulI] = NULLHANDLE ;
                     }
                }
           memset (psubclass->aitem,
                   0,
                   (sizeof (psubclass->aitem) / sizeof (psubclass->aitem [0])) * sizeof (KLEISTEITEM)) ;
           psubclass->ulCount = LONGFROMMP (mp1) ;
           memcpy (psubclass->aitem,
                   PVOIDFROMMP (mp2),
                   psubclass->ulCount * sizeof (KLEISTEITEM)) ;
           WinInvalidateRect (psubclass->hwndToolbar,
                              NULL,
                              FALSE) ;
           return (MRESULT) FALSE ;
           }
      case WMPKLEISTE_QUERYHANDLE:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->aitem[ulI].ulBitmap == LONGFROMMP (mp1))
                     {
                     return MRFROMLONG (ahbm_g [psubclass->aitem[ulI].ulBitmap - IDKLEISTE_LOW]) ;
                     }
                }
           return MRFROMLONG (0) ;
           }
      case WMPKLEISTE_REMOVEDATA:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->ahwnd [ulI])
                     {
                     WinDestroyWindow (psubclass->ahwnd [ulI]) ;
                     psubclass->ahwnd [ulI] = NULLHANDLE ;
                     }
                psubclass->aitem[ulI].ulBitmap = 0 ;
                psubclass->aitem[ulI].ulCommand = 0 ;
                }
           psubclass->ulCount = 0 ;
           WinInvalidateRect (psubclass->hwndToolbar,
                              NULL,
                              FALSE) ;
           return (MRESULT) FALSE ;
           }
      }
   return psubclass->pfnwp (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

MRESULT EXPENTRY ToolbarWndProc (HWND   hwnd,
                                 MSG    msg,
                                 MPARAM mp1,
                                 MPARAM mp2)
   {
   PSUBCLASS psubclass ;

   {
   ULONG ulI ;

   for  (ulI = 0, psubclass = psubclass_g; ulI < ulCount_g; ulI++, psubclass = psubclass->psubclassNext)
        {
        if   (psubclass->hwndToolbar == hwnd)
             {
             break ;
             }
        }
   }
   switch (msg)
      {
      case WM_COMMAND:
           return WinSendMsg (psubclass->hwndSubclass,
                              msg,
                              mp1,
                              mp2) ;
      case WM_DESTROY:
           {
           ULONG ulI ;

           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->ahwnd [ulI])
                     {
                     WinDestroyWindow (psubclass->ahwnd [ulI]) ;
                     psubclass->ahwnd [ulI] = NULLHANDLE ;
                     }
                }
           break ;
           }
      case WM_PAINT:
           {
           BTNCDATA btncd ;
           HPS      hps ;
           MRESULT  mr ;
           RECTL    rcl ;
           RECTL    rclSave ;
           ULONG    ulI ;

           mr = WinDefWindowProc (hwnd,
                                  msg,
                                  mp1,
                                  mp2) ;
           hps = WinGetPS (hwnd) ;
           WinQueryWindowRect (hwnd,
                               &rclSave) ;
           WinFillRect (hps,
                        &rclSave,
                        SYSCLR_MENU) ;
           WinDrawBorder (hps,
                          &rclSave,
                          1,
                          1,
                          0,
                          0,
                          DB_RAISED) ;
           rclSave.yBottom++ ;
           rclSave.yTop-- ;
           qsort (psubclass->aitem,
                  psubclass->ulCount,
                  sizeof (KLEISTEITEM),
                  KLeisteSort) ;
           for  (ulI = 0; ulI < psubclass->ulCount; ulI++)
                {
                if   (psubclass->aitem[ulI].fActive)
                     {
                     rcl = rclSave ;
                     rcl.xLeft += 4 ;
                     if   (psubclass->aitem[ulI].ulBitmap &&
                           psubclass->aitem[ulI].ulBitmap != IDKLEISTE_PLCEHLDR &&
                           psubclass->aitem[ulI].ulCommand)
                          {
                          if   (psubclass->ahwnd [ulI])
                               {
                               WinSetWindowPos (psubclass->ahwnd [ulI],
                                                HWND_TOP,
                                                rcl.xLeft,
                                                rcl.yBottom,
                                                psubclass->ulHeight,
                                                psubclass->ulHeight,
                                                SWP_MOVE) ;
                               WinInvalidateRect (psubclass->ahwnd [ulI],
                                                  NULL,
                                                  FALSE) ;
                               }
                          else
                               {
                               memset (&btncd,
                                       0,
                                       sizeof (BTNCDATA)) ;
                               btncd.cb = sizeof (BTNCDATA) ;
                               btncd.hImage = ahbm_g [psubclass->aitem[ulI].ulBitmap - IDKLEISTE_LOW] ;
                               psubclass->ahwnd [ulI] = WinCreateWindow (hwnd,
                                                                         WC_BUTTON,
                                                                         "",
                                                                         BS_BITMAP | BS_NOPOINTERFOCUS | BS_PUSHBUTTON | WS_VISIBLE,
                                                                         rcl.xLeft,
                                                                         rcl.yBottom,
                                                                         psubclass->ulHeight,
                                                                         psubclass->ulHeight,
                                                                         hwnd,
                                                                         HWND_TOP,
                                                                         psubclass->aitem[ulI].ulCommand,
                                                                         (PVOID) &btncd,
                                                                         NULL) ;
                               WinEnableWindow (psubclass->ahwnd [ulI],
                                                psubclass->aitem[ulI].fEnable) ;
                               }
                          rclSave.xLeft += psubclass->ulHeight ;
                          }
                     else
                          {
                          if   (!psubclass->aitem[ulI].ulBitmap ||
                                psubclass->aitem[ulI].ulBitmap == IDKLEISTE_PLCEHLDR)
                               {
                               rclSave.xLeft += 8 ;
                               }
                          }
                     }
                }
           WinReleasePS (hps) ;
           return mr ;
           }
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }
   
