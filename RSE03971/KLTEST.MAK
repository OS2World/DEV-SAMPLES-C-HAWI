# $Header: Y:/Projekte/Redaktion SE/Knopfleiste/Source/rcs/KLTEST.MAK 1.1 1996/12/29 13:49:44 HaWi Exp $

.all: \
   KLTEST.EXE

KLTEST.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

KLTEST.OBJ: \
    KLEISTE.H \
    $*.C \
    $*.H \
    $*.MAK
       ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

KLTEST.EXE: \
   {$(LIB)}OS2386.LIB \
   KLEISTE.LIB \
   $*.OBJ \
   $*.RES \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /BASE:65536 /NOE /NOL /PACKD:0xFfffFfff /PM:PM $*.OBJ OS2386.LIB KLEISTE.LIB $*.DEF
      RC.EXE -x $*.RES $*.EXE

