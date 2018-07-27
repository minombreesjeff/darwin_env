# Microsoft Developer Studio Project File - Name="tools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tools - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tools.mak" CFG="tools - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tools - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tools - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tools - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\ncbitool.lib"

!ELSEIF  "$(CFG)" == "tools - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tools___Win32_Release"
# PROP BASE Intermediate_Dir "tools___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "tools___Win32_Release"
# PROP Intermediate_Dir "tools___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "..\ddv" /I "..\..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /Zi /I ".." /I "..\ddv" /I "..\..\ddv" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\ncbitool.lib"
# ADD LIB32 /nologo /out:"tools___Win32_Release\ncbitool.lib"

!ENDIF 

# Begin Target

# Name "tools - Win32 Debug"
# Name "tools - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\actutils.c
# End Source File
# Begin Source File

SOURCE=.\bandalg0.c
# End Source File
# Begin Source File

SOURCE=.\bandalg1.c
# End Source File
# Begin Source File

SOURCE=.\bandalg2.c
# End Source File
# Begin Source File

SOURCE=.\bandalg3.c
# End Source File
# Begin Source File

SOURCE=.\bandalg4.c
# End Source File
# Begin Source File

SOURCE=.\bandalg5.c
# End Source File
# Begin Source File

SOURCE=.\bandalgn.c
# End Source File
# Begin Source File

SOURCE=.\blast.c
# End Source File
# Begin Source File

SOURCE=.\blastkar.c
# End Source File
# Begin Source File

SOURCE=.\blastool.c
# End Source File
# Begin Source File

SOURCE=.\blastutl.c
# End Source File
# Begin Source File

SOURCE=.\bxmlobj.c
# End Source File
# Begin Source File

SOURCE=.\csim.c
# End Source File
# Begin Source File

SOURCE=.\db_slist.c
# End Source File
# Begin Source File

SOURCE=.\dust.c
# End Source File
# Begin Source File

SOURCE=.\falign.c
# End Source File
# Begin Source File

SOURCE=.\fdlobj.c
# End Source File
# Begin Source File

SOURCE=.\g_any.c
# End Source File
# Begin Source File

SOURCE=.\gapxdrop.c
# End Source File
# Begin Source File

SOURCE=.\impatool.c
# End Source File
# Begin Source File

SOURCE=.\kappa.c
# End Source File
# Begin Source File

SOURCE=.\lookup.c
# End Source File
# Begin Source File

SOURCE=.\mbalign.c
# End Source File
# Begin Source File

SOURCE=.\mblast.c
# End Source File
# Begin Source File

SOURCE=.\mbutils.c
# End Source File
# Begin Source File

SOURCE=.\ncbisam.c
# End Source File
# Begin Source File

SOURCE=.\ncbisort.c
# End Source File
# Begin Source File

SOURCE=.\newkar.c
# End Source File
# Begin Source File

SOURCE=.\pattern1.c
# End Source File
# Begin Source File

SOURCE=.\pobutil.c
# End Source File
# Begin Source File

SOURCE=.\posit.c
# End Source File
# Begin Source File

SOURCE=.\posit2.c
# End Source File
# Begin Source File

SOURCE=.\pseed3.c
# End Source File
# Begin Source File

SOURCE=.\readdb.c
# End Source File
# Begin Source File

SOURCE=.\rpsutil.c
# End Source File
# Begin Source File

SOURCE=.\salign.c
# End Source File
# Begin Source File

SOURCE=.\salptool.c
# End Source File
# Begin Source File

SOURCE=.\sec.c
# End Source File
# Begin Source File

SOURCE=.\seg.c
# End Source File
# Begin Source File

SOURCE=.\sim2.c
# End Source File
# Begin Source File

SOURCE=.\sim3.c
# End Source File
# Begin Source File

SOURCE=.\sim4.c
# End Source File
# Begin Source File

SOURCE=.\simutil.c
# End Source File
# Begin Source File

SOURCE=.\slist.c
# End Source File
# Begin Source File

SOURCE=.\splutil.c
# End Source File
# Begin Source File

SOURCE=.\stsutil.c
# End Source File
# Begin Source File

SOURCE=.\suggadj.c
# End Source File
# Begin Source File

SOURCE=.\suggest.c
# End Source File
# Begin Source File

SOURCE=.\suggslp.c
# End Source File
# Begin Source File

SOURCE=.\tfuns.c
# End Source File
# Begin Source File

SOURCE=.\toasn3.c
# End Source File
# Begin Source File

SOURCE=.\toporg.c
# End Source File
# Begin Source File

SOURCE=.\txalign.c
# End Source File
# Begin Source File

SOURCE=.\urkbias.c
# End Source File
# Begin Source File

