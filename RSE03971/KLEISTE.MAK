# $Header: Y:/Projekte/Redaktion SE/Knopfleiste/Source/rcs/KLEISTE.MAK 1.1 1996/12/29 13:49:41 HaWi Exp $

.all: \
   KLEISTE.DLL \
   KLEISTE.LIB

KLEISTE.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

KLEISTE.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Ge- /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

KLEISTE.LIB: \
   $*.DEF \
   $*.MAK
      IMPLIB.EXE /NOLOGO $*.LIB $*.DEF
      ILIB /C /NOBA /NOBR /NOE /NOLOGO $*.LIB;

KLEISTE.DLL: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}\OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOE /NOL /PACKD $*.OBJ OS2386.LIB $*.DEF
      RC.EXE -x $*.RES $*.DLL
      
