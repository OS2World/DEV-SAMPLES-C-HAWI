# $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/DRIVES.MAK 1.1 1996/03/05 18:04:09 HaWi Exp $

.all: \
   DRIVES.EXE


DRIVES.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

DRIVES.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DRIVES.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOD: /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<
      RC.EXE -x2 $*.RES $*.EXE

