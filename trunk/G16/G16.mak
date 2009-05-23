# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=G16 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to G16 - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "G16 - Win32 Release" && "$(CFG)" !=\
 "G16 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "G16.mak" CFG="G16 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "G16 - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "G16 - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "G16 - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "G16 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "temp"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\temp

ALL : "..\..\..\..\Photoshop\Plugins\G16.8bi"\
 ".\temp\G16.tmp" ".\temp\G16.pipl"

CLEAN : 
	-@erase "$(INTDIR)\PIUtilities.obj"
	-@erase "$(INTDIR)\G16.obj"
	-@erase "$(INTDIR)\G16.pipl"
	-@erase "$(INTDIR)\G16.res"
	-@erase "$(INTDIR)\G16.tmp"
	-@erase "$(INTDIR)\G16Scripting.obj"
	-@erase "$(INTDIR)\G16UIWin.obj"
	-@erase "$(INTDIR)\WinDialogUtils.obj"
	-@erase "$(INTDIR)\WinUtilities.obj"
	-@erase "$(OUTDIR)\G16.exp"
	-@erase "$(OUTDIR)\G16.lib"
	-@erase "..\..\..\..\Photoshop\Plugins\G16.8bi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "Common" /I "..\..\Common\Headers" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D MSWindows=1 /YX /c
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /I "Common" /I "..\..\Common\Headers" /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /D MSWindows=1\
 /Fp"$(INTDIR)/G16.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\temp/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "Common" /i "..\..\Common\Headers" /i "..\..\Common\Rez-files" /d "NDEBUG" /d MSWindows=1
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/G16.res" /i "Common" /i\
 "..\..\Common\Headers" /i "..\..\Common\Rez-files" /d "NDEBUG" /d\
 MSWindows=1 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/G16.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib kernel32.lib gdi32.lib comdlg32.lib msvcrt.lib version.lib /nologo /entry:"DLLInit@12" /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\Photoshop\Plugins\G16.8bi"
# SUBTRACT LINK32 /incremental:yes
LINK32_FLAGS=user32.lib kernel32.lib gdi32.lib comdlg32.lib msvcrt.lib\
 version.lib /nologo /entry:"DLLInit@12" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/G16.pdb" /machine:I386 /def:".\G16.Def"\
 /out:"..\..\..\..\Photoshop\Plugins\G16.8bi"\
 /implib:"$(OUTDIR)/G16.lib" 
DEF_FILE= \
	".\G16.Def"
LINK32_OBJS= \
	"$(INTDIR)\PIUtilities.obj" \
	"$(INTDIR)\G16.obj" \
	"$(INTDIR)\G16.res" \
	"$(INTDIR)\G16Scripting.obj" \
	"$(INTDIR)\G16UIWin.obj" \
	"$(INTDIR)\WinDialogUtils.obj" \
	"$(INTDIR)\WinUtilities.obj"

"..\..\..\..\Photoshop\Plugins\G16.8bi" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "G16 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "temp"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\temp

ALL : "..\..\..\..\Photoshop\Plugins\G16.8bi"\
 ".\temp\G16.tmp" ".\temp\G16.pipl"

CLEAN : 
	-@erase "$(INTDIR)\PIUtilities.obj"
	-@erase "$(INTDIR)\G16.obj"
	-@erase "$(INTDIR)\G16.pipl"
	-@erase "$(INTDIR)\G16.res"
	-@erase "$(INTDIR)\G16.tmp"
	-@erase "$(INTDIR)\G16Scripting.obj"
	-@erase "$(INTDIR)\G16UIWin.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinDialogUtils.obj"
	-@erase "$(INTDIR)\WinUtilities.obj"
	-@erase "$(OUTDIR)\G16.exp"
	-@erase "$(OUTDIR)\G16.lib"
	-@erase "$(OUTDIR)\G16.pdb"
	-@erase "..\..\..\..\Photoshop\Plugins\G16.8bi"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "Common" /I "..\..\Common\Headers" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D MSWindows=1 /YX /c
CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "Common" /I\
 "..\..\Common\Headers" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D MSWindows=1\
 /Fp"$(INTDIR)/G16.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\temp/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "Common" /i "..\..\Common\Headers" /i "..\..\Common\Rez-files" /d "_DEBUG" /d MSWindows=1
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/G16.res" /i "Common" /i\
 "..\..\Common\Headers" /i "..\..\Common\Rez-files" /d "_DEBUG" /d\
 MSWindows=1 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/G16.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib kernel32.lib gdi32.lib comdlg32.lib msvcrt.lib version.lib /nologo /entry:"DLLInit@12" /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\Photoshop\Plugins\G16.8bi"
LINK32_FLAGS=user32.lib kernel32.lib gdi32.lib comdlg32.lib msvcrt.lib\
 version.lib /nologo /entry:"DLLInit@12" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/G16.pdb" /debug /machine:I386\
 /def:".\G16.Def" /out:"..\..\..\..\Photoshop\Plugins\G16.8bi"\
 /implib:"$(OUTDIR)/G16.lib" 
