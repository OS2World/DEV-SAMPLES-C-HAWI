# $Header: Y:/Projekte/Redaktion OS!2 Inside/Gute Verbindung/Source/rcs/FTPEXMPL.MAK 1.1 1997/01/03 09:49:54 HaWi Exp $

.all: \
   FTPEXMPL.EXE

FTPEXMPL.RES: \
   $*.H \
   $*.RC \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

FTPEXMPL.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

FTPEXMPL.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   {$(LIB)}\FTPAPI.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /IG /NOE /NOL /PACKD $*.OBJ OS2386.LIB FTPAPI.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

