static unsigned char sqla_program_id[40] = 
{111,65,65,66,65,69,67,67,85,83,69,82,73,68,32,32,73,79,83,50,
76,48,48,49,102,66,90,80,83,88,77,75,48,32,32,32,32,32,32,32};


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
#line 1 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"
/* *******************************************************************
   *** File:   IOS2L001.SQC                                        ***
   *** Author: Harald (HaWi) Wilhelm                               ***
   *** Date:   07.11.1993                                          ***
   ******************************************************************* */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <sql.h>
#include <sqlcodes.h>
#include <sqlenv.h>

#include <stdlib.h>
#include <string.h>
      
#include "IOS2L001.H"


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

#line 19 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/
#line 21 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

   char  SZNAME [65] ;
   long  ULCOUNT ;
   long  ULKEY ;
   short IND ;

/*
EXEC SQL END DECLARE SECTION ;
*/
#line 26 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"


typedef ULONG (*PDXF) (MSG, MPARAM, MPARAM) ;

HWND hwndIOS2P_g = NULLHANDLE ;

/* *******************************************************************
   *** Function: IOS2L_Register                                    ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_Register (const HWND hwnd)
   {
   hwndIOS2P_g = hwnd ;
   _beginthread (IOS2L_Thread,
                 NULL,
                 81919,
                 (PVOID) 0) ;
   return IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_Thread                                      ***
   ******************************************************************* */
VOID IOS2L_Thread (VOID * pv)
   {
   HAB  hab ;
   HMQ  hmq ;
   HWND hwnd ;
   QMSG qmsg ;

   if   ((hab = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab,
                                        0)) != NULLHANDLE)
             {
             if   (WinRegisterClass (hab,
                                     WCP_IOS2L,
                                     (PFNWP) IOS2L_WndProc,
                                     0,
                                     sizeof (PVOID)))
                  {
                  hwnd = WinCreateWindow (HWND_OBJECT,
                                          WCP_IOS2L,
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
                  while (WinGetMsg (hab,
                                    &qmsg,
                                    0,
                                    0,
                                    0))
                     {
                     WinDispatchMsg (hab,
                                     &qmsg) ;
                     }
                  WinDestroyWindow (hwnd) ;
                  }
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab) ;
        }
   }

/* *******************************************************************
   *** Function: IOS2L_WndProc                                     ***
   ******************************************************************* */
MRESULT EXPENTRY IOS2L_WndProc (const HWND hwnd,
                                const MSG msg,
                                const MPARAM mp1,
                                MPARAM mp2)
   {
   static const PDXF apdxf [] = { IOS2L_TableIOS2L } ;

   switch (msg)
      {
      case WM_CREATE:
           if   (IOS2L_OpenDatabase () != IOS2L_NOERROR)
                {
                WinPostMsg (hwndIOS2P_g,
                            WMP_IOS2LOPENERROR,
                            MPFROMHWND (hwnd),
                            MPVOID) ;
                }
           else
                {
                WinPostMsg (hwndIOS2P_g,
                            WMP_IOS2LOPENED,
                            MPFROMHWND (hwnd),
                            MPVOID) ;
                }
           break ;
      case WM_DESTROY:
           IOS2L_CloseDatabase () ;
           break ;
      case WMP_IOS2LDELETE:
      case WMP_IOS2LINSERT:
      case WMP_IOS2LREQUEST:
      case WMP_IOS2LUPDATE:
           (*apdxf [((PIOS2LINFO) mp2)->ulEbene - 1]) (msg,
                                                       mp1,
                                                       mp2) ;
           return (MRESULT) FALSE ;
      }
   return WinDefWindowProc (hwnd,
                            msg,
                            mp1,
                            mp2) ;
   }

/* *******************************************************************
   *** Function: IOS2L_AllocInfo                                   ***
   ******************************************************************* */
PIOS2LINFO EXPENTRY IOS2L_AllocInfo (VOID)
   {
   return calloc (1,
                  sizeof (IOS2LINFO)) ;
   }

