/* $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/PRELOADR.C 1.1 1996/03/05 09:37:28 HaWi Exp $ */

/* Standard System Kopfdateien */
#define  INCL_BASE
#define  INCL_DOSDEVIOCTL
#define  INCL_GPI
#define  INCL_PM
#define  INCL_SPLDOSPRINT
#include <os2.h>

/* Einige C Kopfdateien */
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Programmeinstieg */
INT main (INT argc,
          PSZ argv [])
   {
   APIRET      rc ;
   CHAR        szName [CCHMAXPATH] ;
   RESULTCODES resc ;
   ULONG       ul ;
   ULONG       ulCount ;

   /* Preload Zaehler zuruecksetzen */
   ulCount = 0 ;
   /* Sind ueberhaupt Parameter angegeben worden? */
   if   (argc > 1)
        {
        /* Ja, in einer Schleife durch alle Parameter */
        for  (ul = 1; ul < argc; ul++)
             {
             /* Initialmeldung ausgeben */
             fprintf (stdout,
                      "PRELOADR.EXE - Preloading %s\n",
                      argv [ul]) ;
             /* Prozess in den Speicher laden, aber nicht ausfuehren */
             rc = DosExecPgm (szName,
                              sizeof (szName),
                              EXEC_LOAD,
                              0,
                              0,
                              &resc,
                              argv [ul]) ;
             /* Gab es einen Fehler? */
             if   (rc)
                  {
                  /* Ja, ausgeben */
                  fprintf (stdout,
                           "PRELOADR.EXE - Preload error: %lx\n",
                           rc) ;
                  }
             else
                  {
                  /* Nein, Preload war erfolgreich */
                  fprintf (stdout,
                           "PRELOADR.EXE - Preload successfully\n") ;
                  /* Preload Zaehler erhoehen */
                  ulCount++ ;
                  }
             }
        }
   else
        {
        /* Fehlermeldung da keine Parameter angegeben wurden */
        fprintf (stdout,
                 "PRELOADR.EXE - No applications to preload\n") ;
        }
   /* Wurden Prozesse in den Speicher geladen? */
   if   (ulCount)
        {
        /* Ja, entsprechende Meldung ausgeben */
        fprintf (stdout,
                 "PRELOADR.EXE - Now sleeping. Press CTRL C to stop preload\n") ;
        /* Diesen Prozess in den Tiefschlaf versetzen */
        DosSleep (-1) ;
        }
   else
        {
        /* Meldung dass keine Prozesse erfolgreich preloaded wurden */
        fprintf (stdout,
                 "PRELOADR.EXE - No applications preloaded\n") ;
        }
   /* Und zurueck */
   return 0 ;
   }

