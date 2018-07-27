# Microsoft Developer Studio Project File - Name="netcli" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=netcli - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netcli.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netcli.mak" CFG="netcli - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netcli - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "netcli - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netcli - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W2 /Gm /GX /ZI /Od /I "..\.." /I "..\..\ddv" /I "..\..\cn3d" /I "..\..\access" /I "..\..\asnstat" /I "..\..\connect\lbapi" /I "..\..\connect" /I "..\..\asnlib" /I "..\..\vibrant" /I "..\..\biostruc" /I "..\..\object" /I "..\..\api" /I "..\..\cdromlib" /I "..\..\desktop" /I "..\..\tools" /I "..\..\corelib" /I "..\taxon1\common" /I "..\vibnet" /I "..\entrez\client" /I "..\nsclilib" /I "..\medarch\client" /I "..\id1arch" /I "..\taxon1\taxon2" /I "..\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "netcli - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "netcli___Win32_Release"
# PROP BASE Intermediate_Dir "netcli___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "netcli___Win32_Release"
# PROP Intermediate_Dir "netcli___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W2 /Gm /GX /ZI /Od /I "..\.." /I "..\..\ddv" /I "..\..\cn3d" /I "..\..\access" /I "..\..\asnstat" /I "..\..\connect\lbapi" /I "..\..\connect" /I "..\..\asnlib" /I "..\..\vibrant" /I "..\..\biostruc" /I "..\..\object" /I "..\..\api" /I "..\..\cdromlib" /I "..\..\desktop" /I "..\..\tools" /I "..\..\corelib" /I "..\taxon1\common" /I "..\vibnet" /I "..\entrez\client" /I "..\nsclilib" /I "..\medarch\client" /I "..\id1arch" /I "..\taxon1\taxon2" /I "..\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /I /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W2 /Gm /GX /Zi /I "..\.." /I "..\..\ddv" /I "..\..\cn3d" /I "..\..\access" /I "..\..\asnstat" /I "..\..\connect\lbapi" /I "..\..\connect" /I "..\..\asnlib" /I "..\..\vibrant" /I "..\..\biostruc" /I "..\..\object" /I "..\..\api" /I "..\..\cdromlib" /I "..\..\desktop" /I "..\..\tools" /I "..\..\corelib" /I "..\taxon1\common" /I "..\vibnet" /I "..\entrez\client" /I "..\nsclilib" /I "..\medarch\client" /I "..\id1arch" /I "..\taxon1\taxon2" /I "..\blast3\client" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "netcli - Win32 Debug"
# Name "netcli - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\connect\ncbi_misc.c
# End Source File
# Begin Source File

SOURCE=..\..\connect\ncbi_priv.c
# End Source File
# Begin Source File

SOURCE=.\ncbicli.c
# End Source File
# Begin Source File

SOURCE=.\ncbiurl.c
# End Source File
# Begin Source File

SOURCE=.\ni_debug.c
# End Source File
# Begin Source File

SOURCE=.\ni_disp.c
# End Source File
# Begin Source File

SOURCE=.\ni_encrs.c
# End Source File
# Begin Source File

SOURCE=.\ni_error.c
# End Source File
# Begin Source File

SOURCE=.\ni_lib_.c
# End Source File
# Begin Source File

SOURCE=.\ni_msg.c
# End Source File
# Begin Source File

SOURCE=.\ni_www.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\connect\ncbi_misc.h
# End Source File
# Begin Source File

SOURCE=..\..\connect\ncbi_priv.h
# End Source File
# Begin Source File

SOURCE=.\ncbicli.h
# End Source File
# Begin Source File

SOURCE=.\ncbiurl.h
# End Source File
# Begin Source File

SOURCE=.\ni_error.h
# End Source File
# Begin Source File

SOURCE=.\ni_lib_.h
# End Source File
# Begin Source File

SOURCE=.\ni_msg.h
# End Source File
# End Group
# End Target
# End Project
