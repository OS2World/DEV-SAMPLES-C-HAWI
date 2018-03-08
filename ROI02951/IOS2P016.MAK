# $Header: D:\projekte\dde - die zweite\source\RCS\IOS2P016.MAK 1.1 1995/05/25 15:27:31 HaWi Exp $

.all: \
  .\IOS2P016.EXE

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /C %s

.\IOS2P016.EXE: \
    .\IOS2P016.obj \
    .\IOS2P016.res \
    {$(LIB)}OS2386.lib \
    {$(LIB)}IOS2P016.DEF \
    IOS2P016.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 /BASE:65536 +
        .\IOS2P016.obj
        IOS2P016.EXE
        
        OS2386
        IOS2P016.DEF;
<<
      @echo WF::BIND::Resource Bind
      rc.exe -x2 .\IOS2P016.res IOS2P016.EXE

.\IOS2P016.res: \
    "D:\Desktop\Projekte\DDE - Die Zweite\Source\IOS2P016.RC" \
    {$(INCLUDE)}IOS2P016.H \
    IOS2P016.MAK

.\IOS2P016.obj: \
    "D:\Desktop\Projekte\DDE - Die Zweite\Source\IOS2P016.C" \
    {"D:\Desktop\Projekte\DDE - Die Zweite\Source;$(INCLUDE);"}IOS2P016.H \
    IOS2P016.MAK
