# Microsoft Developer Studio Project File - Name="cn3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cn3d - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cn3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cn3d.mak" CFG="cn3d - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cn3d - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "cn3d - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cn3d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_OPENGL" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib wsock32.lib vibnet.lib ncbidesk.lib ncbimmdb.lib ncbitool.lib ncbinacc.lib netentr.lib netcli.lib vibrant.lib ncbicdr.lib ncbiobj.lib ncbi.lib ncbiid1.lib ddvlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\cdromlib\debug" /libpath:"..\cdromlib\ncbicdr___Win32_Debug" /libpath:"..\corelib\debug" /libpath:"..\vibrant\debug" /libpath:"..\object\debug" /libpath:"..\biostruc\debug" /libpath:"..\desktop\debug" /libpath:"..\tools\debug" /libpath:"..\network\entrez\client\debug" /libpath:"..\network\vibnet\debug" /libpath:"..\network\nsclilib\debug" /libpath:"..\network\taxon1\taxon2\debug" /libpath:"..\network\id1arch\debug" /libpath:"..\network\medarch\client\debug" /libpath:"..\network\blast3\client\debug" /libpath:"..\cn3d\ncbicn3d___Win32_Debug" /libpath:"..\ddv\ddvlib___Win32_Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "cn3d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cn3d___Win32_Release0"
# PROP BASE Intermediate_Dir "cn3d___Win32_Release0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "cn3d___Win32_Release"
# PROP Intermediate_Dir "cn3d___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_OPENGL" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /Zi /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_WINDOWS" /D "_OPENGL" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib wsock32.lib vibnet.lib ncbidesk.lib ncbimmdb.lib ncbitool.lib ncbinacc.lib netentr.lib netcli.lib vibrant.lib ncbicdr.lib ncbiobj.lib ncbi.lib ncbiid1.lib ddvlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\cdromlib\debug" /libpath:"..\cdromlib\ncbicdr___Win32_Debug" /libpath:"..\corelib\debug" /libpath:"..\vibrant\debug" /libpath:"..\object\debug" /libpath:"..\biostruc\debug" /libpath:"..\desktop\debug" /libpath:"..\tools\debug" /libpath:"..\network\entrez\client\debug" /libpath:"..\network\vibnet\debug" /libpath:"..\network\nsclilib\debug" /libpath:"..\network\taxon1\taxon2\debug" /libpath:"..\network\taxon1\common\debug" /libpath:"..\network\id1arch\debug" /libpath:"..\network\medarch\client\debug" /libpath:"..\network\blast3\client\debug" /libpath:"..\cn3d\ncbicn3d___Win32_Debug" /libpath:"..\ddv\ddvlib___Win32_Debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glaux.lib wsock32.lib vibnet.lib ncbidesk.lib ncbimmdb.lib ncbitool.lib ncbinacc.lib netentr.lib netcli.lib vibrant.lib ncbicdr.lib ncbiobj.lib ncbi.lib ncbiid1.lib ddvlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\cdromlib\ncbicdr___Win32_Release" /libpath:"..\cdromlib\ncbinacc___Win32_Release" /libpath:"..\corelib\ncbi___Win32_Release" /libpath:"..\vibrant\vibrant___Win32_Release" /libpath:"..\object\object___Win32_Release" /libpath:"..\biostruc\mmdb___Win32_Release" /libpath:"..\desktop\desktop___Win32_Release" /libpath:"..\tools\tools___Win32_Release" /libpath:"..\network\entrez\client\netentr___Win32_Release" /libpath:"..\network\vibnet\vibnet___Win32_Release" /libpath:"..\network\nsclilib\netcli___Win32_Release" /libpath:"..\network\taxon1\taxon2\ncbitxc2___Win32_Release" /libpath:"..\network\id1arch\ncbiid1___Win32_Release" /libpath:"..\network\medarch\client\medarch___Win32_Release" /libpath:"..\network\blast3\client\netblast___Win32_Release" /libpath:"..\cn3d\ncbicn3d___Win32_Release" /libpath:"..\ddv\ddvlib___Win32_Release"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "cn3d - Win32 Debug"
# Name "cn3d - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\link\mswin\cn3d.rc
# End Source File
# Begin Source File

SOURCE=.\cn3dmain.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
