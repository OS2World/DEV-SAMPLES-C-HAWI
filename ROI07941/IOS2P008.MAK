# $Header: D:\projekte\dde - das dicke ende\source\RCS\IOS2P008.MAK 1.1 1995/05/25 15:25:14 HaWi Exp $

.all: \
   IOS2P008.EXE

IOS2P008.OBJ: \
   $*.C \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /C %s

IOS2P008.EXE: \
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
