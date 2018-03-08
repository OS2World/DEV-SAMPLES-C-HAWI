# $Header: D:\projekte\fensterln mit os!2\source\RCS\PM03.MAK 1.1 1995/05/25 15:34:00 HaWi Exp $

.all: \
  PM03.EXE

PM03.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

PM03.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

PM03.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 /BASE:65536 +
         $*.OBJ
         $*.EXE
         
         OS2386
         $*.DEF;
<<
      rc.exe $*.RES $*.EXE
