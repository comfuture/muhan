# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

!IF "$(CFG)" == ""
CFG=muhan - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to muhan - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "muhan - Win32 Release" && "$(CFG)" != "muhan - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "muhan.mak" CFG="muhan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "muhan - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "muhan - Win32 Debug" (based on "Win32 (x86) External Target")
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
CPP=cl.exe

!IF  "$(CFG)" == "muhan - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP BASE Cmd_Line "NMAKE /f muhan.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "muhan.exe"
# PROP BASE Bsc_Name "muhan.bsc"
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# PROP Cmd_Line "NMAKE /f muhan.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "muhan.exe"
# PROP Bsc_Name "muhan.bsc"
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\muhan.exe" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "CHECKDOUBLE" /D "COMPRESS" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "COMPRESS" /D "_CONSOLE" /D "CHECKDOUBLE" /D "DEBUG" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "COMPRESS" /D "_CONSOLE" /D "CHECKDOUBLE"\
 /D "DEBUG" /Fp"$(INTDIR)/muhan.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/muhan.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/muhan.pdb" /machine:I386 /out:"$(OUTDIR)/muhan.exe" 
LINK32_OBJS= \
	"$(INTDIR)/action.obj" \
	"$(INTDIR)/alias.obj" \
	"$(INTDIR)/board.obj" \
	"$(INTDIR)/bank.obj" \
	"$(INTDIR)/command1.obj" \
	"$(INTDIR)/command2.obj" \
	"$(INTDIR)/command3.obj" \
	"$(INTDIR)/command4.obj" \
	"$(INTDIR)/command5.obj" \
	"$(INTDIR)/command6.obj" \
	"$(INTDIR)/command7.obj" \
	"$(INTDIR)/command8.obj" \
	"$(INTDIR)/command9.obj" \
	"$(INTDIR)/command10.obj" \
	"$(INTDIR)/command11.obj" \
	"$(INTDIR)/command12.obj" \
	"$(INTDIR)/compress.obj" \
	"$(INTDIR)/creature.obj" \
	"$(INTDIR)/dm1.obj" \
	"$(INTDIR)/dm2.obj" \
	"$(INTDIR)/dm3.obj" \
	"$(INTDIR)/dm4.obj" \
	"$(INTDIR)/dm5.obj" \
	"$(INTDIR)/dm6.obj" \
	"$(INTDIR)/files1.obj" \
	"$(INTDIR)/files2.obj" \
	"$(INTDIR)/files3.obj" \
	"$(INTDIR)/global.obj" \
	"$(INTDIR)/io.obj" \
	"$(INTDIR)/magic1.obj" \
	"$(INTDIR)/magic2.obj" \
	"$(INTDIR)/magic3.obj" \
	"$(INTDIR)/magic4.obj" \
	"$(INTDIR)/magic5.obj" \
	"$(INTDIR)/magic6.obj" \
	"$(INTDIR)/magic7.obj" \
	"$(INTDIR)/magic8.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/misc.obj" \
	"$(INTDIR)/object.obj" \
	"$(INTDIR)/player.obj" \
	"$(INTDIR)/post.obj" \
	"$(INTDIR)/room.obj" \
	"$(INTDIR)/special1.obj" \
	"$(INTDIR)/strstr.obj" \
	"$(INTDIR)/update.obj"

"$(OUTDIR)\muhan.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "muhan - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP BASE Cmd_Line "NMAKE /f muhan.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "muhan.exe"
# PROP BASE Bsc_Name "muhan.bsc"
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# PROP Cmd_Line "NMAKE /f muhan.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "muhan.exe"
# PROP Bsc_Name "muhan.bsc"
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\muhan.exe" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "CHECKDOUBLE" /D "COMPRESS" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "CHECKDOUBLE" /D "COMPRESS" /D "_CONSOLE" /D "DEBUG" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "COMPRESS" /D "CHECKDOUBLE" /D "_CONSOLE"\
 /D "DEBUG" /Fp"$(INTDIR)/muhan.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/muhan.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/muhan.pdb" /debug /machine:I386 /out:"$(OUTDIR)/muhan.exe" 
