# $Header: D:/projekte/wps\040security/source/RCS/DESKSECU.MAK 1.1 1995/06/13 20:08:02 HaWi Exp $

.all: \
   DESKSECU.DLL

DESKSECU.C: \
   $*.IDL \
   $*.MAK
      sc.exe -C128000 -S128000 $*.IDL

DESKSECU.OBJ: \
   $*.C \
   $*.IH \
   $*.MAK
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

DESKSECU.DLL: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   {$(LIB)}SOMTK.LIB \
   $*.DEF \
   $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /NOI /NOLOGO /A:1 +
         $*.OBJ
         $*.DLL
        
         OS2386 +
         SOMTK
         $*.DEF;
<<
