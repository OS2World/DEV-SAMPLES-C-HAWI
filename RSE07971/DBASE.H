/* $Header: D:\Projekte\Redaktion SE\DBase intim (2)\Source\RCS\DBASE.H 1.1 1997/04/22 19:18:53 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DBASE_H
   #define _DBASE_H

   #define FILE_DB3     0x03
   #define FILE_DB3MEMO 0x83
   #define FILE_DB4MEMO 0x8B
   #define FILE_FPMEMO  0xF5

   #define FIELD_CHARACTER 'C'
   #define FIELD_NUMERIC   'N'
   #define FIELD_LOGICAL   'L'
   #define FIELD_DATE      'D'
   #define FIELD_MEMO      'M'

   #define LOGICALFALSE  'f'
   #define LOGICALFALSE2 'F'
   #define LOGICALFALSE3 'n'
   #define LOGICALFALSE4 'N'
   #define LOGICALTRUE   't'
   #define LOGICALTRUE2  'T'
   #define LOGICALTRUE3  'y'
   #define LOGICALTRUE4  'Y'

   #define FOOTPRINT "DBASE"

   #define HEADEREND 0x0D
   #define DATAEND   0x1A

   #define RECORDDELETED '*'

   typedef enum
      {
      RC_OK = 0,
      RC_FILECOPY,
      RC_FILECREATE,
      RC_FILEOPEN,
      RC_FILEPOSITION,
      RC_FILEREAD,
      RC_FILEWRITE,
      RC_INVALIDDBF,
      RC_INVALIDFILE,
      RC_INVALIDFILEHANDLE,
      RC_MEMORYERROR,
      RC_MISSINGPDBF,
      RC_MISSINGFIELD,
      RC_NOMEMO,
      RC_NOTFOUND,
      RC_RECORDDELETED,
      RC_RECORDNOTDELETED,
      RC_STRINGSOUNDEXNOTOK,
      RC_STRINGSOUNDEXOK,
      RC_WRONGFIELD,
      RC_WRONGFIELDLENGTH,
      RC_WRONGFIELDNUMBER,
      RC_WRONGFIELDTYPE,
      RC_WRONGRECORD
      } RC ;

   #pragma pack(1)
   typedef struct
      {
      CHAR szName [11] ;
      CHAR chType ;
      CHAR chLength ;
      CHAR chDecimal ;
      } DBFCREATE, *PDBFCREATE ;

   typedef struct
      {
      CHAR   szName [11] ;
      CHAR   chType ;
      PVOID  pvAddress ;
      CHAR   chLength ;
      CHAR   chDecimal ;
      USHORT usDummy1 ;
      CHAR   chArea ;
      CHAR   achDummy2 [11] ;
      } DBFFIELD, *PDBFFIELD ;

   typedef struct
      {
      CHAR achDummy [20] ;
      } DBFHEADER3, *PDBFHEADER3 ;

   typedef struct
      {
      CHAR chDummy1 [2] ;
      CHAR chPendingTransaction ;
      CHAR chCryption ;
      CHAR achDummy [12] ;
      CHAR chMDX ;
      CHAR achDummy2 [3] ;
      } DBFHEADER4, *PDBFHEADER4 ;

   typedef struct
      {
      CHAR chYear ;
      CHAR chMonth ;
      CHAR chDay ;
      } TDATE, *PTDATE ;

   typedef struct
      {
      ULONG ulYear ;
      ULONG ulMonth ;
      ULONG ulDay ;
      } TDATE2, *PTDATE2 ;

   typedef struct
      {
      CHAR   chVersion ;
      TDATE  tdate ;
      ULONG  ulNumRecords ;
      USHORT usLenHeader ;
      USHORT usLenRecord ;
      union
         {
         DBFHEADER3 dbfheader3 ;
         DBFHEADER4 dbfheader4 ;
         } ;
      } DBFHEADER, *PDBFHEADER ;

   typedef struct
      {
      CHAR achDummy [496] ;
      } MEMHEADER3, *PMEMHEADER3 ;

   typedef struct
      {
      CHAR   chVersion ;
      CHAR   achDummy [3] ;
      USHORT usLenMemo ;
      CHAR   achDummy2 [490] ;
      } MEMHEADER4, *PMEMHEADER4 ;

   typedef struct
      {
      ULONG ulNext ;
      ULONG ulDummy ;
      CHAR  szFilename [8] ;
      union
         {
         MEMHEADER3 memheader3 ;
         MEMHEADER4 memheader4 ;
         } ;
      } MEMHEADER, *PMEMHEADER ;

   typedef struct
      {
      BOOL      fChanged ;
      BOOL      fExtendedMemo ;
      CHAR      chType ;
      CHAR      szFootprint [8] ;
      CHAR      szFilename [CCHMAXPATH] ;
      CHAR      szMemoFilename [CCHMAXPATH] ;
      DBFHEADER dbfheader ;
      FILE*     pfile ;
      FILE*     pfileMemo ;
      MEMHEADER memheader ;
      PDBFFIELD pdbffield ;
      PSZ       pszCache ;
      PSZ       pszCacheOriginal ;
      ULONG     ulCurrentRecord ;
      ULONG     ulLenMemo ;
      ULONG     ulNumFields ;
      } DBF, *PDBF ;
   #pragma pack()

   RC          CheckMemo (PDBF) ;
   RC          CheckPDBF (PDBF) ;
   RC          ClearRecord (PDBF) ;
   RC EXPENTRY DBFClose (PDBF) ;
   RC EXPENTRY DBFCreate (PDBF*, PSZ, ULONG, PDBFCREATE) ;
   RC EXPENTRY DBFFLDCount (PDBF, PULONG) ;
   RC EXPENTRY DBFFLDDecimal (PDBF, PSZ, PULONG) ;
   RC EXPENTRY DBFFLDGetCharacter (PDBF, PSZ, PSZ) ;
   RC EXPENTRY DBFFLDGetDate (PDBF, PSZ, PTDATE2) ;
   RC EXPENTRY DBFFLDGetLogical (PDBF, PSZ, PBOOL) ;
   RC EXPENTRY DBFFLDGetNumeric (PDBF, PSZ, double*) ;
   RC EXPENTRY DBFFLDLength (PDBF, PSZ, PULONG) ;
   RC EXPENTRY DBFFLDName (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFFLDNGetCharacter (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFFLDNGetDate (PDBF, ULONG, PTDATE2) ;
   RC EXPENTRY DBFFLDNGetLogical (PDBF, ULONG, PBOOL) ;
   RC EXPENTRY DBFFLDNGetNumeric (PDBF, ULONG, double*) ;
   RC EXPENTRY DBFFLDNPutCharacter (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFFLDNPutDate (PDBF, ULONG, PTDATE2) ;
   RC EXPENTRY DBFFLDNPutLogical (PDBF, ULONG, PBOOL) ;
   RC EXPENTRY DBFFLDNPutNumeric (PDBF, ULONG, double*) ;
   RC EXPENTRY DBFFLDNumber (PDBF, PSZ, PULONG) ;
   RC EXPENTRY DBFFLDPutCharacter (PDBF, PSZ, PSZ) ;
   RC EXPENTRY DBFFLDPutDate (PDBF, PSZ, PTDATE2) ;
   RC EXPENTRY DBFFLDPutLogical (PDBF, PSZ, PBOOL) ;
   RC EXPENTRY DBFFLDPutNumeric (PDBF, PSZ, double*) ;
   RC EXPENTRY DBFFLDType (PDBF, PSZ, PCHAR) ;
   RC EXPENTRY DBFFlush (PDBF) ;
   RC EXPENTRY DBFMEMDelete (PDBF, PSZ) ;
   RC EXPENTRY DBFMEMExist (PDBF, PSZ) ;
   RC EXPENTRY DBFMEMLength (PDBF, PSZ, PULONG) ;
   RC EXPENTRY DBFMEMRead (PDBF, PSZ, PSZ) ;
   RC EXPENTRY DBFMEMWrite (PDBF, PSZ, PSZ) ;
   RC EXPENTRY DBFMEMNDelete (PDBF, ULONG) ;
   RC EXPENTRY DBFMEMNExist (PDBF, ULONG) ;
   RC EXPENTRY DBFMEMNLength (PDBF, ULONG, PULONG) ;
   RC EXPENTRY DBFMEMNRead (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFMEMNWrite (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFOpen (PDBF*, PSZ) ;
   RC EXPENTRY DBFPack (PSZ) ;
   RC EXPENTRY DBFRECAppend (PDBF, PULONG) ;
   RC EXPENTRY DBFRECBottom (PDBF) ;
   RC EXPENTRY DBFRECCount (PDBF, PULONG) ;
   RC EXPENTRY DBFRECDelete (PDBF, ULONG) ;
   RC EXPENTRY DBFRECDeleted (PDBF) ;
   RC EXPENTRY DBFRECGo (PDBF, ULONG) ;
   RC EXPENTRY DBFRECLocateCharacter (PDBF, PSZ, PSZ) ;
   RC EXPENTRY DBFRECLocateDate (PDBF, PSZ, PTDATE2) ;
   RC EXPENTRY DBFRECLocateLogical (PDBF, PSZ, PBOOL) ;
   RC EXPENTRY DBFRECLocateNumeric (PDBF, PSZ, double*) ;
   RC EXPENTRY DBFRECNLocateCharacter (PDBF, ULONG, PSZ) ;
   RC EXPENTRY DBFRECNLocateDate (PDBF, ULONG, PTDATE2) ;
   RC EXPENTRY DBFRECNLocateLogical (PDBF, ULONG, PBOOL) ;
   RC EXPENTRY DBFRECNLocateNumeric (PDBF, ULONG, double*) ;
   RC EXPENTRY DBFRECNumber (PDBF, PULONG) ;
   RC EXPENTRY DBFRECRecall (PDBF, ULONG) ;
   RC EXPENTRY DBFRECRefresh (PDBF) ;
   RC EXPENTRY DBFRECSkip (PDBF, LONG) ;
   RC EXPENTRY DBFRECTop (PDBF) ;
   RC EXPENTRY DBFRECWrite (PDBF, ULONG) ;
   RC EXPENTRY DBFUTLCut (PSZ, ULONG) ;
   RC EXPENTRY DBFUTLPad (PSZ, ULONG) ;
   RC EXPENTRY DBFUTLSoundex (PSZ, PSZ) ;
   RC EXPENTRY DBFType (PDBF, PCHAR) ;
   RC          FindField (PDBF, PSZ, PULONG) ;
   RC          ReadRecord (PDBF, ULONG) ;
   RC          WriteRecord (PDBF, ULONG) ;

#endif /* _DBASE_H */

#ifdef __cplusplus
   }
#endif