LINK32_OBJS= \
	"$(INTDIR)/action.obj" \
	"$(INTDIR)/alias.obj" \
	"$(INTDIR)/board.obj" \
	"$(INTDIR)/bank.obj" \
	"$(INTDIR)/command1.obj" \
	"$(INTDIR)/command2.obj" \
	"$(INTDIR)/command3.obj" \
	"$(INTDIR)/command4.obj" \
	"$(INTDIR)/command5.obj" \
	"$(INTDIR)/command6.obj" \
	"$(INTDIR)/command7.obj" \
	"$(INTDIR)/command8.obj" \
	"$(INTDIR)/command9.obj" \
	"$(INTDIR)/command10.obj" \
	"$(INTDIR)/command11.obj" \
	"$(INTDIR)/command12.obj" \
	"$(INTDIR)/compress.obj" \
	"$(INTDIR)/creature.obj" \
	"$(INTDIR)/dm1.obj" \
	"$(INTDIR)/dm2.obj" \
	"$(INTDIR)/dm3.obj" \
	"$(INTDIR)/dm4.obj" \
	"$(INTDIR)/dm5.obj" \
	"$(INTDIR)/dm6.obj" \
	"$(INTDIR)/files1.obj" \
	"$(INTDIR)/files2.obj" \
	"$(INTDIR)/files3.obj" \
	"$(INTDIR)/global.obj" \
	"$(INTDIR)/io.obj" \
	"$(INTDIR)/magic1.obj" \
	"$(INTDIR)/magic2.obj" \
	"$(INTDIR)/magic3.obj" \
	"$(INTDIR)/magic4.obj" \
	"$(INTDIR)/magic5.obj" \
	"$(INTDIR)/magic6.obj" \
	"$(INTDIR)/magic7.obj" \
	"$(INTDIR)/magic8.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/misc.obj" \
	"$(INTDIR)/object.obj" \
	"$(INTDIR)/player.obj" \
	"$(INTDIR)/post.obj" \
	"$(INTDIR)/room.obj" \
	"$(INTDIR)/special1.obj" \
	"$(INTDIR)/strstr.obj" \
	"$(INTDIR)/update.obj"

"$(OUTDIR)\muhan.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "muhan - Win32 Release"
# Name "muhan - Win32 Debug"

!IF  "$(CFG)" == "muhan - Win32 Release"

!ELSEIF  "$(CFG)" == "muhan - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\action.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\alias.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\alias.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\board.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\board.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bank.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\bank.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command1.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command1.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command2.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command2.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command3.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command3.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command4.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command4.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command5.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command5.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command6.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command6.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command7.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command7.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command8.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command8.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command9.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command9.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command10.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command10.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command11.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command11.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\command12.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\command12.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\compress.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\compress.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\creature.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\creature.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm1.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm1.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm2.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm2.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm3.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm3.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm4.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm4.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm5.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm5.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dm6.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\dm6.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\files1.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\files1.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\files2.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\files2.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\files3.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\files3.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\global.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\global.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\io.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\io.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic1.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic1.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic2.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic2.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic3.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic3.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic4.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic4.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic5.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic5.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic6.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic6.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic7.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic7.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic8.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\magic8.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\main.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\misc.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\object.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\object.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\player.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\player.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\post.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\post.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\room.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\room.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\special1.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\specail1.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\strstr.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\strstr.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
################################################################################
# Begin Source File

SOURCE=.\update.c
DEP_CPP_COMMA=\
	".\mstruct.h"\
	".\mextern.h"\
	".\mtype.h"\
	".\board.h"\
        ".\kstbl.h"\
        ".\update.h"\

NODEP_CPP_COMMA=\
	".\..\..\usr\local\include\dmalloc.h"\
	
"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_COMMA) "$(INTDIR)"

# End Source File
# End Target
# End Project

################################################################################
