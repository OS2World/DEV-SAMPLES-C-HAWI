# $Header: D:\projekte\klick und fallen lassen\source\RCS\IOS2L004.MAK 1.1 1995/05/25 18:33:56 HaWi Exp $

.all: \
  .\IOS2L004.DLL \
  .\IOS2L004.LIB

.SUFFIXES:

.SUFFIXES: .DEF .C

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

.DEF.LIB:
      @echo WF::LIB::Import Lib
      implib.exe %|fF.LIB %s

.\IOS2L004.DLL: \
    .\IOS2L004.obj \
    {$(LIB)}OS2386.lib \
    {$(LIB)}IOS2L004.DEF \
    IOS2L004.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 +
        .\IOS2L004.obj
        IOS2L004.DLL
        
        OS2386
        IOS2L004.DEF;
<<

.\IOS2L004.obj: \
    "D:\Desktop\Projekte\Klick und fallen lassen\Source\IOS2L004.C" \
    IOS2L004.MAK

.\IOS2L004.LIB: \
    "D:\Desktop\Projekte\Klick und fallen lassen\Source\IOS2L004.DEF" \
    IOS2L004.MAK
