# $Header: D:/PROJEKTE/Kleine Helferlein!1/Source/rcs/USRGRP.MAK 1.1 1995/12/01 11:18:28 HaWi Exp $

.all: \
   USRGRP.EXE

USRGRP.RES: \
   $*.RC \
   $*.H \
   $*.MAK
      RC.EXE -r $*.RC $*.RES

USRGRP.OBJ: \
   $*.C \
   $*.H \
   $*.MAK
      ICC.EXE /C /Fi /Gm /O /Oc /Q /Si /Wcnddcleffgeniniobsordparprorearettruuse %s

USRGRP.EXE: \
   $*.OBJ \
   $*.RES \
   {$(LIB)}OS2386.LIB \
   {$(LIB)}NETAPI.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE @<<
         /A:1 /BASE:65536 /E:2 /NOLOGO /PACKD:0xFfffFfff /PM:PM +
         $*.OBJ
         $*.EXE

         OS2386 +
         NETAPI
         $*.DEF;
<<
      RC.EXE -x2 $*.RES $*.EXE