SOURCE=.\urkcnsrt.c
# End Source File
# Begin Source File

SOURCE=.\urkdust.c
# End Source File
# Begin Source File

SOURCE=.\urkepi.c
# End Source File
# Begin Source File

SOURCE=.\urkfltr.c
# End Source File
# Begin Source File

SOURCE=.\urkpcc.c
# End Source File
# Begin Source File

SOURCE=.\urkptpf.c
# End Source File
# Begin Source File

SOURCE=.\urksigu.c
# End Source File
# Begin Source File

SOURCE=.\urktree.c
# End Source File
# Begin Source File

SOURCE=.\urkutil.c
# End Source File
# Begin Source File

SOURCE=.\vecscrn.c
# End Source File
# Begin Source File

SOURCE=.\xmlblast.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\actutils.h
# End Source File
# Begin Source File

SOURCE=.\bandalgn.h
# End Source File
# Begin Source File

SOURCE=.\blast.h
# End Source File
# Begin Source File

SOURCE=.\blastdef.h
# End Source File
# Begin Source File

SOURCE=.\blastkar.h
# End Source File
# Begin Source File

SOURCE=.\blastpri.h
# End Source File
# Begin Source File

SOURCE=.\blstxml.h
# End Source File
# Begin Source File

SOURCE=.\bxmlobj.h
# End Source File
# Begin Source File

SOURCE=.\db_list.h
# End Source File
# Begin Source File

SOURCE=.\dust.h
# End Source File
# Begin Source File

SOURCE=.\gapxdrop.h
# End Source File
# Begin Source File

SOURCE=.\list.h
# End Source File
# Begin Source File

SOURCE=.\lnfac.h
# End Source File
# Begin Source File

SOURCE=.\lookup.h
# End Source File
# Begin Source File

SOURCE=.\mbalign.h
# End Source File
# Begin Source File

SOURCE=.\mblast.h
# End Source File
# Begin Source File

SOURCE=.\mbutils.h
# End Source File
# Begin Source File

SOURCE=.\ncbisam.h
# End Source File
# Begin Source File

SOURCE=.\ncbisami.h
# End Source File
# Begin Source File

SOURCE=.\ncbisort.h
# End Source File
# Begin Source File

SOURCE=.\ncbisrti.h
# End Source File
# Begin Source File

SOURCE=.\needleman.h
# End Source File
# Begin Source File

SOURCE=.\pobutil.h
# End Source File
# Begin Source File

SOURCE=.\posit.h
# End Source File
# Begin Source File

SOURCE=.\profiles.h
# End Source File
# Begin Source File

SOURCE=.\readdb.h
# End Source File
# Begin Source File

SOURCE=.\rpsutil.h
# End Source File
# Begin Source File

SOURCE=.\salign.h
# End Source File
# Begin Source File

SOURCE=.\salptool.h
# End Source File
# Begin Source File

SOURCE=.\sec.h
# End Source File
# Begin Source File

SOURCE=.\seed.h
# End Source File
# Begin Source File

SOURCE=.\seg.h
# End Source File
# Begin Source File

SOURCE=.\simutil.h
# End Source File
# Begin Source File

SOURCE=.\splutil.h
# End Source File
# Begin Source File

SOURCE=.\stsutil.h
# End Source File
# Begin Source File

SOURCE=.\suggest.h
# End Source File
# Begin Source File

SOURCE=.\suggslp.h
# End Source File
# Begin Source File

SOURCE=.\terr.h
# End Source File
# Begin Source File

SOURCE=.\tfuns.h
# End Source File
# Begin Source File

SOURCE=.\toasn3.h
# End Source File
# Begin Source File

SOURCE=.\toporg.h
# End Source File
# Begin Source File

SOURCE=.\txalign.h
# End Source File
# Begin Source File

SOURCE=.\urkbias.h
# End Source File
# Begin Source File

SOURCE=.\urkcnsrt.h
# End Source File
# Begin Source File

SOURCE=.\urkdust.h
# End Source File
# Begin Source File

SOURCE=.\urkepi.h
# End Source File
# Begin Source File

SOURCE=.\urkfltr.h
# End Source File
# Begin Source File

SOURCE=.\urkpcc.h
# End Source File
# Begin Source File

SOURCE=.\urkptpf.h
# End Source File
# Begin Source File

SOURCE=.\urksigu.h
# End Source File
# Begin Source File

SOURCE=.\urktree.h
# End Source File
# Begin Source File

SOURCE=.\urkutil.h
# End Source File
# Begin Source File

SOURCE=.\vecscrn.h
# End Source File
# Begin Source File

SOURCE=.\xmlblast.h
# End Source File
# End Group
# End Target
# End Project
