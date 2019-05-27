# Microsoft Visual C++ generated build script - Do not modify

PROJ = PVNLET
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 000900000000,000
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\MSVC\PROJECTS\PVNFIS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = PVNLET.CPP  
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AT /Od /D "_DEBUG" /D "_DOS" /FR /Fd"PVNLET.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /O1 /Ox /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /CO /STACK:5120
LFLAGS_R_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /STACK:5120
LIBS_D_DEXE = oldnames slibce
LIBS_R_DEXE = oldnames slibce
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = CFILE.SBR \
		PVNLET.SBR


CFILE_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\cfile.h


PVNLET_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\cfile.h


all:	$(PROJ).EXE

CFILE.OBJ:	CFILE.CPP $(CFILE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CFILE.CPP

PVNLET.OBJ:	PVNLET.CPP $(PVNLET_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c PVNLET.CPP

$(PROJ).EXE::	CFILE.OBJ PVNLET.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
CFILE.OBJ +
PVNLET.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\dozultra\lib\+
c:\msvc\penright\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
