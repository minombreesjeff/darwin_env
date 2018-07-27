# Microsoft Developer Studio Project File - Name="exe_sequin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=exe_sequin - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "exe_sequin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "exe_sequin.mak" CFG="exe_sequin - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "exe_sequin - Win32 DebugDLL" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDLL"
# PROP BASE Intermediate_Dir "DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /Z7 /Od /I "..\..\..\.." /I "..\..\..\..\api" /I "..\..\..\..\asnstat" /I "..\..\..\..\ddv" /I "..\..\..\..\access" /I "..\..\..\..\desktop" /I "..\..\..\..\object" /I "..\..\..\..\asnlib" /I "..\..\..\..\vibrant" /I "..\..\..\..\network\vibnet" /I "..\..\..\..\cdromlib" /I "..\..\..\..\tools" /I "..\..\..\..\connect" /I "..\..\..\..\corelib" /I "..\..\..\..\network\taxon1\taxon2" /I "..\..\..\..\network\nsclilib" /I "..\..\..\..\network\blast3\client" /I "..\..\..\..\network\taxon1\common" /I "..\..\..\..\network\medarch\client" /I "..\..\..\..\network\id1arch" /I "..\..\..\..\biostruc" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386
# ADD LINK32 oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"DebugDLL/sequin.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Target

# Name "exe_sequin - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\link\mswin\sequin.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin10.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin6.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin7.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin8.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin9.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\sequin\sequinx.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\sequin\sequin.h
# End Source File
# End Group
# End Target
# End Project
