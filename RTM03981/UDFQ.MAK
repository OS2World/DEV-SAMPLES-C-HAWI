.all: \
   UDFQ.EXE

UDFQ.OBJ: \
   $*.C \
   UDF.H \
   $*.MAK
      ICC.EXE /C /Gm /O /Oc /Q /Wcnddcleffgeniniobsordparprorearettruuse %s

UDFQ.EXE: \
   $*.OBJ \
   UDF.LIB \
   $*.DEF \
   $*.MAK
      ILINK.EXE /A:1 /E:2 /NOD: /NOE /NOL /PACKD $*.OBJ UDF.LIB $*.DEF

