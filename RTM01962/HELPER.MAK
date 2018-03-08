# $Header: D:/Projekte/Kleine\040Helferlein/Source/RCS/HELPER.MAK 1.1 1995/10/31 17:45:15 HaWi Exp $

.all: \
   HELPER.EXE \
   HELPER.DLL

HELPER.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

HELPER.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Oc /Si /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DUMMY.OBJ: \
   $*.C \
   HELPER.MAK
      ICC.EXE %s

HELPER.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
        /A:1 /BASE:65536 /NOE /NOFREE /NOI /NOLOGO /PACKD:0xFfffFfff /PM:PM +
        $*.OBJ
        $*.EXE

        OS2386
        $*.DEF;
<<

HELPER.DLL: \
   DUMMY.OBJ \
   $*.RES \
   HELPER2.DEF \
   $*.MAK
      ILINK.EXE @<<
         DUMMY.OBJ
         $*.DLL

         OS2386
         HELPER2.DEF;
<<
      RC.EXE $*.RES $*.DLL


