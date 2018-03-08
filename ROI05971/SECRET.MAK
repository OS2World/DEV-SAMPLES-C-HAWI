# $Header: y:/projekte/redaktion os!2 inside/Ganz geheim/Source/rcs/SECRET.MAK 1.1 1997/03/07 16:00:02 HAWI Exp $

.all: \
  SECRET.EXE

SECRET.RES: \
   $*.RC \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

SECRET.OBJ: \
   $*.C \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

SECRET.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

