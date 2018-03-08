# $Header: D:/Projekte/SOM!WPS Inside!1/Source/rcs/ADRESSE.MAK 1.1 1996/04/10 13:27:43 HaWi Exp $

.all: \
   ADRESSE.DLL

ADRESSE.RES: \
   $*.RC \
   $*.ICO \
   $*.IH \
   $*.RH \
   $*.MAK
      rc.exe -r $*.RC $*.RES

ADRESSE.OBJ: \
   $*.C \
   $*.IH \
   $*.RH \
   $*.MAK
      icc.exe /C /Ge- /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

ADRESSE.DLL: \
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
      rc.exe -x2 $*.res $*.DLL

