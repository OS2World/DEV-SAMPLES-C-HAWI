# $Header: D:/Projekte/Hin und Her/Source/rcs/DROPTEST.MAK 1.1 1996/06/23 21:42:35 HaWi Exp $

.all: \
   DROPTEST.EXE

DROPTEST.RES: \
   $*.H \
   $*.RC \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

DROPTEST.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

DROPTEST.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

