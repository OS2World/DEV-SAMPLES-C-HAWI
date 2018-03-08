/* $Header: D:\Projekte\Redaktion OS!2 Inside\Auswertungen satt\Source\RCS\AUSWERT1.C 1.1 1997/05/03 16:58:46 HAWI Exp $ */

#define  INCL_BASE
#include <os2.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "D:\Progs\XACT\XACTRPC\XACTRPC.H"

INT main (VOID)
   {
   APIRET    rc ;
   BOOL      f ;
   CHAR      sz [CCHMAXPATH] ;
   LStyle    lstyle ;
   PID       pid ;
   STARTDATA stdata ;
   XSINFO*   xsinfo ;
   ULONG     ul ;
   ULONG     ulColumns ;
   ULONG     ulLines ;
   ULONG     ulDrive ;
   ULONG     ulGrafik ;
   ULONG     ulMask ;
   ULONG     ulSession ;
   ULONG     ulTable ;

   f = FALSE ;
   while ((xsinfo = XS_BeginSession ("")) == NULL)
      {
      if   (!f)
           {
           memset (&stdata,
                   0,
                   sizeof (STARTDATA)) ;
           stdata.Length = sizeof (STARTDATA) ;
           stdata.Related = SSF_RELATED_INDEPENDENT ;
           stdata.FgBg = SSF_FGBG_FORE ;
           stdata.TraceOpt = SSF_TRACEOPT_NONE ;
           stdata.PgmTitle = "XACTOS2" ;
           stdata.PgmName = "D:\\PROGS\\XACT\\XACTOS2.EXE" ;
           stdata.PgmInputs = "-b" ;
           stdata.InheritOpt = SSF_INHERTOPT_SHELL ;
           stdata.SessionType = SSF_TYPE_PM ;
           stdata.PgmControl = SSF_CONTROL_VISIBLE ;
           stdata.ObjectBuffer = sz ;
           stdata.ObjectBuffLen = sizeof (sz) ;
           rc = DosStartSession (&stdata,
                                 &ulSession,
                                 &pid) ;
           if   (rc == 0 || rc == ERROR_SMG_START_IN_BACKGROUND)
                {
                f = TRUE ;
                }
           else
                {
                return 1 ;
                }
           }
        }
   DosQueryCurrentDisk (&ulDrive,
                        &ulMask) ;
   sz [0] = ulDrive + '@' ;
   sz [1] = ':' ;
   sz [2] = '\\' ;
   ul = sizeof (sz) - 3 ;
   DosQueryCurrentDir (ulDrive,
                       &sz [3],
                       &ul);
   strcat (sz,
           "\\AUS1.CSV") ;
   if   ((ulTable = XT_Open (xsinfo,
                             0,
                             sz,
                             0)) != 0)
        {
        XT_SetTitles (xsinfo,
                      ulTable,
                      "Title",
                      "Subtitle",
                      "Caption") ;
        XT_ReadCell (xsinfo,
                     ulTable,
                     1,
                     1,
                     sz) ;
        ulColumns = XT_NumOfColumns (xsinfo,
                                     ulTable,
                                     INQ_USED) ;
        ulLines = XT_NumOfLines (xsinfo,
                                 ulTable,
                                 INQ_USED) ;
        XT_WriteCell (xsinfo,
                      ulTable,
                      ulLines - 1,
                      ulColumns - 1,
                      "12") ;
        sz [0] = ulDrive + '@' ;
        sz [1] = ':' ;
        sz [2] = '\\' ;
        ul = sizeof (sz) - 3 ;
        DosQueryCurrentDir (ulDrive,
                            &sz [3],
                            &ul);
        strcat (sz,
                "\\AUS1.XTF") ;
        XT_Save (xsinfo,
                 ulTable,
                 sz) ;
        if   (XT_SelectAll (xsinfo,
                            ulTable,
                            1) == 0)
             {
             if   ((ulGrafik = XT_UseStylesheet (xsinfo,
                                                 ulTable,
                                                 "D:\\Progs\\XACT\\Vorlagen\\BAR.STL",
                                                 COL_DEVIATION,
                                                 0)) != 0)
                   {
                   memset (&lstyle,
                           0,
                           sizeof (LStyle)) ;
                   lstyle.endStyle = LSTYLE_ARROW ;
                   lstyle.type = LTYP_SOLID ;
                   lstyle.mask = LM_ENDSTYLE | LM_TYPE ;
                   XG_LineStyle (xsinfo,
                                 &lstyle) ;
                   XG_Line (xsinfo,
                            ulGrafik,
                            100.,
                            100.,
                            100.,
                            100.) ;
                   DosQueryCurrentDisk (&ulDrive,
                                        &ulMask) ;
                   sz [0] = ulDrive + '@' ;
                   sz [1] = ':' ;
                   sz [2] = '\\' ;
                   ul = sizeof (sz) - 3 ;
                   DosQueryCurrentDir (ulDrive,
                                       &sz [3],
                                       &ul);
                   strcat (sz,
                           "\\AUS1.BMP") ;
                   XG_Export (xsinfo,
                              ulGrafik,
                              XGE_BMP,
                              72,
                              BMP_COL_8,
                              sz) ;
                   XG_Close (xsinfo,
                             ulGrafik) ;
                   }
             }
        XT_Close (xsinfo,
                  ulTable) ;
        }
   XS_EndSession (xsinfo,
                  1) ;
   return 0 ;
   }

