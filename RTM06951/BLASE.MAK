# $Header: D:/projekte/blasen\040im\040os!2/source/RCS/BLASE.MAK 1.1 1995/07/02 13:43:22 HaWi Exp $

.all: \
   BLASE.DLL \
   BLASE.LIB

BLASE.RES: \
   $*.RC \
   $*.H \
   $*.PH \
   $*.MAK
      rc.exe -r $*.RC $*.RES

BLASE.OBJ: \
   $*.C \
   $*.H \
   $*.PH \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

BLASE.LIB: \
   $*.DEF \
   $*.MAK
      implib.exe /NOLOGO $*.LIB $*.DEF

BLASE.DLL: \
   $*.OBJ \
   $*.RES \
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
   rc.exe -x2 $*.RES $*.DLL

