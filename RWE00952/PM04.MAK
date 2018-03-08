# $Header: D:\projekte\fensterln mit os!2\source\RCS\PM04.MAK 1.1 1995/05/25 15:34:55 HaWi Exp $

.all: \
  PM04.EXE

PM04.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

PM04.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

PM04.EXE: \
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
