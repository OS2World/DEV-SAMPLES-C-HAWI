/* $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\UDF.H 1.2 1998/01/19 01:37:25 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _UDF_H
   #define _UDF_H

   /* DBASE Stuff */

   #define FILE_DB3     0x03
   #define FILE_DB3MEMO 0x83
   #define FILE_DB4MEMO 0x8B
   #define FILE_FPMEMO  0xF5

   #define FIELD_CHARACTER 'C'
   #define FIELD_NUMERIC   'N'
   #define FIELD_LOGICAL   'L'
   #define FIELD_DATE      'D'
   #define FIELD_MEMO      'M'

   #define FOOTPRINT_DBF    "DBF"
   #define FOOTPRINT_DBFIND "DBFIND"

   #define INDEX_CHARACTER 0
   #define INDEX_NUMERIC   1

   #define INDEX_NDX 'D'
   #define INDEX_NTX 'T'

   #define LOGICAL_FALSE  'f'
   #define LOGICAL_FALSE2 'F'
   #define LOGICAL_FALSE3 'n'
   #define LOGICAL_FALSE4 'N'
   #define LOGICAL_TRUE   't'
   #define LOGICAL_TRUE2  'T'
   #define LOGICAL_TRUE3  'y'
   #define LOGICAL_TRUE4  'Y'

   #define HEADER_END 0x0D
   #define DATA_END   0x1A

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
      RC_INVALIDDBFINDEX,
      RC_INVALIDFILE,
      RC_INVALIDFILEHANDLE,
      RC_MEMORYERROR,
      RC_MISSINGFIELD,
      RC_MISSINGPDBF,
      RC_MISSINGPDBFINDEX,
      RC_NOCURRENTINDEX,
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
      RC_WRONGINDEX,
      RC_WRONGRECORD,

      RC_APPALREADYEXIST,
      RC_APPNOTFOUND,
      RC_INVALIDPRF,
      RC_MISSINGPPRF,
      RC_TOPALREADYEXIST,
      RC_TOPNOTFOUND
      } RC ;

   #pragma pack(1)
   typedef struct
      {
      char szName [11] ;
      char chType ;
      char chLength ;
      char chDecimal ;
      } DBFCREATE, *PDBFCREATE ;

   typedef struct
      {
      char           szName [11] ;
      char           chType ;
      void*          pvAddress ;
      char           chLength ;
      char           chDecimal ;
      unsigned short usDummy1 ;
      char           chArea ;
      char           achDummy2 [11] ;
      } DBFFIELD, *PDBFFIELD ;

   typedef struct
      {
      char achDummy [20] ;
      } DBFHEADER3, *PDBFHEADER3 ;

   typedef struct
      {
      char chDummy1 [2] ;
      char chPendingTransaction ;
      char chCryption ;
      char achDummy [12] ;
      char chMDX ;
      char achDummy2 [3] ;
      } DBFHEADER4, *PDBFHEADER4 ;

   typedef struct
      {
      char chYear ;
      char chMonth ;
      char chDay ;
      } TDATE, *PTDATE ;

   typedef struct
      {
      unsigned long ulYear ;
      unsigned long ulMonth ;
      unsigned long ulDay ;
      } TDATE2, *PTDATE2 ;

   typedef struct
      {
      char           chVersion ;
      TDATE          tdate ;
      unsigned long  ulNumRecords ;
      unsigned short usLenHeader ;
      unsigned short usLenRecord ;
      union
         {
         DBFHEADER3  dbfheader3 ;
         DBFHEADER4  dbfheader4 ;
         } ;
      } DBFHEADER, *PDBFHEADER ;

   typedef struct
      {
      char achDummy [496] ;
      } MEMHEADER3, *PMEMHEADER3 ;

   typedef struct
      {
      char           chVersion ;
      char           achDummy [3] ;
      unsigned short usLenMemo ;
      char           achDummy2 [490] ;
      } MEMHEADER4, *PMEMHEADER4 ;

   typedef struct
      {
      unsigned long ulNext ;
      unsigned long ulDummy ;
      char          szFilename [8] ;
      union
         {
         MEMHEADER3 memheader3 ;
         MEMHEADER4 memheader4 ;
         } ;
      } MEMHEADER, *PMEMHEADER ;

   typedef struct
      {
      unsigned long  ulRoot ;
      unsigned long  ulCount ;
      unsigned long  ulDummy ;
      unsigned short usLen ;
      unsigned short usMax ;
      unsigned short usType ;
      unsigned long  ulSize ;
      char           chDummy ;
      char           chUnique ;
      char           szName [488] ;
      } NDXHEADER, *PNDXHEADER ;

   typedef struct
      {
      unsigned long ulDummy ;
      } NTXHEADER, *PTDXHEADER ;

   typedef struct
      {
      unsigned long ulLeft ;
      unsigned long ulRecord ;
      char          szKey [1] ;
      } NDXKEY, *PNDXKEY ;

   typedef struct
      {
      unsigned long ulNum ;
      } NDXPAGE, *PNDXPAGE ;

   typedef struct
      {
      PDBFFIELD     pdbffield ;
      unsigned long ulNum ;
      } DBFFLD, *PDBFFLD ;

   typedef struct
      {
      unsigned long fExtended ;
      char          szFilename [261] ;
      FILE*         pfile ;
      MEMHEADER     memheader ;
      unsigned long ulLen ;
      } DBFMEM, *PDBFMEM ;

   typedef struct
      {
      char         szFilename [261] ;
      char         szFootprint [8] ;
      FILE*        pfile ;
      union
         {
         NDXHEADER ndxheader ;
         NTXHEADER ntxheader ;
         } ;
      } DBFINDEX, *PDBFINDEX ;

   typedef struct
      {
      PDBFINDEX     pdbfindex ;
      unsigned long ulCurrent ;
      unsigned long ulNum ;
      unsigned long ulHighest ;
      } DBFIND, *PDBFIND ;

   typedef struct
      {
      char*         pszCache ;
      char*         pszCacheOriginal ;
      unsigned long ulCurrent ;
      } DBFREC, *PDBFREC ;

   typedef struct
      {
      unsigned long fChanged ;
      char          chType ;
      char          szFilename [261] ;
      char          szFootprint [8] ;
      DBFHEADER     dbfheader ;
      DBFFLD        dbffld ;
      DBFIND        dbfind ;
      DBFMEM        dbfmem ;
      DBFREC        dbfrec ;
      FILE*         pfile ;
      } DBF, *PDBF ;
   #pragma pack()

   RC CheckPDBF (PDBF) ;
   RC CheckPDBFINDEX (PDBF, PDBFINDEX) ;
   RC CheckPDBFMEM (PDBF) ;
   RC ClearRecord (PDBF) ;
   RC DBFClose (PDBF) ;
   RC DBFCreate (PDBF*, char*, unsigned long, PDBFCREATE) ;
   RC DBFFLDCount (PDBF, unsigned long*) ;
   RC DBFFLDDecimal (PDBF, char*, unsigned long*) ;
   RC DBFFLDGetCharacter (PDBF, char*, char*) ;
   RC DBFFLDGetDate (PDBF, char*, PTDATE2) ;
   RC DBFFLDGetLogical (PDBF, char*, unsigned long*) ;
   RC DBFFLDGetNumeric (PDBF, char*, double*) ;
   RC DBFFLDLength (PDBF, char*, unsigned long*) ;
   RC DBFFLDName (PDBF, unsigned long, char*) ;
   RC DBFFLDNDecimal (PDBF, unsigned long, unsigned long*) ;
   RC DBFFLDNGetCharacter (PDBF, unsigned long, char*) ;
   RC DBFFLDNGetDate (PDBF, unsigned long, PTDATE2) ;
   RC DBFFLDNGetLogical (PDBF, unsigned long, unsigned long*) ;
   RC DBFFLDNGetNumeric (PDBF, unsigned long, double*) ;
   RC DBFFLDNLength (PDBF, unsigned long, unsigned long*) ;
   RC DBFFLDNPutCharacter (PDBF, unsigned long, char*) ;
   RC DBFFLDNPutDate (PDBF, unsigned long, PTDATE2) ;
   RC DBFFLDNPutLogical (PDBF, unsigned long, unsigned long*) ;
   RC DBFFLDNPutNumeric (PDBF, unsigned long, double*) ;
   RC DBFFLDNType (PDBF, unsigned long, char*) ;
   RC DBFFLDNumber (PDBF, char*, unsigned long*) ;
   RC DBFFLDPutCharacter (PDBF, char*, char*) ;
   RC DBFFLDPutDate (PDBF, char*, PTDATE2) ;
   RC DBFFLDPutLogical (PDBF, char*, unsigned long*) ;
   RC DBFFLDPutNumeric (PDBF, char*, double*) ;
   RC DBFFLDType (PDBF, char*, char*) ;
   RC DBFFlush (PDBF) ;
   RC DBFINDBottom (PDBF) ;
   RC DBFINDClose (PDBF, PDBFINDEX) ;
   RC DBFINDCreate (PDBF, char*, char*, PDBFINDEX*) ;
   RC DBFINDKey (PDBF, char*) ;
   RC DBFINDLength (PDBF, unsigned long*) ;
   RC DBFINDOpen (PDBF, char*, PDBFINDEX*) ;
   RC DBFINDReindex (PDBF) ;
   RC DBFINDSeekCharacter (PDBF, char*) ;
   RC DBFINDSeekDate (PDBF, PTDATE2) ;
   RC DBFINDSeekLogical (PDBF, unsigned long*) ;
   RC DBFINDSeekNumeric (PDBF, double*) ;
   RC DBFINDSet (PDBF, PDBFIND) ;
   RC DBFINDSkip (PDBF, long) ;
   RC DBFINDTop (PDBF) ;
   RC DBFINDType (PDBF) ;
   RC DBFMEMDelete (PDBF, char*) ;
   RC DBFMEMExist (PDBF, char*) ;
   RC DBFMEMLength (PDBF, char*, unsigned long*) ;
   RC DBFMEMNDelete (PDBF, unsigned long) ;
   RC DBFMEMNExist (PDBF, unsigned long) ;
   RC DBFMEMNLength (PDBF, unsigned long, unsigned long*) ;
   RC DBFMEMNRead (PDBF, unsigned long, char*) ;
   RC DBFMEMNWrite (PDBF, unsigned long, char*) ;
   RC DBFMEMRead (PDBF, char*, char*) ;
   RC DBFMEMWrite (PDBF, char*, char*) ;
   RC DBFOpen (PDBF*, char*) ;
   RC DBFPack (char*) ;
   RC DBFRECAppend (PDBF, unsigned long*) ;
   RC DBFRECBottom (PDBF) ;
   RC DBFRECCount (PDBF, unsigned long*) ;
   RC DBFRECDelete (PDBF, unsigned long) ;
   RC DBFRECDeleted (PDBF) ;
   RC DBFRECGo (PDBF, unsigned long) ;
   RC DBFRECLocateCharacter (PDBF, char*, char*) ;
   RC DBFRECLocateDate (PDBF, char*, PTDATE2) ;
   RC DBFRECLocateLogical (PDBF, char*, unsigned long*) ;
   RC DBFRECLocateNumeric (PDBF, char*, double*) ;
   RC DBFRECNLocateCharacter (PDBF, unsigned long, char*) ;
   RC DBFRECNLocateDate (PDBF, unsigned long, PTDATE2) ;
   RC DBFRECNLocateLogical (PDBF, unsigned long, unsigned long*) ;
   RC DBFRECNLocateNumeric (PDBF, unsigned long, double*) ;
   RC DBFRECNumber (PDBF, unsigned long*) ;
   RC DBFRECRecall (PDBF, unsigned long) ;
   RC DBFRECRefresh (PDBF) ;
   RC DBFRECSkip (PDBF, long) ;
   RC DBFRECTop (PDBF) ;
   RC DBFRECWrite (PDBF, unsigned long) ;
   RC DBFUTLCut (char*, unsigned long) ;
   RC DBFUTLPad (char*, unsigned long) ;
   RC DBFUTLSoundex (char*, char*) ;
   RC DBFType (PDBF, char*) ;
   RC FindField (PDBF, char*, unsigned long*) ;
   RC FindIndex (PDBF, PDBFINDEX, unsigned long*) ;
   RC FindMemo (PDBF, unsigned long, unsigned long*) ;
   RC ReadRecord (PDBF, unsigned long) ;
   RC WriteRecord (PDBF, unsigned long) ;

   /* PRF Stuff */

   #define FOOTPRINT_PRF "PRF"

   typedef struct
      {
      char          szComment [261] ;
      char          szTopic [261] ;
      char          szValue [261] ;
      unsigned long fActive ;
      } PRFTOP, *PPRFTOP ;

   typedef struct
      {
      char          szApplication [261] ;
      char          szComment [261] ;
      PPRFTOP       pprftop ;
      unsigned long fActive ;
      unsigned long ulNum ;
      } PRFAPP, *PPRFAPP ;

   typedef struct
      {
      char          szFilename [261] ;
      FILE*         pfile ;
      PPRFAPP       pprfapp ;
      unsigned long ulNum ;
      } PRFFIL, *PPRFFIL ;

   typedef struct
      {
      char   szFootprint [8] ;
      PRFFIL prffil ;
      } PRF, *PPRF ;

   typedef enum
      {
      PRFTYPE_INSERT = 0,
      PRFTYPE_INSERTREPLACE,
      PRFTYPE_REPLACE
      } PRFTYPE ;

   RC   CheckPPRF (PPRF) ;
   void CutString (char*) ;
   RC   FindApplication (PPRF, char*, unsigned long*) ;
   RC   FindTopic (PPRF, char*, char*, unsigned long*, unsigned long*) ;
   RC   PRFAPPCount (PPRF, unsigned long*) ;
   RC   PRFAPPDelete (PPRF, char*) ;
   RC   PRFAPPGetActive (PPRF, char*, unsigned long*) ;
   RC   PRFAPPNLocate (PPRF, unsigned long, PPRFAPP) ;
   RC   PRFAPPPutActive (PPRF, char*, unsigned long) ;
   RC   PRFAPPReplace (PPRF, PPRFAPP, PRFTYPE) ;
   RC   PRFClose (PPRF) ;
   RC   PRFOpen (PPRF*, char*) ;
   RC   PRFTOPCount (PPRF, char*, unsigned long*) ;
   RC   PRFTOPDelete (PPRF, char*, char*) ;
   RC   PRFTOPGetActive (PPRF, char*, char*, unsigned long*) ;
   RC   PRFTOPGetValue (PPRF, char*, char*, char*) ;
   RC   PRFTOPLocate (PPRF, char*, PPRFTOP) ;
   RC   PRFTOPNLocate (PPRF, char*, unsigned long, PPRFTOP) ;
   RC   PRFTOPPutActive (PPRF, char*, char*, unsigned long) ;
   RC   PRFTOPPutValue (PPRF, char*, char*, char*) ;
   RC   PRFTOPReplace (PPRF, char*, PPRFTOP, PRFTYPE) ;
   RC   ReadPPRF (PPRF) ;
   RC   WritePPRF (PPRF) ;

#endif /* _UDF_H */

#ifdef __cplusplus
   }
#endif