/* *******************************************************************
   *** Function: IOS2L_AllocData                                   ***
   ******************************************************************* */
PIOS2LINFO EXPENTRY IOS2L_AllocData (const ULONG ulType)
   {
   PIOS2LINFO         pios2linfo ;
   PVOID              pv ;
   static const ULONG ulTables [] = { sizeof (IOS2LDATA) } ;

   pios2linfo = IOS2L_AllocInfo () ;
   pv = calloc (1,
                ulTables [ulType - 1]) ;
   pios2linfo->ulEbene = ulType ;
   pios2linfo->pData = pv ;
   return pios2linfo ;
   }

/* *******************************************************************
   *** Function: IOS2L_FreeInfo                                    ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_FreeInfo (PIOS2LINFO pios2linfo)
   {
   free (pios2linfo) ;
   return IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_FreeData                                    ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_FreeData (PIOS2LINFO pios2linfo)
   {
   if   (pios2linfo->pData != NULL)
        {
        free (pios2linfo->pData) ;
        pios2linfo->pData = NULL ;
        }
   return IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_CreateDB                                    ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_CreateDB (const CHAR ch)
   {
   sqlecrdb ("IOS2L",
             ch,
             NULL,
             &sqlca) ;
   if   (SQLCODE != SQL_RC_OK)
        {
        return IOS2L_ERROR ;
        }
   sqlabind ("IOS2L002.BND",
             "IOS2L",
             0,
             "ERROR.LOG",
             SQL_FMT_DEF,
             &sqlca) ;
   if   (SQLCODE != SQL_RC_OK)
        {
        return IOS2L_ERROR ;
        }
   if   ((IOS2L_OpenDatabase ()) != IOS2L_NOERROR)
        {
        return IOS2L_ERROR ;
        }
   if   ((IOS2L_CreateTable ()) != IOS2L_NOERROR)
        {
        return IOS2L_ERROR ;
        }
   sqlabind ("IOS2L001.BND",
             "IOS2L",
             0,
             "ERROR.LOG",
             SQL_FMT_DEF,
             &sqlca) ;
   return (SQLCODE != SQL_RC_OK) ? IOS2L_ERROR : IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_DropDB                                      ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_DropDB (VOID)
   {
   IOS2L_CloseDatabase () ;
   sqledrpd ("IOS2L",
             &sqlca) ;
   return (SQLCODE != SQL_RC_OK) ? IOS2L_ERROR : IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_OpenDatabase                                ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_OpenDatabase (VOID)
   {
   sqlestar () ;
   
/*
EXEC SQL CONNECT TO IOS2L ;
*/

{
  sqlestrd_api("IOS2L","",'S',&sqlca);
}
#line 247 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

   return (SQLCODE != SQL_RC_OK) ? IOS2L_ERROR : IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_CloseDatabase                               ***
   ******************************************************************* */
ULONG EXPENTRY IOS2L_CloseDatabase (VOID)
   {
   
/*
EXEC SQL CONNECT RESET ;
*/

{
  sqlestpd_api(&sqlca);
}
#line 256 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

   return IOS2L_NOERROR ;
   }

/* *******************************************************************
   *** Function: IOS2L_TableIOS2L                                  ***
   ******************************************************************* */
