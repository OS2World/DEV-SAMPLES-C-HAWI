.all: \
  HWITERF.DLL

HWITERF.res: \
    $*.RC \
    $*.ico \
    $*.ih \
    $*.MAK
         rc.exe -r $*.RC $*.RES

HWITERF.c: \
    $*.idl \
    $*.MAK
      sc.exe -C128000 -S128000 $*.IDL

HWITERF.obj: \
    $*.c \
    $*.ih \
    $*.MAK
         icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

HWITERF.DLL: \
    $*.obj \
    $*.res \
    {$(LIB)}OS2386.lib \
    {$(LIB)}SOMTK.lib \
    $*.DEF \
    $*.MAK
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 +
        $*.obj
        $*.DLL
        
        OS2386 +
        SOMTK
        $*.DEF;
<<
      rc.exe -x2 $*.res $*.DLL
