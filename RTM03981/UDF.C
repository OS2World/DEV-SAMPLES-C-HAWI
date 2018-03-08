/* $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\UDF.C 1.7 1998/01/25 19:24:17 HAWI Exp $ */

/* Include C Header */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Include DB2 Header */
#include <sqludf.h>

/* Include UDF Header */
#include "UDF.H"

/* My SQLSTATEs */
#define SQLUDF_STATE_DBFCLOSE            "38600"
#define SQLUDF_STATE_DBFFLDCOUNT         "38601"
#define SQLUDF_STATE_DBFFLDNDECIMAL      "38602"
#define SQLUDF_STATE_DBFFLDNGETCHARACTER "38603"
#define SQLUDF_STATE_DBFFLDNGETDATE      "38604"
#define SQLUDF_STATE_DBFFLDNGETLOGICAL   "38605"
#define SQLUDF_STATE_DBFFLDNGETMEMO      "38606"
#define SQLUDF_STATE_DBFFLDNGETNUMERIC   "38607"
#define SQLUDF_STATE_DBFFLDNLENGTH       "38608"
#define SQLUDF_STATE_DBFFLDNTYPE         "38609"
#define SQLUDF_STATE_DBFOPEN             "38610"
#define SQLUDF_STATE_DBFRECGO            "38611"
#define SQLUDF_STATE_DBFRECSKIP          "38612"
#define SQLUDF_STATE_DBFRECTOP           "38613"
#define SQLUDF_STATE_PRFCLOSE            "38614"
#define SQLUDF_STATE_PRFLOCATE           "38615"
#define SQLUDF_STATE_PRFOPEN             "38616"
#define SQLUDF_STATE_WRONGFIELDCOUNT     "38617"

/* Struct for my scratchpad usage */
typedef struct
   {
   char**           papszResult ;
   PDBF             pdbf ;
   PPRF             pprf ;
   SQLUDF_NULLIND** pasqludfnullind ;
   unsigned long    ulRow ;
   } SCRATCHPAD, *PSCRATCHPAD ;

void SQL_API_FN UDF (SQLUDF_INTEGER* pulFieldCount, SQLUDF_VARCHAR* pszParameter, ...)
   {
   char               chType ;
   char               szDBFFilename [_MAX_PATH] ;
   char               szIFilename [_MAX_PATH] ;
   char*              apszResult [255] ;
   char*              psz ;
   char*              pszSQLFunctionname ;
   char*              pszSQLMessage ;
   char*              pszSQLSpecificname ;
   char*              pszSQLState ;
   double             dNumeric ;
   PRFTOP             prftop ;
   PSCRATCHPAD        pscratchpad ;
   RC                 rc ;
   SQLUDF_CALL_TYPE*  psqludfcalltype ;
   SQLUDF_DBINFO*     psqludfdbinfo ;
   SQLUDF_NULLIND*    asqludfnullind [255] ;
   SQLUDF_SCRATCHPAD* psqludfscratchpad ;
   TDATE2             tdate2 ;
   unsigned long      fLogical ;
   unsigned long      ul ;
   unsigned long      ulColumn ;
   unsigned long      ulCount ;
   unsigned long      ulDecimal ;
   unsigned long      ulLength ;
   va_list            valist ;
   void*              pv ;

   /* Running counter for parameters */
   ulCount = 0 ;
   /* Start analyzing parameters with Count of columns in this database */
   va_start (valist, pulFieldCount) ;
   ulCount++ ;
   /* For future use */
   pszParameter = va_arg (valist, SQLUDF_VARCHAR*) ;
   ulCount++ ;
   /* Calculate parameters passed */
   ul = 11 + (*pulFieldCount * 2) ;
   /* Loop thru parameters */
   while (ulCount < ul)
      {
      /* These are the result fields for every column in the serviced database */
      if   (ulCount > 1 && ulCount < 2 + *pulFieldCount)
           {
           apszResult [ulCount - 2] = va_arg (valist, SQLUDF_VARCHAR*) ;
           }
      /* These are the indicator fields for the first two fix parameters */
      if   (ulCount > 1 + *pulFieldCount && ulCount < 4 + *pulFieldCount)
           {
           pv = va_arg (valist, SQLUDF_NULLIND*) ;
           }
      /* These are the indicator fields for every column in the serviced database */
      if   (ulCount > 3 + *pulFieldCount && ulCount < 4 + (*pulFieldCount * 2))
           {
           asqludfnullind [ulCount - 4 + *pulFieldCount] = va_arg (valist, SQLUDF_NULLIND*) ;
           }
      /* Heres room for the SQLSTATE */
      if   (ulCount == 4 + (*pulFieldCount * 2))
           {
           pszSQLState = va_arg (valist, char*) ;
           }
      /* The name of the function */
      if   (ulCount == 5 + (*pulFieldCount * 2))
           {
           pszSQLFunctionname = va_arg (valist, char*) ;
           }
      /* The specific name */
      if   (ulCount == 6 + (*pulFieldCount * 2))
           {
           pszSQLSpecificname = va_arg (valist, char*) ;
           }
      /* Room for a descriptive return code text */
      if   (ulCount == 7 + (*pulFieldCount * 2))
           {
           pszSQLMessage = va_arg (valist, char*) ;
           }
      /* The scratchpad pointer, map to my own layout */
      if   (ulCount == 8 + (*pulFieldCount * 2))
           {
           psqludfscratchpad = va_arg (valist, SQLUDF_SCRATCHPAD*) ;
           pscratchpad = (PSCRATCHPAD) &psqludfscratchpad->data [0] ;
           }
      /* The call type (OPEN, FETCH, CLOSE */
      if   (ulCount == 9 + (*pulFieldCount * 2))
           {
           psqludfcalltype = va_arg (valist, SQLUDF_CALL_TYPE*) ;
           }
      /* Additional DB2 info */
      if   (ulCount == 10 + (*pulFieldCount * 2))
           {
           psqludfdbinfo = va_arg (valist, SQLUDF_DBINFO*) ;
           }
      /* Increase running pointer */
      ulCount++ ;
      }
   /* End parameter analyzing */
   va_end (valist) ;
   /* So what do I have to do? */
   switch (*psqludfcalltype)
      {
      /* The wake up call */
      case SQLUDF_FIRST_CALL:
           /* Pre-set OK */
           strcpy (pszSQLState, SQLUDF_STATE_OK) ;
           /* Alloc place for 255 result pointer */
           pscratchpad->papszResult = calloc (255, sizeof (char*)) ;
           /* Alloc place for 255 indicator pointer */
           pscratchpad->pasqludfnullind = calloc (255, sizeof (SQLUDF_NULLIND*)) ;
           /* Copy Result pointer from temporary storage to scratchpad pointer */
           memcpy (pscratchpad->papszResult, apszResult, 255 * sizeof (char*)) ;
           /* Copy indicator pointer from temporary storage to scratchpad pointer */
           memcpy (pscratchpad->pasqludfnullind, asqludfnullind, 255 * sizeof (SQLUDF_NULLIND*)) ;
           /* Search for DB2PATH in Environment */
           strcpy (szIFilename, getenv ("DB2PATH")) ;
           /* And add the FUNCTION/INI combination */
           strcat (szIFilename, "\\FUNCTION\\UDF.INI") ;
           /* Open INI */
           if   ((rc = PRFOpen (&pscratchpad->pprf, szIFilename)) == RC_OK)
                {
                /* Is SPECIFIC qualified? */
                if   ((psz = strchr (pszSQLSpecificname, '.')) != 0)
                     {
                     /* Yes, cut off creator */
                     psz++ ;
                     }
                else
                     {
                     psz = pszSQLSpecificname ;
                     }
                /* Search for DIRECTORY entry in INI */
                strcpy (prftop.szTopic, "DIRECTORY") ;
                if   ((rc = PRFTOPLocate (pscratchpad->pprf, psz, &prftop)) == RC_OK)
                     {
                     strcpy (szDBFFilename, prftop.szValue) ;
                     /* Search for DATA entry in INI */
                     strcpy (prftop.szTopic, "DATA") ;
                     if   ((rc = PRFTOPLocate (pscratchpad->pprf, psz, &prftop)) == RC_OK)
                          {
                          /* Concatinate if needed */
                          if   (*szDBFFilename)
                               {
                               strcat (szDBFFilename, "\\") ;
                               }
                          strcat (szDBFFilename, prftop.szValue) ;
                          /* Open DBF file */
                          if   ((rc = DBFOpen (&pscratchpad->pdbf, szDBFFilename)) == RC_OK)
                               {
                               /* Query count of columns in DBF file */
                               if   ((rc = DBFFLDCount (pscratchpad->pdbf, &ulCount)) == RC_OK)
                                    {
                                    /* They must match */
                                    if   (ulCount != *pulFieldCount)
                                         {
                                         strcpy (pszSQLState, SQLUDF_STATE_WRONGFIELDCOUNT) ;
                                         }
                                    }
                               else
                                    {
                                    strcpy (pszSQLState, SQLUDF_STATE_DBFFLDCOUNT) ;
                                    }
                               }
                          else
                               {
                               strcpy (pszSQLState, SQLUDF_STATE_DBFOPEN) ;
                               }
                          }
                     else
                          {
                          strcpy (pszSQLState, SQLUDF_STATE_PRFLOCATE) ;
                          }
                     }
                else
                     {
                     strcpy (pszSQLState, SQLUDF_STATE_PRFLOCATE) ;
                     }
                if   ((rc = PRFClose (pscratchpad->pprf)) != RC_OK)
                     {
                     strcpy (pszSQLState, SQLUDF_STATE_PRFCLOSE) ;
                     }
                }
           else
                {
                strcpy (pszSQLState, SQLUDF_STATE_PRFOPEN) ;
                }
           break ;
      /* DB2 asks for rows */
      case SQLUDF_NORMAL_CALL:
           /* Pre-set OK */
           strcpy (pszSQLState, SQLUDF_STATE_OK) ;
           /* Go to next record */
           if   ((rc = DBFRECGo (pscratchpad->pdbf, pscratchpad->ulRow + 1)) != RC_OK)
                {
                if   (rc == RC_WRONGRECORD)
                     {
                     /* Yes, report EOF */
                     strcpy (pszSQLState, "02000") ;
                     }
                else
                     {
                     /* No, report error */
                     strcpy (pszSQLState, SQLUDF_STATE_DBFRECGO) ;
                     }
                }
           else
                {
                /* DB2 tells what columns in what order */
                for  (ul = 0; ul < psqludfdbinfo->numtfcol; ul++)
                     {
                     /* Loop Until error */
                     if   (strcmp (pszSQLState, SQLUDF_STATE_OK) != 0)
                          {
                          break ;
                          }
                     /* Is it not an empty column? */
                     if   (psqludfdbinfo->tfcolumn [ul])
                          {
                          /* Yes, make it easier */
                          ulColumn = psqludfdbinfo->tfcolumn [ul] ;
                          /* Query Length of column */
                          if   ((rc = DBFFLDNLength (pscratchpad->pdbf, ulColumn, &ulLength)) == RC_OK)
                               {
                               /* Query type of column */
                               if   ((rc = DBFFLDNType (pscratchpad->pdbf, ulColumn, &chType)) == RC_OK)
                                    {
                                    /* What type? */
                                    switch (chType)
                                       {
                                       /* Character */
                                       case FIELD_CHARACTER:
                                            /* Copy contents into result buffer */
                                            if   ((rc = DBFFLDNGetCharacter (pscratchpad->pdbf, ulColumn, pscratchpad->papszResult [ulColumn - 1])) != RC_OK)
                                                 {
                                                 strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNGETCHARACTER) ;
                                                 }
                                            break ;
                                       /* Date */
                                       case FIELD_DATE:
                                            /* Query date */
                                            if   ((rc = DBFFLDNGetDate (pscratchpad->pdbf, ulColumn, &tdate2)) == RC_OK)
                                                 {
                                                 /* Convert to CHAR(12) value */
                                                 sprintf (pscratchpad->papszResult [ulColumn - 1], "%04u-%02u-%02u", tdate2.ulYear, tdate2.ulMonth, tdate2.ulDay) ;
                                                 }
                                            else
                                                 {
                                                 strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNGETDATE) ;
                                                 }
                                            break ;
                                       /* Logical */
                                       case FIELD_LOGICAL:
                                            /* Query Logical */
                                            if   ((rc = DBFFLDNGetLogical (pscratchpad->pdbf, ulColumn, &fLogical)) == RC_OK)
                                                 {
                                                 /* Convert to CHAR(1) value of Y or N */
                                                 pscratchpad->papszResult [ulColumn - 1] [0] = (fLogical) ? 'Y' : 'N' ;
                                                 pscratchpad->papszResult [ulColumn - 1] [1] = 0 ;
                                                 }
                                            else
                                                 {
                                                 strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNGETLOGICAL) ;
                                                 }
                                            break ;
                                       /* Memo */
                                       case FIELD_MEMO:
                                            /* Does a memo for this record exist */
                                            if   ((rc = DBFMEMNExist (pscratchpad->pdbf, ulColumn)) == RC_OK)
                                                 {
                                                 /* Query length of memo */
                                                 if   ((rc = DBFMEMNLength (pscratchpad->pdbf, ulColumn, &ulLength)) == RC_OK)
                                                      {
                                                      /* Read memo into result buffer */
                                                      rc = DBFMEMNRead (pscratchpad->pdbf, ulColumn, pscratchpad->papszResult [ulColumn - 1]) ;
                                                      }
                                                 }
                                            break ;
                                       /* Numeric */
                                       case FIELD_NUMERIC:
                                            /* Query numeric value */
                                            if   ((rc = DBFFLDNGetNumeric (pscratchpad->pdbf, ulColumn, &dNumeric)) == RC_OK)
                                                 {
                                                 /* Query decimal places */
                                                 if   ((rc = DBFFLDNDecimal (pscratchpad->pdbf, ulColumn, &ulDecimal)) == RC_OK)
                                                      {
                                                      /* If decimal places */
                                                      if   (ulDecimal)
                                                           {
                                                           /* Build VARCHAR value */
                                                           sprintf (pscratchpad->papszResult [ulColumn - 1], "%-f", dNumeric) ;
                                                           }
                                                      /* If no decimal places */
                                                      else
                                                           {
                                                           /* Return as INT */
                                                           *((unsigned long*) pscratchpad->papszResult [ulColumn - 1]) = dNumeric ;
                                                           }
                                                      }
                                                 else
                                                      {
                                                      strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNDECIMAL) ;
                                                      }
                                                 }
                                            else
                                                 {
                                                 strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNGETNUMERIC) ;
                                                 }
                                            break ;
                                       }
                                    }
                               else
                                    {
                                    strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNTYPE) ;
                                    }
                               }
                          else
                               {
                               strcpy (pszSQLState, SQLUDF_STATE_DBFFLDNLENGTH) ;
                               }
                          }
                     }
                /* Go to next row */
                pscratchpad->ulRow++ ;
                }
           break ;
      /* Ah, final call */
      case SQLUDF_FINAL_CALL:
           /* Pre-set OK */
           strcpy (pszSQLState, SQLUDF_STATE_OK) ;
           /* Remove result pointers */
           if   (pscratchpad->papszResult)
                {
                free (pscratchpad->papszResult) ;
                }
           /* Remove indicator pointers */
           if   (pscratchpad->pasqludfnullind)
                {
                free (pscratchpad->pasqludfnullind) ;
                }
           /* If DBF file open */
           if   (pscratchpad->pdbf)
                {
                /* Close it */
                if   ((rc = DBFClose (pscratchpad->pdbf)) != RC_OK)
                     {
                     strcpy (pszSQLState, SQLUDF_STATE_DBFCLOSE) ;
                     }
                }
           break ;
      }
   return ;
   }

