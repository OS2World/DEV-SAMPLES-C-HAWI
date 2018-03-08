.all: \
  HWITER.hlp

HWITER.hlp: \
    $*.CPF \
    HWITERH.MAK
      ipfcomp.exe /PC "ICC.EXE /P+ /W0" /KP /S $*.CPF
