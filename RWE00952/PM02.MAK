# $Header: D:\projekte\fensterln mit os!2\source\RCS\PM02.MAK 1.1 1995/05/25 15:33:01 HaWi Exp $

.all: \
  PM02.EXE

PM02.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

PM02.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

PM02.EXE: \
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
