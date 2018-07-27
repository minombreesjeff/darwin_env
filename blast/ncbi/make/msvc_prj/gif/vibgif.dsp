# Microsoft Developer Studio Project File - Name="vibgif" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=vibgif - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vibgif.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vibgif.mak" CFG="vibgif - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vibgif - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vibgif - Win32 DebugDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDLL"
# PROP BASE Intermediate_Dir "DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GR /Z7 /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\gif" /I "..\..\..\vibrant" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "vibgif - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\vibrant\drawing.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\gdfontg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\gdfontl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\gdfontmb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\gdfonts.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\gdfontt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\mapping.c
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\ncbidraw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\pictogif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\picture.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\vibrant\drawingp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\mappingp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\ncbidraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\gif\ncbigif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\picture.h
# End Source File
# End Group
# End Target
# End Project
