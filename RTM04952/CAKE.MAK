# $Header: D:\projekte\kuchengrafik unter os!2\source\RCS\CAKE.MAK 1.1 1995/05/25 18:46:08 HaWi Exp $

.all: \
   CAKE.DLL \
   CAKE.LIB

CAKE.obj: \
   $*.C \
   $*.H \
   $*.PH \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

CAKE.LIB: \
   $*.DEF \
   $*.MAK
      implib.exe /NOLOGO $*.LIB $*.DEF

CAKE.DLL: \
   $*.OBJ \
   {$(LIB)}OS2386.lib \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 +
         $*.OBJ
         $*.DLL
        
         OS2386
         $*.DEF;
<<