/* Here starts DBASE stuff */
RC CheckPDBF (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   (pdbf)
        {
        if   (strcmp (pdbf->szFootprint, FOOTPRINT_DBF) == 0)
             {
             if   (!pdbf->pfile)
                  {
                  rc = RC_INVALIDFILEHANDLE ;
                  }
             }
        else
             {
             rc = RC_INVALIDDBF ;
             }
        }
   else
        {
        rc = RC_MISSINGPDBF ;
        }
   return rc ;
   }

RC CheckPDBFINDEX (PDBF pdbf, PDBFINDEX pdbfindex)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (pdbf->dbfind.ulNum)
             {
             if   (pdbfindex)
                  {
                  if   (strcmp (pdbfindex->szFootprint, FOOTPRINT_DBFIND) == 0)
                       {
                       if   (!pdbfindex->pfile)
                            {
                            rc = RC_INVALIDFILEHANDLE ;
                            }
                       }
                  else
                       {
                       rc = RC_INVALIDDBFINDEX ;
                       }
                  }
             else
                  {
                  rc = RC_MISSINGPDBFINDEX ;
                  }
             }
        else
             {
             rc = RC_INVALIDDBFINDEX ;
             }
        }
   return rc ;
   }

RC CheckPDBFMEM (PDBF pdbf)
   {
   char ach [4] ;
   RC   rc = RC_OK ;

   if   (fseek (pdbf->dbfmem.pfile, 0, SEEK_SET) == 0)
        {
        if   (fread (&pdbf->dbfmem.memheader, sizeof (MEMHEADER), 1, pdbf->dbfmem.pfile) != 0)
             {
             pdbf->dbfmem.ulLen = 512 ;
             if   (pdbf->dbfmem.memheader.ulNext > 1)
                  {
                  if   (fseek (pdbf->dbfmem.pfile, pdbf->dbfmem.ulLen, SEEK_SET) == 0)
                       {
                       if   (fread (ach, sizeof (ach), 1, pdbf->dbfmem.pfile) != 0)
                            {
                            if   ((ach [0] == 0xFF) && (ach [1] == 0xFF) && (ach [2] == 0x08) && (ach [3] == 0x00))
                                 {
                                 pdbf->dbfmem.fExtended = 1 ;
                                 pdbf->dbfmem.ulLen = pdbf->dbfmem.memheader.memheader4.usLenMemo ;
                                 }
                            }
                       else
                            {
                            rc = RC_FILEREAD ;
                            }
                       }
                  else
                       {
                       rc = RC_FILEPOSITION ;
                       }
                  }
             }
        else
             {
             rc = RC_FILEREAD ;
             }
        }
   else
        {
        rc = RC_FILEPOSITION ;
        }
   return rc ;
   }

RC ClearRecord (PDBF pdbf)
   {
   RC rc = RC_OK ;

   memset (pdbf->dbfrec.pszCache, ' ', pdbf->dbfheader.usLenRecord) ;
   return rc ;
   }

RC DBFClose (PDBF pdbf)
   {
   PDBFINDEX     pdbfindex ;
   RC            rc  ;
   time_t        timet ;
   struct tm*    tms ;
   unsigned long ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (pdbf->fChanged)
             {
             timet = time (NULL) ;
             tms = localtime (&timet) ;
             pdbf->dbfheader.tdate.chYear = tms->tm_year ;
             pdbf->dbfheader.tdate.chMonth = tms->tm_mon + 1 ;
             pdbf->dbfheader.tdate.chDay = tms->tm_mday ;
             if   (fseek (pdbf->pfile, 0, SEEK_SET) == 0)
                  {
                  if   (fwrite (&pdbf->dbfheader, sizeof (DBFHEADER), 1, pdbf->pfile) == 0)
                       {
                       rc = RC_FILEWRITE ;
                       }
                  }
             else
                  {
                  rc = RC_FILEPOSITION ;
                  }
             }
        if   (pdbf->dbffld.pdbffield)
             {
             free (pdbf->dbffld.pdbffield) ;
             }
        if   (pdbf->dbfrec.pszCache)
             {
             free (pdbf->dbfrec.pszCache) ;
             }
        if   (pdbf->dbfrec.pszCacheOriginal)
             {
             free (pdbf->dbfrec.pszCacheOriginal) ;
             }
        if   (pdbf->dbfmem.pfile)
             {
             fclose (pdbf->dbfmem.pfile) ;
             }
        if   (pdbf->pfile)
             {
             fclose (pdbf->pfile) ;
             }
        if   (pdbf->dbfind.ulNum)
             {
             for  (ulI = 0, pdbfindex = pdbf->dbfind.pdbfindex; ulI < pdbf->dbfind.ulHighest; ulI++, pdbfindex++)
                  {
                  if   (pdbfindex->pfile)
                       {
                       fclose (pdbfindex->pfile) ;
                       }
                  }
             free (pdbf->dbfind.pdbfindex) ;
             }
        memset (pdbf, 0, sizeof (DBF)) ;
        free (pdbf) ;
        }
   return rc ;
   }

