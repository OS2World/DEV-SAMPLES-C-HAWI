/* */

if Rxfuncquery('SQLDBS') <> 0 then do
   rc = Rxfuncadd('SQLDBS','DB2AR','SQLDBS')
   end

if Rxfuncquery('SQLEXEC') <> 0 then do
   rc = Rxfuncadd('SQLEXEC','DB2AR','SQLEXEC')
   end

db = "TEST"
drive = "C"

call SQLDBS 'CREATE DATABASE 'db' ON 'drive
call CHECKERR 'CREATE'

call SQLEXEC 'CONNECT TO 'db
call CHECKERR 'CONNECT 'db

statement = 'CREATE TABLE ADRESSE (KEY INT, NAME VARCHAR(64))'
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'CREATE ADRESSE'

statement = 'CREATE TABLE PARTNER (KEY INT, ADRESSE INT, NAME VARCHAR(64))'
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'CREATE PARTNER'

statement = "INSERT INTO ADRESSE VALUES (1, 'Michael Jackson')"
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'INSERT ADRESSE'

statement = "INSERT INTO ADRESSE VALUES (2, 'Gloria Estafan')"
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'INSERT ADRESSE'

statement = "INSERT INTO PARTNER VALUES (1, 1, 'Doris Day')"
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'INSERT PARTNER'

statement = "DELETE FROM PARTNER WHERE KEY = 1"
call SQLEXEC 'EXECUTE IMMEDIATE :statement'
call CHECKERR 'DELETE PARTNER'

statement = 'SELECT * FROM ADRESSE'
call SQLEXEC 'PREPARE s1 FROM :statement'
call CHECKERR 'PREPARE'

call SQLEXEC 'DECLARE c1 CURSOR FOR s1'
call CHECKERR 'DECLARE'

call SQLEXEC 'OPEN c1'

do while (SQLCA.SQLCODE = 0)
   call SQLEXEC 'FETCH c1 INTO :key, :name'
   
   if (SQLCA.SQLCODE = 0) then do
      say 'SchlÅssel: 'key
      say 'Name:      'name
      end
   end

call SQLEXEC 'CLOSE c1'
call CHECKERR 'CLOSE'

call SQLEXEC 'CONNECT RESET'

exit 0

CHECKERR:
   arg errloc
   if (SQLCA.SQLCODE = 0) then do
      return 0
      end
   else
      say 'TEST.CMD -' errloc
      say 'SQLCODE  -' SQLCA.SQLCODE
      say 'MESSAGE  -' SQLMSG
      if (SQLCA.SQLCODE < 0) then do
         call SQLEXEC 'CONNECT RESET'
         exit
         end
      else do
         say 'TEST.CMD - Fortsetzung des Programmes mit Warnings'
         return 0
         end
      end
   return 0

