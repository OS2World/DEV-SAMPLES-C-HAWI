# $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF02.MAK 1.1 1996/01/26 09:50:50 HaWi Exp $

.all: \
   PMEINF02.EXE

PMEINF02.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

PMEINF02.OBJ: \
    $*.C \
    $*.H \
    $*.MAK
       ICC.EXE /C /Fi /Gm /Si /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Oc %s

PMEINF02.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<
      RC.EXE $*.RES $*.EXE


