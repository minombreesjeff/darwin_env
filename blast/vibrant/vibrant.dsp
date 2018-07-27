# Microsoft Developer Studio Project File - Name="vibrant" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=vibrant - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vibrant.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vibrant.mak" CFG="vibrant - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vibrant - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "vibrant - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vibrant - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_OPENGL" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "vibrant - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vibrant___Win32_Release"
# PROP BASE Intermediate_Dir "vibrant___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vibrant___Win32_Release"
# PROP Intermediate_Dir "vibrant___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_OPENGL" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /Zi /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "_OPENGL" /D "WIN32" /D "_MBCS" /YX /FD /c
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

# Name "vibrant - Win32 Debug"
# Name "vibrant - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\diagnost.c
# End Source File
# Begin Source File

SOURCE=.\document.c
# End Source File
# Begin Source File

SOURCE=.\drawing.c
# End Source File
# Begin Source File

SOURCE=.\image.c
# End Source File
# Begin Source File

SOURCE=.\imagelst.c
# End Source File
# Begin Source File

SOURCE=.\mapping.c
# End Source File
# Begin Source File

SOURCE=.\ncbidraw.c
# End Source File
# Begin Source File

SOURCE=.\netscape.c
# End Source File
# Begin Source File

SOURCE=.\odlbox.c
# End Source File
# Begin Source File

SOURCE=.\palette.c
# End Source File
# Begin Source File

SOURCE=.\pictur3d.c
# End Source File
# Begin Source File

SOURCE=.\picture.c
# End Source File
# Begin Source File

SOURCE=.\prim3d1.c
# End Source File
# Begin Source File

SOURCE=.\prim3d2.c
# End Source File
# Begin Source File

SOURCE=.\prim3d3.c
# End Source File
# Begin Source File

SOURCE=.\prim3d4.c
# End Source File
# Begin Source File

SOURCE=.\prim3d5.c
# End Source File
# Begin Source File

SOURCE=.\shim3d.c
# End Source File
# Begin Source File

SOURCE=.\table.c
# End Source File
# Begin Source File

SOURCE=.\treeview.c
# End Source File
# Begin Source File

SOURCE=.\vibbutns.c
# End Source File
# Begin Source File

SOURCE=.\vibextra.c
# End Source File
# Begin Source File

SOURCE=.\vibfiles.c
# End Source File
# Begin Source File

SOURCE=.\vibforms.c
# End Source File
# Begin Source File

SOURCE=.\vibgroup.c
# End Source File
# Begin Source File

SOURCE=.\viblists.c
# End Source File
# Begin Source File

SOURCE=.\vibmenus.c
# End Source File
# Begin Source File

SOURCE=.\vibmouse.c
# End Source File
# Begin Source File

SOURCE=.\vibprmpt.c
# End Source File
# Begin Source File

SOURCE=.\vibsbars.c
# End Source File
# Begin Source File

SOURCE=.\vibslate.c
# End Source File
# Begin Source File

SOURCE=.\vibtexts.c
# End Source File
# Begin Source File

SOURCE=.\vibutils.c
# End Source File
# Begin Source File

SOURCE=.\vibwndws.c
# End Source File
# Begin Source File

SOURCE=.\viewer.c
# End Source File
# Begin Source File

SOURCE=.\viewer3d.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\diagnost.h
# End Source File
# Begin Source File

SOURCE=.\document.h
# End Source File
# Begin Source File

SOURCE=.\drawingp.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\imagelst.h
# End Source File
# Begin Source File

SOURCE=.\mappingp.h
# End Source File
# Begin Source File

SOURCE=.\ncbidraw.h
# End Source File
# Begin Source File

SOURCE=.\ncbiport.h
# End Source File
# Begin Source File

SOURCE=.\netscape.h
# End Source File
# Begin Source File

SOURCE=.\odlbox.h
# End Source File
# Begin Source File

SOURCE=.\panels.h
# End Source File
# Begin Source File

SOURCE=.\pdiagnos.h
# End Source File
# Begin Source File

SOURCE=.\pictur3d.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\picturep.h
# End Source File
# Begin Source File

SOURCE=.\ppict3d.h
# End Source File
# Begin Source File

SOURCE=.\shim3d.h
# End Source File
# Begin Source File

SOURCE=.\treeview.h
# End Source File
# Begin Source File

SOURCE=.\treevint.h
# End Source File
# Begin Source File

SOURCE=.\vibdefns.h
# End Source File
# Begin Source File

SOURCE=.\vibforms.h
# End Source File
# Begin Source File

SOURCE=.\vibincld.h
# End Source File
# Begin Source File

SOURCE=.\vibmouse.h
# End Source File
# Begin Source File

SOURCE=.\vibprocs.h
# End Source File
# Begin Source File

SOURCE=.\vibrant.h
# End Source File
# Begin Source File

SOURCE=.\vibtypes.h
# End Source File
# Begin Source File

SOURCE=.\viewer.h
# End Source File
# Begin Source File

SOURCE=.\viewer3d.h
# End Source File
# Begin Source File

SOURCE=.\viewerp.h
# End Source File
# End Group
# End Target
# End Project
