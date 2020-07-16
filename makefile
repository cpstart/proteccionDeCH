CPPFLAGS = -I.
CFLAGS = -c -Zi -Gm- -O2 -errorReport:prompt -WX- -Gd -Oy- -Oi -MT -EHsc -nologo 
LDFLAGS = -OPT:REF -OPT:ICF -ERRORREPORT:PROMPT -NOLOGO 
LDLIBS = 
MAINFILE = main.cpp
EXECUTABLE = protegerCH.exe

$(EXECUTABLE) : main.obj
	link $(LDFLAGS) $(LDLIBS) main.obj /OUT:$(EXECUTABLE)
	
main.obj : $(MAINFILE)	
	cl $(CFLAGS) $(MAINFILE) $(CPPFLAGS) /Fo:main.obj
	
clean:
	del *.obj *.exe *.ilk *.pdb *.idb *.exp *.lib
	
#Archivo makefile