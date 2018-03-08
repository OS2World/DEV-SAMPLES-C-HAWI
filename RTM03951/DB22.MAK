# $Header: D:\projekte\gib mir die daten.1\source\RCS\DB22.MAK 1.1 1995/05/25 15:53:05 HaWi Exp $

.all: \
  DB22.DLL \
  DB22.LIB

.SUFFIXES:

.SUFFIXES: .DEF .C .SQC

.SQC.C:
      @echo WF::COMPILE::SQL Precompile
      sqlprep.exe /B /P /# %s ADRESSE

.C.OBJ:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Q /Wcnddcleffgeniniobsordparprorearettruuse /Fi /Si /O /Gm /Ge- /C %s

.DEF.LIB:
      @echo WF::LIB::Import Lib
      implib.exe %|fF.LIB %s

DB22.DLL: \
    DB221.OBJ \
    DB222.OBJ \
    {$(LIB)}OS2386.LIB \
    {$(LIB)}SQL_DYN.LIB \
    DB22.DEF \
    DB22.MAK
      @echo WF::LINK::Link
      link386.exe @<<
        /PM:PM /F /PACKD: /NOI /NOLOGO /A:4 +
        DB221.OBJ +
        DB222.OBJ
        DB22.DLL
        
        OS2386 +
        SQL_DYN
        DB22.DEF;
<<

DB221.C: \
    DB221.SQC \
    DB22.H \
    DB22.MAK

DB222.C: \
    DB222.SQC \
    DB22.H \
    DB22.MAK

DB221.OBJ: \
    DB221.C \
    DB22.H \
    DB22.MAK

DB222.OBJ: \
    DB222.C \
    DB22.H \
    DB22.MAK

DB22.LIB: \
    DB22.DEF \
    DB22.MAK
