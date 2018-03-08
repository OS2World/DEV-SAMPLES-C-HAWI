# $Header: D:/Projekte/Alles\040geregelt/Source/RCS/DREID.MAK 1.1 1995/10/04 12:08:06 HaWi Exp $

.all: \
   DREID.DLL \
   DREID.LIB

DREID.RES: \
   $*.RC \
   $*.H \
   $*.PH \
   $*.MAK
      RC.EXE -r %s

DREID.OBJ: \
   $*.C \
   $*.H \
   $*.PH \
   $*.MAK
      ICC.EXE /C /Fi /Ge- /Gm /O /Oc /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

DREID.LIB: \
   $*.DEF \
   $*.MAK
      IMPLIB.EXE /NOLOGO $*.LIB $*.DEF
      ILIB /C /NOBA /NOBR /NOE /NOLOGO $*.LIB;

DREID.DLL: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.DLL

         OS2386
         $*.DEF;
<<
      RC $*.RES $*.DLL


