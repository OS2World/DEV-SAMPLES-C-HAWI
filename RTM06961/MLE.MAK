# $Header: D:/Projekte/Tief im MLE!1/Source/rcs/MLE.MAK 1.1 1996/04/16 21:03:38 HaWi Exp HaWi $

.all: \
   MLE.EXE

MLE.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

MLE.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
       ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

MLE.EXE: \
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
      RC.EXE -x2 $*.RES $*.EXE