ULONG IOS2L_TableIOS2L (const MSG msg,
                        const MPARAM mp1,
                        MPARAM mp2)
   {
   BOOL       fOpen ;
   PIOS2LINFO pios2linfoReceive ;
   PIOS2LDATA pios2ldataReceive ;
   PIOS2LINFO pios2linfoSend ;
   PIOS2LDATA pios2ldataSend ;
   ULONG      ulCount ;

   pios2linfoReceive = mp2 ;
   pios2ldataReceive = pios2linfoReceive->pData ;
   switch (msg)
      {
      case WMP_IOS2LDELETE:
           switch (LONGFROMMP (mp1))
              {
              case IOS2L_DELETEPART:
                   ULKEY = pios2ldataReceive->ulKey ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto DelError ;
*/
#line 283 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto DelError ;
*/
#line 284 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto DelNotFound ;
*/
#line 285 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL DELETE
                              FROM   IOS2L
                              WHERE  ULKEY = :ULKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,1,0L);
    sqlasetv(1,0,496,4,&ULKEY,0L,0L);
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
#line 288 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
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
#line 289 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulRC = IOS2L_NOERROR ;
                   goto DelOk ;
                    DelError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto DelErrorEnd ;
                    DelNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    DelErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 298 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 299 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 300 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}
#line 301 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                    DelOk:
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              }
           break;
      case WMP_IOS2LINSERT:
           switch (LONGFROMMP (mp1))
              {
              case IOS2L_INSERTPART:
                   strcpy (SZNAME,
                           pios2ldataReceive->szName) ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto InsError ;
*/
#line 316 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto InsError ;
*/
#line 317 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto InsNotFound ;
*/
#line 318 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL LOCK
                              TABLE  IOS2L
                              IN     EXCLUSIVE MODE ;
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
#line 321 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL SELECT   MAX (ULKEY)
                              INTO   :ULKEY :IND
                              FROM   IOS2L ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,1,2,0L);
    sqlasetv(2,0,497,4,&ULKEY,&IND,0L);
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
#line 324 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   ULKEY = (IND < 0) ? 1 : ++ULKEY ;
                   
/*
EXEC SQL INSERT
                              INTO   IOS2L
                              VALUES (:ULKEY,
                                      :SZNAME) ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,2,3,0L);
    sqlasetv(1,0,496,4,&ULKEY,0L,0L);
    sqlasetv(1,1,460,65,SZNAME,0L,0L);
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
#line 329 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
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
#line 330 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulRC = IOS2L_NOERROR ;
                   goto InsOk ;
                    InsError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto InsErrorEnd ;
                    InsNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    InsErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 339 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 340 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 341 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}
#line 342 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                    InsOk:
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              }
           break;
      case WMP_IOS2LREQUEST:
           switch (LONGFROMMP (mp1))
              {
              case IOS2L_REQUESTALL:
                   fOpen = FALSE ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqAllError ;
*/
#line 356 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqAllError ;
*/
#line 357 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqAllNotFound ;
*/
#line 358 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL DECLARE cursor1 CURSOR FOR
                              SELECT   ULKEY
                                FROM   IOS2L
                                ORDER  BY ULKEY ;
*/
#line 362 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
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
#line 363 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   fOpen = TRUE ;
                   ulCount = 0 ;
                   while (SQLCODE == SQL_RC_OK)
                      {
                      pios2linfoSend = IOS2L_AllocData (IOS2L_DATA) ;
                      pios2ldataSend = pios2linfoSend->pData ;
                      *pios2linfoSend = *pios2linfoReceive ;
                      pios2linfoSend->ulSatzNr = ++ulCount ;
                      pios2linfoSend->ulRC = IOS2L_NOERROR ;
                      pios2linfoSend->pData = pios2ldataSend ;
                      
/*
EXEC SQL FETCH    cursor1
                                 INTO   :ULKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,1,4,0L);
    sqlasetv(2,0,496,4,&ULKEY,0L,0L);
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
#line 375 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                      pios2ldataSend->ulKey = ULKEY ;
                      WinPostMsg (pios2linfoSend->hwndSender,
                                  WMP_IOS2LSEND,
                                  mp1,
                                  MPFROMP (pios2linfoSend)) ;
                      }
                    ReqAllError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto ReqAllErrorEnd ;
                    ReqAllNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    ReqAllErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 388 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 389 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 390 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   if   (fOpen)
                        {
                        pios2linfoReceive->ulRC = IOS2L_END ;
                        IOS2L_FreeData (pios2linfoSend) ;
                        IOS2L_FreeInfo (pios2linfoSend) ;
                        
/*
EXEC SQL CLOSE cursor1 ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)20,5,0,0,0L);
  sqlastop(0L);
}
#line 396 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

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
#line 400 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                        }
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              case IOS2L_REQUESTCOUNT:
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqCountError ;
*/
#line 408 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqCountError ;
*/
#line 409 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqCountNotFound ;
*/
#line 410 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL SELECT   COUNT (*)
                              INTO   :ULCOUNT
                                      FROM   IOS2L ;
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
#line 413 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulEID = ULCOUNT ;
                   
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
#line 415 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulRC = IOS2L_NOERROR ;
                   goto ReqCountOk ;
                    ReqCountError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto ReqCountErrorEnd ;
                    ReqCountNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    ReqCountErrorEnd:
                   pios2linfoReceive->ulEID = 0 ;
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 425 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 426 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 427 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}
#line 428 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                    ReqCountOk:
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              case IOS2L_REQUESTPART:
                   ULKEY = pios2ldataReceive->ulKey ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto ReqPartError ;
