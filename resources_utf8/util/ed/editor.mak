# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=editor - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to editor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "editor - Win32 Release" && "$(CFG)" != "editor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "editor.mak" CFG="editor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "editor - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "editor - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "editor - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "editor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\editor.exe"

CLEAN : 
	-@erase ".\editor.exe"
	-@erase ".\editor.obj"
	-@erase ".\editor4.obj"
	-@erase ".\compress.obj"
	-@erase ".\editor3.obj"
	-@erase ".\editor2.obj"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "COMPRESS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "COMPRESS" /Fp"editor.pch" /YX /c 
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/editor.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/editor.pdb" /machine:I386 /out:"$(OUTDIR)/editor.exe" 
LINK32_OBJS= \
	"$(INTDIR)/editor.obj" \
	"$(INTDIR)/editor4.obj" \
	"$(INTDIR)/compress.obj" \
	"$(INTDIR)/editor3.obj" \
	"$(INTDIR)/editor2.obj"

"$(OUTDIR)\editor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "editor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\editor.exe"

CLEAN : 
	-@erase ".\vc40.pdb"
	-@erase ".\vc40.idb"
	-@erase ".\editor.exe"
	-@erase ".\editor.obj"
	-@erase ".\editor4.obj"
	-@erase ".\compress.obj"
	-@erase ".\editor3.obj"
	-@erase ".\editor2.obj"
	-@erase ".\editor.ilk"
	-@erase ".\editor.pdb"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "COMPRESS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "COMPRESS" /Fp"editor.pch" /YX /c 
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/editor.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/editor.pdb" /debug /machine:I386 /out:"$(OUTDIR)/editor.exe" 
LINK32_OBJS= \
	"$(INTDIR)/editor.obj" \
	"$(INTDIR)/editor4.obj" \
	"$(INTDIR)/compress.obj" \
	"$(INTDIR)/editor3.obj" \
	"$(INTDIR)/editor2.obj"

"$(OUTDIR)\editor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

.c.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "editor - Win32 Release"
# Name "editor - Win32 Debug"

!IF  "$(CFG)" == "editor - Win32 Release"

!ELSEIF  "$(CFG)" == "editor - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\editor.c
DEP_CPP_EDITO=\
	".\mstruct.h"\
	".\editor.h"\
	".\mtype.h"\
	

"$(INTDIR)\editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\editor2.c
DEP_CPP_EDITOR=\
	".\mstruct.h"\
	".\editor.h"\
	".\mtype.h"\
	".\mextern.h"\
	".\kstbl.h"\
	

"$(INTDIR)\editor2.obj" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\editor3.c

!IF  "$(CFG)" == "editor - Win32 Release"

DEP_CPP_EDITOR3=\
	".\mstruct.h"\
	".\mtype.h"\
	".\mextern.h"\
	

"$(INTDIR)\editor3.obj" : $(SOURCE) $(DEP_CPP_EDITOR3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "editor - Win32 Debug"

DEP_CPP_EDITOR3=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	

"$(INTDIR)\editor3.obj" : $(SOURCE) $(DEP_CPP_EDITOR3) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\editor4.c
DEP_CPP_EDITOR4=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	

"$(INTDIR)\editor4.obj" : $(SOURCE) $(DEP_CPP_EDITOR4) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\compress.c
DEP_CPP_COMPR=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	

"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMPR) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
