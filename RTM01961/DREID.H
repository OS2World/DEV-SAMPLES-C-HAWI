/* $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREID.H 1.2 1995/10/04 13:06:28 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DREID_H
   #define _DREID_H

   /* Klassenname fÅr den PM */
   #define DREID_CLASSNAME "DREID"

   /* Basis fÅr private PM Messages */
   #define WMP (WM_USER + 1000)

   /* Optionen dieser Klasse */
   #define DREID_BOTTOM   1
   #define DREID_CENTER   2
   #define DREID_DISABLED 4
   #define DREID_IN       8
   #define DREID_LEFT     16
   #define DREID_OUT      32
   #define DREID_RIGHT    64
   #define DREID_TOP      128
   #define DREID_VISIBLE  256

   /* Deklaration der Registrierfunktion */
   BOOL EXPENTRY DREIDRegister (HAB) ;

#endif /* _DREID_H */

#ifdef __cplusplus
   }
#endif


