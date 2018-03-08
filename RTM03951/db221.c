static unsigned char sqla_program_id[40] = 
{111,65,65,66,65,69,67,67,85,83,69,82,73,68,32,32,68,66,50,50,
49,32,32,32,65,65,73,111,76,86,67,76,48,32,32,32,32,32,32,32};


/* Operating System Control Parameters */
#ifndef SQL_API_RC
#define SQL_STRUCTURE struct
#define SQL_API_RC int
#define SQL_POINTER
#ifdef  SQL16TO32
#define SQL_API_FN far pascal _loadds
#else
#define SQL_API_FN _System
#endif
#endif

struct sqlca;
struct sqlda;
SQL_API_RC SQL_API_FN  sqlaaloc(unsigned short,
                                unsigned short,
                                unsigned short,
                                void *);
SQL_API_RC SQL_API_FN  sqlacall(unsigned short,
                                unsigned short,
                                unsigned short,
                                unsigned short,
                                void *);
SQL_API_RC SQL_API_FN  sqladloc(unsigned short,
                                void *);
SQL_API_RC SQL_API_FN  sqlasets(unsigned short,
                                void *,
                                void *);
SQL_API_RC SQL_API_FN  sqlasetv(unsigned short,
                                unsigned short,
                                unsigned short,
                                unsigned short,
                                void *,
                                void *,
                                void *);
SQL_API_RC SQL_API_FN  sqlastop(void *);
SQL_API_RC SQL_API_FN  sqlastrt(void *,
                                void *,
                                struct sqlca *);
SQL_API_RC SQL_API_FN  sqlausda(unsigned short,
                                struct sqlda *,
                                void *);
SQL_API_RC SQL_API_FN  sqlestrd_api(unsigned char *,
                                    unsigned char *,
                                    unsigned char,
                                    struct sqlca *);
SQL_API_RC SQL_API_FN  sqlestpd_api(struct sqlca *);
/* $History: DB221.SQC $ */
/*  */
/* *****************  Version 2  ***************** */
/* User: Hawi         Date: 2/21/95    Time: 9:10a */
/* Updated in $/Toolbox/GibMirDieDaten */
/* Aendern pulldown added. */
/* Aendern dialog added. */
/* Missing ulEbene switch added for INSERT and DELETE. */
/*  */
/* *****************  Version 1  ***************** */
/* User: Hawi         Date: 2/20/95    Time: 9:51p */
/* Created in $/Toolbox/GibMirDieDaten */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <sql.h>
#include <sqlcodes.h>
#include <sqlenv.h>

#include <stdlib.h>
#include <string.h>
      
#include "DB22.H"


/*
EXEC SQL INCLUDE sqlca ;
*/

/* SQL Communication Area - SQLCA - structures and constants */

#ifndef SQLCODE

/* SQL Communication Area - SQLCA */
SQL_STRUCTURE sqlca  {

   unsigned char  sqlcaid[8];    /* Eyecatcher = 'SQLCA   ' */
   long           sqlcabc;       /* SQLCA size = 136 bytes  */
   long           sqlcode;       /* SQL return code */
   short          sqlerrml;      /* Length for SQLERRMC */
   unsigned char  sqlerrmc[70];  /* Error message tokens */
   unsigned char  sqlerrp[8];    /* Diagnostic information */
   long           sqlerrd[6];    /* Diagnostic information */
   unsigned char  sqlwarn[11];   /* Warning flags */
   unsigned char  sqlstate[5];   /* SQLSTATE */
};

/* Size of SQLCA */
#define   SQLCA_SIZE     sizeof(struct sqlca)

