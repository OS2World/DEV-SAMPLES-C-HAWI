/* $Header: D:\Projekte\Redaktion SE\DBase intim (2)\Source\RCS\DBASE.C 1.1 1997/04/22 19:19:06 HAWI Exp $ */

#define  INCL_BASE
#include <os2.h>

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "DBASE.H"

/* Query memo file information */
RC CheckMemo (PDBF pdbf)
   {
   BYTE ab [4] ;
   RC   rc = RC_OK ;

   if   (fseek (pdbf->pfileMemo, 0, SEEK_SET) == 0)
        {
        if   (fread (&pdbf->memheader, sizeof (MEMHEADER), 1, pdbf->pfileMemo) != 0)
             {
             pdbf->ulLenMemo = 512 ;
             if   (pdbf->memheader.ulNext > 1)
                  {
                  if   (fseek (pdbf->pfileMemo, pdbf->ulLenMemo, SEEK_SET) == 0)
                       {
                       if   (fread (ab, sizeof (ab), 1, pdbf->pfileMemo) != 0)
                            {
                            if   ((ab [0] == 0xFF) && (ab [1] == 0xFF) && (ab [2] == 0x08) && (ab [3] == 0x00))
                                 {
                                 pdbf->fExtendedMemo = TRUE ;
                                 pdbf->ulLenMemo = pdbf->memheader.memheader4.usLenMemo ;
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

/* Check the pdbf parameter */
RC CheckPDBF (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   (pdbf)
        {
        if   (strcmp (pdbf->szFootprint, FOOTPRINT) == 0)
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

/* Empty the record buffer */
RC ClearRecord (PDBF pdbf)
   {
   RC rc = RC_OK ;

   memset (pdbf->pszCache, ' ', pdbf->dbfheader.usLenRecord) ;
   return rc ;
   }

/* Close Database */
RC EXPENTRY DBFClose (PDBF pdbf)
   {
   RC         rc = RC_OK ;
   time_t     timet ;
   struct tm* tms ;

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
        if   (pdbf->pdbffield)
             {
             free (pdbf->pdbffield) ;
             }
        if   (pdbf->pszCache)
             {
             free (pdbf->pszCache) ;
             }
        if   (pdbf->pszCacheOriginal)
             {
             free (pdbf->pszCacheOriginal) ;
             }
        if   (pdbf->pfileMemo)
             {
             fclose (pdbf->pfileMemo) ;
             }
        if   (pdbf->pfile)
             {
             fclose (pdbf->pfile) ;
             }
        memset (pdbf, 0, sizeof (DBF)) ;
        free (pdbf) ;
        }
   return rc ;
   }

/* Create Datenbase */
RC EXPENTRY DBFCreate (PDBF* ppdbf, PSZ pszFilename, ULONG ulFieldCount, PDBFCREATE pdbfcreate)
   {
   BOOL       fMemo ;
   CHAR       ch = HEADEREND ;
   CHAR       ch2 = DATAEND ;
   DBFFIELD   dbffield ;
   PDBF       pdbf ;
   PDBFCREATE pdbfcreate2 ;
   PSZ        psz ;
   RC         rc = RC_OK ;
   ULONG      ulI ;

   if   (ppdbf)
        {
        if   ((pdbf = calloc (sizeof (DBF), 1)) != 0)
             {
             *ppdbf = pdbf ;
             if   ((pdbf->pfile = fopen (pszFilename, "wb")) != 0)
                  {
                  pdbf->fChanged = TRUE ;
                  pdbf->ulLenMemo = 512 ;
                  strcpy (pdbf->szFilename, pszFilename) ;
                  strcpy (pdbf->szFootprint, FOOTPRINT) ;
                  pdbf->dbfheader.chVersion = FILE_DB3 ;
                  pdbf->dbfheader.usLenHeader = (ulFieldCount * sizeof (DBFFIELD)) + sizeof (DBFHEADER) + 1 ;
                  if   (fwrite (&pdbf->dbfheader, sizeof (DBFHEADER), 1, pdbf->pfile) != 0)
                       {
                       fMemo = FALSE ;
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
                                    fMemo = TRUE ;
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
                                           strcpy (pdbf->szMemoFilename, pszFilename) ;
                                           if   ((psz = strrchr (pdbf->szMemoFilename, '.')) != 0)
                                                {
                                                strcpy ((psz + 1), "DBT") ;
                                                }
                                           if   ((pdbf->pfileMemo = fopen (pdbf->szMemoFilename, "wb")) != 0)
                                                {
                                                pdbf->memheader.ulNext = 1 ;
                                                if   (fwrite (&pdbf->memheader, sizeof (MEMHEADER), 1, pdbf->pfileMemo) == 0)
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

/* Query Number of Fields in Database */
RC EXPENTRY DBFFLDCount (PDBF pdbf, PULONG pul)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->ulNumFields ;
        }
   return rc ;
   }

/* Query Number of decimal digits for a named field */
RC EXPENTRY DBFFLDDecimal (PDBF pdbf, PSZ pszColumn, PULONG pulFieldDecimal)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             *pulFieldDecimal = (pdbf->pdbffield + ulFieldNumber - 1)->chDecimal ;
             }
        }
   return rc ;
   }

/* Get text content from a named field */
RC EXPENTRY DBFFLDGetCharacter (PDBF pdbf, PSZ pszColumn, PSZ pszCharacter)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

/* Get date content from a named field */
RC EXPENTRY DBFFLDGetDate (PDBF pdbf, PSZ pszColumn, PTDATE2 ptdate2)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

/* Get bool content from a named field */
RC EXPENTRY DBFFLDGetLogical (PDBF pdbf, PSZ pszColumn, PBOOL pfLogical)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

/* Get numeric content from a named field */
RC EXPENTRY DBFFLDGetNumeric (PDBF pdbf, PSZ pszColumn, double* pdNumeric)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNGetNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

/* Query field length of a named field */
RC EXPENTRY DBFFLDLength (PDBF pdbf, PSZ pszColumn, PULONG pulFieldLength)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             *pulFieldLength = (pdbf->pdbffield + ulFieldNumber - 1)->chLength ;
             }
        }
   return rc ;
   }

/* Query field name of an indexed field */
RC EXPENTRY DBFFLDName (PDBF pdbf, ULONG ulFieldNumber, PSZ pszColumn)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             strcpy (pszColumn, (pdbf->pdbffield + ulFieldNumber - 1)->szName) ;
             }
        else
             {
             rc = RC_WRONGFIELDNUMBER ;
             }
        }
   return rc ;
   }

/* Get text content from an indexed field */
RC EXPENTRY DBFFLDNGetCharacter (PDBF pdbf, ULONG ulFieldNumber, PSZ pszCharacter)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  memcpy (pszCharacter, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  DBFUTLCut (pszCharacter, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
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

/* Get date content from an indexed field */
RC EXPENTRY DBFFLDNGetDate (PDBF pdbf, ULONG ulFieldNumber, PTDATE2 ptdate2)
   {
   CHAR sz [CCHMAXPATH] ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, 8) ;
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

/* Get bool content from an indexed field */
RC EXPENTRY DBFFLDNGetLogical (PDBF pdbf, ULONG ulFieldNumber, PBOOL pfLogical)
   {
   CHAR ch ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  ch = *((PSZ) (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress) ;
                  *pfLogical = (ch == LOGICALFALSE || ch == LOGICALFALSE2 || ch == LOGICALFALSE3 || ch == LOGICALFALSE4 || ch == ' ') ? FALSE : TRUE ;
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

/* Get numeric content from an indexed field */
RC EXPENTRY DBFFLDNGetNumeric (PDBF pdbf, ULONG ulFieldNumber, double* pdNumeric)
   {
   CHAR sz [CCHMAXPATH] ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_NUMERIC)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
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

/* Put text value into an indexed field */
RC EXPENTRY DBFFLDNPutCharacter (PDBF pdbf, ULONG ulFieldNumber, PSZ pszCharacter)
   {
   RC    rc = RC_OK ;
   ULONG ul ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  ul = strlen (pszCharacter) ;
                  if   (ul <= (pdbf->pdbffield + ulFieldNumber - 1)->chLength)
                       {
                       memcpy ((pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, pszCharacter, ul) ;
                       DBFUTLPad (pszCharacter, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
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

/* Put date value into an indexed field */
RC EXPENTRY DBFFLDNPutDate (PDBF pdbf, ULONG ulFieldNumber, PTDATE2 ptdate2)
   {
   CHAR szValue [261] ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  sprintf (szValue, "%04u%02u%02u", ptdate2->ulYear, ptdate2->ulMonth, ptdate2->ulDay) ;
                  memcpy ((pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, szValue, 8) ;
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

/* Put bool value into an indexed field */
RC EXPENTRY DBFFLDNPutLogical (PDBF pdbf, ULONG ulFieldNumber, PBOOL pfLogical)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  *((PSZ) (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress) = (*pfLogical) ? LOGICALTRUE4 : LOGICALFALSE4 ;
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

/* Put numeric value into an indexed field */
RC EXPENTRY DBFFLDNPutNumeric (PDBF pdbf, ULONG ulFieldNumber, double* pdNumeric)
   {
   CHAR szFormat [32] ;
   CHAR szValue [261] ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_NUMERIC || (pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  if   ((pdbf->pdbffield + ulFieldNumber - 1)->chDecimal)
                       {
                       sprintf (szFormat, "%% %u.%uf", (pdbf->pdbffield + ulFieldNumber - 1)->chLength, (pdbf->pdbffield + ulFieldNumber - 1)->chDecimal) ;
                       sprintf (szValue, szFormat, *pdNumeric) ;
                       }
                  else
                       {
                       sprintf (szFormat, "%% %uu", (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                       sprintf (szValue, szFormat, (ULONG) *pdNumeric) ;
                       }
                  memcpy ((pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, szValue, strlen (szValue)) ;
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

/* Query field index of a named field */
RC EXPENTRY DBFFLDNumber (PDBF pdbf, PSZ pszColumn, PULONG pulFieldNumber)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = FindField (pdbf, pszColumn, pulFieldNumber) ;
        }
   return rc ;
   }

/* Put text value into a named field */
RC EXPENTRY DBFFLDPutCharacter (PDBF pdbf, PSZ pszColumn, PSZ pszCharacter)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

/* Put date value into a named field */
RC EXPENTRY DBFFLDPutDate (PDBF pdbf, PSZ pszColumn, PTDATE2 ptdate2)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

/* Put bool value into a named field */
RC EXPENTRY DBFFLDPutLogical (PDBF pdbf, PSZ pszColumn, PBOOL pfLogical)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

/* Put numeric value into a named field */
RC EXPENTRY DBFFLDPutNumeric (PDBF pdbf, PSZ pszColumn, double* pdNumeric)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFFLDNPutNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

/* Query field type of a named field */
RC EXPENTRY DBFFLDType (PDBF pdbf, PSZ pszColumn, PCHAR pchFieldType)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             *pchFieldType = (pdbf->pdbffield + ulFieldNumber - 1)->chType ;
             }
        }
   return rc ;
   }

/* Flush database */
RC EXPENTRY DBFFlush (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        fflush (pdbf->pfile) ;
        }
   return rc ;
   }

/* Delete a memo entry of a named field */
RC EXPENTRY DBFMEMDelete (PDBF pdbf, PSZ pszColumn)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNDelete (pdbf, ulFieldNumber) ;
             }
        }
   return rc ;
   }

/* Check for a memo entry of a named field */
RC EXPENTRY DBFMEMExist (PDBF pdbf, PSZ pszColumn)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNExist (pdbf, ulFieldNumber) ;
             }
        }
   return rc ;
   }

/* Query length of a memo entry of a named field */
RC EXPENTRY DBFMEMLength (PDBF pdbf, PSZ pszColumn, PULONG pulFieldLength)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNLength (pdbf, ulFieldNumber, pulFieldLength) ;
             }
        }
   return rc ;
   }

/* Read a memo entry of a named field */
RC EXPENTRY DBFMEMRead (PDBF pdbf, PSZ pszColumn, PSZ pszMemo)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNRead (pdbf, ulFieldNumber, pszMemo) ;
             }
        }
   return rc ;
   }

/* Write a memo entry of a named field */
RC EXPENTRY DBFMEMWrite (PDBF pdbf, PSZ pszColumn, PSZ pszMemo)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFMEMNWrite (pdbf, ulFieldNumber, pszMemo) ;
             }
        }
   return rc ;
   }

