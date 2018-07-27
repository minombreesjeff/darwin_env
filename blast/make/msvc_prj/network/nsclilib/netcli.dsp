# Microsoft Developer Studio Project File - Name="netcli" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=netcli - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netcli.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netcli.mak" CFG="netcli - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netcli - Win32 DebugMT" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugMT"
# PROP BASE Intermediate_Dir "DebugMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugMT"
# PROP Intermediate_Dir "DebugMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /GR /Zi /Od /I "..\..\..\.." /I "..\..\..\..\network\nsclilib" /I "..\..\..\..\asnlib" /I "..\..\..\..\asnstat" /I "..\..\..\..\corelib" /I "..\..\..\..\connect" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "netcli - Win32 DebugMT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ncbicli.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ncbiurl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_debug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_disp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_encrs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_error.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_lib_.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_msg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_serv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_www.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ncbicli.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ncbiurl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_lib_.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\network\nsclilib\ni_msg.h
# End Source File
# End Group
# End Target
# End Project
