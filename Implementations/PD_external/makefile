current:
	echo make pd_linux, pd_nt, pd_irix5, pd_irix6, or pd_darwin

clean: ; rm -f *.pd_linux *.pd_darwin *.o

# ----------------------- NT -----------------------

pd_nt: spatdif.dll

.SUFFIXES: .obj .dll

PDNTCFLAGS = /W3 /WX /DNT /DPD /nologo
VC="D:\Program Files\Microsoft Visual Studio\Vc98"

PDNTINCLUDE = /I. /I\tcl\include /I..\..\src /I$(VC)\include

PDNTLDIR = $(VC)\lib
PDNTLIB = $(PDNTLDIR)\libc.lib \
	$(PDNTLDIR)\oldnames.lib \
	$(PDNTLDIR)\kernel32.lib \
	..\..\bin\pd.lib 

.c.dll:
	cl $(PDNTCFLAGS) $(PDNTINCLUDE) /c $*.c
	link /dll /export:$*_setup $*.obj $(PDNTLIB)

# override explicitly for tilde objects like this:
dspobj~.dll: dspobj~.c; 
	cl $(PDNTCFLAGS) $(PDNTINCLUDE) /c $*.c
	link /dll /export:dspobj_tilde_setup $*.obj $(PDNTLIB)

# ----------------------- IRIX 5.x -----------------------

pd_irix5: spatdif.pd_irix5 

.SUFFIXES: .pd_irix5

SGICFLAGS5 = -o32 -DPD -DUNIX -DIRIX -O2


SGIINCLUDE =  -I../../src/

.c.pd_irix5:
	cc $(SGICFLAGS5) $(SGIINCLUDE) -o $*.o -c $*.c
	ld -elf -shared -rdata_shared -o $*.pd_irix5 $*.o
	rm $*.o

# ----------------------- LINUX i386 -----------------------

pd_linux: spatdif.pd_linux 

.SUFFIXES: .pd_linux

LINUXCFLAGS = -DPD -O2 -funroll-loops -fomit-frame-pointer \
    -Wall -W -Wshadow -Wstrict-prototypes -Werror \
    -Wno-unused -Wno-parentheses -Wno-switch

LINUXINCLUDE =  -I../../src

.c.pd_linux:
	cc $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm $*.o

# ----------------------- Mac OSX -----------------------

pd_darwin: spatdif.pd_darwin 

.SUFFIXES: .pd_darwin

DARWINCFLAGS = -arch i386 -arch x86_64 -x c++  -DPD -O0 -Wall -W -Wstrict-prototypes -Wno-unused
SPATDIFSRC = ../../spatdiflib/src
SPATDIFINCLUDE = -I ../../spatdiflib/src
LIBRARY_PATH = -L ../../spatdiflib/src
LIBS =  -lstdc++ #-lspatdif

.c.pd_darwin:
	make -C $(SPATDIFSRC)
	cc $(DARWINCFLAGS) $(SPATDIFINCLUDE) -o $*.o -c $*.c
	cc -arch i386 -arch x86_64 -bundle -flat_namespace -undefined suppress  $(LIBRARY_PATH) $(LIBS) -o $*.pd_darwin $*.o $(SPATDIFSRC)/*.o $(SPATDIFSRC)/libs/libjson/Objects_static/*.o
	rm -f $*.o

