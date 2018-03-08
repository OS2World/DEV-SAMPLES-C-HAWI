# $Header: D:\projekte\gib mir die daten\source\RCS\IOS2P001.MAK 1.1 1995/05/25 15:49:41 HaWi Exp $

.all: \
  .\IOS2P001.EXE

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /C %s

.\IOS2P001.EXE: \
    .\IOS2P001.obj \
    .\IOS2P001.res \
    {$(LIB)}OS2386.lib \
    {$(LIB)}IOS2L001.lib \
    {$(LIB)}IOS2P001.DEF \
    IOS2P001.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 /BASE:65536 +
        .\IOS2P001.obj
        IOS2P001.EXE
        
        OS2386 +
        IOS2L001
        IOS2P001.DEF;
<<
      @echo WF::BIND::Resource Bind
      rc.exe -x2 .\IOS2P001.res IOS2P001.EXE

.\IOS2P001.res: \
    "D:\Desktop\Projekte\Gib mir die Daten\Source\IOS2P001.RC" \
    {$(INCLUDE)}IOS2P001.H \
    IOS2P001.MAK

.\IOS2P001.obj: \
    "D:\Desktop\Projekte\Gib mir die Daten\Source\IOS2P001.C" \
    {"D:\Desktop\Projekte\Gib mir die Daten\Source;$(INCLUDE);"}IOS2P001.H \
    {"D:\Desktop\Projekte\Gib mir die Daten\Source;$(INCLUDE);"}IOS2L001.H \
    IOS2P001.MAK