/* Delete a memo entry of an indexed field */
RC EXPENTRY DBFMEMNDelete (PDBF pdbf, ULONG ulFieldNumber)
   {
   CHAR   sz [CCHMAXPATH] ;
   double dNumeric ;
   RC     rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                  dNumeric = 0 ;
                  sscanf (sz, " %lf", &dNumeric) ;
                  if   (dNumeric)
                       {
                       memset ((pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, ' ', (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
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
        }
   return rc ;
   }

/* Check for a memo entry of an indexed field */
RC EXPENTRY DBFMEMNExist (PDBF pdbf, ULONG ulFieldNumber)
   {
   CHAR   sz [CCHMAXPATH] ;
   double dNumeric ;
   RC     rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                  dNumeric = 0 ;
                  sscanf (sz, " %lf", &dNumeric) ;
                  if   (!dNumeric)
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
        }
   return rc ;
   }

/* Query length of a memo entry of an indexed field */
RC EXPENTRY DBFMEMNLength (PDBF pdbf, ULONG ulFieldNumber, PULONG pulFieldLength)
   {
   CHAR   sz [CCHMAXPATH] ;
   double dNumeric ;
   PSZ    psz ;
   RC     rc = RC_OK ;
   ULONG  ul ;
   ULONG  ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                  dNumeric = 0 ;
                  sscanf (sz, " %lf", &dNumeric) ;
                  if   (dNumeric)
                       {
                       ul = dNumeric * pdbf->ulLenMemo ;
                       if   (pdbf->fExtendedMemo)
                            {
                            ul += 8 ;
                            }
                       if   (fseek (pdbf->pfileMemo, ul, SEEK_SET) == 0)
                            {
                            if   ((psz = calloc (65535, 1)) != 0)
                                 {
                                 if   (fread (psz, 65535, 1, pdbf->pfileMemo) == 0)
                                      {
                                      if   (!feof (pdbf->pfileMemo))
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
        }
   return rc ;
   }

/* Read a memo entry of an indexed field */
RC EXPENTRY DBFMEMNRead (PDBF pdbf, ULONG ulFieldNumber, PSZ pszMemo)
   {
   CHAR   sz [CCHMAXPATH] ;
   double dNumeric ;
   PSZ    psz ;
   RC     rc = RC_OK ;
   ULONG  ul ;
   ULONG  ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                  sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
                  dNumeric = 0 ;
                  sscanf (sz, " %lf", &dNumeric) ;
                  if   (dNumeric)
                       {
                       ul = dNumeric * pdbf->ulLenMemo ;
                       if   (pdbf->fExtendedMemo)
                            {
                            ul += 8 ;
                            }
                       if   (fseek (pdbf->pfileMemo, ul, SEEK_SET) == 0)
                            {
                            if   ((psz = calloc (65535, 1)) != 0)
                                 {
                                 if   (fread (psz, 65535, 1, pdbf->pfileMemo) == 0)
                                      {
                                      if   (!feof (pdbf->pfileMemo))
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
        }
   return rc ;
   }

/* Write a memo entry of an indexed field */
RC EXPENTRY DBFMEMNWrite (PDBF pdbf, ULONG ulFieldNumber, PSZ pszMemo)
   {
   CHAR   ch = 0 ;
   CHAR   sz [4] ;
   double dNumeric ;
   RC     rc = RC_OK ;
   ULONG  ul ;
   ULONG  ul2 ;
   ULONG  ul3 ;
   ULONG  ulI ;
   ULONG  ulLength ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_MEMO)
                  {
                  if   (fseek (pdbf->pfileMemo, 0, SEEK_END) == 0)
                       {
                       dNumeric = pdbf->memheader.ulNext ;
                       ulLength = strlen (pszMemo) ;
                       if   (pdbf->fExtendedMemo)
                            {
                            ul3 = ulLength / pdbf->ulLenMemo ;
                            if   (ulLength % pdbf->ulLenMemo)
                                 {
                                 ul3++ ;
                                 }
                            }
                       else
                            {
                            ul = ulLength + 2 ;
                            ul3 = ul / pdbf->ulLenMemo ;
                            if   (ul % pdbf->ulLenMemo)
                                 {
                                 ul3++ ;
                                 }
                            }
                       pdbf->memheader.ulNext += ul3 ;
                       ul = ftell (pdbf->pfileMemo) ;
                       ul2 = ul / pdbf->ulLenMemo ;
                       ul2++ ;
                       if   (ul % pdbf->ulLenMemo)
                            {
                            ul2 *= pdbf->ulLenMemo ;
                            ul2 -= ul ;
                            for  (ulI = 0; ulI < ul2; ulI++)
                                 {
                                 if   (fwrite (&ch, 1, 1, pdbf->pfileMemo) == 0)
                                      {
                                      rc = RC_FILEWRITE ;
                                      break ;
                                      }
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (pdbf->fExtendedMemo)
                                 {
                                 sz [0] = 0xFF ;
                                 sz [1] = 0xFF ;
                                 sz [2] = 0x08 ;
                                 sz [3] = 0x00 ;
                                 if   (fwrite (sz, 4, 1, pdbf->pfileMemo) != 0)
                                      {
                                      if   (fwrite (&ulLength, sizeof (ULONG), 1, pdbf->pfileMemo) == 0)
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
                            if   (fwrite (pszMemo, ulLength, 1, pdbf->pfileMemo) == 0)
                                 {
                                 rc = RC_FILEWRITE ;
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (!pdbf->fExtendedMemo)
                                 {
                                 sz [0] = 0x1A ;
                                 sz [1] = 0x1A ;
                                 if   (fwrite (sz, 2, 1, pdbf->pfileMemo) == 0)
                                      {
                                      rc = RC_FILEWRITE ;
                                      }
                                 }
                            }
                       if   (rc == RC_OK)
                            {
                            if   (fseek (pdbf->pfileMemo, 0, SEEK_SET) == 0)
                                 {
                                 if   (fwrite (&pdbf->memheader, sizeof (MEMHEADER), 1, pdbf->pfileMemo) != 0)
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

/* Open database */
RC EXPENTRY DBFOpen (PDBF* ppdbf, PSZ pszFilename)
   {
   BOOL  fMemo ;
   PDBF  pdbf ;
   PSZ   psz ;
   RC    rc = RC_OK ;
   ULONG ulFieldPosition = 1 ;
   ULONG ulI ;

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
                            pdbf->ulNumFields = (pdbf->dbfheader.usLenHeader - 1 - sizeof (DBFHEADER)) / sizeof (DBFFIELD) ;
                            if   ((pdbf->pdbffield = (PDBFFIELD) malloc (pdbf->ulNumFields * sizeof (DBFFIELD))) != 0)
                                 {
                                 if   ((pdbf->pszCache = malloc (pdbf->dbfheader.usLenRecord)) != 0)
                                      {
                                      if   ((pdbf->pszCacheOriginal = malloc (pdbf->dbfheader.usLenRecord)) != 0)
                                           {
                                           if   (fread (pdbf->pdbffield, sizeof (DBFFIELD), pdbf->ulNumFields, pdbf->pfile) != 0)
                                                {
                                                fMemo = FALSE ;
                                                for  (ulI = 0; ulI < pdbf->ulNumFields; ulI++)
                                                     {
                                                     (pdbf->pdbffield + ulI)->pvAddress = pdbf->pszCache + ulFieldPosition ;
                                                     ulFieldPosition += (pdbf->pdbffield + ulI)->chLength ;
                                                     if   ((pdbf->pdbffield + ulI)->chType == FIELD_MEMO)
                                                          {
                                                          fMemo = TRUE ;
                                                          }
                                                     }
                                                if   (fMemo)
                                                     {
                                                     strcpy (pdbf->szMemoFilename, pszFilename) ;
                                                     if   ((psz = strrchr (pdbf->szMemoFilename, '.')) != 0)
                                                          {
                                                          strcpy ((psz + 1), "DBT") ;
                                                          }
                                                     if   ((pdbf->pfileMemo = fopen (pdbf->szMemoFilename, "rb+")) == 0)
                                                          {
                                                          rc = RC_FILEOPEN ;
                                                          }
                                                     rc = CheckMemo (pdbf) ;
                                                     }
                                                if   (rc == RC_OK)
                                                     {
                                                     strcpy (pdbf->szFilename, pszFilename) ;
                                                     strcpy (pdbf->szFootprint, FOOTPRINT) ;
                                                     if   (pdbf->dbfheader.ulNumRecords)
                                                          {
                                                          pdbf->ulCurrentRecord = 1 ;
                                                          rc = DBFRECGo (pdbf, pdbf->ulCurrentRecord) ;
                                                          }
                                                     else
                                                          {
                                                          pdbf->ulCurrentRecord = 0 ;
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

/* Pack database */
RC EXPENTRY DBFPack (PSZ pszFilename)
   {
   CHAR      ch = HEADEREND ;
   CHAR      ch2 = DATAEND ;
   CHAR      szFilename [CCHMAXPATH] ;
   DBFHEADER dbfheader ;
   FILE*     pfileIn ;
   FILE*     pfileOut ;
   PVOID     pv ;
   RC        rc = RC_OK ;
   ULONG     ulI ;
   ULONG     ulNumFields ;
   ULONG     ulNumRecords ;

   if   ((pfileIn = fopen (pszFilename, "rb+")) != 0)
        {
        strcpy (szFilename,
                tmpnam (NULL)) ;
        if   ((pfileOut = fopen (szFilename,
                                 "wb")) != NULL)
             {
             if   ((pv = malloc (65535)) != 0)
                  {
                  if   (fseek (pfileIn, 0, SEEK_SET) == 0)
                       {
                       if   (fseek (pfileOut, 0, SEEK_SET) == 0)
                            {
                            if   (fread (&dbfheader, sizeof (DBFHEADER), 1, pfileIn) != 0)
                                 {
                                 ulNumFields = (dbfheader.usLenHeader - 1 - sizeof (DBFHEADER)) / sizeof (DBFFIELD) ;
                                 if   (fwrite (&dbfheader, sizeof (DBFHEADER), 1, pfileOut) != 0)
                                      {
                                      if   (fread (pv, sizeof (DBFFIELD), ulNumFields, pfileIn) != 0)
                                           {
                                           if   (fwrite (pv, sizeof (DBFFIELD), ulNumFields, pfileOut) != 0)
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
                                                                         if   (*((PCHAR) pv) == ' ')
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
                  if   (!DosCopy (szFilename,
                                  pszFilename,
                                  DCPY_EXISTING))
                       {
                       rc = RC_FILECOPY ;
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

/* Append empty record to database */
RC EXPENTRY DBFRECAppend (PDBF pdbf, PULONG pulRecord)
   {
   CHAR ch = DATAEND ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ClearRecord (pdbf)) == RC_OK)
             {
             if   (fseek (pdbf->pfile, -sizeof (ch), SEEK_END) == 0)
                  {
                  if   (fwrite (pdbf->pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                       {
                       if   (fwrite (&ch, sizeof (ch), 1, pdbf->pfile) != 0)
                            {
                            pdbf->fChanged = TRUE ;
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

/* Go to the last record */
RC EXPENTRY DBFRECBottom (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = DBFRECGo (pdbf, pdbf->dbfheader.ulNumRecords) ;
        }
   return rc ;
   }

/* Query count of record (includes deleted records) */
RC EXPENTRY DBFRECCount (PDBF pdbf, PULONG pul)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->dbfheader.ulNumRecords ;
        }
   return rc ;
   }

/* Delete the record with a given record number */
RC EXPENTRY DBFRECDelete (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ReadRecord (pdbf, ulRecord)) == RC_OK)
             {
             memcpy (pdbf->pszCache, pdbf->pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
             *pdbf->pszCache = RECORDDELETED ;
             rc = WriteRecord (pdbf, ulRecord) ;
             }
        }
   return rc ;
   }

/* Is the current record deleted? */
RC EXPENTRY DBFRECDeleted (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = (*pdbf->pszCache == RECORDDELETED) ? RC_RECORDDELETED : RC_RECORDNOTDELETED ;
        }
   return rc ;
   }

/* Go to a given record */
RC EXPENTRY DBFRECGo (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = ReadRecord (pdbf, ulRecord) ;
        }
   return rc ;
   }

/* Locate a text value in the named field within all records starting with the first record */
RC EXPENTRY DBFRECLocateCharacter (PDBF pdbf, PSZ pszColumn, PSZ pszCharacter)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateCharacter (pdbf, ulFieldNumber, pszCharacter) ;
             }
        }
   return rc ;
   }

/* Locate a date value in the named field within all records starting with the first record */
RC EXPENTRY DBFRECLocateDate (PDBF pdbf, PSZ pszColumn, PTDATE2 ptdate2)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateDate (pdbf, ulFieldNumber, ptdate2) ;
             }
        }
   return rc ;
   }

/* Locate a bool value in the named field within all records starting with the first record */
RC EXPENTRY DBFRECLocateLogical (PDBF pdbf, PSZ pszColumn, PBOOL pfLogical)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateLogical (pdbf, ulFieldNumber, pfLogical) ;
             }
        }
   return rc ;
   }

/* Locate a numeric value in the named field within all records starting with the first record */
RC EXPENTRY DBFRECLocateNumeric (PDBF pdbf, PSZ pszColumn, double* pdNumeric)
   {
   RC    rc = RC_OK ;
   ULONG ulFieldNumber ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = FindField (pdbf, pszColumn, &ulFieldNumber)) == RC_OK)
             {
             rc = DBFRECNLocateNumeric (pdbf, ulFieldNumber, pdNumeric) ;
             }
        }
   return rc ;
   }

/* Locate a text value in an indexed field within all records starting with the first record */
RC EXPENTRY DBFRECNLocateCharacter (PDBF pdbf, ULONG ulFieldNumber, PSZ pszCharacter)
   {
   CHAR  sz [261] ;
   RC    rc = RC_NOTFOUND ;
   ULONG ulCount ;
   ULONG ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_CHARACTER)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                                      DBFUTLCut (sz, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
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

/* Locate a date value in an indexed field within all records starting with the first record */
RC EXPENTRY DBFRECNLocateDate (PDBF pdbf, ULONG ulFieldNumber, PTDATE2 ptdate2)
   {
   CHAR   sz [CCHMAXPATH] ;
   TDATE2 tdate2 ;
   RC     rc = RC_NOTFOUND ;
   ULONG  ulCount ;
   ULONG  ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_DATE)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, 8) ;
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

/* Locate a bool value in an indexed field within all records starting with the first record */
RC EXPENTRY DBFRECNLocateLogical (PDBF pdbf, ULONG ulFieldNumber, PBOOL pfLogical)
   {
   BOOL  fLogical ;
   CHAR  ch ;
   RC    rc = RC_NOTFOUND ;
   ULONG ulCount ;
   ULONG ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      ch = *((PSZ) (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress) ;
                                      fLogical = (ch == LOGICALFALSE || ch == LOGICALFALSE2 || ch == LOGICALFALSE3 || ch == LOGICALFALSE4 || ch == ' ') ? FALSE : TRUE ;
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

/* Locate a numeric value in an indexed field within all records starting with the first record */
RC EXPENTRY DBFRECNLocateNumeric (PDBF pdbf, ULONG ulFieldNumber, double* pdNumeric)
   {
   CHAR   sz [CCHMAXPATH] ;
   double dNumeric ;
   RC     rc = RC_NOTFOUND ;
   ULONG  ulCount ;
   ULONG  ulI ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   (ulFieldNumber && ulFieldNumber <= pdbf->ulNumFields)
             {
             if   ((pdbf->pdbffield + ulFieldNumber - 1)->chType == FIELD_LOGICAL)
                  {
                  if   ((rc = DBFRECCount (pdbf, &ulCount)) == RC_OK)
                       {
                       for  (ulI = 1; ulI <= ulCount; ulI++)
                            {
                            if   ((rc = DBFRECGo (pdbf, ulI)) == RC_OK)
                                 {
                                 if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDNOTDELETED)
                                      {
                                      memcpy (sz, (pdbf->pdbffield + ulFieldNumber - 1)->pvAddress, (pdbf->pdbffield + ulFieldNumber - 1)->chLength) ;
                                      sz [(pdbf->pdbffield + ulFieldNumber - 1)->chLength] = 0 ;
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

/* Query record number of current record */
RC EXPENTRY DBFRECNumber (PDBF pdbf, PULONG pul)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pul = pdbf->ulCurrentRecord ;
        }
   return rc ;
   }

/* Undelete a previously deleted record with the given record number */
RC EXPENTRY DBFRECRecall (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        if   ((rc = ReadRecord (pdbf, ulRecord)) == RC_OK)
             {
             if   ((rc = DBFRECDeleted (pdbf)) == RC_RECORDDELETED)
                  {
                  memcpy (pdbf->pszCache, pdbf->pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
                  *pdbf->pszCache = ' ' ;
                  rc = WriteRecord (pdbf, ulRecord) ;
                  }
             }
        }
   return rc ;
   }

/* Undo changes to the current record, use original values */
RC EXPENTRY DBFRECRefresh (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        memcpy (pdbf->pszCache, pdbf->pszCacheOriginal, pdbf->dbfheader.usLenRecord) ;
        }
   return rc ;
   }

/* Relatively go to a next record. Positive values go forth, negative values go back */
RC EXPENTRY DBFRECSkip (PDBF pdbf, LONG lRecords)
   {
   LONG l ;
   RC   rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        l = pdbf->ulCurrentRecord ;
        if   (lRecords < 0)
             {
             l = pdbf->ulCurrentRecord - abs (lRecords) ;
             if   (l < 1)
                  {
                  l = 1 ;
                  }
             }
        else
             {
             if   (lRecords > 0)
                  {
                  l = pdbf->ulCurrentRecord + abs (lRecords) ;
                  if   (l > pdbf->dbfheader.ulNumRecords)
                       {
                       l = pdbf->dbfheader.ulNumRecords ;
                       }
                  }
             }
        rc = DBFRECGo (pdbf, l) ;
        }
   return rc ;
   }

/* Go to the first record */
RC EXPENTRY DBFRECTop (PDBF pdbf)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = DBFRECGo (pdbf, 1) ;
        }
   return rc ;
   }

/* Write the record changes for the given record number to disk */
RC EXPENTRY DBFRECWrite (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        rc = WriteRecord (pdbf, ulRecord) ;
        }
   return rc ;
   }

/* Query database type */
RC EXPENTRY DBFType (PDBF pdbf, PCHAR pch)
   {
   RC rc = RC_OK ;

   if   ((rc = CheckPDBF (pdbf)) == RC_OK)
        {
        *pch = pdbf->chType ;
        }
   return rc ;
   }

/* Cut trailing spaces from a text value */
RC EXPENTRY DBFUTLCut (PSZ psz, ULONG ulFieldLength)
   {
   RC    rc = RC_OK ;
   ULONG ulI ;

   ulI = ulFieldLength - 1 ;
   if   (ulI >= 0)
        {
        while (psz [ulI] == ' ' && ulI >= 0)
           {
           psz [ulI--] = 0 ;
           }
        }
   return rc ;
   }

/* Pad a text value with spaces to a given length */
RC EXPENTRY DBFUTLPad (PSZ psz, ULONG ulFieldLength)
   {
   RC    rc = RC_OK ;
   ULONG ulI ;

   ulI = ulFieldLength - 1 ;
   if   (ulI >= 0)
        {
        while ((psz [ulI] == ' ' || psz [ulI] == 0 ) && ulI >= 0)
           {
           psz [ulI--] = ' ' ;
           }
        }
   return rc ;
   }

/* Compare two text values with the soundex algorithm */
RC EXPENTRY DBFUTLSoundex (PSZ psz1, PSZ psz2)
   {
   CHAR ch1 ;
   CHAR ch2 ;
   CHAR szValue [] = "01230120022455012623010202" ;
   PSZ  psz ;
   PSZ  pszNew1 ;
   PSZ  pszNew1Save ;
   PSZ  pszNew2 ;
   PSZ  pszNew2Save ;
   RC   rc = RC_STRINGSOUNDEXNOTOK ;

   if   (isascii (*psz1) &&
         isascii (*psz2))
        {
        pszNew1 = calloc (2,
                          strlen (psz1)) ;
        pszNew1Save = pszNew1 ;
        strupr (pszNew1) ;
        pszNew2 = calloc (2,
                          strlen (psz2)) ;
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
        if   (strcmp (pszNew1Save,
                      pszNew2Save) == 0)
             {
             rc = RC_STRINGSOUNDEXOK ;
             }
        free (pszNew1Save) ;
        free (pszNew2Save) ;
        }
   return rc ;
   }

/* Query the index of a field with the help of it's name */
RC FindField (PDBF pdbf, PSZ pszColumn, PULONG pulFieldNumber)
   {
   BOOL      f = FALSE ;
   PDBFFIELD pdbffield ;
   RC        rc = RC_OK ;
   ULONG     ulI ;

   if   (pszColumn)
        {
        for  (ulI = 0, pdbffield = pdbf->pdbffield; ulI < pdbf->ulNumFields; ulI++, pdbffield++)
             {
             if   (stricmp (pdbffield->szName, pszColumn) == 0)
                  {
                  f = TRUE ;
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

/* Read a given record */
RC ReadRecord (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   (ulRecord > 0 && ulRecord <= pdbf->dbfheader.ulNumRecords)
        {
        if   (fseek (pdbf->pfile, pdbf->dbfheader.usLenHeader + ((ulRecord - 1) * pdbf->dbfheader.usLenRecord), SEEK_SET) == 0)
             {
             if   (fread (pdbf->pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                  {
                  pdbf->ulCurrentRecord = ulRecord ;
                  memcpy (pdbf->pszCacheOriginal, pdbf->pszCache, pdbf->dbfheader.usLenRecord) ;
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

/* Write a given record */
RC WriteRecord (PDBF pdbf, ULONG ulRecord)
   {
   RC rc = RC_OK ;

   if   (ulRecord > 0 && ulRecord <= pdbf->dbfheader.ulNumRecords)
        {
        if   (fseek (pdbf->pfile, pdbf->dbfheader.usLenHeader + ((ulRecord - 1) * pdbf->dbfheader.usLenRecord), SEEK_SET) == 0)
             {
             if   (fwrite (pdbf->pszCache, pdbf->dbfheader.usLenRecord, 1, pdbf->pfile) != 0)
                  {
                  pdbf->fChanged = TRUE ;
                  pdbf->ulCurrentRecord = ulRecord ;
                  memcpy (pdbf->pszCacheOriginal, pdbf->pszCache, pdbf->dbfheader.usLenRecord) ;
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

