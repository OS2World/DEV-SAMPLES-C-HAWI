# $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBARTEST.MAK 1.1 1995/05/25 15:38:05 HaWi Exp $

.all: \
  SBARTEST.EXE

SBARTEST.OBJ: \
   $*.C \
   $*.H \
   SBAR.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

SBARTEST.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   SBAR.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 /BASE:65536 +
         $*.OBJ
         $*.EXE
         
         OS2386 +
         SBAR
         $*.DEF;
<<
