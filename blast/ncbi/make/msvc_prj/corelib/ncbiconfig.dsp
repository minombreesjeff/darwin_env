# Microsoft Developer Studio Project File - Name="ncbiconfig" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=ncbiconfig - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbiconfig.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbiconfig.mak" CFG="ncbiconfig - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbiconfig - Win32 DebugDLL" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
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
# Begin Target

# Name "ncbiconfig - Win32 DebugDLL"
# Begin Source File

SOURCE=..\..\..\corelib\ncbilcl.msw
# Begin Custom Build
InputDir=\toolkit\ncbi\corelib
InputPath=..\..\..\corelib\ncbilcl.msw

"$(InputDir)\ncbilcl.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
    copy $(InputPath) $(InputDir)\ncbilcl.h

# End Custom Build
# End Source File
# End Target
# End Project
