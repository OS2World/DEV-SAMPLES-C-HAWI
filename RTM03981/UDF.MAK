# $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\UDF.MAK 1.1 1998/01/16 21:21:10 HAWI Exp $

.all: \
   UDF.DLL \
   UDF.LIB

UDF.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Ge- /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

UDF.LIB: \
   $*.DEF \
   $*.MAK
      IMPLIB.EXE /NOLOGO $*.LIB $*.DEF
      ILIB /C /NOBA /NOBR /NOE /NOLOGO $*.LIB;

UDF.DLL: \
   $*.OBJ \
   {$(LIB)}DB2API.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOD: /NOE /NOL /PACKD $*.OBJ DB2API.LIB $*.DEF

