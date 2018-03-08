# $Header: D:/projekte/blasen\040im\040os!2/source/RCS/TSTBLASE.MAK 1.1 1995/07/02 13:41:55 HaWi Exp $

.all: \
   TSTBLASE.EXE \
   TSTBLASE.DLL

TSTBLASE.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

TSTBLASE.OBJ: \
   $*.C \
   $*.H \
   BLASE.H \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /C %s

DUMMY.OBJ: \
   $*.C \
   TSTBLASE.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Fi /Si /Gm /Ge- /C %s

TSTBLASE.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   BLASE.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
        /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 /BASE:65536 +
        $*.OBJ
        $*.EXE

        OS2386 +
        BLASE
        $*.DEF;
<<

TSTBLASE.DLL: \
   DUMMY.OBJ \
   $*.RES \
   TSTBLAS2.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 +
         DUMMY.OBJ
         $*.DLL

         OS2386
         TSTBLAS2.DEF;
<<
      rc.exe -x2 $*.RES $*.DLL

