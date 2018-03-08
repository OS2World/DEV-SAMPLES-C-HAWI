# $Header: D:/Projekte/Viel System!1/Source/rcs/SYSLVL.MAK 1.1 1995/11/26 15:22:45 HaWi Exp $

.all: \
   SYSLVL.EXE

SYSLVL.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

SYSLVL.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Oc /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

SYSLVL.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<
      RC.EXE -x2 $*.RES $*.EXE