#define   SQLCODE        sqlca.sqlcode
#define   SQLWARN0       sqlca.sqlwarn[0]
#define   SQLWARN1       sqlca.sqlwarn[1]
#define   SQLWARN2       sqlca.sqlwarn[2]
#define   SQLWARN3       sqlca.sqlwarn[3]
#define   SQLWARN4       sqlca.sqlwarn[4]
#define   SQLWARN5       sqlca.sqlwarn[5]
#define   SQLWARN6       sqlca.sqlwarn[6]
#define   SQLWARN7       sqlca.sqlwarn[7]
#define   SQLWARN8       sqlca.sqlwarn[8]
#define   SQLWARN9       sqlca.sqlwarn[9]
#define   SQLWARNA       sqlca.sqlwarn[10]

#endif

struct sqlca sqlca;



/* Hostvariablen */

/*
EXEC SQL BEGIN DECLARE SECTION;
*/

   char  SZADRNAME [65] ;
   char  SZADRTELEFON [65] ;
   char  SZADRVORNAME [65] ;
   long  ULADRKEY ;
   char  SZANSNAME [65] ;
   char  SZANSTELEFON [65] ;
   char  SZANSVORNAME [65] ;
   long  ULANSKEY ;
   long  ULCOUNT ;
   short IND ;

/*
EXEC SQL END DECLARE SECTION ;
*/


/* Trick zum Vermeiden der Umkopiererei von Anwendungsdaten in Hostvariablen */
#define SZADRNAME    pdbinfoReceive->dbadr.szName
#define SZADRTELEFON pdbinfoReceive->dbadr.szTelefon
#define SZADRVORNAME pdbinfoReceive->dbadr.szVorname
#define ULADRKEY     pdbinfoReceive->dbadr.ulKey
#define SZANSNAME    pdbinfoReceive->dbans.szName
#define SZANSTELEFON pdbinfoReceive->dbans.szTelefon
#define SZANSVORNAME pdbinfoReceive->dbans.szVorname
#define ULANSKEY     pdbinfoReceive->dbans.ulKey
#define ULCOUNT      pdbinfoReceive->ulEID

typedef ULONG (*PDXF) (MSG, MPARAM, MPARAM) ;

/* Handle des anfordernden Clients */
HWND hwnd_g = NULLHANDLE ;

/* Allokieren der Kommunikationsdaten */
PDBINFO EXPENTRY AllocInfo (VOID)
   {
   return calloc (1,
                  sizeof (DBINFO)) ;
   }

/* Freigeben der Kommunikationsdaten */
ULONG EXPENTRY FreeInfo (PDBINFO pdbinfo)
   {
   free (pdbinfo) ;
   return DBC_NOERROR ;
   }

/* Schliessen der Datenbank */
ULONG EXPENTRY CloseDatabase (VOID)
   {
   
/*
EXEC SQL CONNECT RESET ;
*/

{
  sqlestpd_api(&sqlca);
}

   return DBC_NOERROR ;
   }

/* Anlegen der Datenbank. Hier wird der eigentliche Anlegen Thread gestartet */
ULONG EXPENTRY CreateDatabase (CHAR ch)
   {
   ULONG ul ;

   ul = ch ;
   _beginthread (ThreadCreateDatabase,
                 NULL,
                 81919,
                 (PVOID) ul) ;
   return TRUE ;
   }

/* Lîschen der Datenbank */
ULONG EXPENTRY DropDatabase (VOID)
   {
   CloseDatabase () ;
   sqledrpd (DBK_DATENBANK,
             &sqlca) ;
   return (SQLCODE != SQL_RC_OK) ? DBC_ERROR : DBC_NOERROR ;
   }

/* ôffnen der Datenbank */
ULONG EXPENTRY OpenDatabase (VOID)
   {
   sqlestar () ;
   
/*
EXEC SQL CONNECT TO ADRESSE ;
*/

{
  sqlestrd_api("ADRESSE","",'S',&sqlca);
}

   return (SQLCODE != SQL_RC_OK) ? DBC_ERROR : DBC_NOERROR ;
   }

/* Start der DB Engine */
ULONG EXPENTRY Register (HWND hwnd)
   {
   hwnd_g = hwnd ;
   return _beginthread (ThreadWndProc,
                        NULL,
                        81919,
                        (PVOID) 0) ;
   }

