/* $Header: y:/projekte/redaktion os!2 inside/Ganz geheim/Source/rcs/GENERATE.C 1.2 1997/03/07 15:57:51 HAWI Exp $ */

/* OS/2 Kopfdateien */
#define  INCL_BASE
#define  INCL_PM
#include <os2.h>

/* C Kopfdateien */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TXK_ERROR   FALSE
#define TXK_NOERROR TRUE
   
BOOL TRegister (HINI, PSZ, PSZ, PULONG) ;

INT main (INT argc,
          PSZ argv [])
   {
   BOOL  f ;
   PSZ   apsz [] = { "Secret" } ;
   PSZ   pszHeader = "\n Name                       Shareware        Code\n ------------------------------------------------" ;
   ULONG ul ;
   ULONG ul2 ;

   f = FALSE ;
   /* Gibt es Parameter */
   if   (argc == 3)
        {
        /* Welche Shareware soll registriert werden */
        for  (ul = 0; ul < sizeof (apsz) / sizeof (apsz [0]); ul++)
             {
             /* Gefunden? */
             if   (strcmp (apsz [ul],
                           argv [1]) == 0)
                  {
                  f = TRUE ;
                  /* Registriercodes berechnen */
                  TRegister (NULLHANDLE,
                             argv [1],
                             argv [2],
                             &ul2) ;
                  /* Registrierdaten ausgeben */
                  fprintf (stdout,
                           " %s\n %-26s %-14s %6lu\n",
                           pszHeader,
                           argv [2],
                           apsz [ul],
                           ul2) ;
                  break ;
                  }
             }
        }
   /* Im Fehlerfall Banner ausgeben */
   if   (!f)
        {
        fprintf (stdout,
                 "\nGENERATE.EXE Application RegisterName\n") ;
        }
   return 0 ;
   }

/* Diese Funktion muss in jeder Sharewareanwendungen enthalten sein und berechnet einen Registriercode */
BOOL TRegister (HINI   hini,
                PSZ    pszApplication,
                PSZ    pszName,
                PULONG pul)
   {
   CHAR  sz [128] ;
   CHAR  sz2 [128] ;
   ULONG ulCode ;
   ULONG ulI ;
   ULONG ulL ;
   ULONG ulN ;

   /* Wenn HINI Åbergeben dann Daten aus INI auslesen, ansonsten nur Berechnung */
   if   (hini)
        {
        /* Wurde Name Åbergeben? */
        if   (pszName)
             {
             /* Ja, in INI eintragen */
             PrfWriteProfileString (hini,
                                    pszApplication,
                                    "RegisterName",
                                    pszName) ;
             }
        /* Wurde Code Åbergeben? */
        if   (pul)
             {
             /* Ja */
             _ltoa (*pul,
                    sz2,
                    10) ;
             /* In INI eintragen */
             PrfWriteProfileString (hini,
                                    pszApplication,
                                    "RegisterCode",
                                    sz2) ;
             }
        /* Programmname */
        strcpy (sz,
                pszApplication) ;
        /* Name wieder auslesen */
        PrfQueryProfileString (hini,
                               pszApplication,
                               "RegisterName",
                               " ",
                               sz2,
                               sizeof (sz2)) ;
        /* Programmname und Benutzername konkatinieren */                       
        strcat (sz,
                sz2) ;
        /* Code wieder auslesen */        
        PrfQueryProfileString (hini,
                               pszApplication,
                               "RegisterCode",
                               "0",
                               sz2,
                               sizeof (sz2)) ;
        ulCode = atol (sz2) ;
        }
   else
        {
        strcpy (sz,
                pszApplication) ;
        strcat (sz,
                pszName) ;
        ulCode = *pul ;
        }
   /* LÑnge des Applikationsnamens */
   ulL = strlen (sz) ;
   if   (ulL >= 5)
        {
        /* Startwert */
        ulN = 123456 ;
        /* FÅr jedes Zeichen (Programmname und Benutzername */
        for  (ulI = 0; ulI < ulL; ulI++)
             {
             /* Multiplizieren */
             ulN *= 421 ;
             /* Ein bisschen Zeichenarithmetik */
             ulN += 54773 + sz [ulI] ;
             ulN %= 259200 ;
             }
        }
   else
        {
        return TXK_ERROR ;
        }
   if   (hini)
        {
        if   (ulCode != ulN)
             {
             return TXK_ERROR ;
             }
        }
   if   (pul)
        {
        *pul = ulN ;
        }
   return TXK_NOERROR ;
   }

