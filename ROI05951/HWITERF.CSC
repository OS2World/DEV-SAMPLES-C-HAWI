#include <wpfolder.sc>

class:
   HWITerF,
   external stem = hwiterf,
   local,
   external prefix = hwiterf_,
   classprefix = hwiterfM_,
   major version = 1,
   minor version = 2;
       
parent:
   WPFolder;

passthru: C.ih;
   #define  INCL_BASE
   #define  INCL_PM
   #include <os2.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <time.h>

   #define IDN_Termine 1000
endpassthru;

data:
   HMODULE hmod, class;

methods:
   override wpclsInitData, class;
   override wpclsQueryIconData, class;
   override wpclsQueryStyle, class;
   override wpclsQueryTitle, class;
   override wpDragOver;
