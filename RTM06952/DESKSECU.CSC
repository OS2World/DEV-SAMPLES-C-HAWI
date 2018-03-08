# $Header: D:/projekte/wps\040security/source/RCS/DESKSECU.CSC 1.1 1995/06/13 20:10:07 HaWi Exp $

#include <wpdesk.sc>

class:
   DeskSecu,
   external stem = desksecu,
   local,
   external prefix = desksecu_,
   classprefix = desksecuM_,
   major version = 1,
   minor version = 1;
       
parent:
   WPDesktop;

passthru: C.ih;
   #define  INCL_BASE
   #define  INCL_PM
   #include <os2.h>
   #include <stdlib.h>
   #include <string.h>

endpassthru;

methods:
   override wpFilterPopupMenu ;
   override wpModifyPopupMenu ;
