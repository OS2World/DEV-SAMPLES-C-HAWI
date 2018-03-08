# $Header: D:/Projekte/Attraktive Attribute/Source/rcs/EAEDITOR.MAK 1.1 1996/06/01 10:41:28 HaWi Exp $

.all: \
   EAEDITOR.EXE

EAEDITOR.RES: \
   $*.H \
   $*.RC \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

EAEDITOR.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /G3 /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

EAEDITOR.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOD: /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

