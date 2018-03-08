static unsigned char sqla_program_id[40] = 
{111,65,65,66,65,69,67,67,85,83,69,82,73,68,32,32,73,79,83,50,
76,48,48,50,87,66,88,78,83,88,77,75,48,32,32,32,32,32,32,32};


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
#line 1 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"
/* *******************************************************************
   *** File:   IOS2L002.SQC                                        ***
   *** Author: Harald (HaWi) Wilhelm                               ***
   *** Date:   07.11.1993                                          ***
   ******************************************************************* */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <sql.h>
#include <sqlcodes.h>

#include "IOS2L001.H"


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

#line 15 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"


/* *******************************************************************
   *** Function: IOS2L_CreateTable                                 ***
   ******************************************************************* */
ULONG IOS2L_CreateTable (VOID)
   {
   
/*
EXEC SQL WHENEVER SQLERROR goto DBError ;
*/
#line 22 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"

   
/*
EXEC SQL WHENEVER SQLWARNING goto DBError ;
*/
#line 23 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"

   
/*
EXEC SQL WHENEVER NOT FOUND goto DBError ;
*/
#line 24 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"

   
/*
EXEC SQL CREATE TABLE IOS2L
                         (ULKEY  INT         NOT NULL,
                          SZNAME VARCHAR(64) NOT NULL) ;
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
#line 27 "D:\\daten\\redaktion os!2 inside\\projekte\\93.11.05!gib mir die daten\\source\\IOS2L002.SQC"

   return IOS2L_NOERROR ;
    DBError:
   return IOS2L_ERROR ;
   }
   