# $Header: D:\Projekte\Redaktion OS!2 Inside\Auswertungen satt\Source\RCS\AUSWERT1.MAK 1.1 1997/05/03 16:58:26 HAWI Exp $

.all: \
   AUSWERT1.EXE

AUSWERT1.OBJ: \
   $*.C \
   $*.MAK
      ICC.EXE /C /Gm /Q /Ti /Wcnddcleffgeniniobsordparprorearettruuse %s

AUSWERT1.EXE: \
   $*.OBJ \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /DE /E:2 /NOE /NOL /PACKD $*.OBJ D:\Progs\XACT\XACTRPC\XACTRPC.LIB OS2386.LIB $*.DEF

