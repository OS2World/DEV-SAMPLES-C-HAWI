# $Header: D:/Projekte/SOM!WPS Inside!1/Source/rcs/ADRESSEF.MAK 1.1 1996/04/10 13:27:45 HaWi Exp $

.all: \
   ADRESSEF.DLL

ADRESSEF.RES: \
   $*.RC \
   $*.ICO \
   $*.IH \
   $*.MAK
      rc.exe -r $*.RC $*.RES

ADRESSEF.OBJ: \
   $*.C \
   $*.IH \
   $*.MAK
      icc.exe /C /Ge- /Gm /O /Oc /Ss /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

ADRESSEF.DLL: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   {$(LIB)}SOMTK.LIB \
   $*.DEF \
   $*.MAK
      ilink.exe @<<
         /A:1 /E:2 /NOD: /NOE /NOFREE /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.DLL

         OS2386 +
         SOMTK
         $*.DEF;
<<
      rc.exe -x2 $*.RES $*.DLL

