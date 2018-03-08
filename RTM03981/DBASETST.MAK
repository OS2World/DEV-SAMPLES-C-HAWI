# $Header: D:\Projekte\Redaktion Toolbox\Eingeschleust\Source\RCS\DBASETST.MAK 1.1 1998/01/17 22:10:43 HAWI Exp $

.all: \
   DBASETST.EXE

DBASETST.OBJ: \
    UDF.H \
    $*.C \
    $*.MAK
       ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DBASETST.EXE: \
   UDF.LIB \
   $*.OBJ \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /BASE:65536 /NOE /NOL /PACKD:0xFfffFfff $*.OBJ UDF.LIB $*.DEF