DEF_FILE= \
	".\G16.Def"
LINK32_OBJS= \
	"$(INTDIR)\PIUtilities.obj" \
	"$(INTDIR)\G16.obj" \
	"$(INTDIR)\G16.res" \
	"$(INTDIR)\G16Scripting.obj" \
	"$(INTDIR)\G16UIWin.obj" \
	"$(INTDIR)\WinDialogUtils.obj" \
	"$(INTDIR)\WinUtilities.obj"

"..\..\..\..\Photoshop\Plugins\G16.8bi" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
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

# Name "G16 - Win32 Release"
# Name "G16 - Win32 Debug"

!IF  "$(CFG)" == "G16 - Win32 Release"

!ELSEIF  "$(CFG)" == "G16 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\G16UIWin.c
DEP_CPP_SIMPL=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIFormat.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	".\Common\G16.h"\
	
NODEP_CPP_SIMPL=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\G16UIWin.obj" : $(SOURCE) $(DEP_CPP_SIMPL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\G16.rc
NODEP_RSC_SIMPLE=\
	".\G16.pipl"\
	

"$(INTDIR)\G16.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Common\G16Scripting.c
DEP_CPP_SIMPLEF=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIFormat.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	".\Common\G16.h"\
	
NODEP_CPP_SIMPLEF=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\G16Scripting.obj" : $(SOURCE) $(DEP_CPP_SIMPLEF)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Common\G16.h

!IF  "$(CFG)" == "G16 - Win32 Release"

!ELSEIF  "$(CFG)" == "G16 - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Common\G16.r

!IF  "$(CFG)" == "G16 - Win32 Release"

# Begin Custom Build - Compiling PiPL resource...
IntDir=.\temp
InputPath=.\Common\G16.r
InputName=G16

BuildCmds= \
	cl /I..\..\Common\Headers /I..\..\Common\Rez-files /EP /DMSWindows=1\
  /Tc$(InputPath) > $(IntDir)\$(InputName).tmp \
	..\..\Resources\cnvtpipl $(IntDir)\$(InputName).tmp\
  $(IntDir)\$(InputName).pipl \
	

"$(IntDir)\$(InputName).tmp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(IntDir)\$(InputName).pipl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "G16 - Win32 Debug"

# Begin Custom Build - Compiling PiPL resource...
IntDir=.\temp
InputPath=.\Common\G16.r
InputName=G16

BuildCmds= \
	cl /I..\..\Common\Headers /I..\..\Common\Rez-files /EP /DMSWindows=1\
  /Tc$(InputPath) > $(IntDir)\$(InputName).tmp \
	..\..\Resources\cnvtpipl $(IntDir)\$(InputName).tmp\
  $(IntDir)\$(InputName).pipl \
	

"$(IntDir)\$(InputName).tmp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(IntDir)\$(InputName).pipl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Common\G16.c
DEP_CPP_SIMPLEFO=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIFormat.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	".\Common\G16.h"\
	
NODEP_CPP_SIMPLEFO=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\G16.obj" : $(SOURCE) $(DEP_CPP_SIMPLEFO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE="\Adobe\Photoshop SDK\Examples\Common\Sources\WinUtilities.c"
DEP_CPP_WINUT=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	
NODEP_CPP_WINUT=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\WinUtilities.obj" : $(SOURCE) $(DEP_CPP_WINUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE="\Adobe\Photoshop SDK\Examples\Common\Sources\WinDialogUtils.c"
DEP_CPP_WINDI=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	
NODEP_CPP_WINDI=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\WinDialogUtils.obj" : $(SOURCE) $(DEP_CPP_WINDI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE="\Adobe\Photoshop SDK\Examples\Common\Sources\PIUtilities.c"
DEP_CPP_PIUTI=\
	"..\..\Common\Headers\DialogUtilities.h"\
	"..\..\Common\Headers\PIAbout.h"\
	"..\..\Common\Headers\PIActions.h"\
	"..\..\Common\Headers\PIGeneral.h"\
	"..\..\Common\Headers\PITypes.h"\
	"..\..\Common\Headers\PIUtilities.h"\
	"..\..\Common\Headers\WinDialogUtils.h"\
	"..\..\Common\Headers\WinUtilities.h"\
	
NODEP_CPP_PIUTI=\
	"..\..\Common\Headers\AERegistry.h"\
	"..\..\Common\Headers\AEUserTermTypes.r"\
	"..\..\Common\Headers\AppleEvents.r"\
	"..\..\Common\Headers\LowMem.h"\
	"..\..\Common\Headers\Types.r"\
	

"$(INTDIR)\PIUtilities.obj" : $(SOURCE) $(DEP_CPP_PIUTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\G16.Def

!IF  "$(CFG)" == "G16 - Win32 Release"

!ELSEIF  "$(CFG)" == "G16 - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