RC DBFCreate (PDBF* ppdbf, char* pszFilename, unsigned long ulFieldCount, PDBFCREATE pdbfcreate)
   {
   char          ch = HEADER_END ;
   char          ch2 = DATA_END ;
   DBFFIELD      dbffield ;
   PDBF          pdbf ;
   PDBFCREATE    pdbfcreate2 ;
   char*         psz ;
   RC            rc = RC_OK ;
   unsigned long fMemo ;
   unsigned long ulI ;

   if   (ppdbf)
        {
        if   ((pdbf = calloc (sizeof (DBF), 1)) != 0)
             {
             *ppdbf = pdbf ;
             if   ((pdbf->pfile = fopen (pszFilename, "wb")) != 0)
                  {
                  pdbf->fChanged = 1 ;
                  pdbf->dbfmem.ulLen = 512 ;
                  strcpy (pdbf->szFilename, pszFilename) ;
                  strcpy (pdbf->szFootprint, FOOTPRINT_DBF) ;
                  pdbf->dbfheader.chVersion = FILE_DB3 ;
                  pdbf->dbfheader.usLenHeader = (ulFieldCount * sizeof (DBFFIELD)) + sizeof (DBFHEADER) + 1 ;
                  if   (fwrite (&pdbf->dbfheader, sizeof (DBFHEADER), 1, pdbf->pfile) != 0)
                       {
                       fMemo = 0 ;
                       pdbf->dbfheader.usLenRecord = 1 ;
                       for  (ulI = 0, pdbfcreate2 = pdbfcreate; ulI < ulFieldCount; ulI++, pdbfcreate2++)
                            {
                            memset (&dbffield, 0, sizeof (DBFFIELD)) ;
                            strcpy (dbffield.szName, pdbfcreate2->szName) ;
                            strupr (dbffield.szName) ;
                            dbffield.chType = pdbfcreate2->chType ;
                            switch (dbffield.chType)
                               {
                               case FIELD_CHARACTER:
                                    if   (pdbfcreate2->chLength == 0 || pdbfcreate2->chLength > 250)
                                         {
                                         rc = RC_WRONGFIELDLENGTH ;
                                         }
                                    else
                                         {
                                         dbffield.chLength = pdbfcreate2->chLength ;
                                         }
                                    break ;
                               case FIELD_DATE:
                                    dbffield.chLength = 8 ;
                                    break ;
                               case FIELD_LOGICAL:
                                    dbffield.chLength = 1 ;
                                    break ;
                               case FIELD_MEMO:
                                    dbffield.chLength = 10 ;
                                    pdbf->dbfheader.chVersion = FILE_DB3MEMO ;
                                    fMemo = 1 ;
                                    break ;
                               case FIELD_NUMERIC:
                                    if   (pdbfcreate2->chLength == 0 || pdbfcreate2->chLength > 20 || pdbfcreate2->chLength < pdbfcreate2->chDecimal + 1)
                                         {
                                         rc = RC_WRONGFIELDLENGTH ;
                                         }
                                    else
                                         {
                                         dbffield.chLength = pdbfcreate2->chLength ;
                                         dbffield.chDecimal = pdbfcreate2->chDecimal ;
                                         }
                                    break ;
                               default:
                                    rc = RC_WRONGFIELDTYPE ;
                                    break ;
                               }
                            pdbf->dbfheader.usLenRecord += dbffield.chLength ;
                            if   (fwrite (&dbffield, sizeof (DBFFIELD), 1, pdbf->pfile) == 0)
                                 {
                                 rc = RC_FILEWRITE ;
                                 break ;
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (fwrite (&ch, 1, 1, pdbf->pfile) != 0)
                                 {
                                 if   (fwrite (&ch2, 1, 1, pdbf->pfile) != 0)
                                      {
                                      if   (fMemo)
                                           {
                                           strcpy (pdbf->dbfmem.szFilename, pszFilename) ;
                                           if   ((psz = strrchr (pdbf->dbfmem.szFilename, '.')) != 0)
                                                {
                                                strcpy ((psz + 1), "DBT") ;
                                                }
                                           if   ((pdbf->dbfmem.pfile = fopen (pdbf->dbfmem.szFilename, "wb")) != 0)
                                                {
                                                pdbf->dbfmem.memheader.ulNext = 1 ;
                                                if   (fwrite (&pdbf->dbfmem.memheader, sizeof (MEMHEADER), 1, pdbf->dbfmem.pfile) == 0)
                                                     {
                                                     rc = RC_FILEWRITE ;
                                                     }
                                                }
                                           else
                                                {
                                                rc = RC_FILECREATE ;
                                                }
                                           }
                                      if   ((rc = DBFClose (pdbf)) == RC_OK)
                                           {
                                           rc = DBFOpen (ppdbf, pszFilename) ;
                                           }
                                      }
                                 else
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            else
                                 {
                                 rc = RC_FILEWRITE ;
                                 }
                            }
                       }
                  else
                       {
                       rc = RC_FILEWRITE ;
                       }
                  }
             else
                  {
                  rc = RC_FILECREATE ;
                  }
             }
        else
             {
             rc = RC_MEMORYERROR ;
             }
        }
   return rc ;
   }

RC DBFFLDCount (PDBF pdbf, unsigned long* pul)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->dbffld.ulNum ;
        }
   return rc ;
   }

RC DBFFLDDecimal (PDBF pdbf, char* pszColumn, unsigned long* pulFieldDecimal)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNDecimal (pdbf, ulFieldNumber, pulFieldDecimal) ;
             }
        }
   return rc ;
   }

RC DBFFLDGetCharacter (PDBF pdbf, char* pszColumn, char* pszCharacter)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

RC DBFFLDGetDate (PDBF pdbf, char* pszColumn, PTDATE2 ptdate2)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

RC DBFFLDGetLogical (PDBF pdbf, char* pszColumn, unsigned long* pfLogical)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

RC DBFFLDGetNumeric (PDBF pdbf, char* pszColumn, double* pdNumeric)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

RC DBFFLDLength (PDBF pdbf, char* pszColumn, unsigned long* pulFieldLength)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             *pulFieldLength = (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength ;
             }
        }
   return rc ;
   }

RC DBFFLDName (PDBF pdbf, unsigned long ulFieldNumber, char* pszColumn)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             strcpy (pszColumn, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->szName) ;
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNDecimal (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pulFieldDecimal)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_NUMERIC)
                  {
                  *pulFieldDecimal = (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chDecimal ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNGetCharacter (PDBF pdbf, unsigned long ulFieldNumber, char* pszCharacter)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  memcpy (pszCharacter, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                  DBFUTLCut (pszCharacter, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNGetDate (PDBF pdbf, unsigned long ulFieldNumber, PTDATE2 ptdate2)
   {
   char sz [_MAX_PATH] ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, 8) ;
                  sz [8] = 0 ;
                  ptdate2->ulDay = 1 ;
                  ptdate2->ulMonth = 1 ;
                  ptdate2->ulYear = 1900 ;
                  sscanf (sz, " %4u%2u%2u", &ptdate2->ulYear, &ptdate2->ulMonth, &ptdate2->ulDay) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNGetLogical (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pfLogical)
   {
   char ch ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  ch = *((char*) (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress) ;
                  *pfLogical = (ch == LOGICAL_FALSE || ch == LOGICAL_FALSE2 || ch == LOGICAL_FALSE3 || ch == LOGICAL_FALSE4 || ch == ' ') ? 0 : 1 ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNGetNumeric (PDBF pdbf, unsigned long ulFieldNumber, double* pdNumeric)
   {
   char sz [_MAX_PATH] ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_NUMERIC)
                  {
                  memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                  *pdNumeric = 0 ;
                  sscanf (sz, " %lf", pdNumeric) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNLength (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pulFieldLength)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             *pulFieldLength = (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength ;
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNPutCharacter (PDBF pdbf, unsigned long ulFieldNumber, char* pszCharacter)
   {
   RC            rc ;
   unsigned long ul ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  ul = strlen (pszCharacter) ;
                  if   (ul <= (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength)
                       {
                       memcpy ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, pszCharacter, ul) ;
                       DBFUTLPad (pszCharacter, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                       }
                  else
                       {
                       rc = RC_WRONGFIELDLENGTH ;
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNPutDate (PDBF pdbf, unsigned long ulFieldNumber, PTDATE2 ptdate2)
   {
   char szValue [_MAX_PATH] ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  sprintf (szValue, "%04u%02u%02u", ptdate2->ulYear, ptdate2->ulMonth, ptdate2->ulDay) ;
                  memcpy ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, szValue, 8) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNPutLogical (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pfLogical)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  *((char*) (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress) = (*pfLogical) ? LOGICAL_TRUE4 : LOGICAL_FALSE4 ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNPutNumeric (PDBF pdbf, unsigned long ulFieldNumber, double* pdNumeric)
   {
   char szFormat [32] ;
   char szValue [_MAX_PATH] ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_NUMERIC || (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chDecimal)
                       {
                       sprintf (szFormat, "%% %u.%uf", (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chDecimal) ;
                       sprintf (szValue, szFormat, *pdNumeric) ;
                       }
                  else
                       {
                       sprintf (szFormat, "%% %uu", (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                       sprintf (szValue, szFormat, (unsigned long) *pdNumeric) ;
                       }
                  memcpy ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, szValue, strlen (szValue)) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNType (PDBF pdbf, unsigned long ulFieldNumber, char* pchFieldType)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             *pchFieldType = (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType ;
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFFLDNumber (PDBF pdbf, char* pszColumn, unsigned long* pulFieldNumber)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = FindField (pdbf, pszColumn, pulFieldNumber) ;
        }
   return rc ;
   }

RC DBFFLDPutCharacter (PDBF pdbf, char* pszColumn, char* pszCharacter)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

RC DBFFLDPutDate (PDBF pdbf, char* pszColumn, PTDATE2 ptdate2)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

RC DBFFLDPutLogical (PDBF pdbf, char* pszColumn, unsigned long* pfLogical)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

RC DBFFLDPutNumeric (PDBF pdbf, char* pszColumn, double* pdNumeric)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

RC DBFFLDType (PDBF pdbf, char* pszColumn, char* pchFieldType)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             *pchFieldType = (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType ;
             }
        }
   return rc ;
   }

RC DBFFlush (PDBF pdbf)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        fflush (pdbf->pfile) ;
        }
   return rc ;
   }

RC DBFINDBottom (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   (pdbf->dbfind.ulCurrent)
        {
        if   ((rc = CheckPDBFINDEX (pdbf, (pdbf->dbfind.pdbfindex + pdbf->dbfind.ulCurrent - 1))) == RC_OK)
             {
             }
        }
   else
        {
        rc = RC_NOCURRENTINDEX ;
        }
   return rc ;
   }

RC DBFINDClose (PDBF pdbf, PDBFINDEX pdbfindex)
   {
   RC            rc ;
   unsigned long ulIndexNumber ;

   if   ((rc = CheckPDBFINDEX (pdbf, pdbfindex)) == RC_OK)
        {
        if   ((rc = FindIndex (pdbf, pdbfindex, &ulIndexNumber)) == RC_OK)
             {
             fclose ((pdbf->dbfind.pdbfindex + ulIndexNumber - 1)->pfile) ;
             memset ((pdbf->dbfind.pdbfindex + ulIndexNumber - 1), 0, sizeof (DBFINDEX)) ;
             pdbf->dbfind.ulNum-- ;
             if   (!pdbf->dbfind.ulNum)
                  {
                  free (pdbf->dbfind.pdbfindex) ;
                  pdbf->dbfind.pdbfindex = 0 ;
                  }
             }
        }
   return rc ;
   }

RC DBFINDOpen (PDBF pdbf, char* pszFilename, PDBFINDEX* ppdbfindex)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (!pdbf->dbfind.ulHighest)
             {
             if   ((pdbf->dbfind.pdbfindex = malloc (sizeof (DBFINDEX))) == 0)
                  {
                  rc = RC_MEMORYERROR ;
                  }
             }
        else
             {
             if   ((pdbf->dbfind.pdbfindex = realloc (pdbf->dbfind.pdbfindex, (pdbf->dbfind.ulHighest + 1) * sizeof (DBFINDEX))) == 0)
                  {
                  rc = RC_MEMORYERROR ;
                  }
             }
        if   (rc == RC_OK)
             {
             memset ((pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest), 0, sizeof (DBFINDEX)) ;
             if   (((pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest)->pfile = fopen (pszFilename, "rb+")) != 0)
                  {
                  if   (fread (&(pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest)->ndxheader, sizeof (NDXHEADER), 1, (pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest)->pfile) == 0)
                       {
                       rc = RC_FILEREAD ;
                       }
                  }
             else
                  {
                  rc = RC_FILEOPEN ;
                  }
             }
        if   (rc == RC_OK)
             {
             strcpy ((pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest)->szFilename, pszFilename) ;
             strcpy ((pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest)->szFootprint, FOOTPRINT_DBFIND) ;
             *ppdbfindex = (pdbf->dbfind.pdbfindex + pdbf->dbfind.ulHighest) ;
             pdbf->dbfind.ulHighest++ ;
             pdbf->dbfind.ulNum++ ;
             pdbf->dbfind.ulCurrent = pdbf->dbfind.ulHighest ;
             }
        }
   return rc ;
   }

RC DBFINDTop (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   (pdbf->dbfind.ulCurrent)
        {
        if   ((rc = CheckPDBFINDEX (pdbf, (pdbf->dbfind.pdbfindex + pdbf->dbfind.ulCurrent - 1))) == RC_OK)
             {
             }
        }
   else
        {
        rc = RC_NOCURRENTINDEX ;
        }
   return rc ;
   }

RC DBFMEMDelete (PDBF pdbf, char* pszColumn)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNDelete (pdbf, ulFieldNumber) ;
             }
        }
   return rc ;
   }

RC DBFMEMExist (PDBF pdbf, char* pszColumn)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNExist (pdbf, ulFieldNumber) ;
             }
        }
   return rc ;
   }

RC DBFMEMLength (PDBF pdbf, char* pszColumn, unsigned long* pulFieldLength)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNLength (pdbf, ulFieldNumber, pulFieldLength) ;
             }
        }
   return rc ;
   }