*/
#line 437 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto ReqPartError ;
*/
#line 438 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto ReqPartNotFound ;
*/
#line 439 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL SELECT   SZNAME
                              INTO   :SZNAME
                              FROM   IOS2L
                              WHERE  ULKEY = :ULKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,6,0L);
    sqlasetv(1,0,496,4,&ULKEY,0L,0L);
  sqlaaloc(2,1,7,0L);
    sqlasetv(2,0,460,65,SZNAME,0L,0L);
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
#line 443 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   strcpy (pios2ldataReceive->szName,
                           SZNAME) ;
                   
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
#line 446 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulRC = IOS2L_NOERROR ;
                   goto ReqPartOk ;
                    ReqPartError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto ReqPartErrorEnd ;
                    ReqPartNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    ReqPartErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 455 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 456 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 457 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}
#line 458 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                    ReqPartOk:
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              }
           break ;
      case WMP_IOS2LUPDATE:
           switch (LONGFROMMP (mp1))
              {
              case IOS2L_UPDATEPART:
                   ULKEY = pios2ldataReceive->ulKey ;
                   strcpy (SZNAME,
                           pios2ldataReceive->szName) ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto UpdError ;
*/
#line 474 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING goto UpdError ;
*/
#line 475 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND goto UpdNotFound ;
*/
#line 476 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL UPDATE   IOS2L
                              SET    SZNAME = :SZNAME
                              WHERE  ULKEY = :ULKEY ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,2,8,0L);
    sqlasetv(1,0,460,65,SZNAME,0L,0L);
    sqlasetv(1,1,496,4,&ULKEY,0L,0L);
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
#line 479 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
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
#line 480 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   pios2linfoReceive->ulRC = IOS2L_NOERROR ;
                   goto UpdOk ;
                    UpdError:
                   pios2linfoReceive->ulRC = IOS2L_ERROR ;
                   goto UpdErrorEnd ;
                    UpdNotFound:
                   pios2linfoReceive->ulRC = IOS2L_NOTFOUND ;
                    UpdErrorEnd:
                   
/*
EXEC SQL WHENEVER SQLERROR continue ;
*/
#line 489 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER SQLWARNING continue ;
*/
#line 490 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL WHENEVER NOT FOUND continue ;
*/
#line 491 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                   
/*
EXEC SQL ROLLBACK WORK ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)28,0,0,0,0L);
  sqlastop(0L);
}
#line 492 "D:\\Daten\\Redaktion OS!2 Inside\\Projekte\\93.11.05!Gib mir die Daten\\Source\\.\\IOS2L001.SQC"

                    UpdOk:
                   WinPostMsg (pios2linfoReceive->hwndSender,
                               WMP_IOS2LSEND,
                               mp1,
                               mp2) ;
                   return IOS2L_NOERROR ;
              }
           break ;
      }
   return IOS2L_NOERROR ;
   }
