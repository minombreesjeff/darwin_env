# Microsoft Developer Studio Project File - Name="blastapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=blastapi - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blastapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blastapi.mak" CFG="blastapi - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blastapi - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "blastapi___Win32_DebugDLL0"
# PROP BASE Intermediate_Dir "blastapi___Win32_DebugDLL0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\.." /I "..\..\..\..\..\corelib" /I "..\..\..\..\..\api" /I "..\..\..\..\..\object" /I "..\..\..\..\..\asnlib" /I "..\..\..\..\..\tools" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "blastapi - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_format.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_input.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_seq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_seqalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\multiseq_src.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\seqsrc_readdb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\twoseq_api.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_format.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_input.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_seq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\blast_seqalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\multiseq_src.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\seqsrc_readdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\api\twoseq_api.h
# End Source File
# End Group
# End Target
# End Project