/* Verarbeitung der ADR SQL Tabelle */
ULONG TableAdr (MSG    msg,
                MPARAM mp1,
                MPARAM mp2)
   {
   BOOL    fOpen ;
   PDBINFO pdbinfoReceive ;
   PDBINFO pdbinfoSend ;
   ULONG   ulCount ;

   /* Kommunikationsstruktur immer in MPARAM2 */
   pdbinfoReceive = PVOIDFROMMP (mp2) ;
   /* Was wird gewÅnscht? */
   switch (msg)
      {
      /* Lîschen */
      case WMPDB_DELETE:
           /* Was lîschen? */
           switch (LONGFROMMP (mp1))
              {
              /* Einen Eintrag */
              case DBK_DELETEPART:
                   
/*
EXEC SQL WHENEVER SQLERROR goto DelError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto DelError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto DelNotFound ;
*/

                   
/*
EXEC SQL DELETE FROM ADR WHERE ULKEY = :ULADRKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,1,0L);
    sqlasetv(1,0,496,4,&ULADRKEY,0L,0L);
  sqlacall((unsigned short)24,1,1,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto DelError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto DelError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto DelNotFound;
}

  sqlastop(0L);
}

                   
/*
EXEC SQL COMMIT WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)21,0,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto DelError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto DelError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto DelNotFound;
}

  sqlastop(0L);
}

                   pdbinfoReceive->ulRC = DBC_NOERROR ;
                   goto DelOk ;
                    DelError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto DelErrorEnd ;
                    DelNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    DelErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                    DelOk:
                   /* Ergebnis zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              }
           break ;
      /* EinfÅgen */
      case WMPDB_INSERT:
           /* Was einfÅgen? */
           switch (LONGFROMMP (mp1))
              {
              /* Einen Eintrag */
              case DBK_INSERTPART:
                   
/*
EXEC SQL WHENEVER SQLERROR goto InsError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto InsError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto InsNotFound ;
*/

                   /* FÅr diesen Moment Tabelle sperren */
                   
/*
EXEC SQL LOCK TABLE ADR IN EXCLUSIVE MODE ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)24,2,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto InsError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto InsError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto InsNotFound;
}

  sqlastop(0L);
}

                   /* Den hîchsten Wert ermitteln */
                   
/*
EXEC SQL SELECT MAX (ULKEY) INTO :ULADRKEY :IND FROM ADR ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,1,2,0L);
    sqlasetv(2,0,497,4,&ULADRKEY,&IND,0L);
  sqlacall((unsigned short)24,3,0,2,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto InsError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto InsError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto InsNotFound;
}

  sqlastop(0L);
}

                   /* Neuen Key ermitteln */
                   ULADRKEY = (IND < 0) ? 1 : ++ULADRKEY ;
                   
/*
EXEC SQL INSERT INTO ADR VALUES (:ULADRKEY, :SZADRNAME, :SZADRVORNAME, :SZADRTELEFON) ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,4,3,0L);
    sqlasetv(1,0,496,4,&ULADRKEY,0L,0L);
    sqlasetv(1,1,460,65,SZADRNAME,0L,0L);
    sqlasetv(1,2,460,65,SZADRVORNAME,0L,0L);
    sqlasetv(1,3,460,65,SZADRTELEFON,0L,0L);
  sqlacall((unsigned short)24,4,1,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto InsError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto InsError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto InsNotFound;
}

  sqlastop(0L);
}

                   
/*
EXEC SQL COMMIT WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)21,0,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto InsError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto InsError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto InsNotFound;
}

  sqlastop(0L);
}

                   pdbinfoReceive->ulRC = DBC_NOERROR ;
                   goto InsOk ;
                    InsError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto InsErrorEnd ;
                    InsNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    InsErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                    InsOk:
                   /* Ergebnis zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              }
           break ;
      /* Lesen */
      case WMPDB_REQUEST:
           /* Was lesen? */
           switch (LONGFROMMP (mp1))
              {
              /* Alle EintrÑge */
              case DBK_REQUESTALL:
                   fOpen = FALSE ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqAllError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqAllError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqAllNotFound ;
*/

                   
/*
EXEC SQL DECLARE cursor1 CURSOR FOR SELECT ULKEY, SZNAME, SZVORNAME, SZTELEFON FROM ADR ORDER BY ULKEY ;
*/

                   
/*
EXEC SQL OPEN cursor1 ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)26,5,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqAllError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqAllError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqAllNotFound;
}

  sqlastop(0L);
}

                   fOpen = TRUE ;
                   ulCount = 0 ;
                   /* In einer Schleife alle EintrÑge lesen */
                   while (SQLCODE == SQL_RC_OK)
                      {
                      pdbinfoSend = AllocInfo () ;
                      *pdbinfoSend = *pdbinfoReceive ;
                      pdbinfoSend->ulSatzNr = ++ulCount ;
                      pdbinfoSend->ulRC = DBC_NOERROR ;
                      
/*
EXEC SQL FETCH cursor1 INTO :ULADRKEY, :SZADRNAME, :SZADRVORNAME, :SZADRTELEFON ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,4,4,0L);
    sqlasetv(2,0,496,4,&ULADRKEY,0L,0L);
    sqlasetv(2,1,460,65,SZADRNAME,0L,0L);
    sqlasetv(2,2,460,65,SZADRVORNAME,0L,0L);
    sqlasetv(2,3,460,65,SZADRTELEFON,0L,0L);
  sqlacall((unsigned short)25,5,0,2,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqAllError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqAllError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqAllNotFound;
}

  sqlastop(0L);
}

                      pdbinfoSend->dbadr = pdbinfoReceive->dbadr ;
                      WinPostMsg (pdbinfoSend->hwnd,
                                  WMPDB_SEND,
                                  mp1,
                                  MPFROMP (pdbinfoSend)) ;
                      }
                    ReqAllError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto ReqAllErrorEnd ;
                    ReqAllNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    ReqAllErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   if   (fOpen)
                        {
                        pdbinfoReceive->ulRC = DBC_END ;
                        FreeInfo (pdbinfoSend) ;
                        
/*
EXEC SQL CLOSE cursor1 ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)20,5,0,0,0L);
  sqlastop(0L);
}

                        }
                   else
                        {
                        
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                        }
                   /* Endekennung bzw. Fehler zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              /* Die Anzahl der EintrÑge */
              case DBK_REQUESTCOUNT:
                   pdbinfoReceive->ulEID = 0 ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqCountError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqCountError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqCountNotFound ;
*/

                   
/*
EXEC SQL SELECT COUNT (*) INTO :ULCOUNT FROM ADR ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,1,5,0L);
    sqlasetv(2,0,496,4,&ULCOUNT,0L,0L);
  sqlacall((unsigned short)24,6,0,2,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqCountError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqCountError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqCountNotFound;
}

  sqlastop(0L);
}

                   
/*
EXEC SQL COMMIT WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)21,0,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqCountError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqCountError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqCountNotFound;
}

  sqlastop(0L);
}

                   pdbinfoReceive->ulRC = DBC_NOERROR ;
                   goto ReqCountOk ;
                    ReqCountError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto ReqCountErrorEnd ;
                    ReqCountNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    ReqCountErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                    ReqCountOk:
                   /* Ergebnis zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              /* Einen Eintrag */
              case DBK_REQUESTPART:
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqPartError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqPartError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqPartNotFound ;
*/

                   
/*
EXEC SQL SELECT SZNAME, SZVORNAME, SZTELEFON INTO :SZADRNAME, :SZADRVORNAME, :SZADRTELEFON FROM ADR WHERE ULKEY = :ULADRKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,6,0L);
    sqlasetv(1,0,496,4,&ULADRKEY,0L,0L);
  sqlaaloc(2,3,7,0L);
    sqlasetv(2,0,460,65,SZADRNAME,0L,0L);
    sqlasetv(2,1,460,65,SZADRVORNAME,0L,0L);
    sqlasetv(2,2,460,65,SZADRTELEFON,0L,0L);
  sqlacall((unsigned short)24,7,1,2,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqPartError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqPartError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqPartNotFound;
}

  sqlastop(0L);
}

                   
/*
EXEC SQL COMMIT WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)21,0,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto ReqPartError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto ReqPartError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto ReqPartNotFound;
}

  sqlastop(0L);
}

                   pdbinfoReceive->ulRC = DBC_NOERROR ;
                   goto ReqPartOk ;
                    ReqPartError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto ReqPartErrorEnd ;
                    ReqPartNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    ReqPartErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                    ReqPartOk:
                   /* Ergebnis zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              }
           break ;
      /* Ersetzen */
      case WMPDB_UPDATE:
           /* Was ersetzen? */
           switch (LONGFROMMP (mp1))
              {
              /* Einen Eintrag */
              case DBK_UPDATEPART:
                   
/*
EXEC SQL WHENEVER SQLERROR goto UpdError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto UpdError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto UpdNotFound ;
*/

                   
/*
EXEC SQL UPDATE ADR SET SZNAME = :SZADRNAME, SZVORNAME = :SZADRVORNAME, SZTELEFON = :SZADRTELEFON WHERE ULKEY = :ULADRKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,4,8,0L);
    sqlasetv(1,0,460,65,SZADRNAME,0L,0L);
    sqlasetv(1,1,460,65,SZADRVORNAME,0L,0L);
    sqlasetv(1,2,460,65,SZADRTELEFON,0L,0L);
    sqlasetv(1,3,496,4,&ULADRKEY,0L,0L);
  sqlacall((unsigned short)24,8,1,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto UpdError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto UpdError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto UpdNotFound;
}

  sqlastop(0L);
}

                   
/*
EXEC SQL COMMIT WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)21,0,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto UpdError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto UpdError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto UpdNotFound;
}

  sqlastop(0L);
}

                   pdbinfoReceive->ulRC = DBC_NOERROR ;
                   goto UpdOk ;
                    UpdError:
                   pdbinfoReceive->ulRC = DBC_ERROR ;
                   goto UpdErrorEnd ;
                    UpdNotFound:
                   pdbinfoReceive->ulRC = DBC_NOTFOUND ;
                    UpdErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}

                    UpdOk:
                   /* Ergebnis zurÅckschicken */
                   WinPostMsg (pdbinfoReceive->hwnd,
                               WMPDB_SEND,
                               mp1,
                               mp2) ;
                   return DBC_NOERROR ;
              }
           break ;
      }
   return DBC_ERROR ;
   }

/* Verarbeitung der ANS SQL Tabelle */
/*    nur Dummy zum Aufzeigen der Viel-Tabellen Verarbeitung */
ULONG TableAns (MSG    msg,
                MPARAM mp1,
                MPARAM mp2)
   {
   return DBC_ERROR ;
   }

/* Der eigentliche Thread zum Anlegen der Datenbank */
VOID ThreadCreateDatabase (VOID* pv)
   {
   CHAR  ch ;
   ULONG ulRC ;

   ulRC = DBC_ERROR ;
   ch = (ULONG) pv ;
   /* Das Anlegen */
   sqlecrdb (DBK_DATENBANK,
             ch,
             NULL,
             &sqlca) ;
   if   (SQLCODE == SQL_RC_OK)
        {
        /* Zuerst das Modul mit den CREATEs binden */
        sqlabind (DBK_CREATEBIND,
                  DBK_DATENBANK,
                  0,
                  DBK_LOGFILE,
                  SQL_FMT_DEF,
                  &sqlca) ;
        if   (SQLCODE == SQL_RC_OK)
             {
             /* Datenbank îffnen */
             if   (OpenDatabase () == DBC_NOERROR)
                  {
                  /* Tabellen anlegen */
                  if   (CreateTables () == DBC_NOERROR)
                       {
                       /* Jetzt erst die Zugriffsmodule binden */
                       sqlabind (DBK_BIND,
                                 DBK_DATENBANK,
                                 0,
                                 DBK_LOGFILE,
                                 SQL_FMT_DEF,
                                 &sqlca) ;
                       ulRC = DBC_NOERROR ;
                       }
                  }
             }
        }
   /* Abschluss des Anlegen der Datenbank an Anwendung POSTen */
   WinPostMsg (hwnd_g,
               WMPDB_CREATE,
               MPFROMLONG (ulRC),
               MPVOID) ;
   _endthread () ;
   }

/* Der DB Engine Thread */
VOID ThreadWndProc (VOID* pv)
   {
   HAB  hab ;
   HMQ  hmq ;
   HWND hwnd ;
   QMSG qmsg ;

   /* PM Anmeldung */
   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        /* Messagequeue anlegen */
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             /* Secondary Threads mit PM dÅrfen beim Shutdown nicht hÑngen */
             WinCancelShutdown (hmq,
                                TRUE) ;
             /* DB Engine Windowklasse registrieren */
             if   (WinRegisterClass (hab,
                                     WCPDB22,
                                     (PFNWP) WndProc,
                                     0,
                                     sizeof (PVOID)))
                  {
                  /* Objektfenster anlegen */
                  hwnd = WinCreateWindow (HWND_OBJECT,
                                          WCPDB22,
                                          "",
                                          WS_DISABLED,
                                          0,
                                          0,
                                          0,
                                          0,
                                          HWND_OBJECT,
                                          HWND_BOTTOM,
                                          1,
                                          NULL,
                                          NULL) ;
                  /* Messageloop dieses Threads */
                  while (WinGetMsg (hab,
                                    &qmsg,
                                    0,
                                    0,
                                    0))
                     {
                     WinDispatchMsg (hab,
                                     &qmsg) ;
                     }
                  /* Objektfenster lîschen */
                  WinDestroyWindow (hwnd) ;
                  }
             /* Messageque lîschen */
             WinDestroyMsgQueue (hmq) ;
             }
        /* Beim PM abmelden */
        WinTerminate (hab) ;
        }
   /* Thread stoppen */
   _endthread () ;
   }

/* Die Fensterprozedur des Objektfensters */
MRESULT EXPENTRY WndProc (HWND   hwnd,
                          MSG    msg,
                          MPARAM mp1,
                          MPARAM mp2)
   {
   /* Array mit den Tabellenfunktionen. Jede Tabelle hat eine eigene Zugriffsfunktion */
   static const PDXF apdxf [] = { TableAdr,
                                  TableAns } ;

   switch (msg)
      {
      case WM_CREATE:
           /* Die Anwendung Åber Erfolg oder Mi·erfolg des ôffnens der Datenbank informieren */
           WinPostMsg (hwnd_g,
                       WMPDB_OPEN,
                       MPFROMLONG (OpenDatabase ()),
                       MPFROMHWND (hwnd)) ;
           break ;
      case WM_DESTROY:
           /* Datenbank schlie·en */
           CloseDatabase () ;
           break ;
      case WMPDB_DELETE:
      case WMPDB_INSERT:
      case WMPDB_REQUEST:
      case WMPDB_UPDATE:
           /* Datenbankaktionen auf Tabellenfunktionen verteilen */
           (*apdxf [((PDBINFO) PVOIDFROMMP (mp2))->ulEbene - 1]) (msg,
                                                                  mp1,
                                                                  mp2) ;
           return (MRESULT) FALSE ;
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }
