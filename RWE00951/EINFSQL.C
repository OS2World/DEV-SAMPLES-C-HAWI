static unsigned char sqla_program_id[40] = 
{111,65,65,66,65,69,67,67,85,83,69,82,73,68,32,32,69,73,78,70,
83,81,76,32,65,65,112,108,81,75,69,76,48,32,32,32,32,32,32,32};


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
/* $History: $ */

#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

#include <sql.h>
#include <sqlcodes.h>
#include <sqlenv.h>

#include <stdlib.h>
#include <string.h>
      
#include "EINFSQL.H"


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




/*
EXEC SQL BEGIN DECLARE SECTION;
*/

   char  SZNAME [65] ;
   char  SZORT     [65] ;
   char  SZPLZ     [9] ;
   char  SZSTRASSE [65] ;
   char  SZTELEFON [65] ;
   char  SZVORNAME [65] ;
   long  ULSCHLUESSEL ;
   short IND ;

/*
EXEC SQL END DECLARE SECTION ;
*/


#define SZNAME       adresse.szName
#define SZORT        adresse.szOrt
#define SZPLZ        adresse.szPlz
#define SZSTRASSE    adresse.szStrasse
#define SZTELEFON    adresse.szTelefon
#define SZVORNAME    adresse.szVorname
#define ULSCHLUESSEL adresse.ulSchluessel

HAB hab_g = NULLHANDLE ;

INT main (VOID)
   {
   HMQ hmq ;
   
   if   ((hab_g = WinInitialize (QV_OS2)) != NULLHANDLE)
        {
        if   ((hmq = WinCreateMsgQueue (hab_g,
                                        0)) != NULLHANDLE)
             {
             WinDlgBox (HWND_DESKTOP,
                        HWND_DESKTOP,
                        DlgWndProc,
                        NULLHANDLE,
                        IDD,
                        NULL) ;
             WinDestroyMsgQueue (hmq) ;
             }
        WinTerminate (hab_g) ;
        }
   return 0 ;
   }

MRESULT EXPENTRY DlgWndProc (HWND   hwndDlg,
                             MSG    msg,
                             MPARAM mp1,
                             MPARAM mp2)
   {
   switch (msg)
      {
      case WM_COMMAND:
           {
           ADRESSE adresse ;
           PDLG    pdlg ;
           ULONG   ul ;
           ULONG   ul2 ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           switch (COMMANDMSG (&msg)->cmd)
              {
              case DID_CANCEL:
                   
/*
EXEC SQL CONNECT RESET ;
*/

{
  sqlestpd_api(&sqlca);
}

                   WinDismissDlg (hwndDlg,
                                  DID_CANCEL) ;
                   return (MRESULT) FALSE ;
              case IDP_AENDERN:
                   if   (pdlg->plistCurrent)
                        {
                        adresse.ulSchluessel = pdlg->plistCurrent->ulSchluessel ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_NAME,
                                             sizeof (adresse.szName),
                                             adresse.szName) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_ORT,
                                             sizeof (adresse.szOrt),
                                             adresse.szOrt) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_PLZ,
                                             sizeof (adresse.szPlz),
                                             adresse.szPlz) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_STRASSE,
                                             sizeof (adresse.szStrasse),
                                             adresse.szStrasse) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_TELEFON,
                                             sizeof (adresse.szTelefon),
                                             adresse.szTelefon) ;
                        WinQueryDlgItemText (hwndDlg,
                                             IDE_VORNAME,
                                             sizeof (adresse.szVorname),
                                             adresse.szVorname) ;
                        
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
EXEC SQL UPDATE ADRESSE SET SZNAME=:SZNAME, SZVORNAME=:SZVORNAME, SZSTRASSE=:SZSTRASSE, SZPLZ=:SZPLZ, SZORT=:SZORT, SZTELEFON=:SZTELEFON WHERE ULSCHLUESSEL=:ULSCHLUESSEL ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,7,1,0L);
    sqlasetv(1,0,460,65,SZNAME,0L,0L);
    sqlasetv(1,1,460,65,SZVORNAME,0L,0L);
    sqlasetv(1,2,460,65,SZSTRASSE,0L,0L);
    sqlasetv(1,3,460,9,SZPLZ,0L,0L);
    sqlasetv(1,4,460,65,SZORT,0L,0L);
    sqlasetv(1,5,460,65,SZTELEFON,0L,0L);
    sqlasetv(1,6,496,4,&ULSCHLUESSEL,0L,0L);
  sqlacall((unsigned short)24,1,1,0,0L);
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

                        goto UpdOk ;
                         UpdError:
                        goto UpdErrorEnd ;
                         UpdNotFound:
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

                        }
                    UpdOk:
                   return (MRESULT) FALSE ;
              case IDP_ANFANG:
                   WinPostMsg (hwndDlg,
                               WMP_READ,
                               MPFROMLONG (K_ANFANG),
                               MPFROMLONG (0)) ;
                   return (MRESULT) FALSE ;
              case IDP_ENDE:
                   WinPostMsg (hwndDlg,
                               WMP_READ,
                               MPFROMLONG (K_ENDE),
                               MPFROMLONG (0)) ;
                   return (MRESULT) FALSE ;
              case IDP_LOESCHEN:
                   if   (pdlg->plistCurrent)
                        {
                        adresse.ulSchluessel = pdlg->plistCurrent->ulSchluessel ;
                        
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
EXEC SQL DELETE FROM ADRESSE WHERE ULSCHLUESSEL=:ULSCHLUESSEL ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,2,0L);
    sqlasetv(1,0,496,4,&ULSCHLUESSEL,0L,0L);
  sqlacall((unsigned short)24,2,1,0,0L);
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

                        goto DelOk ;
                         DelError:
                        goto DelErrorEnd ;
                         DelNotFound:
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
                        ul = K_ANFANG ;
                        ul2 = 0 ;
                        if   (pdlg->plistCurrent->plistPrev)
                             {
                             ul = 0 ;
                             ul2 = pdlg->plistCurrent->plistPrev->ulSchluessel ;
                             }
                        else
                             {
                             if   (pdlg->plistCurrent->plistNext)
                                  {
                                  ul = 0 ;
                                  ul2 = pdlg->plistCurrent->plistNext->ulSchluessel ;
                                  }
                             }
                        WinPostMsg (hwndDlg,
                                    WMP_REREAD,
                                    MPFROMLONG (ul),
                                    MPFROMLONG (ul2)) ;
                        }
                   return (MRESULT) FALSE ;
              case IDP_NEU:
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_NAME,
                                        sizeof (adresse.szName),
                                        adresse.szName) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_ORT,
                                        sizeof (adresse.szOrt),
                                        adresse.szOrt) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_PLZ,
                                        sizeof (adresse.szPlz),
                                        adresse.szPlz) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_STRASSE,
                                        sizeof (adresse.szStrasse),
                                        adresse.szStrasse) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_TELEFON,
                                        sizeof (adresse.szTelefon),
                                        adresse.szTelefon) ;
                   WinQueryDlgItemText (hwndDlg,
                                        IDE_VORNAME,
                                        sizeof (adresse.szVorname),
                                        adresse.szVorname) ;
                   
/*
EXEC SQL WHENEVER SQLERROR goto InsError ;
*/

                   
/*
EXEC SQL WHENEVER SQLWARNING goto InsError ;
*/

                   
/*
EXEC SQL WHENEVER NOT FOUND goto InsNotFound ;
*/

                   
/*
EXEC SQL LOCK TABLE ADRESSE IN EXCLUSIVE MODE ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)24,3,0,0,0L);
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
EXEC SQL SELECT MAX (ULSCHLUESSEL) INTO :ULSCHLUESSEL :IND FROM ADRESSE ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,1,3,0L);
    sqlasetv(2,0,497,4,&ULSCHLUESSEL,&IND,0L);
  sqlacall((unsigned short)24,4,0,2,0L);
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

                   adresse.ulSchluessel = (IND < 0) ? 1 : ++adresse.ulSchluessel ;
                   
/*
EXEC SQL INSERT INTO ADRESSE VALUES (:ULSCHLUESSEL, :SZNAME, :SZVORNAME, :SZSTRASSE, :SZPLZ, :SZORT, :SZTELEFON) ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,7,4,0L);
    sqlasetv(1,0,496,4,&ULSCHLUESSEL,0L,0L);
    sqlasetv(1,1,460,65,SZNAME,0L,0L);
    sqlasetv(1,2,460,65,SZVORNAME,0L,0L);
    sqlasetv(1,3,460,65,SZSTRASSE,0L,0L);
    sqlasetv(1,4,460,9,SZPLZ,0L,0L);
    sqlasetv(1,5,460,65,SZORT,0L,0L);
    sqlasetv(1,6,460,65,SZTELEFON,0L,0L);
  sqlacall((unsigned short)24,5,1,0,0L);
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

                   goto InsOk ;
                    InsError:
                   goto InsErrorEnd ;
                    InsNotFound:
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
                   WinPostMsg (hwndDlg,
                               WMP_REREAD,
                               MPFROMLONG (0),
                               MPFROMLONG (adresse.ulSchluessel)) ;
                   return (MRESULT) FALSE ;
              case IDP_VOR:
                   WinPostMsg (hwndDlg,
                               WMP_READ,
                               MPFROMLONG (K_VOR),
                               MPFROMLONG (0)) ;
                   return (MRESULT) FALSE ;
              case IDP_ZURUECK:
                   WinPostMsg (hwndDlg,
                               WMP_READ,
                               MPFROMLONG (K_ZURUECK),
                               MPFROMLONG (0)) ;
                   return (MRESULT) FALSE ;
              }
           break ;
           }
      case WM_DESTROY:
           {
           PDLG pdlg ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           free (pdlg) ;
           break ;
           }
      case WM_INITDLG:
           {
           PDLG pdlg ;

           pdlg = calloc (1,
                          sizeof (DLG)) ;
           WinSetWindowPtr (hwndDlg,
                            QWL_USER,
                            pdlg) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_NAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_ORT,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_PLZ,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (8),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_STRASSE,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_TELEFON,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinSendDlgItemMsg (hwndDlg,
                              IDE_VORNAME,
                              EM_SETTEXTLIMIT,
                              MPFROMSHORT (64),
                              MPVOID) ;
           WinPostMsg (hwndDlg,
                       WMP_MAININIT,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      case WMP_ENABLE:
           {
           BOOL af [7] ;
           PDLG pdlg ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           if   (!pdlg->ulCount)
                {
                af [K_AENDERN] = FALSE ;
                af [K_ANFANG] = FALSE ;
                af [K_ENDE] = FALSE ;
                af [K_LOESCHEN] = FALSE ;
                af [K_NEU] = TRUE ;
                af [K_VOR] = FALSE ;
                af [K_ZURUECK] = FALSE ;
                }
           if   (pdlg->ulCount == 1)
                {
                af [K_AENDERN] = TRUE ;
                af [K_ANFANG] = FALSE ;
                af [K_ENDE] = FALSE ;
                af [K_LOESCHEN] = TRUE ;
                af [K_NEU] = TRUE ;
                af [K_VOR] = FALSE ;
                af [K_ZURUECK] = FALSE ;
                }
           if   (pdlg->ulCount > 1)
                {
                af [K_AENDERN] = TRUE ;
                af [K_ANFANG] = (pdlg->plistCurrent->plistPrev) ? TRUE : FALSE ;
                af [K_ENDE] = (pdlg->plistCurrent->plistNext) ? TRUE : FALSE ;
                af [K_LOESCHEN] = TRUE ;
                af [K_NEU] = TRUE ;
                af [K_VOR] = af [K_ENDE] ;
                af [K_ZURUECK] = af [K_ANFANG] ;
                }
           WinEnableControl (hwndDlg,
                             IDP_AENDERN,
                             af [K_AENDERN]) ;
           WinEnableControl (hwndDlg,
                             IDP_ANFANG,
                             af [K_ANFANG]) ;
           WinEnableControl (hwndDlg,
                             IDP_ENDE,
                             af [K_ENDE]) ;
           WinEnableControl (hwndDlg,
                             IDP_LOESCHEN,
                             af [K_LOESCHEN]) ;
           WinEnableControl (hwndDlg,
                             IDP_NEU,
                             af [K_NEU]) ;
           WinEnableControl (hwndDlg,
                             IDP_VOR,
                             af [K_VOR]) ;
           WinEnableControl (hwndDlg,
                             IDP_ZURUECK,
                             af [K_ZURUECK]) ;
           return (MRESULT) FALSE ;
           }
      case WMP_MAININIT:
           sqlestar () ;
           
/*
EXEC SQL CONNECT TO ADRESSE ;
*/

{
  sqlestrd_api("ADRESSE","",'S',&sqlca);
}

           if   (SQLCODE == SQL_RC_OK)
                {
                WinPostMsg (hwndDlg,
                            WMP_REREAD,
                            MPFROMLONG (K_ANFANG),
                            MPFROMLONG (0)) ;
                }
           else
                {
                WinPostMsg (hwndDlg,
                            WM_COMMAND,
                            MPFROMSHORT (DID_CANCEL),
                            MPFROM2SHORT (CMDSRC_PUSHBUTTON, TRUE)) ;
                }
           return (MRESULT) FALSE ;
      case WMP_READ:
           {
           ADRESSE adresse ;
           PDLG    pdlg ;
           PLIST   plist ;
           PLIST   plistPrev ;
           ULONG   ulI ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           adresse.ulSchluessel = 0 ;
           memset (&adresse,
                   0,
                   sizeof (ADRESSE)) ;
           if   (LONGFROMMP (mp1))
                {
                switch (LONGFROMMP (mp1))
                   {
                   case K_ANFANG:
                        if   (pdlg->plistRoot)
                             {
                             pdlg->plistCurrent = pdlg->plistRoot ;
                             adresse.ulSchluessel = pdlg->plistRoot->ulSchluessel ;
                             }
                        break ;
                   case K_ENDE:
                        if   (pdlg->plistRoot)
                             {
                             for  (ulI = 0, plist = pdlg->plistRoot;
                                   ulI < pdlg->ulCount;
                                   ulI++, plistPrev = plist, plist = plist->plistNext) ;
                             pdlg->plistCurrent = plistPrev ;
                             adresse.ulSchluessel = plistPrev->ulSchluessel ;
                             }
                        break ;
                   case K_VOR:
                        if   (pdlg->plistCurrent)
                             {
                             if   (pdlg->plistCurrent->plistNext)
                                  {
                                  pdlg->plistCurrent = pdlg->plistCurrent->plistNext ;
                                  adresse.ulSchluessel = pdlg->plistCurrent->ulSchluessel ;
                                  }
                             }
                        break ;
                   case K_ZURUECK:
                        if   (pdlg->plistCurrent)
                             {
                             if   (pdlg->plistCurrent->plistPrev)
                                  {
                                  pdlg->plistCurrent = pdlg->plistCurrent->plistPrev ;
                                  adresse.ulSchluessel = pdlg->plistCurrent->ulSchluessel ;
                                  }
                             }
                        break ;
                   }
                }
           else
                {
                for  (ulI = 0, plist = pdlg->plistRoot;
                      ulI < pdlg->ulCount;
                      ulI++, plistPrev = plist, plist = plist->plistNext)
                     {
                     if   (plist->ulSchluessel == LONGFROMMP (mp2))
                          {
                          pdlg->plistCurrent = plist ;
                          adresse.ulSchluessel = LONGFROMMP (mp2) ;
                          break ;
                          }
                     }
                }
           if   (adresse.ulSchluessel)
                {
                
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
EXEC SQL SELECT * INTO :ULSCHLUESSEL, :SZNAME, :SZVORNAME, :SZSTRASSE, :SZPLZ, :SZORT, :SZTELEFON FROM ADRESSE WHERE ULSCHLUESSEL=:ULSCHLUESSEL ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(1,1,5,0L);
    sqlasetv(1,0,496,4,&ULSCHLUESSEL,0L,0L);
  sqlaaloc(2,7,6,0L);
    sqlasetv(2,0,496,4,&ULSCHLUESSEL,0L,0L);
    sqlasetv(2,1,460,65,SZNAME,0L,0L);
    sqlasetv(2,2,460,65,SZVORNAME,0L,0L);
    sqlasetv(2,3,460,65,SZSTRASSE,0L,0L);
    sqlasetv(2,4,460,9,SZPLZ,0L,0L);
    sqlasetv(2,5,460,65,SZORT,0L,0L);
    sqlasetv(2,6,460,65,SZTELEFON,0L,0L);
  sqlacall((unsigned short)24,6,1,2,0L);
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

                goto ReqPartOk ;
                 ReqPartError:
                goto ReqPartErrorEnd ;
                 ReqPartNotFound:
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

                }
            ReqPartOk:
           WinSetDlgItemText (hwndDlg,
                              IDE_NAME,
                              adresse.szName) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_ORT,
                              adresse.szOrt) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_PLZ,
                              adresse.szPlz) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_STRASSE,
                              adresse.szStrasse) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_TELEFON,
                              adresse.szTelefon) ;
           WinSetDlgItemText (hwndDlg,
                              IDE_VORNAME,
                              adresse.szVorname) ;
           WinSetDlgItemShort (hwndDlg,
                               IDT_SCHLUESSEL,
                               adresse.ulSchluessel,
                               FALSE) ;
           WinPostMsg (hwndDlg,
                       WMP_ENABLE,
                       MPVOID,
                       MPVOID) ;
           return (MRESULT) FALSE ;
           }
      case WMP_REREAD:
           {
           ADRESSE adresse ;
           BOOL    f ;
           PDLG    pdlg ;
           PLIST   plist ;
           PLIST   plistPrev ;
           ULONG   ulI ;

           pdlg = WinQueryWindowPtr (hwndDlg,
                                     QWL_USER) ;
           if   (pdlg->ulCount)
                {
                for  (ulI = 1, plistPrev = pdlg->plistRoot, plist = plistPrev->plistNext;
                      ulI < pdlg->ulCount;
                      ulI++, plistPrev = plist, plist = plist->plistNext)
                     {
                     free (plistPrev) ;
                     }
                free (plist) ;
                pdlg->plistCurrent = NULL ;
                pdlg->plistRoot = NULL ;
                pdlg->ulCount = 0 ;
                }
           f = FALSE ;
           
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
EXEC SQL DECLARE cursor1 CURSOR FOR SELECT ULSCHLUESSEL, SZNAME FROM ADRESSE ORDER BY SZNAME ;
*/

           
/*
EXEC SQL OPEN cursor1 ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)26,7,0,0,0L);
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

           f = TRUE ;
           while (SQLCODE == SQL_RC_OK)
              {
              if   (!pdlg->ulCount)
                   {
                   pdlg->plistRoot = calloc (1,
                                             sizeof (LIST)) ;
                   plist = pdlg->plistRoot ;
                   pdlg->ulCount++ ;
                   }
              else
                   {
                   for  (ulI = 0, plist = pdlg->plistRoot;
                         ulI < pdlg->ulCount;
                         ulI++, plistPrev = plist, plist = plist->plistNext) ;
                   plistPrev->plistNext = calloc (1,
                                                  sizeof (LIST)) ;
                   plist = plistPrev->plistNext ;
                   plist->plistPrev = plistPrev ;
                   pdlg->ulCount++ ;
                   }
              
/*
EXEC SQL FETCH cursor1 INTO :ULSCHLUESSEL, :SZNAME ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlaaloc(2,2,7,0L);
    sqlasetv(2,0,496,4,&ULSCHLUESSEL,0L,0L);
    sqlasetv(2,1,460,65,SZNAME,0L,0L);
  sqlacall((unsigned short)25,7,0,2,0L);
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

              plist->ulSchluessel = adresse.ulSchluessel ;
              }
            ReqAllError:
           goto ReqAllErrorEnd ;
            ReqAllNotFound:
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

           if   (f)
                {
                free (plist) ;
                pdlg->ulCount-- ;
                if   (!pdlg->ulCount)
                     {
                     pdlg->plistRoot = NULL ;
                     }
                else
                     {
                     plistPrev->plistNext = NULL ;
                     }
                
/*
EXEC SQL CLOSE cursor1 ;
*/

{
  sqlastrt(sqla_program_id,0L,&sqlca);
  sqlacall((unsigned short)20,7,0,0,0L);
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
           WinPostMsg (hwndDlg,
                       WMP_READ,
                       mp1,
                       mp2) ;
           return (MRESULT) FALSE ;
           }
      }
   return (WinDefDlgProc (hwndDlg,
                          msg,
                          mp1,
                          mp2)) ;
   }
