/* $Header: D:\Projekte\Redaktion SE\DBase intim\Source\RCS\DBASE.H 1.4 1997/04/21 13:48:20 HAWI Exp $ */

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
      CHAR   chDummy2 [11] ;
      } DBFFIELD, *PDBFFIELD ;

   typedef struct
      {
      CHAR chDummy [20] ;
      } DBFHEADER3, *PDBFHEADER3 ;

   typedef struct
      {
      CHAR  chDummy1 [2] ;
      CHAR  chPendingTransaction ;
      CHAR  chCryption ;
      CHAR  achDummy [12] ;
      CHAR  chMDX ;
      CHAR  chDummy2 [3] ;
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
      BOOL      fChanged ;
      CHAR      chType ;
      CHAR      szFootprint [8] ;
      CHAR      szFilename [CCHMAXPATH] ;
      DBFHEADER dbfheader ;
      FILE*     pfile ;
      PDBFFIELD pdbffield ;
      PVOID     pvCache ;
      PVOID     pvCacheOriginal ;
      ULONG     ulCurrentRecord ;
      ULONG     ulNumFields ;
      } DBF, *PDBF ;
   #pragma pack()

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
   RC          CheckPDBF (PDBF) ;
   RC          ClearRecord (PDBF) ;
   RC          FindPDBF (PDBF, PSZ, PULONG) ;
   RC          ReadPDBF (PDBF, ULONG) ;
   RC          WritePDBF (PDBF, ULONG) ;

#endif /* _DBASE_H */

#ifdef __cplusplus
   }
#endif

