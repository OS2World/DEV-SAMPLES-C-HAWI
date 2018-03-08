#define HWITerF_Class_Source
#include "HWITERF.ih"

SOM_Scope void SOMLINK hwiterf_wpclsInitData (M_HWITerF* somSelf)
   {
   M_HWITerFData* somThis ;
   PSZ            psz ;

   somThis = M_HWITerFGetData (somSelf) ;
   M_HWITerF_parent_M_WPFolder_wpclsInitData (somSelf) ;
   psz = _somLocateClassFile (SOMClassMgrObject,
                              SOM_IdFromString (_somGetClassName (somSelf)),
                              1,
                              1) ;
   DosQueryModuleHandle (psz,
                         &_hmod) ;
   }

SOM_Scope ULONG SOMLINK hwiterf_wpclsQueryIconData (M_HWITerF* somSelf,
                                                    PICONINFO  pIconInfo)
   {
   M_HWITerFData* somThis ;

   somThis = M_HWITerFGetData (somSelf) ;
   if   (pIconInfo)
        {
        pIconInfo->fFormat = ICON_RESOURCE ;
        pIconInfo->hmod = _hmod ;
        pIconInfo->resid = IDN_Termine ;
        }
   return sizeof (ICONINFO) ;
   }

SOM_Scope ULONG SOMLINK hwiterf_wpclsQueryStyle (M_HWITerF* somSelf)
   {
   return (M_HWITerF_parent_M_WPFolder_wpclsQueryStyle (somSelf) | CLSSTYLE_NEVERTEMPLATE) ;
   }

SOM_Scope PSZ SOMLINK hwiterf_wpclsQueryTitle (M_HWITerF* somSelf)
   {
   return "ReminderFolder" ;
   }

SOM_Scope MRESULT SOMLINK hwiterf_wpDragOver (HWITerF*  somSelf, 
                                              HWND      hwndCnr,
                                              PDRAGINFO pdrgInfo)
   {
   BOOL      f ;
   CHAR      sz [CCHMAXPATH] ;
   MRESULT   mr ;
   PDRAGITEM pditem ;
   ULONG     ul ;
   
   f = TRUE ;
   for  (ul = 0; ul < pdrgInfo->cditem; ul++)
        {
        pditem = DrgQueryDragitemPtr (pdrgInfo,
                                      ul) ;
        DrgQueryStrName (pditem->hstrType,
                         sizeof (sz),
                         sz) ;
        if   (!strstr (sz,
                       "DRT_TERMIN"))
             {
             f = FALSE ;
             break ;
             }
        }
   mr = (f) ? MRFROM2SHORT (DOR_DROP, pdrgInfo->usOperation) : MRFROM2SHORT (DOR_NEVERDROP, pdrgInfo->usOperation) ;
   return mr ;
   }