RC DBFMEMNDelete (PDBF pdbf, unsigned long ulFieldNumber)
   {
   RC            rc ;
   unsigned long ulRecordNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindMemo (pdbf, ulFieldNumber, &ulRecordNumber)) == RC_OK)
             {
             memset ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, ' ', (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
             }
        }
   return rc ;
   }

RC DBFMEMNExist (PDBF pdbf, unsigned long ulFieldNumber)
   {
   RC            rc ;
   unsigned long ulRecordNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = FindMemo (pdbf, ulFieldNumber, &ulRecordNumber) ;
        }
   return rc ;
   }

RC DBFMEMNLength (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pulFieldLength)
   {
   char*         psz ;
   RC            rc ;
   unsigned long ul ;
   unsigned long ulI ;
   unsigned long ulRecordNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindMemo (pdbf, ulFieldNumber, &ulRecordNumber)) == RC_OK)
             {
             ul = ulRecordNumber * pdbf->dbfmem.ulLen ;
             if   (pdbf->dbfmem.fExtended)
                  {
                  ul += 8 ;
                  }
             if   (fseek (pdbf->dbfmem.pfile, ul, SEEK_SET) == 0)
                  {
                  if   ((psz = calloc (65535, 1)) != 0)
                       {
                       if   (fread (psz, 65535, 1, pdbf->dbfmem.pfile) == 0)
                            {
                            if   (!feof (pdbf->dbfmem.pfile))
                                 {
                                 rc = RC_FILEREAD ;
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            *pulFieldLength = 0 ;
                            for  (ulI = 0; ulI < 65534; ulI++)
                                 {
                                 if   ((*(psz + ulI) == 0x1A && *(psz + ulI + 1) == 0x1A) || !*(psz + ulI))
                                      {
                                      break ;
                                      }
                                 (*pulFieldLength)++ ;
                                 }
                            }
                       free (psz) ;
                       }
                  else
                       {
                       rc = RC_MEMORYERROR ;
                       }
                  }
             else
                  {
                  rc = RC_FILEPOSITION ;
                  }
             }
        }
   return rc ;
   }

RC DBFMEMNRead (PDBF pdbf, unsigned long ulFieldNumber, char* pszMemo)
   {
   char*         psz ;
   RC            rc ;
   unsigned long ul ;
   unsigned long ulI ;
   unsigned long ulRecordNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindMemo (pdbf, ulFieldNumber, &ulRecordNumber)) == RC_OK)
             {
             ul = ulRecordNumber * pdbf->dbfmem.ulLen ;
             if   (pdbf->dbfmem.fExtended)
                  {
                  ul += 8 ;
                  }
             if   (fseek (pdbf->dbfmem.pfile, ul, SEEK_SET) == 0)
                  {
                  if   ((psz = calloc (65535, 1)) != 0)
                       {
                       if   (fread (psz, 65535, 1, pdbf->dbfmem.pfile) == 0)
                            {
                            if   (!feof (pdbf->dbfmem.pfile))
                                 {
                                 rc = RC_FILEREAD ;
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            ul = 0 ;
                            for  (ulI = 0; ulI < 65534; ulI++)
                                 {
                                 if   ((*(psz + ulI) == 0x1A && *(psz + ulI + 1) == 0x1A) || !*(psz + ulI))
                                      {
                                      break ;
                                      }
                                 ul++ ;
                                 }
                            memcpy (pszMemo, psz, ul) ;
                            *(pszMemo + ul) = 0 ;
                            }
                       free (psz) ;
                       }
                  else
                       {
                       rc = RC_MEMORYERROR ;
                       }
                  }
             else
                  {
                  rc = RC_FILEPOSITION ;
                  }
             }
        }
   return rc ;
   }

