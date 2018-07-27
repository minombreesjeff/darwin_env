# Microsoft Developer Studio Project File - Name="ncbicn3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbicn3d - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbicn3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbicn3d.mak" CFG="ncbicn3d - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbicn3d - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ncbicn3d - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ncbicn3d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ncbicn3d___Win32_Debug"
# PROP BASE Intermediate_Dir "ncbicn3d___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ncbicn3d___Win32_Debug"
# PROP Intermediate_Dir "ncbicn3d___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_OPENGL" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ncbicn3d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ncbicn3d___Win32_Release"
# PROP BASE Intermediate_Dir "ncbicn3d___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ncbicn3d___Win32_Release"
# PROP Intermediate_Dir "ncbicn3d___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_OPENGL" /YX /FD /GZ /c
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

# Name "ncbicn3d - Win32 Debug"
# Name "ncbicn3d - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\algorend.c
# End Source File
# Begin Source File

SOURCE=.\cn3dentr.c
# End Source File
# Begin Source File

SOURCE=.\cn3dmatn.c
# End Source File
# Begin Source File

SOURCE=.\cn3dmesh.c
# End Source File
# Begin Source File

SOURCE=.\cn3dmodl.c
# End Source File
# Begin Source File

SOURCE=.\cn3dmsel.c
# End Source File
# Begin Source File

SOURCE=.\cn3dmsg.c
# End Source File
# Begin Source File

SOURCE=.\cn3dopen.c
# End Source File
# Begin Source File

SOURCE=.\cn3dsave.c
# End Source File
# Begin Source File

SOURCE=.\cn3dshim.c
# End Source File
# Begin Source File

SOURCE=.\cn3dstyl.c
# End Source File
# Begin Source File

SOURCE=.\cn3dwin.c
# End Source File
# Begin Source File

SOURCE=.\cn3dxprt.c
# End Source File
# Begin Source File

SOURCE=.\seqcons.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\algorend.h
# End Source File
# Begin Source File

SOURCE=.\cn3dentr.h
# End Source File
# Begin Source File

SOURCE=.\cn3dmain.h
# End Source File
# Begin Source File

SOURCE=.\cn3dmesh.h
# End Source File
# Begin Source File

SOURCE=.\cn3dmodl.h
# End Source File
# Begin Source File

SOURCE=.\cn3dmsel.h
# End Source File
# Begin Source File

SOURCE=.\cn3dmsg.h
# End Source File
# Begin Source File

SOURCE=.\cn3dopen.h
# End Source File
# Begin Source File

SOURCE=.\cn3dsave.h
# End Source File
# Begin Source File

SOURCE=.\cn3dshim.h
# End Source File
# Begin Source File

SOURCE=.\cn3dstyl.h
# End Source File
# Begin Source File

SOURCE=.\cn3dxprt.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\seqcons.h
# End Source File
# End Group
# End Target
# End Project
