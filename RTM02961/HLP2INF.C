/* $Header: D:/PROJEKTE/Kleine Helferlein!1/Source/rcs/HLP2INF.C 1.2 1995/11/28 13:23:36 HaWi Exp $ */

/* Kopfdateien fuer OS/2 */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Einige Konstanten */
#define HLP2INF_IGNORE 0
#define HLP2INF_INF    1
#define HLP2INF_HLP    16

/* Funktionsdeklarationen */
INT  main (INT, CHAR * []) ;
VOID Banner (VOID) ;

/* Programmstart */
INT main (INT  argc,
          CHAR *argv [])
   {
   CHAR  szFilename [CCHMAXPATH] = "" ;
   FILE* file ;
   ULONG ulArgument = HLP2INF_IGNORE ;
   ULONG ulI ;

   /* In einer Schleife durch alle Kommandozeilenparameter */
   for  (ulI = 1; ulI < argc; ulI++)
        {
        switch (argv [ulI] [0])
           {
           /* auf - bzw. / testen */
           case '-':
           case '/':
                switch (argv [ulI] [1])
                   {
                   /* Dateiname */
                   case 'f':
                   case 'F':
                        strcpy (szFilename,
                                &argv [ulI] [2]) ;
                        break ;
                   /* INF nach HLP konvertieren */
                   case 'h':
                   case 'H':
                        ulArgument = HLP2INF_HLP ;
                        break ;
                   /* HLP nach INF konvertieren */
                   case 'i':
                   case 'I':
                        ulArgument = HLP2INF_INF ;
                        break ;
                   /* Falscher Parameter erkannt */
                   default:
                        fprintf (stdout,
                                 "\nHLP2INF Fehler 1: Falscher Parameter\n") ;
                        Banner () ;
                        return 1 ;
                   }
                break ;
           }
        }
   /* Wurde ueberhaupt ein Parameter eingegeben? */
   if   (!ulArgument)
        {
        /* Nein, Fehler */
        fprintf (stdout,
                 "\nHLP2INF Fehler 2: Fehlender Parameter\n") ;
        /* Ausgabe Funktionsbanner */
        Banner () ;
        /* Programmabbruch */
        return 2 ;
        }
   /* Wurde ein Dateiname angegeben */
   if   (!*szFilename)
        {
        /* Nein, Fehler */
        fprintf (stdout,
                 "\nHLP2INF Fehler 3: Dateiname nicht angegeben\n") ;
        /* Ausgabe Funktionsbanner */
        Banner () ;
        /* Programmabbruch */
        return 3 ;
        }
   /* Datei im Binaermodus oeffnen */
   if   ((file = fopen (szFilename,
                        "r+b")) != NULL)
        {
        /* Datei an die dritte Position fahren */
        if   (!fseek (file,
                      3,
                      SEEK_SET))
             {
             /* Inhalt an dieser Position lesen */
             ulI = fgetc (file) ;
             /* Ist HLP bzw. INF Kennung vorhanden */
             if   ((ulI == HLP2INF_HLP) ||
                   (ulI == HLP2INF_INF))
                  {
                  /* Ja, wieder an die dritte Stelle fahren */
                  fseek (file,
                         3,
                         SEEK_SET) ;
                  /* Was soll den gemacht werden? */
                  switch (ulArgument)
                     {
                     /* INF --> HLP? */
                     case HLP2INF_HLP:
                          /* Ja, Byte umschiessen */
                          fputc (HLP2INF_HLP,
                                 file) ;
                          break ;
                     /* HLP --> INF? */
                     case HLP2INF_INF:
                          /* Ja, Byte umschiessen */
                          fputc (HLP2INF_INF,
                                 file) ;
                          break ;
                     }
                  }
             else
                  {
                  /* Datei wieder schliessen */
                  fclose (file) ;
                  /* Die Datei enthaelt nicht die INF bzw. HLP Kennung */
                  fprintf (stdout,
                           "\nHLP2INF Fehler 4: Datei ist keine *.HLP bzw. keine *.INF Datei\n") ;
                  /* Ausgabe Funktionsbanner */
                  Banner () ;
                  /* Programmabbruch */
                  return 4 ;
                  }
             }
        else
             {
             /* Datei wieder schliessen */
             fclose (file) ;
             /* Die Datei konnte nicht gelesen werden */
             fprintf (stdout,
                      "\nHLP2INF Fehler 5: Datei konnte nicht gelesen werden\n") ;
             /* Ausgabe Funktionsbanner */
             Banner () ;
             /* Programmabbruch */
             return 5 ;
             }
        /* Datei wieder schliessen */
        fclose (file) ;
        /* Programmende */
        return 0 ;
        }
   else
        {
        /* Datei konnte nicht geoeffnet werden */
        fprintf (stdout,
                 "\nHLP2INF Fehler 6: Fehler beim ™ffnen der Datei\n") ;
        /* Ausgabe Funktionsbanner */
        Banner () ;
        /* Programmabbruch */
        return 6 ;
        }
   }

/* Funktion zur Ausgabe der korrekten Syntax */
VOID Banner (VOID)
   {
   /* Korrekte Syntax ausgeben */
   fprintf (stdout,
            "%s%s%s%s",
            "\nHLP2INF Syntax:    HLP2INF -i|-h -fDateiname\n\n",
            "         Parameter: -h Generiere *.HLP Format\n",
            "                    -i Generiere *.INF Format\n",
            "                    -fDateiname\n") ;
   }


