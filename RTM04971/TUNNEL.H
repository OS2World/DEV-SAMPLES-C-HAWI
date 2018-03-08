/* $Header: D:\Projekte\Redaktion Toolbox\Getunnelt\Source\RCS\TUNNEL.H 1.1 1997/04/27 16:29:57 HAWI Exp $ */

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _TUNNEL_H
   #define _TUNNEL_H

   /* Fuer 32 Bit PM */
   #ifndef _MSG_
      #define _MSG_
      typedef unsigned long MSG ;
   #endif

   /* Instanzdaten des Dialoges */
   typedef struct
      {
      HWND        hwndApplet ;
      HWND        hwndTunnel ;
      JEAPPLETHND hndApplet ;
      } DLG, *PDLG ;

   /* Prozedurdeklarationen */
   INT              main (VOID) ;
   MRESULT EXPENTRY DlgWndProc (HWND, MSG, MPARAM, MPARAM) ; 
   
   /* Konstantdeklarationen */
   #define IDD   1000
   #define IDT_1 1001

#endif /* _TUNNEL_H */

#ifdef __cplusplus
   }
#endif

