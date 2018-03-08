/* $Header: D:\Projekte\Redaktion SE\DBase intim\Source\RCS\DBASETST.C 1.2 1997/04/21 13:15:01 HAWI Exp $ */

#define  INCL_BASE
#include <os2.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DBASE.H"

INT main (INT, CHAR * []) ;

INT main (INT argc,
          PSZ argv [])
   {
   BOOL      fLogical ;
   CHAR      chType ;
   CHAR      szCharacter [261] ;
   CHAR      szName [261] ;
   PDBF      pdbf ;
   DBFCREATE dbfcreate [] = {{ "NAME",    FIELD_CHARACTER, 64, 0 },
                             { "VORNAME", FIELD_CHARACTER, 64, 0 },
                             { "PLZ",     FIELD_NUMERIC,    5, 0 },
                             { "GEBURT",  FIELD_DATE,       0, 0 },
                             { "UMSATZ",  FIELD_NUMERIC,    9, 2 },
                             { "LEDIG",   FIELD_LOGICAL,    1, 0 }} ;
   double    dNumeric ;
   RC        rc ;
   TDATE2    tdate2 ;
   ULONG     ulCount ;
   ULONG     ulFLDCount ;
   ULONG     ulI ;
   ULONG     ulJ ;

   if   (argc == 2)
        {
        if   ((rc = DBFCreate (&pdbf, argv [1], sizeof (dbfcreate) / sizeof (dbfcreate [0]), dbfcreate)) == RC_OK)
             {
             rc = DBFRECAppend (pdbf, &ulCount) ;
             strcpy (szCharacter, "Wilhelm") ;
             rc = DBFFLDPutCharacter (pdbf, "NAME", szCharacter) ;
             strcpy (szCharacter, "Harald") ;
             rc = DBFFLDPutCharacter (pdbf, "VORNAME", szCharacter) ;
             dNumeric = 50765 ;
             rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
             tdate2.ulDay = 1 ;
             tdate2.ulMonth = 2 ;
             tdate2.ulYear = 3456 ;
             rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
             dNumeric = 12345.67 ;
             rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
             fLogical = TRUE ;
             rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
             rc = DBFRECWrite (pdbf, ulCount) ;
             rc = DBFRECAppend (pdbf, &ulCount) ;
             strcpy (szCharacter, "Jackson") ;
             rc = DBFFLDPutCharacter (pdbf, "NAME", szCharacter) ;
             strcpy (szCharacter, "Michael") ;
             rc = DBFFLDPutCharacter (pdbf, "VORNAME", szCharacter) ;
             dNumeric = 12345 ;
             rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
             tdate2.ulDay = 2 ;
             tdate2.ulMonth = 3 ;
             tdate2.ulYear = 4567 ;
             rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
             dNumeric = 23456.78 ;
             rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
             fLogical = FALSE ;
             rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
             rc = DBFRECWrite (pdbf, ulCount) ;
             rc = DBFRECAppend (pdbf, &ulCount) ;
             strcpy (szCharacter, "Watson") ;
             rc = DBFFLDPutCharacter (pdbf, "NAME", szCharacter) ;
             strcpy (szCharacter, "Johnny (Guitar)") ;
             rc = DBFFLDPutCharacter (pdbf, "VORNAME", szCharacter) ;
             dNumeric = 12345 ;
             rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
             tdate2.ulDay = 3 ;
             tdate2.ulMonth = 4 ;
             tdate2.ulYear = 5678 ;
             rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
             dNumeric = 345.67 ;
             rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
             fLogical = FALSE ;
             rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
             rc = DBFRECWrite (pdbf, ulCount) ;
             rc = DBFRECDelete (pdbf, 2) ;
             rc = DBFClose (pdbf) ;
             }
        rc = DBFPack (argv [1]) ;
        if   ((rc = DBFOpen (&pdbf, argv [1])) == RC_OK)
             {
             if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                  {
                  if   ((rc = DBFFLDCount (pdbf, &ulFLDCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      for  (ulJ = 1; ulJ <= ulFLDCount; ulJ++)
                                           {
                                           if   ((rc = DBFFLDName (pdbf, ulJ, szName)) == RC_OK)
                                                {
                                                if   ((rc = DBFFLDType (pdbf, szName, &chType)) == RC_OK)
                                                     {
                                                     switch (chType)
                                                        {
                                                        case FIELD_CHARACTER:
                                                             rc = DBFFLDGetCharacter (pdbf, szName, szCharacter) ;
                                                             break ;
                                                        case FIELD_DATE:
                                                             rc = DBFFLDGetDate (pdbf, szName, &tdate2) ;
                                                             break ;
                                                        case FIELD_LOGICAL:
                                                             rc = DBFFLDGetLogical (pdbf, szName, &fLogical) ;
                                                             break ;
                                                        case FIELD_NUMERIC:
                                                             rc = DBFFLDGetNumeric (pdbf, szName, &dNumeric) ;
                                                             break ;
                                                        }
                                                     }
                                                }
                                           }
                                      }
                                 }
                            }
                       }
                  }
             rc = DBFRECSkip (pdbf, -ulCount) ;
             rc = DBFRECTop (pdbf) ;
             rc = DBFRECBottom (pdbf) ;
             rc = DBFClose (pdbf) ;
             }
        }
   return 0 ;
   }

