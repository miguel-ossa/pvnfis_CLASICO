# Microsoft Visual C++ generated build script - Do not modify

PROJ = PVNFIS
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
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
FIRSTCPP = PVNFIS.CPP  
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /O1 /D "_DEBUG" /D "_DOS" /FR /Fd"PVNFIS.PDB"
CFLAGS_R_DEXE = /nologo /Zr /G3 /FPc /W3 /Gf /AS /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:8192 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames slibce 
LIBS_R_DEXE = slibce oldnames 
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
SBRS = CCOMM.SBR \
		CFILE.SBR \
		CIMPRE.SBR \
		CRELOJ.SBR \
		PVNFIS.SBR


CCOMM_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\creloj.h \
	c:\msvc\projects\pvnfis\cfile.h \
	c:\msvc\projects\pvnfis\ccomm.h


CFILE_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\cfile.h


CIMPRE_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\cimpre.h \
	c:\msvc\projects\pvnfis\ccomm.h \
	c:\msvc\projects\pvnfis\cfile.h \
	c:\msvc\projects\pvnfis\creloj.h


CRELOJ_DEP = c:\msvc\projects\pvnfis\creloj.h


PVNFIS_DEP = c:\msvc\projects\pvnfis\constant.h \
	c:\msvc\projects\pvnfis\cimpre.h \
	c:\msvc\projects\pvnfis\ccomm.h \
	c:\msvc\projects\pvnfis\cfile.h


all:	$(PROJ).EXE

CCOMM.OBJ:	CCOMM.CPP $(CCOMM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CCOMM.CPP

CFILE.OBJ:	CFILE.CPP $(CFILE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CFILE.CPP

CIMPRE.OBJ:	CIMPRE.CPP $(CIMPRE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CIMPRE.CPP

CRELOJ.OBJ:	CRELOJ.CPP $(CRELOJ_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CRELOJ.CPP

PVNFIS.OBJ:	PVNFIS.CPP $(PVNFIS_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c PVNFIS.CPP

$(PROJ).EXE::	CCOMM.OBJ CFILE.OBJ CIMPRE.OBJ CRELOJ.OBJ PVNFIS.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
CCOMM.OBJ +
CFILE.OBJ +
CIMPRE.OBJ +
CRELOJ.OBJ +
PVNFIS.OBJ +
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
