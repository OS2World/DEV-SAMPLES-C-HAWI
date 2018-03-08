# $Header: D:\Projekte\Redaktion Toolbox\An der langen Leine\Source\RCS\NSDDE.MAK 1.0 1997/08/29 16:05:48 HAWI Exp $

.all: \
   NSDDE.EXE

NSDDE.RES: \
   $*.H \
   $*.RC \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

NSDDE.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

NSDDE.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOD: /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