RC DBFMEMNWrite (PDBF pdbf, unsigned long ulFieldNumber, char* pszMemo)
   {
   char          ch = 0 ;
   char          sz [4] ;
   double        dNumeric ;
   RC            rc ;
   unsigned long ul ;
   unsigned long ul2 ;
   unsigned long ul3 ;
   unsigned long ulI ;
   unsigned long ulLength ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  if   (fseek (pdbf->dbfmem.pfile, 0, SEEK_END) == 0)
                       {
                       dNumeric = pdbf->dbfmem.memheader.ulNext ;
                       ulLength = strlen (pszMemo) ;
                       if   (pdbf->dbfmem.fExtended)
                            {
                            ul3 = ulLength / pdbf->dbfmem.ulLen ;
                            if   (ulLength % pdbf->dbfmem.ulLen)
                                 {
                                 ul3++ ;
                                 }
                            }
                       else
                            {
                            ul = ulLength + 2 ;
                            ul3 = ul / pdbf->dbfmem.ulLen ;
                            if   (ul % pdbf->dbfmem.ulLen)
                                 {
                                 ul3++ ;
                                 }
                            }
                       pdbf->dbfmem.memheader.ulNext += ul3 ;
                       ul = ftell (pdbf->dbfmem.pfile) ;
                       ul2 = ul / pdbf->dbfmem.ulLen ;
                       ul2++ ;
                       if   (ul % pdbf->dbfmem.ulLen)
                            {
                            ul2 *= pdbf->dbfmem.ulLen ;
                            ul2 -= ul ;
                            for  (ulI = 0; ulI < ul2; ulI++)
                                 {
                                 if   (fwrite (&ch, 1, 1, pdbf->dbfmem.pfile) == 0)
                                      {
                                      rc = RC_FILEWRITE ;
                                      break ;
                                      }
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (pdbf->dbfmem.fExtended)
                                 {
                                 sz [0] = 0xFF ;
                                 sz [1] = 0xFF ;
                                 sz [2] = 0x08 ;
                                 sz [3] = 0x00 ;
                                 if   (fwrite (sz, 4, 1, pdbf->dbfmem.pfile) != 0)
                                      {
                                      if   (fwrite (&ulLength, sizeof (unsigned long), 1, pdbf->dbfmem.pfile) == 0)
                                           {
                                           rc = RC_FILEWRITE ;
                                           }
                                      }
                                 else
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (fwrite (pszMemo, ulLength, 1, pdbf->dbfmem.pfile) == 0)
                                 {
                                 rc = RC_FILEWRITE ;
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (!pdbf->dbfmem.fExtended)
                                 {
                                 sz [0] = 0x1A ;
                                 sz [1] = 0x1A ;
                                 if   (fwrite (sz, 2, 1, pdbf->dbfmem.pfile) == 0)
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (fseek (pdbf->dbfmem.pfile, 0, SEEK_SET) == 0)
                                 {
                                 if   (fwrite (&pdbf->dbfmem.memheader, sizeof (MEMHEADER), 1, pdbf->dbfmem.pfile) != 0)
                                      {
                                      rc = DBFFLDNPutNumeric (pdbf, ulFieldNumber, &dNumeric) ;
                                      }
                                 else
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            else
                                 {
                                 rc = RC_FILEPOSITION ;
                                 }
                            }
                       }
                  else
                       {
                       rc = RC_FILEPOSITION ;
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFMEMRead (PDBF pdbf, char* pszColumn, char* pszMemo)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNRead (pdbf, ulFieldNumber, pszMemo) ;
             }
        }
   return rc ;
   }

RC DBFMEMWrite (PDBF pdbf, char* pszColumn, char* pszMemo)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNWrite (pdbf, ulFieldNumber, pszMemo) ;
             }
        }
   return rc ;
   }

RC DBFOpen (PDBF* ppdbf, char* pszFilename)
   {
   char*         psz ;
   PDBF          pdbf ;
   RC            rc = RC_OK ;
   unsigned long fMemo ;
   unsigned long ulFieldPosition = 1 ;
   unsigned long ulI ;

   if   (ppdbf)
        {
        if   ((pdbf = calloc (sizeof (DBF), 1)) != 0)
             {
             *ppdbf = pdbf ;
             if   ((pdbf->pfile = fopen (pszFilename, "rb+")) != 0)
                  {
                  if   (fread (&pdbf->dbfheader, sizeof (DBFHEADER), 1, pdbf->pfile) != 0)
                       {
                       if   (pdbf->dbfheader.chVersion == FILE_DB3 || pdbf->dbfheader.chVersion == FILE_DB3MEMO)
                            {
                            pdbf->chType = pdbf->dbfheader.chVersion ;
                            pdbf->dbffld.ulNum = (pdbf->dbfheader.usLenHeader - 1 - sizeof (DBFHEADER)) / sizeof (DBFFIELD) ;
                            if   ((pdbf->dbffld.pdbffield = (PDBFFIELD) malloc (pdbf->dbffld.ulNum * sizeof (DBFFIELD))) != 0)
                                 {
                                 if   ((pdbf->dbfrec.pszCache = malloc (pdbf->dbfheader.usLenRecord)) != 0)
                                      {
                                      if   ((pdbf->dbfrec.pszCacheOriginal = malloc (pdbf->dbfheader.usLenRecord)) != 0)
                                           {
                                           if   (fread (pdbf->dbffld.pdbffield, sizeof (DBFFIELD), pdbf->dbffld.ulNum, pdbf->pfile) != 0)
                                                {
                                                fMemo = 0 ;
                                                for  (ulI = 0; ulI < pdbf->dbffld.ulNum; ulI++)
                                                     {
                                                     (pdbf->dbffld.pdbffield + ulI)->pvAddress = pdbf->dbfrec.pszCache + ulFieldPosition ;
                                                     ulFieldPosition += (pdbf->dbffld.pdbffield + ulI)->chLength ;
                                                     if   ((pdbf->dbffld.pdbffield + ulI)->chType == FIELD_MEMO)
                                                          {
                                                          fMemo = 1 ;
                                                          }
                                                     }
                                                if   (fMemo)
                                                     {
                                                     strcpy (pdbf->dbfmem.szFilename, pszFilename) ;
                                                     if   ((psz = strrchr (pdbf->dbfmem.szFilename, '.')) != 0)
                                                          {
                                                          strcpy ((psz + 1), "DBT") ;
                                                          }
                                                     if   ((pdbf->dbfmem.pfile = fopen (pdbf->dbfmem.szFilename, "rb+")) == 0)
                                                          {
                                                          rc = RC_FILEOPEN ;
                                                          }
                                                     rc = CheckPDBFMEM (pdbf) ;
                                                     }
                                                if   (rc == RC_OK)
                                                     {
                                                     strcpy (pdbf->szFilename, pszFilename) ;
                                                     strcpy (pdbf->szFootprint, FOOTPRINT_DBF) ;
                                                     if   (pdbf->dbfheader.ulNumRecords)
                                                          {
                                                          pdbf->dbfrec.ulCurrent = 1 ;
                                                          rc = DBFRECGo (pdbf, pdbf->dbfrec.ulCurrent) ;
                                                          }
                                                     else
                                                          {
                                                          pdbf->dbfrec.ulCurrent = 0 ;
                                                          }
                                                     }
                                                }
                                           else
                                                {
                                                rc = RC_FILEREAD ;
                                                }
                                           }
                                      else
                                           {
                                           rc = RC_MEMORYERROR ;
                                           }
                                      }
                                 else
                                      {
                                      rc = RC_MEMORYERROR ;
                                      }
                                 }
                            else
                                 {
                                 rc = RC_MEMORYERROR ;
                                 }
                            }
                       else
                            {
                            rc = RC_INVALIDFILE ;
                            }
                       }
                  else
                       {
                       rc = RC_FILEREAD ;
                       }
                  }
             else
                  {
                  rc = RC_FILEOPEN ;
                  }
             }
        else
             {
             rc = RC_MEMORYERROR ;
             }
        }
   return rc ;
   }

RC DBFPack (char* pszFilename)
   {
   char          ch = HEADER_END ;
   char          ch2 = DATA_END ;
   char          szFilename [_MAX_PATH] ;
   DBFHEADER     dbfheader ;
   FILE*         pfileIn ;
   FILE*         pfileOut ;
   void*         pv ;
   RC            rc = RC_OK ;
   unsigned long ulI ;
   unsigned long ulNum ;
   unsigned long ulNumRecords ;

   if   ((pfileIn = fopen (pszFilename, "rb+")) != 0)
        {
        strcpy (szFilename, tmpnam (NULL)) ;
        if   ((pfileOut = fopen (szFilename, "wb")) != 0)
             {
             if   ((pv = malloc (65535)) != 0)
                  {
                  if   (fseek (pfileIn, 0, SEEK_SET) == 0)
                       {
                       if   (fseek (pfileOut, 0, SEEK_SET) == 0)
                            {
                            if   (fread (&dbfheader, sizeof (DBFHEADER), 1, pfileIn) != 0)
                                 {
                                 ulNum = (dbfheader.usLenHeader - 1 - sizeof (DBFHEADER)) / sizeof (DBFFIELD) ;
                                 if   (fwrite (&dbfheader, sizeof (DBFHEADER), 1, pfileOut) != 0)
                                      {
                                      if   (fread (pv, sizeof (DBFFIELD), ulNum, pfileIn) != 0)
                                           {
                                           if   (fwrite (pv, sizeof (DBFFIELD), ulNum, pfileOut) != 0)
                                                {
                                                if   (fwrite (&ch, 1, 1, pfileOut) != 0)
                                                     {
                                                     if   (dbfheader.ulNumRecords)
                                                          {
                                                          ulNumRecords = dbfheader.ulNumRecords ;
                                                          dbfheader.ulNumRecords = 0 ;
                                                          for  (ulI = 1; ulI <= ulNumRecords; ulI++)
                                                               {
                                                               if   (fseek (pfileIn, dbfheader.usLenHeader + ((ulI - 1) * dbfheader.usLenRecord), SEEK_SET) == 0)
                                                                    {
                                                                    if   (fread (pv, dbfheader.usLenRecord, 1, pfileIn) != 0)
                                                                         {
                                                                         if   (*((char*) pv) == ' ')
                                                                              {
                                                                              if   (fwrite (pv, dbfheader.usLenRecord, 1, pfileOut) != 0)
                                                                                   {
                                                                                   dbfheader.ulNumRecords ++ ;
                                                                                   }
                                                                              else
                                                                                   {
                                                                                   rc = RC_FILEWRITE ;
                                                                                   break ;
                                                                                   }
                                                                              }
                                                                         }
                                                                    else
                                                                         {
                                                                         rc = RC_FILEREAD ;
                                                                         break ;
                                                                         }
                                                                    }
                                                               else
                                                                    {
                                                                    rc = RC_FILEPOSITION ;
                                                                    break ;
                                                                    }
                                                               }
                                                          if   (rc == RC_OK)
                                                               {
                                                               if   (fwrite (&ch2, 1, 1, pfileOut) != 0)
                                                                    {
                                                                    if   (fseek (pfileOut, 0, SEEK_SET) == 0)
                                                                         {
                                                                         if   (fwrite (&dbfheader, sizeof (DBFHEADER), 1, pfileOut) == 0)
                                                                              {
                                                                              rc = RC_FILEWRITE ;
                                                                              }
                                                                         }
                                                                    else
                                                                         {
                                                                         rc = RC_FILEPOSITION ;
                                                                         }
                                                                    }
                                                               else
                                                                    {
                                                                    rc = RC_FILEWRITE ;
                                                                    }
                                                               }
                                                          }
                                                     }
                                                else
                                                     {
                                                     rc = RC_FILEWRITE ;
                                                     }
                                                }
                                           else
                                                {
                                                rc = RC_FILEWRITE ;
                                                }
                                           }
                                      else
                                           {
                                           rc = RC_FILEREAD ;
                                           }
                                      }
                                 else
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            else
                                 {
                                 rc = RC_FILEREAD ;
                                 }
                            }
                       else
                            {
                            rc = RC_FILEPOSITION ;
                            }
                       }
                  else
                       {
                       rc = RC_FILEPOSITION ;
                       }
                  free (pv) ;
                  }
             else
                  {
                  rc = RC_MEMORYERROR ;
                  }
             fclose (pfileIn) ;
             fclose (pfileOut) ;
             if   (rc == RC_OK)
                  {
                  if   ((pfileIn = fopen (szFilename, "rb+")) != 0)
                       {
                       if   ((pfileOut = fopen (pszFilename, "wb")) != 0)
                            {
                            if   ((pv = malloc (65535)) != 0)
                                 {
                                 while (1)
                                    {
                                    memset (pv, 0, 65536) ;
                                    ulI = fread (pv, 1, 32768, pfileIn) ;
                                    fwrite (pv, 1, ulI, pfileOut) ;
                                    if   (ulI < 32768)
                                         {
                                         break ;
                                         }
                                    }
                                 free (pv) ;
                                 }
                            else
                                 {
                                 rc = RC_MEMORYERROR ;
                                 }
                            fclose (pfileOut) ;
                            }
                       else
                            {
                            rc = RC_FILECREATE ;
                            }
                       fclose (pfileIn) ;
                       }
                  else
                       {
                       rc = RC_FILEOPEN ;
                       }
                  }
             remove (szFilename) ;
             }
        else
             {
             fclose (pfileIn) ;
             rc = RC_FILECREATE ;
             }
        }
   else
        {
        rc = RC_FILEOPEN ;
        }
   return rc ;
   }

RC DBFRECAppend (PDBF pdbf, unsigned long* pulRecord)
   {
   char ch = DATA_END ;
   RC   rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ClearRecord (pdbf)) == RC_OK)
             {
             if   (fseek (pdbf->pfile, -sizeof (ch), SEEK_END) == 0)
                  {
                  if   (fwrite (pdbf->dbfrec.pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                       {
                       if   (fwrite (&ch, sizeof (ch), 1, pdbf->pfile) != 0)
                            {
                            pdbf->fChanged = 1 ;
                            pdbf->dbfheader.ulNumRecords++ ;
                            *pulRecord = pdbf->dbfheader.ulNumRecords ;
                            rc = DBFRECGo (pdbf, pdbf->dbfheader.ulNumRecords) ;
                            }
                       else
                            {
                            rc = RC_FILEWRITE ;
                            }
                       }
                  else
                       {
                       rc = RC_FILEWRITE ;
                       }
                  }
             else
                  {
                  rc = RC_FILEPOSITION ;
                  }
             }
        }
   return rc ;
   }

RC DBFRECBottom (PDBF pdbf)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = DBFRECGo (pdbf, pdbf->dbfheader.ulNumRecords) ;
        }
   return rc ;
   }

RC DBFRECCount (PDBF pdbf, unsigned long* pul)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->dbfheader.ulNumRecords ;
        }
   return rc ;
   }

RC DBFRECDelete (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ReadRecord (pdbf, ulRecord)) == RC_OK)
             {
             memcpy (pdbf->dbfrec.pszCache, pdbf->dbfrec.pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
             *pdbf->dbfrec.pszCache = RECORDDELETED ;
             rc = WriteRecord (pdbf, ulRecord) ;
             }
        }
   return rc ;
   }

