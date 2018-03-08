# $Header: D:/PROJEKTE/Die Programmierung des DB2!2/Source/rcs/DB2SPY.MAK 1.1 1995/12/11 11:48:49 HaWi Exp $

.all: \
   DB2SPY.EXE

DB2SPY.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

DB2SPY.C: \
   $*.SQC \
   $*.MAK
      SQLPREP.EXE $*.SQC SAMPLE /B /P

DB2SPY.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Oc /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

DB2SPY.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   {$(LIB)}DB2API.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386 +
         DB2API
         $*.DEF;
<<
      RC.EXE -x2 $*.RES $*.EXE

