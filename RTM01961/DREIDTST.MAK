# $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREIDTST.MAK 1.1 1995/10/04 13:07:44 HaWi Exp $

.all: \
   DREIDTST.EXE

DREIDST.RES: \
   $*.RC \
   $*.H \
   DREID.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

DREIDTST.OBJ: \
   $*.C \
   $*.H \
   DREID.H \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Oc /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

DREIDTST.EXE: \
   $*.OBJ \
   $*.RES \
   DREID.LIB \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386 +
         DREID
         $*.DEF;
<<
      RC.EXE $*.RES $*.EXE


