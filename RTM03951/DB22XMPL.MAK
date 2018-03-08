# $Header: D:\projekte\gib mir die daten.1\source\RCS\DB22XMPL.MAK 1.1 1995/05/25 15:53:50 HaWi Exp $

.all: \
   DB22XMPL.EXE

DB22XMPL.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

DB22XMPL.OBJ: \
   $*.C \
   DB22.H \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

DB22XMPL.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   DB22.LIB\
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:4 /BASE:65536 +
         $*.OBJ
         $*.EXE
         
         OS2386 +
         DB22
         $*.DEF;
<<
      rc.exe $*.RES $*.EXE
