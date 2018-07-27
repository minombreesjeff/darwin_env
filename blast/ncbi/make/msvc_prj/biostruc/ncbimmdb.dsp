# Microsoft Developer Studio Project File - Name="ncbimmdb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbimmdb - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbimmdb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbimmdb.mak" CFG="ncbimmdb - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbimmdb - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ncbimmdb - Win32 DebugDLL"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GR /Z7 /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\access" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\biostruc" /I "..\..\..\biostruc\cdd" /I "..\..\..\biostruc\cn3d" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\tools" /I "..\..\..\network\id1arch" /I "..\..\..\connect" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
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

# Name "ncbimmdb - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\cddposutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\cddutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\corematx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\dvncode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbentr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\objcdd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cn3d\objcn3d.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmime.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\prunebsc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdalgs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdatd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdbwfi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdcpal.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdcpll.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdcprl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdcxei.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrddgri.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdrsmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdsal0.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdsalr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdsalu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdsgoi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdslo0.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdslor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdslou.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdspci.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdspea.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdspel.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdspni.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdttb0.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdttbi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\thrdzsc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\vastsubs.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\cdd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\corematx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\dvncode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbapi4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\mmdbdata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cdd\objcdd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\cn3d\objcn3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\objmmdb3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\prunebsc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\biostruc\strimprt.h
# End Source File
# End Group
# End Target
# End Project