RC DBFRECDeleted (PDBF pdbf)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = (*pdbf->dbfrec.pszCache == RECORDDELETED) ? RC_RECORDDELETED : RC_RECORDNOTDELETED ;
        }
   return rc ;
   }

RC DBFRECGo (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = ReadRecord (pdbf, ulRecord) ;
        }
   return rc ;
   }

RC DBFRECLocateCharacter (PDBF pdbf, char* pszColumn, char* pszCharacter)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

RC DBFRECLocateDate (PDBF pdbf, char* pszColumn, PTDATE2 ptdate2)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

RC DBFRECLocateLogical (PDBF pdbf, char* pszColumn, unsigned long* pfLogical)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

RC DBFRECLocateNumeric (PDBF pdbf, char* pszColumn, double* pdNumeric)
   {
   RC            rc ;
   unsigned long ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

RC DBFRECNLocateCharacter (PDBF pdbf, unsigned long ulFieldNumber, char* pszCharacter)
   {
   char          sz [_MAX_PATH] ;
   RC            rc ;
   unsigned long ulCount ;
   unsigned long ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                                      DBFUTLCut (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                                      if   (strcmp (sz, pszCharacter) == 0)
                                           {
                                           rc = RC_OK ;
                                           break ;
                                           }
                                      }
                                 }
                            else
                                 {
                                 break ;
                                 }
                            }
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFRECNLocateDate (PDBF pdbf, unsigned long ulFieldNumber, PTDATE2 ptdate2)
   {
   char          sz [_MAX_PATH] ;
   RC            rc ;
   TDATE2        tdate2 ;
   unsigned long ulCount ;
   unsigned long ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, 8) ;
                                      sz [8] = 0 ;
                                      sscanf (sz, " %4u%2u%2u", &tdate2.ulYear, &tdate2.ulMonth, &tdate2.ulDay) ;
                                      if   ((tdate2.ulDay == ptdate2->ulDay) &&
                                            (tdate2.ulMonth == ptdate2->ulMonth) &&
                                            (tdate2.ulYear == ptdate2->ulYear))
                                           {
                                           rc = RC_OK ;
                                           break ;
                                           }
                                      }
                                 }
                            else
                                 {
                                 break ;
                                 }
                            }
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFRECNLocateLogical (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pfLogical)
   {
   char          ch ;
   RC            rc ;
   unsigned long fLogical ;
   unsigned long ulCount ;
   unsigned long ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      ch = *((char*) (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress) ;
                                      fLogical = (ch == LOGICAL_FALSE || ch == LOGICAL_FALSE2 || ch == LOGICAL_FALSE3 || ch == LOGICAL_FALSE4 || ch == ' ') ? 0 : 1 ;
                                      if   (fLogical == *pfLogical)
                                           {
                                           rc = RC_OK ;
                                           break ;
                                           }
                                      }
                                 }
                            else
                                 {
                                 break ;
                                 }
                            }
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFRECNLocateNumeric (PDBF pdbf, unsigned long ulFieldNumber, double* pdNumeric)
   {
   char          sz [_MAX_PATH] ;
   double        dNumeric ;
   RC            rc ;
   unsigned long ulCount ;
   unsigned long ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
             {
             if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
                                      sz [(pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                                      dNumeric = 0 ;
                                      sscanf (sz, " %lf", &dNumeric) ;
                                      if   (dNumeric == *pdNumeric)
                                           {
                                           rc = RC_OK ;
                                           break ;
                                           }
                                      }
                                 }
                            else
                                 {
                                 break ;
                                 }
                            }
                       }
                  }
             else
                  {
                  rc = RC_WRONGFIELDTYPE ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC DBFRECNumber (PDBF pdbf, unsigned long* pul)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->dbfrec.ulCurrent ;
        }
   return rc ;
   }

RC DBFRECRecall (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ReadRecord (pdbf, ulRecord)) == RC_OK)
             {
             if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDDELETED)
                  {
                  memcpy (pdbf->dbfrec.pszCache, pdbf->dbfrec.pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
                  *pdbf->dbfrec.pszCache = ' ' ;
                  rc = WriteRecord (pdbf, ulRecord) ;
                  }
             }
        }
   return rc ;
   }

RC DBFRECRefresh (PDBF pdbf)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        memcpy (pdbf->dbfrec.pszCache, pdbf->dbfrec.pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
        }
   return rc ;
   }

RC DBFRECSkip (PDBF pdbf, long lRecords)
   {
   long          l ;
   RC            rc ;
   unsigned long f ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        f = 0 ;
        l = pdbf->dbfrec.ulCurrent ;
        if   (lRecords < 0)
             {
             l = pdbf->dbfrec.ulCurrent - abs (lRecords) ;
             if   (l < 1)
                  {
                  f = 1 ;
                  rc = DBFRECTop (pdbf) ;
                  }
             }
        else
             {
             if   (lRecords > 0)
                  {
                  l = pdbf->dbfrec.ulCurrent + abs (lRecords) ;
                  if   (l > pdbf->dbfheader.ulNumRecords)
                       {
                       f = 1 ;
                       rc = DBFRECBottom (pdbf) ;
                       }
                  }
             }
        if   (!f)
             {
             rc = DBFRECGo (pdbf, l) ;
             }
        }
   return rc ;
   }

RC DBFRECTop (PDBF pdbf)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = DBFRECGo (pdbf, 1) ;
        }
   return rc ;
   }

RC DBFRECWrite (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = WriteRecord (pdbf, ulRecord) ;
        }
   return rc ;
   }

RC DBFType (PDBF pdbf, char* pch)
   {
   RC rc ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pch = pdbf->chType ;
        }
   return rc ;
   }

RC DBFUTLCut (char* psz, unsigned long ulFieldLength)
   {
   RC   rc = RC_OK ;
   long lI ;

   if   (ulFieldLength > 0)
        {
        psz [ulFieldLength] = 0 ;
        lI = ulFieldLength - 1 ;
        while (psz [lI] == ' ' && lI >= 0)
           {
           psz [lI--] = 0 ;
           }
        }
   return rc ;
   }

RC DBFUTLPad (char* psz, unsigned long ulFieldLength)
   {
   RC   rc = RC_OK ;
   long lI ;

   if   (ulFieldLength > 0)
        {
        lI = ulFieldLength - 1 ;
        while ((psz [lI] == ' ' || psz [lI] == 0 ) && lI >= 0)
           {
           psz [lI--] = ' ' ;
           }
        }
   return rc ;
   }

RC DBFUTLSoundex (char* psz1, char* psz2)
   {
   char  ch1 ;
   char  ch2 ;
   char  szValue [] = "01230120022455012623010202" ;
   char* psz ;
   char* pszNew1 ;
   char* pszNew1Save ;
   char* pszNew2 ;
   char* pszNew2Save ;
   RC    rc = RC_STRINGSOUNDEXNOTOK ;

   if   (isascii (*psz1) && isascii (*psz2))
        {
        pszNew1 = calloc (2, strlen (psz1)) ;
        pszNew1Save = pszNew1 ;
        strupr (pszNew1) ;
        pszNew2 = calloc (2, strlen (psz2)) ;
        pszNew2Save = pszNew2 ;
        strupr (pszNew2) ;
        ch1 = *pszNew1++ ;
        psz = pszNew1 ;
        while (ch2 = *pszNew1++)
           {
           if   (isascii (ch2))
                {
                if   (szValue [ch2 - 'A'] != szValue [ch1 - 'A'])
                     {
                     if   (szValue [ch2 - 'A'] != '0')
                          {
                          ch1 = ch2 ;
                          *psz = szValue [ch2 - 'A'] ;
                          psz++ ;
                          }
                     }
                }
           }
        *psz = 0 ;
        ch1 = *pszNew2++ ;
        psz = pszNew2 ;
        while (ch2 = *pszNew2++)
           {
           if   (isascii (ch2))
                {
                if   (szValue [ch2 - 'A'] != szValue [ch1 - 'A'])
                     {
                     if   (szValue [ch2 - 'A'] != '0')
                          {
                          ch1 = ch2 ;
                          *psz = szValue [ch2 - 'A'] ;
                          psz++ ;
                          }
                     }
                }
           }
        *psz = 0 ;
        if   (strcmp (pszNew1Save, pszNew2Save) == 0)
             {
             rc = RC_STRINGSOUNDEXOK ;
             }
        free (pszNew1Save) ;
        free (pszNew2Save) ;
        }
   return rc ;
   }

RC FindField (PDBF pdbf, char* pszColumn, unsigned long* pulFieldNumber)
   {
   PDBFFIELD     pdbffield ;
   RC            rc = RC_OK ;
   unsigned long f = 0 ;
   unsigned long ulI ;

   if   (pszColumn && pulFieldNumber)
        {
        for  (ulI = 0, pdbffield = pdbf->dbffld.pdbffield; ulI < pdbf->dbffld.ulNum; ulI++, pdbffield++)
             {
             if   (stricmp (pdbffield->szName, pszColumn) == 0)
                  {
                  f = 1 ;
                  *pulFieldNumber = ulI + 1 ;
                  break ;
                  }
             }
        }
   if   (!f)
        {
        rc = RC_WRONGFIELD ;
        }
   return rc ;
   }

RC FindIndex (PDBF pdbf, PDBFINDEX pdbfindex, unsigned long* pulIndexNumber)
   {
   RC            rc = RC_OK ;
   unsigned long f = 0 ;
   unsigned long ulI ;

   if   (pdbfindex && pulIndexNumber)
        {
        for  (ulI = 0; ulI < pdbf->dbfind.ulHighest; ulI++)
             {
             if   (memcmp ((pdbf->dbfind.pdbfindex + ulI), pdbfindex, sizeof (DBFINDEX)) == 0)
                  {
                  f = 1 ;
                  *pulIndexNumber = ulI + 1 ;
                  break ;
                  }
             }
        }
   if   (!f)
        {
        rc = RC_WRONGINDEX ;
        }
   return rc ;
   }

