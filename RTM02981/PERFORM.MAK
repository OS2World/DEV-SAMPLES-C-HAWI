# $Header: D:\Projekte\Redaktion Toolbox\Geschwindigkeitsrausch\Source\RCS\PERFORM.MAK 1.2 1997/11/30 14:59:41 HAWI Exp $

.all: \
   PERFORM.EXE

PERFORM.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

PERFORM.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

PERFORM.EXE: \
   $*.DEF \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

