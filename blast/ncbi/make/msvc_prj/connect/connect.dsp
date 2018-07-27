# Microsoft Developer Studio Project File - Name="connect" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=connect - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "connect.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "connect.mak" CFG="connect - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "connect - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
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
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /GR /Z7 /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\vibrant" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\desktop" /I "..\..\..\cdromlib" /I "..\..\..\tools" /I "..\..\..\biostruc" /I "..\..\..\access" /I "..\..\..\connect" /I "..\..\..\cn3d" /I "..\..\..\gif" /I "..\..\..\network\blast3\client" /I "..\..\..\network\nsclilib" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "connect - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\connect\ncbi_ansi_ext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_ansi_ext.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_buffer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_comm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connection.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_core.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_core_c.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_dispd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_dispd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_file_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_ftp_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_heapmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_host_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_host_infop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_http_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_lbsmd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_lbsmd_stub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_memory_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_priv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_priv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_sendmail.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_server_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_server_infop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_service.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_service_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_servicep.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_socket.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_socket_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\connect\ncbi_buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_connutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_core.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_core_c.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_file_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_ftp_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_heapmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_host_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_http_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_memory_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_sendmail.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_server_info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_service.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_service_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_socket.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_socket_connector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\connect\ncbi_util.h
# End Source File
# End Group
# End Target
# End Project
