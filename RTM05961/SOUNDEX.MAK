# $Header: D:/Projekte/Kleine Helferlein!3/Source/rcs/SOUNDEX.MAK 1.2 1996/04/29 13:45:11 HaWi Exp $

.all: \
   SOUNDEX.EXE

SOUNDEX.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

SOUNDEX.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /Q /O /Oc /Wcnddcleffgeniniobsordparprorearettruuse %s

SOUNDEX.EXE: \
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