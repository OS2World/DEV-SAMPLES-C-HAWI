# $Header: D:\Projekte\Redaktion SE\DBase intim\Source\RCS\DBASE.MAK 1.1 1997/04/18 21:41:09 HAWI Exp $

.all: \
   DBASE.DLL \
   DBASE.LIB

DBASE.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Ge- /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DBASE.LIB: \
   $*.DEF \
   $*.MAK
      IMPLIB.EXE /NOLOGO $*.LIB $*.DEF
      ILIB /C /NOBA /NOBR /NOE /NOLOGO $*.LIB;

DBASE.DLL: \
   $*.OBJ \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF

