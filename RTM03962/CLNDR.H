/* $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/CLNDR.H 1.1 1996/02/28 17:03:55 HaWi Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _CLNDR_H
   #define _CLNDR_H

   /* Klassenname des Kalender Kontrollelementes */
   #define AXK_CLASSNAME "CLNDR"

   /* Einige fehlende Message Makros */
   #define MRFROMSH2CH(s, uch1, uch2) ((MRESULT)MAKELONG(s, MAKESHORT(uch1, uch2)))
   #define CHAR1FROMMR(mr)            ((UCHAR)((ULONG)mr))
   #define CHAR2FROMMR(mr)            ((UCHAR)((ULONG)mr >> 8))
   #define CHAR3FROMMR(mr)            ((UCHAR)((ULONG)mr >> 16))
   #define CHAR4FROMMR(mr)            ((UCHAR)((ULONG)mr >> 24))

   /* Private Messages */
   #define WMPAX            (WM_USER + 1070)
   #define WMPAX_INSERTAPP  (WMPAX + 0)
   #define WMPAX_INVALIDATE (WMPAX + 1)
   #define WMPAX_QUERYDATE  (WMPAX + 2)
   #define WMPAX_SETDATE    (WMPAX + 7)

   /* Soll beim Einfuegen von Terminen der Kalender invalidiert werden? */
   #define AK_INSERTIGNORE     0
   #define AK_INSERTINVALIDATE 1

   /* An welchem Wochentag soll der Kalender starten */
   #define AK_STYLEIGNORE    0
   #define AK_STYLEMONDAY    1
   #define AK_STYLETUESDAY   2
   #define AK_STYLEWEDNESDAY 3
   #define AK_STYLETHURSDAY  4
   #define AK_STYLEFRIDAY    5
   #define AK_STYLESATURDAY  6
   #define AK_STYLESUNDAY    7

   /* Parameter zum Steuern beim Datumswechsel */
   #define AK_DATEIGNORE    0
   #define AK_DATEFULL      1
   #define AK_DATENEXTMONTH 2
   #define AK_DATEPREVMONTH 3
   #define AK_DATENEXTYEAR  4
   #define AK_DATEPREVYEAR  5

   /* WM_CONTROL Notifications des Kalenders */
   #define AXN_ENTER  1
   #define AXN_SELECT 2

   /* Konstanten der Listenverwaltung */
   #define TXK_ERROR   FALSE
   #define TXK_NOERROR TRUE

   #define TXK_FIRST  1
   #define TXK_INDEX  2
   #define TXK_INSERT 3
   #define TXK_KEY    4
   #define TXK_LAST   5
   #define TXK_STRING 6
   #define TXK_ULONG  7

   /* Funktionsdeklarationen */
   BOOL EXPENTRY ARegisterClass (HAB) ;

#endif /* _CLNDR_H */

#ifdef __cplusplus
   }
#endif

