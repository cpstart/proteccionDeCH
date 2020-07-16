#CPPFLAGS = -I"C:\libs\mysql++\MT\Release" -I"C:\libs\mysql++\MT\lib" -I"C:\Program Files (x86)\MySQL\MySQL Server 5.7\include" 
CPPFLAGS = -I"F:\Data\libs\mysql++\MT\Release" -I"F:\Data\libs\mysql++\MT\lib" -I"C:\Program Files (x86)\MySQL\MySQL Server 5.7\include" 
CFLAGS = -c -Zi -Gm- -O2 -errorReport:prompt -WX- -Gd -Oy- -Oi -MT -EHsc -nologo -D "MYSQLPP_NO_DLL" 
LDFLAGS = -OPT:REF -OPT:ICF -ERRORREPORT:PROMPT -NOLOGO 
LDLIBS = -LIBPATH:"F:\Data\libs\mysql-conn-c\release\lib" -LIBPATH:"F:\Data\libs\mysql++\MT\Release" "mysqlclient.lib" "mysqlpp.lib" "advapi32.lib"
MAINFILE = mainProteccionCH.cpp
EXECUTABLE = protegerCH.exe

$(EXECUTABLE) : main.obj
	link $(LDFLAGS) $(LDLIBS) main.obj /OUT:$(EXECUTABLE)
	
main.obj : $(MAINFILE)	
	cl $(CFLAGS) $(MAINFILE) $(CPPFLAGS) /Fo:main.obj
	
clean:
	del *.obj *.exe *.ilk *.pdb *.idb *.exp *.lib
	
#Archivo makefile