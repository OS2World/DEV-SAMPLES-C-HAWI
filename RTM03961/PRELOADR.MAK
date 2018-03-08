# $Header: D:/Projekte/Kleine Helferlein!2/Source/rcs/PRELOADR.MAK 1.1 1996/03/05 09:37:28 HaWi Exp $

.all: \
   PRELOADR.EXE

PRELOADR.OBJ: \
   $*.C \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

PRELOADR.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOD: /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:VIO +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<

