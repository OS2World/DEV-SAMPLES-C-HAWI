# $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/NATIONAL.MAK 1.1 1996/02/28 17:03:56 HaWi Exp $

.all: \
   NATIONAL.EXE \
   NATIONAL.GRL \
   NATIONAL.UKL


DUMMY.OBJ: \
   $*.C \
   NATIONAL.MAK
      ICC.EXE %s

NATIONAL.GRS: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r -DLANGUAGE=1 $*.RC $*.GRS

NATIONAL.UKS: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r -DLANGUAGE=2 $*.RC $*.UKS

NATIONAL.OBJ: \
   $*.C \
   CLNDR.H \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

NATIONAL.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   CLNDR.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOD: /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386 +
         CLNDR
         $*.DEF;
<<

NATIONAL.GRL: \
   DUMMY.OBJ \
   $*.GRS \
   NATIONA2.DEF \
   $*.MAK
      ILINK.EXE @<<
         DUMMY.OBJ
         $*.DLL


         NATIONA2.DEF;
<<
      COPY $*.DLL $*.GRL
      COPY $*.GRS $*.RES
      RC.EXE -x2 $*.RES $*.GRL

NATIONAL.UKL: \
   DUMMY.OBJ \
   $*.UKS \
   NATIONA2.DEF \
   $*.MAK
      ILINK.EXE @<<
         DUMMY.OBJ
         $*.DLL


         NATIONA2.DEF;
<<
      COPY $*.DLL $*.UKL
      COPY $*.UKS $*.RES
      RC.EXE -x2 $*.RES $*.UKL