RC FindMemo (PDBF pdbf, unsigned long ulFieldNumber, unsigned long* pulRecordNumber)
   {
   char   sz [_MAX_PATH] ;
   double dNumeric ;
   RC     rc = RC_OK ;

   if   (ulFieldNumber && pulRecordNumber && ulFieldNumber <= pdbf->dbffld.ulNum)
        {
        if   ((pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
             {
             memcpy (sz, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength) ;
             sz [(pdbf->dbffld.pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
             dNumeric = 0 ;
             sscanf (sz, " %lf", &dNumeric) ;
             if   (dNumeric)
                  {
                  *pulRecordNumber = dNumeric ;
                  }
             else
                  {
                  rc = RC_NOMEMO ;
                  }
             }
        else
             {
             rc = RC_WRONGFIELDTYPE ;
             }
        }
   else
        {
        rc = RC_WRONGFIELDNUMBER ;
        }
   return rc ;
   }

RC ReadRecord (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc = RC_OK ;

   if   (ulRecord > 0 && ulRecord <= pdbf->dbfheader.ulNumRecords)
        {
        if   (fseek (pdbf->pfile, pdbf->dbfheader.usLenHeader + ((ulRecord - 1) * pdbf->dbfheader.usLenRecord), SEEK_SET) == 0)
             {
             if   (fread (pdbf->dbfrec.pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                  {
                  pdbf->dbfrec.ulCurrent = ulRecord ;
                  memcpy (pdbf->dbfrec.pszCacheOriginal, pdbf->dbfrec.pszCache, pdbf->dbfheader.usLenRecord) ;
                  }
             else
                  {
                  rc = RC_FILEREAD ;
                  }
             }
        else
             {
             rc = RC_FILEPOSITION ;
             }
        }
   else
        {
        rc = RC_WRONGRECORD ;
        }
   return rc ;
   }

RC WriteRecord (PDBF pdbf, unsigned long ulRecord)
   {
   RC rc = RC_OK ;

   if   (ulRecord > 0 && ulRecord <= pdbf->dbfheader.ulNumRecords)
        {
        if   (fseek (pdbf->pfile, pdbf->dbfheader.usLenHeader + ((ulRecord - 1) * pdbf->dbfheader.usLenRecord), SEEK_SET) == 0)
             {
             if   (fwrite (pdbf->dbfrec.pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                  {
                  pdbf->fChanged = 1 ;
                  pdbf->dbfrec.ulCurrent = ulRecord ;
                  memcpy (pdbf->dbfrec.pszCacheOriginal, pdbf->dbfrec.pszCache, pdbf->dbfheader.usLenRecord) ;
                  }
             else
                  {
                  rc = RC_FILEWRITE ;
                  }
             }
        else
             {
             rc = RC_FILEPOSITION ;
             }
        }
   else
        {
        rc = RC_WRONGRECORD ;
        }
   return rc ;
   }

/* Here starts INI stuff */
RC CheckPPRF (PPRF pprf)
   {
   RC rc = RC_OK ;

   if   (pprf)
        {
        if   (strcmp (pprf->szFootprint, FOOTPRINT_PRF) == 0)
             {
             if   (pprf->prffil.pfile == NULL)
                  {
                  rc = RC_INVALIDFILEHANDLE ;
                  }
             }
        else
             {
             rc = RC_INVALIDPRF ;
             }
        }
   else
        {
        rc = RC_MISSINGPPRF ;
        }
   return rc ;
   }

void CutString (char* pszString)
   {
  long l ;

  l = strlen (pszString) - 1 ;
  if   (l >= 0)
       {
       while (isspace (pszString [l]) && l > 0)
          {
          pszString [l--] = '\0' ;
          }
       while (isspace (*pszString))
          {
          memmove (pszString, pszString + 1, strlen (pszString) - 1) ;
          }
       }
   }

RC FindApplication (PPRF pprf, char* pszApplication, unsigned long* pulApplication)
   {
   RC            rc = RC_APPNOTFOUND ;
   unsigned long ul ;
   unsigned long ulI ;
   unsigned long ulLength ;
   unsigned long ulMax ;

   ulLength = strlen (pszApplication) ;
   if   (pprf->prffil.ulNum)
        {
        for  (ulI = 0; ulI < pprf->prffil.ulNum; ulI++)
             {
             ul = strlen (pprf->prffil.pprfapp[ulI].szApplication) ;
             ulMax = (ulLength > ul) ? ulLength : ul ;
             if   (strnicmp (pszApplication, pprf->prffil.pprfapp[ulI].szApplication, ulMax) == 0)
                  {
                  *pulApplication = ulI + 1 ;
                  rc = RC_OK ;
                  break ;
                  }
             }
        }
   return rc ;
   }

RC FindTopic (PPRF pprf, char* pszApplication, char* pszTopic, unsigned long* pulApplication, unsigned long* pulTopic)
   {
   PPRFAPP       pprfapp ;
   RC            rc ;
   unsigned long ul ;
   unsigned long ulI ;
   unsigned long ulLength ;
   unsigned long ulMax ;

   if   ((rc = FindApplication (pprf, pszApplication, pulApplication)) == RC_OK)
        {
        rc = RC_TOPNOTFOUND ;
        pprfapp = &pprf->prffil.pprfapp [*pulApplication - 1] ;
        ulLength = strlen (pszTopic) ;
        if   (pprfapp->ulNum)
             {
             for  (ulI = 0; ulI < pprfapp->ulNum; ulI++)
                  {
                  ul = strlen (pprfapp->pprftop[ulI].szTopic) ;
                  ulMax = (ulLength > ul) ? ulLength : ul ;
                  if   (strnicmp (pszTopic, pprfapp->pprftop[ulI].szTopic, ulMax) == 0)
                       {
                       *pulTopic = ulI + 1 ;
                       rc = RC_OK ;
                       break ;
                       }
                  }
             }
        }
   return rc ;
   }

RC PRFAPPCount (PPRF pprf, unsigned long* pul)
   {
   RC rc ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        *pul = pprf->prffil.ulNum ;
        }
   return rc ;
   }

RC PRFAPPDelete (PPRF pprf, char* pszApplication)
   {
   RC            rc ;
   unsigned long ulApplication ;
   unsigned long ulI ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
             {
             if   (pprf->prffil.pprfapp[ulApplication - 1].ulNum)
                  {
                  free (pprf->prffil.pprfapp[ulApplication - 1].pprftop) ;
                  }
             if   (pprf->prffil.ulNum == 1)
                  {
                  free (pprf->prffil.pprfapp) ;
                  }
             else
                  {
                  if   (pprf->prffil.ulNum > ulApplication)
                       {
                       for  (ulI = ulApplication - 1; ulI < pprf->prffil.ulNum; ulI++)
                            {
                            memcpy (&pprf->prffil.pprfapp [ulI], &pprf->prffil.pprfapp [ulI + 1], sizeof (PRFAPP)) ;
                            }
                       }
                  pprf->prffil.pprfapp = realloc (pprf->prffil.pprfapp, (pprf->prffil.ulNum - 1) * sizeof (PRFAPP)) ;
                  }
             pprf->prffil.ulNum-- ;
             }
        }
   return rc ;
   }

RC PRFAPPGetActive (PPRF pprf, char* pszApplication, unsigned long* pfActive)
   {
   RC            rc ;
   unsigned long ulApplication ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
             {
             *pfActive = pprf->prffil.pprfapp[ulApplication - 1].fActive ;
             }
        }
   return rc ;
   }

RC PRFAPPNLocate (PPRF pprf, unsigned long ulApplication, PPRFAPP pprfapp)
   {
   RC rc ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   (ulApplication && ulApplication <= pprf->prffil.ulNum)
             {
             memcpy (pprfapp, &pprf->prffil.pprfapp [ulApplication - 1], sizeof (PRFAPP)) ;
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

RC PRFAPPPutActive (PPRF pprf, char* pszApplication, unsigned long fActive)
   {
   RC            rc ;
   unsigned long ulApplication ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
             {
             pprf->prffil.pprfapp[ulApplication - 1].fActive = fActive ;
             }
        }
   return rc ;
   }

RC PRFAPPReplace (PPRF pprf, PPRFAPP pprfapp, PRFTYPE prftype)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        switch (prftype)
           {
           case PRFTYPE_INSERT:
                if   ((rc = FindApplication (pprf, pprfapp->szApplication, &ulApplication)) != RC_OK)
                     {
                     pprf->prffil.ulNum++ ;
                     if   (pprf->prffil.ulNum == 1)
                          {
                          pprf->prffil.pprfapp = malloc (sizeof (PRFAPP)) ;
                          }
                     else
                          {
                          pprf->prffil.pprfapp = realloc (pprf->prffil.pprfapp, pprf->prffil.ulNum * sizeof (PRFAPP)) ;
                          }
                     memcpy (&pprf->prffil.pprfapp [pprf->prffil.ulNum - 1], pprfapp, sizeof (PRFAPP)) ;
                     rc = RC_OK ;
                     }
                else
                     {
                     rc = RC_APPALREADYEXIST ;
                     }
                break ;
           case PRFTYPE_INSERTREPLACE:
                if   ((rc = FindApplication (pprf, pprfapp->szApplication, &ulApplication)) == RC_OK)
                     {
                     rc = PRFAPPReplace (pprf, pprfapp, PRFTYPE_REPLACE) ;
                     }
                else
                     {
                     rc = PRFAPPReplace (pprf, pprfapp, PRFTYPE_INSERT) ;
                     }
                break ;
           case PRFTYPE_REPLACE:
                if   ((rc = FindApplication (pprf, pprfapp->szApplication, &ulApplication)) == RC_OK)
                     {
                     memcpy (&pprf->prffil.pprfapp [ulApplication - 1], pprfapp, sizeof (PRFAPP)) ;
                     }
                break ;
           }
        }
   return rc ;
   }

RC PRFClose (PPRF pprf)
   {
   RC            rc ;
   unsigned long ulI ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((pprf->prffil.pfile = fopen (pprf->prffil.szFilename, "w")) != 0)
             {
             if   ((rc = WritePPRF (pprf)) != RC_OK)
                  {
                  rc = RC_FILEWRITE ;
                  }
             fclose (pprf->prffil.pfile) ;
             }
        if   (pprf->prffil.ulNum)
             {
             for  (ulI = 0; ulI < pprf->prffil.ulNum; ulI++)
                  {
                  if   (pprf->prffil.pprfapp[ulI].ulNum)
                       {
                       free (pprf->prffil.pprfapp[ulI].pprftop) ;
                       }
                  free (pprf->prffil.pprfapp) ;
                  }
             }
        memset (pprf, 0, sizeof (PRF)) ;
        free (pprf) ;
        }
   return rc ;
   }

RC PRFOpen (PPRF* ppprf, char* pszFilename)
   {
   PPRF pprf ;
   RC   rc = RC_OK ;

   if   (ppprf)
        {
        if   ((pprf = calloc (sizeof (PRF), 1)) != 0)
             {
             *ppprf = pprf ;
             strcpy (pprf->szFootprint, FOOTPRINT_PRF) ;
             strcpy (pprf->prffil.szFilename, pszFilename) ;
             if   ((pprf->prffil.pfile = fopen (pszFilename, "r")) != 0)
                  {
                  if   ((rc = ReadPPRF (pprf)) != RC_OK)
                       {
                       rc = RC_FILEREAD ;
                       }
                  fclose (pprf->prffil.pfile) ;
                  }
             }
        else
             {
             rc = RC_MEMORYERROR ;
             }
        }
   return rc ;
   }

RC PRFTOPCount (PPRF pprf, char* pszApplication, unsigned long* pul)
   {
   PPRFAPP       pprfapp ;
   RC            rc ;
   unsigned long ulApplication ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
             {
             pprfapp = &pprf->prffil.pprfapp [ulApplication - 1] ;
             *pul = pprfapp->ulNum ;
             }
        }
   return rc ;
   }

RC PRFTOPDelete (PPRF pprf, char* pszApplication, char* pszTopic)
   {
   PPRFAPP       pprfapp ;
   RC            rc ;
   unsigned long ulApplication ;
   unsigned long ulI ;
   unsigned long ulTopic ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pszTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             pprfapp = &pprf->prffil.pprfapp [ulApplication - 1] ;
             if   (pprfapp->ulNum == 1)
                  {
                  free (pprfapp->pprftop) ;
                  }
             else
                  {
                  if   (pprfapp->ulNum > ulTopic)
                       {
                       for  (ulI = ulTopic - 1; ulI < pprfapp->ulNum; ulI++)
                            {
                            memcpy (&pprfapp->pprftop [ulI], &pprfapp->pprftop [ulI + 1], sizeof (PRFTOP)) ;
                            }
                       }
                  pprfapp->pprftop = realloc (pprfapp->pprftop, (pprfapp->ulNum - 1) * sizeof (PRFTOP)) ;
                  }
             pprfapp->ulNum-- ;
             }
        }
   return rc ;
   }

