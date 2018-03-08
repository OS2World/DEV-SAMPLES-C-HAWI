static unsigned char sqla_program_id[40] = 
{111,65,65,66,65,69,67,67,85,83,69,82,73,68,32,32,68,66,50,50,
50,32,32,32,56,65,122,111,76,86,67,76,48,32,32,32,32,32,32,32};


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
/* $History: DB222.SQC $ */
/*  */
/* *****************  Version 1  ***************** */
/* User: Hawi         Date: 2/20/95    Time: 9:51p */
/* Created in $/Toolbox/GibMirDieDaten */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <sql.h>
#include <sqlcodes.h>

#include "DB22.H"


/*
EXEC SQL INCLUDE sqlca;
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



/* Anlegen der Tabellen mu· in einem eigenem Modul stehen (fÅr das Binden beim Anlegen der Datenbank */
ULONG CreateTables (VOID)
   {
   
/*
EXEC SQL WHENEVER SQLERROR goto DBError ;
*/

   
/*
EXEC SQL WHENEVER SQLWARNING goto DBError ;
*/

   
/*
EXEC SQL WHENEVER NOT FOUND goto DBError ;
*/

   
/*
EXEC SQL CREATE TABLE ADR
                         (ULKEY     INT         NOT NULL,
                          SZNAME    VARCHAR(64) NOT NULL,
                          SZVORNAME VARCHAR(64) NOT NULL,
                          SZTELEFON VARCHAR(64) NOT NULL) ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)24,1,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto DBError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto DBError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto DBError;
}

  sqlastop(0L);
}

   
/*
EXEC SQL CREATE TABLE ANS
                         (ULKEY     INT         NOT NULL,
                          SZNAME    VARCHAR(64) NOT NULL,
                          SZVORNAME VARCHAR(64) NOT NULL,
                          SZTELEFON VARCHAR(64) NOT NULL) ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)24,2,0,0,0L);
if (sqlca.sqlcode < 0)
{
   sqlastop(0L);
   goto DBError;
}

if (((sqlca.sqlcode > 0) && (sqlca.sqlcode != 100)) ||
    ((sqlca.sqlcode == 0) && (sqlca.sqlwarn[0] == 'W')))
{
    sqlastop(0L);
    goto DBError;
}

if (sqlca.sqlcode == 100)
{
   sqlastop(0L);
   goto DBError;
}

  sqlastop(0L);
}

   return DBC_NOERROR ;
    DBError:
   return DBC_ERROR ;
   }
   