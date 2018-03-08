# $Header: y:/projekte/redaktion os!2 inside/Ganz geheim/Source/rcs/GENERATE.MAK 1.1 1997/03/07 15:30:58 HAWI Exp $

.all: \
   GENERATE.EXE

GENERATE.OBJ: \
   $*.C \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

GENERATE.EXE: \
   $*.OBJ \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF

