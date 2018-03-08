/* */

if Rxfuncquery('XrLoadFuncs') <> 0 then do
   Rxfuncadd('XrLoadFuncs','XACTREXX','XrLoadFuncs')
   call XrLoadFuncs
   end

if Rxfuncquery('SysLoadFuncs') <> 0 then do
   Rxfuncadd('SysLoadFuncs','RexxUtil','SysLoadFuncs')
   call SysLoadFuncs
   end

session = startSession('/PM D:\Progs\XACT\XACTOS2.EXE -b')
if session = 0 then do 
   say "Can't open session" 
   return 0 
   end 
     
table = XrTOpen(session,0,'D:\Projekte\Redaktion OS!2 Inside\Auswertungen satt\Source\AUS1.CSV',0) 

rc = XrTSelectAll(session,table,1) 

grafik = XrTUseStylesheet(session,table,'D:\Progs\XACT\Vorlagen\BAR.STL',4,0) 

rc = XrGExport(session,grafik,'BMP',72,'COL_8','D:\Projekte\Redaktion OS!2 Inside\Auswertungen satt\Source\AUS1.BMP')

rc = XrGClose(session,grafik) 

rc = XrTClose(session,table) 

rc = XrEndSession(session,0)

rc = XrDropFuncs

return 0

startSession: procedure 
   Parse Arg startUpString 
   session = XrBeginSession("") 
   if session = 0 then do
         START startUpString
         end
   do while session = 0 
      session = XrBeginSession("") 
      end 
   return session 

