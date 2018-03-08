# $Header: D:/Projekte/Exploring WebExplorer/Source/rcs/WEBEX.MAK 1.2 1996/06/23 15:11:02 HaWi Exp $

.all: \
   WEBEX.EXE

WEBEX.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

WEBEX.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /G3 /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

WEBEX.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   {$(LIB)}WEBEXWIN.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOD: /NOE /NOL /PACKD $*.OBJ OS2386.LIB WEBEXWIN.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

