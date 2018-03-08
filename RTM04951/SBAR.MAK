# $Header: D:\projekte\frame extensions unter os!2\source\RCS\SBAR.MAK 1.1 1995/05/25 15:37:38 HaWi Exp $

.all: \
   SBAR.DLL \
   SBAR.LIB

SBAR.OBJ: \
   $*.C \
   $*.H \
   $*.PH \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

SBAR.LIB: \
   $*.DEF \
   $*.MAK
      implib.exe /NOLOGO $*.LIB $*.DEF

SBAR.DLL: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 +
         $*.OBJ
         $*.DLL
         
         OS2386
         $*.DEF;
<<
