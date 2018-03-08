# $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKETEST.MAK 1.1 1995/05/25 18:46:32 HaWi Exp $

.all: \
   CAKETEST.EXE

CAKETEST.res: \
   $*.RC \
   $*.H \
   CAKE.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

CAKETEST.obj: \
   $*.C \
   $*.H \
   CAKE.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

CAKETEST.EXE: \
   $*.obj \
   $*.res \
   {$(LIB)}OS2386.lib \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 /BASE:65536 +
         $*.obj
         $*.EXE
        
         OS2386 +
         CAKE
         $*.DEF;
<<
      rc.exe $*.res $*.EXE
