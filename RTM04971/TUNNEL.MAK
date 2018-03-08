# $Header: D:\Projekte\Redaktion Toolbox\Getunnelt\Source\RCS\TUNNEL.MAK 1.2 1997/04/27 16:30:57 HAWI Exp $

.all: \
   TUNNEL.EXE

TUNNEL.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

TUNNEL.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

TUNNEL.EXE: \
   $*.DEF \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   {$(LIB)}\JEMPO10.LIB \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB JEMPO10.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

