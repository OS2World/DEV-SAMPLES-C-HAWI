# $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2P004.MAK 1.1 1995/05/25 18:34:16 HaWi Exp $

.all: \
  .\IOS2P004.EXE

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /C %s

.\IOS2P004.EXE: \
    .\IOS2P004.obj \
    .\IOS2P004.res \
    {$(LIB)}OS2386.lib \
    {$(LIB)}IOS2L004.lib \
    {$(LIB)}IOS2P004.DEF \
    IOS2P004.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 /BASE:65536 +
        .\IOS2P004.obj
        IOS2P004.EXE
        
        OS2386 +
        IOS2L004
        IOS2P004.DEF;
<<
      @echo WF::BIND::Resource Bind
      rc.exe -x2 .\IOS2P004.res IOS2P004.EXE

.\IOS2P004.res: \
    "D:\Desktop\Projekte\Klick und fallen lassen\Source\IOS2P004.RC" \
    {$(INCLUDE)}IOS2L004.H \
    {$(INCLUDE)}IOS2P004.H \
    IOS2P004.MAK

.\IOS2P004.obj: \
    "D:\Desktop\Projekte\Klick und fallen lassen\Source\IOS2P004.C" \
    {"D:\Desktop\Projekte\Klick und fallen lassen\Source;$(INCLUDE);"}IOS2P004.H \
    {"D:\Desktop\Projekte\Klick und fallen lassen\Source;$(INCLUDE);"}IOS2L004.H \
    IOS2P004.MAK
