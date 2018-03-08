# $Header: D:\projekte\versteckte informationen\source\RCS\IOS2P009.MAK 1.1 1995/05/25 19:01:31 HaWi Exp $

.all: \
  .\IOS2P009.EXE

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /C %s

.\IOS2P009.EXE: \
    .\IOS2P009.obj \
    .\IOS2P009.res \
    {$(LIB)}OS2386.lib \
    {$(LIB)}IOS2P009.DEF \
    IOS2P009.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 /BASE:65536 +
        .\IOS2P009.obj
        IOS2P009.EXE
        
        OS2386
        IOS2P009.DEF;
<<
      @echo WF::BIND::Resource Bind
      rc.exe -x2 .\IOS2P009.res IOS2P009.EXE

.\IOS2P009.res: \
    "D:\Desktop\Projekte\Versteckte Informationen\Source\IOS2P009.RC" \
    {$(INCLUDE)}IOS2P009.H \
    IOS2P009.MAK

.\IOS2P009.obj: \
    "D:\Desktop\Projekte\Versteckte Informationen\Source\IOS2P009.C" \
    {"D:\Desktop\Projekte\Versteckte Informationen\Source;$(INCLUDE);"}IOS2P009.H \
    IOS2P009.MAK
