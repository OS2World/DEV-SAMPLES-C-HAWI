# $Header: D:/Projekte/Internationalisierte Anwendungen/Source/rcs/CLNDR.MAK 1.1 1996/02/28 17:03:55 HaWi Exp $

.all: \
   CLNDR.DLL \
   CLNDR.LIB

CLNDR.OBJ: \
   $*.C \
   $*.H \
   $*.PH \
   $*.MAK
      ICC.EXE /C /Ge- /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

CLNDR.LIB: \
   $*.DEF \
   $*.MAK
      IMPLIB.EXE /NOLOGO $*.LIB $*.DEF
      ILIB /C /NOBA /NOBR /NOE /NOLOGO $*.LIB;

CLNDR.DLL: \
   $*.OBJ \
   {$(LIB)}OS2386.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /E:2 /NOD: /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.DLL

         OS2386
         $*.DEF;
<<

