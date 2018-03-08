/* $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\UDFQ.C 1.8 1998/01/25 18:37:54 HAWI Exp $ */

/* Include C header */
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include table function header */
#include "UDF.H"

/* Print Banner */
void Banner (unsigned long ul)
   {
   unsigned char* apsz [] = { "Wrong parameter",
                              "Missing DBF III database filename" } ;

   fprintf (stdout,
            "Error %u: %s\n\n%s\n\n%s\n%s\n%s\n%s\n",
            ul,
            apsz [ul - 1],
            "UDFQ -dDBF [-fUDF] [-iINI] [-sspecific]",
            "-d = full qualified filename of DBF III database",
            "-f = overwrite UDF file (default=filename of database with extension UDF)",
            "-s = overwrite SPECIFIC name (default=filename of database w/o extension)",
            "-i = overwrite INI file (default=UDF.INI)") ;
   }

/* Application start */
int main (int argc, char** argv)
   {
   char          chType ;
   char          sz [_MAX_PATH] ;
   char          szDFilename [_MAX_PATH] = "" ;
   char          szFFilename [_MAX_PATH] = "" ;
   char          szFLDName [_MAX_PATH] ;
   char          szIFilename [_MAX_PATH] = "UDF.INI" ;
   char          szSpecific [_MAX_PATH] = "" ;
   char          szTDirectory [_MAX_PATH] ;
   char          szTDrive [_MAX_PATH] ;
   char          szTExtension [_MAX_PATH] ;
   char          szTFilename [_MAX_PATH] ;
   char*         apsz [] = { "EXTERNAL NAME \'UDF!UDF\'",
                             "LANGUAGE C",
                             "PARAMETER STYLE DB2SQL",
                             "NOT DETERMINISTIC",
                             "FENCED",
                             "NOT NULL CALL",
                             "EXTERNAL ACTION",
                             "NO SQL",
                             "SCRATCHPAD",
                             "FINAL CALL",
                             "DISALLOW PARALLEL",
                             "DBINFO;"                           } ;
   FILE*         pfile ;
   PDBF          pdbf ;
   PPRF          pprf ;
   PRFAPP        prfapp ;
   PRFTOP        prftop ;
   RC            rc ;
   unsigned long ulFLDCount ;
   unsigned long ulFLDDecimal ;
   unsigned long ulFLDLength ;
   unsigned long ulI ;

   /* One Parameter is required (-d) */
   if   (argc > 1)
        {
        /* Iterate thru parameters */
        for  (ulI = 1; ulI < argc; ulI++)
             {
             /* Check first character of a parameter */
             switch (argv [ulI] [0])
                {
                /* It must be / or - */
                case '-':
                case '/':
                     /* Check second character of parameter */
                     switch (argv [ulI] [1])
                        {
                        /* This is the required DBASE III database filename */
                        case 'd':
                        case 'D':
                             strcpy (szDFilename, &argv [ulI] [2]) ;
                             break ;
                        /* This overwrites the UDF template file */
                        case 'f':
                        case 'F':
                             strcpy (szFFilename, &argv [ulI] [2]) ;
                             break ;
                        /* This overwrites the INI filename */
                        case 'i':
                        case 'I':
                             strcpy (szIFilename, &argv [ulI] [2]) ;
                             break ;
                        /* This overwrites the SPECIFIC parameter of the CREATE FUNCTION call */
                        case 's':
                        case 'S':
                             strcpy (szSpecific, &argv [ulI] [2]) ;
                             break ;
                        /* Error */
                        default:
                             Banner (1) ;
                             return (1) ;
                        }
                     break ;
                /* Error */
                default:
                     Banner (1) ;
                     return (1) ;
                }
             }
        }
   /* DBF III filename is required */
   if   (*szDFilename)
        {
        /* Split path into elements */
        _splitpath (szDFilename, szTDrive, szTDirectory, szTFilename, szTExtension) ;
        /* If there's no UDF template filename, create one is the name database.UDF */
        if   (!*szFFilename)
             {
             strcpy (szFFilename, szTFilename) ;
             strcat (szFFilename, ".UDF") ;
             }
        /* Overwrite SPECIFIC keyword if parameter exists */
        if   (!*szSpecific)
             {
             strcpy (szSpecific, szTFilename) ;
             }
        /* Open and read INI file */
        if   ((rc = PRFOpen (&pprf, szIFilename)) == RC_OK)
             {
             /* Open UDF template file */
             if   ((pfile = fopen (szFFilename, "w")) != NULL)
                  {
                  /* Open DBF III database */
                  if   ((rc = DBFOpen (&pdbf, szDFilename)) == RC_OK)
                       {
                       /* Query the count of columns */
                       if   ((rc = DBFFLDCount (pdbf, &ulFLDCount)) == RC_OK)
                            {
                            /* Print header of UDF template file */
                            fprintf (pfile, "The CREATE FUNCTION call for the database %s%s\n", szTFilename, szTExtension) ;
                            fprintf (pfile, "=============================================================================\n\n") ;
                            fprintf (pfile, "CONNECT TO sample;\n") ;
                            fprintf (pfile, "CREATE FUNCTION %s (INTEGER,\n", szSpecific) ;
                            fprintf (pfile, "                     VARCHAR(255))\n") ;
                            fprintf (pfile, "   RETURNS TABLE (") ;
                            /* Insert application entry in INI for this DBF III database */
                            memset (&prfapp, 0, sizeof (PRFAPP)) ;
                            strcpy (prfapp.szApplication, szSpecific) ;
                            prfapp.fActive = 1 ;
                            PRFAPPReplace (pprf, &prfapp, PRFTYPE_INSERTREPLACE) ;
                            /* If database filename was full qualified */
                            if   (*szTDrive)
                                 {
                                 /* Build drive/pathname */
                                 strcpy (sz, szTDrive) ;
                                 strcat (sz, "\\") ;
                                 szTDirectory [strlen (szTDirectory) - 1] = 0 ;
                                 }
                            /* else */
                            else
                                 {
                                 /* Query drive/pathname */
                                 _getcwd (sz, _MAX_PATH) ;
                                 }
                            /* Insert entry in INI with drive/directory of DBF III database */
                            memset (&prftop, 0, sizeof (PRFTOP)) ;
                            strcpy (prftop.szTopic, "DIRECTORY") ;
                            strcpy (prftop.szValue, sz) ;
                            prftop.fActive = 1 ;
                            /* Insert entry in INI with file of DBF III database */
                            PRFTOPReplace (pprf, szSpecific, &prftop, PRFTYPE_INSERTREPLACE) ;
                            memset (&prftop, 0, sizeof (PRFTOP)) ;
                            strcpy (prftop.szTopic, "DATA") ;
                            strcpy (prftop.szValue, szTFilename) ;
                            strcat (prftop.szValue, szTExtension) ;
                            prftop.fActive = 1 ;
                            PRFTOPReplace (pprf, szSpecific, &prftop ,PRFTYPE_INSERTREPLACE) ;
                            /* Iterate thru columns */
                            for  (ulI = 1; ulI <= ulFLDCount; ulI++)
                                 {
                                 if   (ulI != 1)
                                      {
                                      fprintf (pfile, ",\n                  ") ;
                                      }
                                 /* Query name of column */
                                 if   ((rc = DBFFLDName (pdbf, ulI, szFLDName)) == RC_OK)
                                      {
                                      /* Print name */
                                      fprintf (pfile, "%-11s", szFLDName) ;
                                      /* Query size of column */
                                      if   ((rc = DBFFLDNLength (pdbf, ulI, &ulFLDLength)) == RC_OK)
                                           {
                                           /* Query type of column */
                                           if   ((rc = DBFFLDNType (pdbf, ulI, &chType)) == RC_OK)
                                                {
                                                /* Check type */
                                                switch (chType)
                                                   {
                                                   /* C maps to VARCHAR */
                                                   case FIELD_CHARACTER:
                                                        fprintf (pfile, "VARCHAR(%u)", ulFLDLength) ;
                                                        break ;
                                                   /* D maps to DATE */
                                                   case FIELD_DATE:
                                                        fprintf (pfile, "DATE") ;
                                                        break ;
                                                   /* L maps to CHAR(1) */
                                                   case FIELD_LOGICAL:
                                                        fprintf (pfile, "CHAR(1)") ;
                                                        break ;
                                                   /* M maps to LONG VARCHAR */
                                                   case FIELD_MEMO:
                                                        fprintf (pfile, "VARCHAR(512)") ;
                                                        break ;
                                                   /* N */
                                                   case FIELD_NUMERIC:
                                                        /* Dies it have decimal places */
                                                        if   ((rc = DBFFLDNDecimal (pdbf, ulI, &ulFLDDecimal)) == RC_OK)
                                                             {
                                                             /* If yes, use CHAR() */
                                                             if   (ulFLDDecimal)
                                                                  {
                                                                  fprintf (pfile, "VARCHAR(%u)", ulFLDLength + ulFLDDecimal + 1) ;
                                                                  }
                                                             /* else */
                                                             else
                                                                  {
                                                                  /* use INT */
                                                                  fprintf (pfile, "INT") ;
                                                                  }
                                                             }
                                                        break ;
                                                   }
                                                }
                                           }
                                      }
                                 }
                            fprintf (pfile, ")\n") ;
                            /* Print SPECIFIC entry */
                            fprintf (pfile, "   SPECIFIC %s\n", szSpecific) ;
                            /* Print rest of static PARAMETERS to CREATE FUNCTION */
                            for  (ulI = 0; ulI < sizeof (apsz) / sizeof (apsz [0]); ulI++)
                                 {
                                 fprintf (pfile, "   %s\n", apsz [ulI]) ;
                                 }
                            fprintf (pfile, "CONNECT RESET;\n") ;
                            /* Print the correct SELECT for this DBASE III database */
                            fprintf (pfile, "\nThe SELECT using the TABLE FUNCTION for the database %s%s\n", szTFilename, szTExtension) ;
                            fprintf (pfile, "=============================================================================\n\n") ;
                            fprintf (pfile, "CONNECT TO sample;\n") ;
                            fprintf (pfile, "SELECT * FROM TABLE (%s(%u,\'\')) AS T;\n", szSpecific, ulFLDCount) ;
                            fprintf (pfile, "CONNECT RESET;\n") ;
                            fprintf (pfile, "\nImportant: The value %u is required for this specific UDF shown above\n\n", ulFLDCount) ;
                            }
                       /* Clode DBF file */
                       rc = DBFClose (pdbf) ;
                       }
                  /* Clode UDF template file */
                  fclose (pfile) ;
                  }
             /* Close INI file */
             rc = PRFClose (pprf) ;
             }
        }
   else
        {
        Banner (2) ;
        return (2) ;
        }
   return 0 ;
   }

