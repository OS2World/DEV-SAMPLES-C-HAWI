# $Header: D:\Projekte\Redaktion SE\DBase intim\Source\RCS\DBASETST.MAK 1.1 1997/04/18 21:41:24 HAWI Exp $

.all: \
   DBASETST.EXE

DBASETST.OBJ: \
    DBASE.H \
    $*.C \
    $*.MAK
       ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DBASETST.EXE: \
   {$(LIB)}OS2386.LIB \
   DBASE.LIB \
   $*.OBJ \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /BASE:65536 /NOE /NOL /PACKD:0xFfffFfff $*.OBJ OS2386.LIB DBASE.LIB $*.DEF


