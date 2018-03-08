# $Header: D:/PROJEKTE/Fensterln mit OS!2!1/Source/rcs/PMEINF01.MAK 1.1 1996/01/26 09:50:29 HaWi Exp $

.all: \
   PMEINF01.EXE

PMEINF01.OBJ: \
    $*.C \
    $*.H \
    $*.MAK
       ICC.EXE /C /Fi /Gm /Si /Q /Wcnddcleffgeniniobsordparprorearettruuse /O /Oc %s

PMEINF01.EXE: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386
         $*.DEF;
<<


