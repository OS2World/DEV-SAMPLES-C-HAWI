# $Header: D:\projekte\fensterln mit os!2\source\RCS\PM01.MAK 1.1 1995/05/25 15:32:10 HaWi Exp $

.all: \
  PM01.EXE

PM01.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

PM01.EXE: \
   $*.OBJ \
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