RC PRFTOPGetActive (PPRF pprf, char* pszApplication, char* pszTopic, unsigned long* pfActive)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pszTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             *pfActive = pprf->prffil.pprfapp[ulApplication - 1].pprftop[ulTopic - 1].fActive ;
             }
        }
   return rc ;
   }

RC PRFTOPGetValue (PPRF pprf, char* pszApplication, char* pszTopic, char* pszValue)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pszTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             strcpy (pszValue, pprf->prffil.pprfapp[ulApplication - 1].pprftop[ulTopic - 1].szValue) ;
             }
        }
   return rc ;
   }

RC PRFTOPLocate (PPRF pprf, char* pszApplication, PPRFTOP pprftop)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pprftop->szTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             memcpy (pprftop, &pprf->prffil.pprfapp[ulApplication - 1].pprftop [ulTopic - 1], sizeof (PRFTOP)) ;
             }
        }
   return rc ;
   }

RC PRFTOPNLocate (PPRF pprf, char* pszApplication, unsigned long ulTopic, PPRFTOP pprftop)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
             {
             if   (ulTopic && ulTopic <= pprf->prffil.pprfapp[ulApplication - 1].ulNum)
                  {
                  memcpy (pprftop, &pprf->prffil.pprfapp[ulApplication - 1].pprftop [ulTopic - 1], sizeof (PRFTOP)) ;
                  }
             else
                  {
                  rc = RC_WRONGFIELDNUMBER ;
                  }
             }
        }
   return rc ;
   }

RC PRFTOPPutActive (PPRF pprf, char* pszApplication, char* pszTopic, unsigned long fActive)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pszTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             pprf->prffil.pprfapp[ulApplication - 1].pprftop[ulTopic - 1].fActive = fActive ;
             }
        }
   return rc ;
   }

RC PRFTOPPutValue (PPRF pprf, char* pszApplication, char* pszTopic, char* pszValue)
   {
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        if   ((rc = FindTopic (pprf, pszApplication, pszTopic, &ulApplication, &ulTopic)) == RC_OK)
             {
             strcpy (pprf->prffil.pprfapp[ulApplication - 1].pprftop[ulTopic - 1].szValue, pszValue) ;
             }
        }
   return rc ;
   }

RC PRFTOPReplace (PPRF pprf, char* pszApplication, PPRFTOP pprftop, PRFTYPE prftype)
   {
   PPRFAPP       pprfapp ;
   PRFAPP        prfapp ;
   RC            rc ;
   unsigned long ulApplication = 0 ;
   unsigned long ulTopic = 0 ;

   if   ((rc = CheckPPRF (pprf)) == RC_OK)
        {
        switch (prftype)
           {
           case PRFTYPE_INSERT:
                if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) == RC_OK)
                     {
                     if   ((rc = FindTopic (pprf, pszApplication, pprftop->szTopic, &ulApplication, &ulTopic)) != RC_OK)
                          {
                          pprfapp = &pprf->prffil.pprfapp [ulApplication - 1] ;
                          pprfapp->ulNum++ ;
                          if   (pprfapp->ulNum == 1)
                               {
                               pprfapp->pprftop = malloc (sizeof (PRFTOP)) ;
                               }
                          else
                               {
                               pprfapp->pprftop = realloc (pprfapp->pprftop, pprfapp->ulNum * sizeof (PRFTOP)) ;
                               }
                          memcpy (&pprfapp->pprftop [pprfapp->ulNum - 1], pprftop, sizeof (PRFTOP)) ;
                          rc = RC_OK ;
                          }
                     else
                          {
                          rc = RC_TOPALREADYEXIST ;
                          }
                     }
                break ;
           case PRFTYPE_INSERTREPLACE:
                if   ((rc = FindApplication (pprf, pszApplication, &ulApplication)) != RC_OK)
                     {
                     memset (&prfapp, 0, sizeof (PRFAPP)) ;
                     strcpy (prfapp.szApplication, pszApplication) ;
                     rc = PRFAPPReplace (pprf, &prfapp, PRFTYPE_INSERT) ;
                     }
                if   (rc == RC_OK)
                     {
                     if   ((rc = FindTopic (pprf, pszApplication, pprftop->szTopic, &ulApplication, &ulTopic)) == RC_OK)
                          {
                          rc = PRFTOPReplace (pprf, pszApplication, pprftop, PRFTYPE_REPLACE) ;
                          }
                     else
                          {
                          rc = PRFTOPReplace (pprf, pszApplication, pprftop, PRFTYPE_INSERT) ;
                          }
                     }
                break ;
           case PRFTYPE_REPLACE:
                if   ((rc = FindTopic (pprf, pszApplication, pprftop->szTopic, &ulApplication, &ulTopic)) == RC_OK)
                     {
                     memcpy (&pprf->prffil.pprfapp[ulApplication - 1].pprftop [ulTopic - 1], pprftop, sizeof (PRFTOP)) ;
                     }
                break ;
           }
        }
   return rc ;
   }

RC ReadPPRF (PPRF pprf)
   {
   char   sz [2048] ;
   char*  psz ;
   char*  psz2 ;
   PRFAPP prfapp ;
   PRFTOP prftop ;
   RC     rc = RC_OK ;

   while (!feof (pprf->prffil.pfile))
      {
      if   (fgets (sz, sizeof (sz) - 1, pprf->prffil.pfile))
           {
           if   ((psz = strchr (sz, '[')) != 0)
                {
                memset (&prfapp, 0, sizeof (PRFAPP)) ;
                if   ((psz2 = strstr (sz, "/*")) != 0)
                     {
                     *psz2 = 0 ;
                     }
                if   ((psz2 = strstr (sz, "//")) != 0)
                     {
                     strcpy (prfapp.szComment, psz2 + 1) ;
                     CutString (prfapp.szComment) ;
                     *psz2 = 0 ;
                     }
                if   ((psz2 = strchr (sz, ']')) != 0)
                     {
                     *psz2 = 0 ;
                     }
                if   (*sz == ';' || *sz == '*')
                     {
                     prfapp.fActive = 0 ;
                     psz += 2 ;
                     }
                else
                     {
                     prfapp.fActive = 1 ;
                     psz += 1 ;
                     }
                strcpy (prfapp.szApplication, psz) ;
                CutString (prfapp.szApplication) ;
                if   ((rc = PRFAPPReplace (pprf, &prfapp, PRFTYPE_INSERTREPLACE)) != RC_OK)
                     {
                     break ;
                     }
                }
           else
                {
                if   (*prfapp.szApplication)
                     {
                     if   ((psz = strchr (sz, '=')) != 0)
                          {
                          memset (&prftop, 0, sizeof (PRFTOP)) ;
                          if   ((psz2 = strstr (sz, "/*")) != 0)
                               {
                               *psz2 = 0 ;
                               }
                          if   ((psz2 = strstr (sz, "//")) != 0)
                               {
                               strcpy (prftop.szComment, psz2 + 1) ;
                               CutString (prftop.szComment) ;
                               *psz2 = 0 ;
                               }
                          sscanf (sz, "%[^=]=%[^\n]\n", prftop.szTopic, prftop.szValue) ;
                          if   (*prftop.szTopic == ';' || *prftop.szTopic == '*')
                               {
                               prftop.fActive = 0 ;
                               strcpy (prftop.szTopic, &prftop.szTopic [1]) ;
                               }
                          else
                               {
                               prftop.fActive = 1 ;
                               }
                          CutString (prftop.szTopic) ;
                          CutString (prftop.szValue) ;
                          if   ((rc = PRFTOPReplace (pprf, prfapp.szApplication, &prftop, PRFTYPE_INSERTREPLACE)) != RC_OK)
                               {
                               break ;
                               }
                          }
                     }
                }
           }
      }
   return rc ;
   }

RC WritePPRF (PPRF pprf)
   {
   PPRFAPP       pprfapp ;
   PPRFTOP       pprftop ;
   RC            rc = RC_OK ;
   unsigned long ulI ;
   unsigned long ulJ ;

   if   (pprf->prffil.ulNum)
        {
        for  (ulI = 0; ulI < pprf->prffil.ulNum; ulI++)
             {
             pprfapp = &pprf->prffil.pprfapp [ulI] ;
             if   (ulI)
                  {
                  fprintf (pprf->prffil.pfile, "\n") ;
                  }
             if   (!pprfapp->fActive)
                  {
                  fprintf (pprf->prffil.pfile, ";") ;
                  }
             fprintf (pprf->prffil.pfile, "[%s]", pprfapp->szApplication) ;
             if   (*pprfapp->szComment)
                  {
                  fprintf (pprf->prffil.pfile, " // %s", pprfapp->szComment) ;
                  }
             fprintf (pprf->prffil.pfile, "\n") ;
             if   (pprfapp->ulNum)
                  {
                  for  (ulJ = 0; ulJ < pprfapp->ulNum; ulJ++)
                       {
                       pprftop = &pprfapp->pprftop [ulJ] ;
                       if   (!pprftop->fActive)
                            {
                            fprintf (pprf->prffil.pfile, ";") ;
                            }
                       fprintf (pprf->prffil.pfile, "%s=%s", pprftop->szTopic, pprftop->szValue) ;
                       if   (*pprftop->szComment)
                            {
                            fprintf (pprf->prffil.pfile, " // %s", pprftop->szComment) ;
                            }
                       fprintf (pprf->prffil.pfile, "\n") ;
                       }
                  }
             }
        }
   return rc ;
   }

