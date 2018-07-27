# Microsoft Developer Studio Project File - Name="vibrant_ogl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=vibrant_ogl - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vibrant_ogl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vibrant_ogl.mak" CFG="vibrant_ogl - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vibrant_ogl - Win32 DebugMT" (based on "Win32 (x86) Static Library")
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
# ADD CPP /nologo /MTd /W3 /GR /Zi /Od /I "..\..\..\.." /I "..\..\..\..\corelib" /I "..\..\..\..\asnstat" /I "..\..\..\..\asnlib" /I "..\..\..\..\vibrant" /I "..\..\..\..\api" /I "..\..\..\..\object" /D "_OPENGL" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "vibrant_ogl - Win32 DebugMT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\vibrant\diagnost.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\document.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\drawing.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\image.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\imagelst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\mapping.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\ncbidraw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\odlbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\palette.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\pictur3d.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\picture.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\prim3d1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\prim3d2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\prim3d3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\prim3d4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\prim3d5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\shim3d.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\treeview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibbutns.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibextra.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibfiles.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibforms.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibgroup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viblists.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibmenus.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibmouse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibprmpt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibsbars.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibslate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibtexts.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibutils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibwndws.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viewer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viewer3d.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\vibrant\diagnost.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\document.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\drawingp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\imagelst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\mappingp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\ncbidraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\ncbiport.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\odlbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\panels.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\pdiagnos.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\pictur3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\picture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\picturep.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\ppict3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\shim3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\treeview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\treevint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vibrant\vibdefns.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibforms.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibincld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibmouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibprocs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibrant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\vibtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viewer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viewer3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\vibrant\viewerp.h
# End Source File
# End Group
# End Target
# End Project
