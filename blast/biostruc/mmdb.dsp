# Microsoft Developer Studio Project File - Name="mmdb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mmdb - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mmdb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mmdb.mak" CFG="mmdb - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mmdb - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "mmdb - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mmdb - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /I ".\cdd" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /GZ /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\ncbimmdb.lib"

!ELSEIF  "$(CFG)" == "mmdb - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mmdb___Win32_Release"
# PROP BASE Intermediate_Dir "mmdb___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "mmdb___Win32_Release"
# PROP Intermediate_Dir "mmdb___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /I ".\cdd" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /GZ /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /Zi /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /I ".\cdd" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\ncbimmdb.lib"
# ADD LIB32 /nologo /out:"mmdb___Win32_Release\ncbimmdb.lib"

!ENDIF 

# Begin Target

# Name "mmdb - Win32 Debug"
# Name "mmdb - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cdd\cddposutil.c
# End Source File
# Begin Source File

SOURCE=.\cdd\cddutil.c
# End Source File
# Begin Source File

SOURCE=.\corematx.c
# End Source File
# Begin Source File

SOURCE=.\dvncode.c
# End Source File
# Begin Source File

SOURCE=.\mmdbapi.c
# End Source File
# Begin Source File

SOURCE=.\mmdbapi1.c
# End Source File
# Begin Source File

SOURCE=.\mmdbapi2.c
# End Source File
# Begin Source File

SOURCE=.\mmdbapi3.c
# End Source File
# Begin Source File

SOURCE=.\mmdbapi4.c
# End Source File
# Begin Source File

SOURCE=.\mmdbentr.c
# End Source File
# Begin Source File

SOURCE=.\cdd\objcdd.c
# End Source File
# Begin Source File

SOURCE=.\objmmdb1.c
# End Source File
# Begin Source File

SOURCE=.\objmmdb2.c
# End Source File
# Begin Source File

SOURCE=.\objmmdb3.c
# End Source File
# Begin Source File

SOURCE=.\prunebsc.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdalgs.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdatd.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdbwfi.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdcpal.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdcpll.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdcprl.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdcxei.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrddgri.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdrsmp.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdsal0.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdsalr.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdsalu.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdsgoi.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdslo0.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdslor.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdslou.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdspci.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdspea.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdspel.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdspni.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdttb0.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdttbi.c
# End Source File
# Begin Source File

SOURCE=.\cdd\thrdzsc.c
# End Source File
# Begin Source File

SOURCE=.\vastsubs.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cdd\cdd.h
# End Source File
# Begin Source File

SOURCE=.\corematx.h
# End Source File
# Begin Source File

SOURCE=.\dvncode.h
# End Source File
# Begin Source File

SOURCE=.\mmdbapi.h
# End Source File
# Begin Source File

SOURCE=.\mmdbapi1.h
# End Source File
# Begin Source File

SOURCE=.\mmdbapi2.h
# End Source File
# Begin Source File

SOURCE=.\mmdbapi3.h
# End Source File
# Begin Source File

SOURCE=.\mmdbapi4.h
# End Source File
# Begin Source File

SOURCE=.\mmdbdata.h
# End Source File
# Begin Source File

SOURCE=.\cdd\objcdd.h
# End Source File
# Begin Source File

SOURCE=.\objmmdb1.h
# End Source File
# Begin Source File

SOURCE=.\objmmdb2.h
# End Source File
# Begin Source File

SOURCE=.\objmmdb3.h
# End Source File
# Begin Source File

SOURCE=.\prunebsc.h
# End Source File
# Begin Source File

SOURCE=.\strimprt.h
# End Source File
# End Group
# End Target
# End Project
