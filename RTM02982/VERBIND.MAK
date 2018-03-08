# $Header: D:\Projekte\Redaktion Toolbox\Verbindungen\Source\RCS\VERBIND.MAK 1.1 1997/11/26 21:39:24 HAWI Exp $

.all: \
   VERBIND.EXE

VERBIND.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      rc.exe -r $*.RC $*.RES

VERBIND.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

VERBIND.EXE: \
   $*.DEF \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x2 $*.RES $*.EXE

