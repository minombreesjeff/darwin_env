# Microsoft Developer Studio Project File - Name="ncbicdr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbicdr - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbicdr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbicdr.mak" CFG="ncbicdr - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbicdr - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ncbicdr - Win32 DebugDLL"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /Z7 /Od /I "..\..\..\.." /I "..\..\..\..\corelib" /I "..\..\..\..\asnstat" /I "..\..\..\..\asnlib" /I "..\..\..\..\api" /I "..\..\..\..\cdromlib" /I "..\..\..\..\object" /I "..\..\..\..\biostruc" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
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

# Name "ncbicdr - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\cdromlib\casn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdconfig.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdentrez.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdnewlib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdromlib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\objentr.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\cdromlib\casn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdentrez.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdnewlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\cdromlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\cdromlib\objentr.h
# End Source File
# End Group
# End Target
# End Project
