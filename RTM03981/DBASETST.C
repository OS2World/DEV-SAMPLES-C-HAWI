/* $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\DBASETST.C 1.1 1998/01/17 22:10:38 HAWI Exp $ */

/* Include OS/2 Header */
#define  INCL_BASE
#include <os2.h>

/* Include C Header */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include UDF Header */
#include "UDF.H"

int main (void)
   {
   char          chType ;
   char          szCharacter [_MAX_PATH] ;
   char          szName [_MAX_PATH] ;
   char*         psz ;
   DBFCREATE     dbfcreate [] = {{ "NAME",    FIELD_CHARACTER, 64, 0 },
                                 { "VORNAME", FIELD_CHARACTER, 64, 0 },
                                 { "PLZ",     FIELD_NUMERIC,    5, 0 },
                                 { "GEBURT",  FIELD_DATE,       0, 0 },
                                 { "UMSATZ",  FIELD_NUMERIC,    9, 2 },
                                 { "LEDIG",   FIELD_LOGICAL,    0, 0 },
                                 { "NOTIZ",   FIELD_MEMO,       0, 0 }} ;
   double        dNumeric ;
   PDBF          pdbf ;
   RC            rc ;
   TDATE2        tdate2 ;
   unsigned long fLogical ;
   unsigned long ul ;
   unsigned long ulFLDCount ;
   unsigned long ulI ;
   unsigned long ulJ ;
   unsigned long ulRecord ;

   /* Create Database */
   if   ((rc = DBFCreate (&pdbf, "TEST.DBF", sizeof (dbfcreate) / sizeof (dbfcreate [0]), dbfcreate)) == RC_OK)
        {
        /* Record 1 anh„ngen */
        rc = DBFRECAppend (pdbf, &ulRecord) ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "NAME", "Wilhelm") ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "VORNAME", "Harald") ;
        /* Numerischen Wert setzen */
        dNumeric = 50765 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
        /* Datumswert setzen */
        tdate2.ulDay = 1 ;
        tdate2.ulMonth = 2 ;
        tdate2.ulYear = 3456 ;
        /* Datumsspalte setzen */
        rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
        /* Numerischen Wert setzen */
        dNumeric = 12345.67 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
        /* Logischen Wert setzen */
        fLogical = TRUE ;
        /* Logische Spalte setzen */
        rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
        /* Record 1 schreiben */
        rc = DBFRECWrite (pdbf, ulRecord) ;
        /* Record 2 anh„ngen */
        rc = DBFRECAppend (pdbf, &ulRecord) ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "NAME", "Jackson") ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "VORNAME", "Michael") ;
        /* Numerischen Wert setzen */
        dNumeric = 12345 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
        /* Datumswert setzen */
        tdate2.ulDay = 2 ;
        tdate2.ulMonth = 3 ;
        tdate2.ulYear = 4567 ;
        /* Datumsspalte setzen */
        rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
        /* Numerischen Wert setzen */
        dNumeric = 23456.78 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
        /* Logischen Wert setzen */
        fLogical = FALSE ;
        /* Logische Spalte setzen */
        rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
        /* Memo Spalte setzen */
        rc = DBFMEMWrite (pdbf, "NOTIZ", "Plattenmillionaer") ;
        /* Record 2 schreiben */
        rc = DBFRECWrite (pdbf, ulRecord) ;
        /* Record 3 anh„ngen */
        rc = DBFRECAppend (pdbf, &ulRecord) ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "NAME", "Watson") ;
        /* Textspalte setzen */
        rc = DBFFLDPutCharacter (pdbf, "VORNAME", "Johnny (Guitar)") ;
        /* Numerischen Wert setzen */
        dNumeric = 12345 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "PLZ", &dNumeric) ;
        /* Datumswert setzen */
        tdate2.ulDay = 3 ;
        tdate2.ulMonth = 4 ;
        tdate2.ulYear = 5678 ;
        /* Datumsspalte setzen */
        rc = DBFFLDPutDate (pdbf, "GEBURT", &tdate2) ;
        /* Numerischen Wert setzen */
        dNumeric = 345.67 ;
        /* Numerische Spalte setzen */
        rc = DBFFLDPutNumeric (pdbf, "UMSATZ", &dNumeric) ;
        /* Logischen Wert setzen */
        fLogical = FALSE ;
        /* Logische Spalte setzen */
        rc = DBFFLDPutLogical (pdbf, "LEDIG", &fLogical) ;
        /* Record 3 schreiben */
        rc = DBFRECWrite (pdbf, ulRecord) ;
        /* Record 2 l”schen */
        rc = DBFRECDelete (pdbf, 2) ;
        /* Datenbank schliessen */
        rc = DBFClose (pdbf) ;
        /* Datenbank packen */
        rc = DBFPack ("TEST.DBF") ;
        /* Datenbank ”ffnen */
        if   ((rc = DBFOpen (&pdbf, "TEST.DBF")) == RC_OK)
             {
             /* Anzahl Records lesen */
             if   ((rc = DBFRECCount (pdbf, &ulRecord)) == RC_OK)
                  {
                  /* Anzahl Spalten lesen */
                  if   ((rc = DBFFLDCount (pdbf, &ulFLDCount)) == RC_OK)
                       {
                       /* Durch alle Records laufen */
                       for  (ulI = 1; ulI <= ulRecord; ulI++)
                            {
                            /* Record positionieren */
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 /* Ist Record gel”scht */
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      /* Fr jeden Record durch alle Spalten */
                                      for  (ulJ = 1; ulJ <= ulFLDCount; ulJ++)
                                           {
                                           /* Name der Spalte lesen */
                                           if   ((rc = DBFFLDName (pdbf, ulJ, szName)) == RC_OK)
                                                {
                                                /* Typ der Spalte lesen */
                                                if   ((rc = DBFFLDType (pdbf, szName, &chType)) == RC_OK)
                                                     {
                                                     /* Typ */
                                                     switch (chType)
                                                        {
                                                        /* Character */
                                                        case FIELD_CHARACTER:
                                                             /* Character lesen */
                                                             rc = DBFFLDGetCharacter (pdbf, szName, szCharacter) ;
                                                             break ;
                                                        /* Date */
                                                        case FIELD_DATE:
                                                             /* Date lesen */
                                                             rc = DBFFLDGetDate (pdbf, szName, &tdate2) ;
                                                             break ;
                                                        /* Logical */
                                                        case FIELD_LOGICAL:
                                                             /* Logical lesen */
                                                             rc = DBFFLDGetLogical (pdbf, szName, &fLogical) ;
                                                             break ;
                                                        /* Memo */
                                                        case FIELD_MEMO:
                                                             /* Gibt es ein Memo? */
                                                             if   ((rc = DBFMEMExist (pdbf, szName)) == RC_OK)
                                                                  {
                                                                  /* Ja, L„nge des Memo ermitteln */
                                                                  if   ((rc = DBFMEMLength (pdbf, szName, &ul)) == RC_OK)
                                                                       {
                                                                       /* Speicher allokieren */
                                                                       psz = malloc (ul + 1) ;
                                                                       /* Memo lesen */
                                                                       rc = DBFMEMRead (pdbf, szName, psz) ;
                                                                       /* Speicher wieder freigeben */
                                                                       free (psz) ;
                                                                       }
                                                                  }
                                                             break ;
                                                        /* Numeric */
                                                        case FIELD_NUMERIC:
                                                             /* Numeric lesen */
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
             /* Skip -Anzahl Record springt an Anfang */
             rc = DBFRECSkip (pdbf, -ulRecord) ;
             /* Das springt auch an den Anfang */
             rc = DBFRECTop (pdbf) ;
             /* Das ans Ende */
             rc = DBFRECBottom (pdbf) ;
             /* Datenbank schlieáen */
             rc = DBFClose (pdbf) ;
             }
        }
   return 0 ;
   }

