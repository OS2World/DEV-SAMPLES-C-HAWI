# $Header: D:\projekte\gib mir die daten\source\RCS\IOS2L001.MAK 1.1 1995/05/25 15:49:07 HaWi Exp $

.all: \
  .\IOS2L001.DLL \
  .\IOS2L001.LIB

.SUFFIXES:

.SUFFIXES: .DEF .c .SQC

.SQC.c:
      @echo WF::COMPILE::SQL Precompile
      sqlprep.exe /B /P %s IOS2L

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

.DEF.LIB:
      @echo WF::LIB::Import Lib
      implib.exe %|fF.LIB %s

.\IOS2L001.DLL: \
    .\IOS2L002.obj \
    .\IOS2L001.obj \
    {$(LIB)}OS2386.lib \
    {$(LIB)}SQL_DYN.lib \
    {$(LIB)}IOS2L001.DEF \
    IOS2L001.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /PM:PM /F /PACKD: /E:2 /NOI /NOLOGO /A:4 +
        .\IOS2L002.obj +
        .\IOS2L001.obj
        IOS2L001.DLL
        
        OS2386 +
        SQL_DYN
        IOS2L001.DEF;
<<

.\IOS2L001.c: \
    "D:\Desktop\Projekte\Gib mir die Daten\Source\IOS2L001.SQC" \
    IOS2L001.MAK

.\IOS2L002.c: \
    "D:\Desktop\Projekte\Gib mir die Daten\Source\IOS2L002.SQC" \
    IOS2L001.MAK

.\IOS2L002.obj: \
    .\IOS2L002.c \
    {"D:\Desktop\Projekte\Gib mir die Daten\Source;$(INCLUDE);"}IOS2L001.H \
    IOS2L001.MAK

.\IOS2L001.obj: \
    .\IOS2L001.c \
    {"D:\Desktop\Projekte\Gib mir die Daten\Source;$(INCLUDE);"}IOS2L001.H \
    IOS2L001.MAK

.\IOS2L001.LIB: \
    "D:\Desktop\Projekte\Gib mir die Daten\Source\IOS2L001.DEF" \
    IOS2L001.MAK
