# Microsoft Developer Studio Project File - Name="ncbi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbi - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbi.mak" CFG="ncbi - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbi - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ncbi - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ncbi - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ncbi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ncbi___Win32_Release"
# PROP BASE Intermediate_Dir "ncbi___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ncbi___Win32_Release"
# PROP Intermediate_Dir "ncbi___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /Zi /I ".." /I "..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
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

# Name "ncbi - Win32 Debug"
# Name "ncbi - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\asnlib\asnbufo.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asndebin.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnenbin.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asngen.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asngenob.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnio.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnlex.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnlext.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnout.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnprint.c
# End Source File
# Begin Source File

SOURCE=..\asnlib\asntypes.c
# End Source File
# Begin Source File

SOURCE=.\binary.c
# End Source File
# Begin Source File

SOURCE=..\connect\con_sock.c
# End Source File
# Begin Source File

SOURCE=..\connect\con_stateless.c
# End Source File
# Begin Source File

SOURCE=..\connect\con_url.c
# End Source File
# Begin Source File

SOURCE=..\connect\connectn.c
# End Source File
# Begin Source File

SOURCE=..\connect\connutil.c
# End Source File
# Begin Source File

SOURCE=.\gifgen.c
# End Source File
# Begin Source File

SOURCE=.\matrix.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_buffer.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_core.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_socket_.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_util.c
# End Source File
# Begin Source File

SOURCE=.\ncbibs.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbibuf.c
# End Source File
# Begin Source File

SOURCE=.\ncbienv.c
# End Source File
# Begin Source File

SOURCE=.\ncbierr.c
# End Source File
# Begin Source File

SOURCE=.\ncbifile.c
# End Source File
# Begin Source File

SOURCE=.\ncbilang.c
# End Source File
# Begin Source File

SOURCE=.\ncbimath.c
# End Source File
# Begin Source File

SOURCE=.\ncbimem.c
# End Source File
# Begin Source File

SOURCE=.\ncbimisc.c
# End Source File
# Begin Source File

SOURCE=.\ncbimsg.c
# End Source File
# Begin Source File

SOURCE=.\ncbiprop.c
# End Source File
# Begin Source File

SOURCE=.\ncbisgml.c
# End Source File
# Begin Source File

SOURCE=..\connect\ncbisock.c
# End Source File
# Begin Source File

SOURCE=.\ncbistr.c
# End Source File
# Begin Source File

SOURCE=.\ncbithr.c
# ADD CPP /D "NCBI_NOTHREADS_AVAIL"
# End Source File
# Begin Source File

SOURCE=.\ncbitime.c
# End Source File
# Begin Source File

SOURCE=.\regex.c
# ADD CPP /D "REGEX_NCBI" /D "REGEX_MALLOC"
# End Source File
# Begin Source File

SOURCE=.\togif.c
# End Source File
# Begin Source File

SOURCE=.\tree.c
# End Source File
# Begin Source File

SOURCE=.\tsprintf.c
# End Source File
# Begin Source File

SOURCE=..\connect\urlquery.c
# End Source File
# Begin Source File

SOURCE=.\wwwutils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\asnlib\asndebin.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnenbin.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnio.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnlex.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnlext.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnout.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asnprint.h
# End Source File
# Begin Source File

SOURCE=..\asnlib\asntypes.h
# End Source File
# Begin Source File

SOURCE=.\binary.h
# End Source File
# Begin Source File

SOURCE=.\btree.h
# End Source File
# Begin Source File

SOURCE=..\connect\con_sock.h
# End Source File
# Begin Source File

SOURCE=..\connect\con_stateless.h
# End Source File
# Begin Source File

SOURCE=..\connect\con_url.h
# End Source File
# Begin Source File

SOURCE=..\connect\connectn.h
# End Source File
# Begin Source File

SOURCE=..\connect\connutil.h
# End Source File
# Begin Source File

SOURCE=.\gifgen.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\ncbi.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_buffer.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_core.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_socket.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbi_util.h
# End Source File
# Begin Source File

SOURCE=.\ncbibs.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbibuf.h
# End Source File
# Begin Source File

SOURCE=.\ncbienv.h
# End Source File
# Begin Source File

SOURCE=.\ncbierr.h
# End Source File
# Begin Source File

SOURCE=.\ncbifile.h
# End Source File
# Begin Source File

SOURCE=.\ncbilang.h
# End Source File
# Begin Source File

SOURCE=.\ncbilcl.h
# End Source File
# Begin Source File

SOURCE=.\ncbimath.h
# End Source File
# Begin Source File

SOURCE=.\ncbimem.h
# End Source File
# Begin Source File

SOURCE=.\ncbimisc.h
# End Source File
# Begin Source File

SOURCE=.\ncbimsg.h
# End Source File
# Begin Source File

SOURCE=.\ncbiprop.h
# End Source File
# Begin Source File

SOURCE=.\ncbisgml.h
# End Source File
# Begin Source File

SOURCE=..\connect\ncbisock.h
# End Source File
# Begin Source File

SOURCE=.\ncbistd.h
# End Source File
# Begin Source File

SOURCE=.\ncbistr.h
# End Source File
# Begin Source File

SOURCE=.\ncbithr.h
# End Source File
# Begin Source File

SOURCE=.\ncbitime.h
# End Source File
# Begin Source File

SOURCE=.\ncbiwin.h
# End Source File
# Begin Source File

SOURCE=.\regex.h
# End Source File
# Begin Source File

SOURCE=.\tree.h
# End Source File
# Begin Source File

SOURCE=.\tsprintf.h
# End Source File
# Begin Source File

SOURCE=..\api\undefwin.h
# End Source File
# Begin Source File

SOURCE=..\connect\urlquery.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ncbilcl.msw

!IF  "$(CFG)" == "ncbi - Win32 Debug"

# Begin Custom Build
InputDir=.
InputPath=.\ncbilcl.msw

"$(InputDir)\ncbilcl.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(InputDir)\ncbilcl.h

# End Custom Build

!ELSEIF  "$(CFG)" == "ncbi - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=.\ncbilcl.msw

"$(InputDir)\ncbilcl.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(InputDir)\ncbilcl.h

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
