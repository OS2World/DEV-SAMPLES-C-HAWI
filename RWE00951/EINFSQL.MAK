# $Header: D:\projekte\einfach sql\source\RCS\EINFSQL.MAK 1.1 1995/05/25 15:29:58 HaWi Exp $

.all: \
  EINFSQL.EXE

EINFSQL.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc -r $*.RC $*.RES

EINFSQL.C: \
   $*.SQC \
   $*.H \
   $*.MAK
      stopdbm
      startdbm
      sqlprep /B /P /# $*.SQC ADRESSE

EINFSQL.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

EINFSQL.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 /BASE:65536 +
         $*.OBJ
         $*.EXE
         
         OS2386 +
         SQL_DYN
         $*.DEF;
<<
      rc $*.RES $*.EXE
