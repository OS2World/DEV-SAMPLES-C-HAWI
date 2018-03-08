# $Header: D:/PROJEKTE/Kleine Helferlein!1/Source/rcs/HLP2INF.MAK 1.1 1995/11/28 13:12:29 HaWi Exp $

.all: \
   HLP2INF.EXE

HLP2INF.OBJ: \
   $*.C \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

HLP2INF.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:VIO +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<


