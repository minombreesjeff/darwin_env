# Microsoft Developer Studio Project File - Name="desktop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=desktop - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "desktop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "desktop.mak" CFG="desktop - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "desktop - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "desktop - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "desktop - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "desktop___Win32_Debug"
# PROP BASE Intermediate_Dir "desktop___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W2 /Gm /GX /ZI /Od /I "..\ddv" /I "../desktop" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"debug\ncbidesk.lib"

!ELSEIF  "$(CFG)" == "desktop - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "desktop___Win32_Release"
# PROP BASE Intermediate_Dir "desktop___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "desktop___Win32_Release"
# PROP Intermediate_Dir "desktop___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W2 /Gm /GX /ZI /Od /I "..\ddv" /I "../desktop" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W2 /Gm /GX /Zi /I "..\ddv" /I "../desktop" /I "..\cn3d" /I "..\access" /I "..\asnstat" /I "..\connect\lbapi" /I "..\connect" /I "..\asnlib" /I "..\vibrant" /I "..\biostruc" /I "..\object" /I "..\api" /I "..\cdromlib" /I "..\desktop" /I "..\tools" /I "..\corelib" /I "..\network\taxon1\common" /I "..\network\vibnet" /I "..\network\entrez\client" /I "..\network\nsclilib" /I "..\network\medarch\client" /I "..\network\id1arch" /I "..\network\taxon1\taxon2" /I "..\network\blast3\client" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"debug\ncbidesk.lib"
# ADD LIB32 /nologo /out:"desktop___Win32_Release\ncbidesk.lib"

!ENDIF 

# Begin Target

# Name "desktop - Win32 Debug"
# Name "desktop - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aacomp.c
# End Source File
# Begin Source File

SOURCE=.\apparam.c
# End Source File
# Begin Source File

SOURCE=.\biosrc.c
# End Source File
# Begin Source File

SOURCE=.\bspview.c
# End Source File
# Begin Source File

SOURCE=.\cdrgn.c
# End Source File
# Begin Source File

SOURCE=.\dlgutil1.c
# End Source File
# Begin Source File

SOURCE=.\dlgutil2.c
# End Source File
# Begin Source File

SOURCE=.\dotmatrx.c
# End Source File
# Begin Source File

SOURCE=.\drawseq.c
# End Source File
# Begin Source File

SOURCE=.\fea2seg.c
# End Source File
# Begin Source File

SOURCE=.\fstyle.c
# End Source File
# Begin Source File

SOURCE=.\gbfview.c
# End Source File
# Begin Source File

SOURCE=.\glbpic.c
# End Source File
# Begin Source File

SOURCE=.\gphdraw.c
# End Source File
# Begin Source File

SOURCE=.\gphview.c
# End Source File
# Begin Source File

SOURCE=.\gtrdraw.c
# End Source File
# Begin Source File

SOURCE=.\gxydraw.c
# End Source File
# Begin Source File

SOURCE=.\import.c
# End Source File
# Begin Source File

SOURCE=.\layout.c
# End Source File
# Begin Source File

SOURCE=.\mapgene.c
# End Source File
# Begin Source File

SOURCE=.\medview.c
# End Source File
# Begin Source File

SOURCE=.\prtgene.c
# End Source File
# Begin Source File

SOURCE=.\pubdesc.c
# End Source File
# Begin Source File

SOURCE=.\saldist.c
# End Source File
# Begin Source File

SOURCE=.\saled.c
# End Source File
# Begin Source File

SOURCE=.\salfiles.c
# End Source File
# Begin Source File

SOURCE=.\salpanel.c
# End Source File
# Begin Source File

SOURCE=.\salparam.c
# End Source File
# Begin Source File

SOURCE=.\salsa.c
# End Source File
# Begin Source File

SOURCE=.\seqanal.c
# End Source File
# Begin Source File

SOURCE=.\seqfltr.c
# End Source File
# Begin Source File

SOURCE=.\seqgraph.c
# End Source File
# Begin Source File

SOURCE=.\seqgrphx.c
# End Source File
# Begin Source File

SOURCE=.\seqmtrx.c
# End Source File
# Begin Source File

SOURCE=.\seqpcc.c
# End Source File
# Begin Source File

SOURCE=.\seqscrl.c
# End Source File
# Begin Source File

SOURCE=.\seqsub.c
# End Source File
# Begin Source File

SOURCE=.\smdlg1.c
# End Source File
# Begin Source File

SOURCE=.\smdlg2.c
# End Source File
# Begin Source File

SOURCE=.\smdlg3.c
# End Source File
# Begin Source File

SOURCE=.\smtools.c
# End Source File
# Begin Source File

SOURCE=.\styleman.c
# End Source File
# Begin Source File

SOURCE=.\stylemui.c
# End Source File
# Begin Source File

SOURCE=.\udvgraph.c
# End Source File
# Begin Source File

SOURCE=.\udvopen.c
# End Source File
# Begin Source File

SOURCE=.\udvpanel.c
# End Source File
# Begin Source File

SOURCE=.\vsm.c
# End Source File
# Begin Source File

SOURCE=.\vsmfile.c
# End Source File
# Begin Source File

SOURCE=.\vsmutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\aacomp.h
# End Source File
# Begin Source File

SOURCE=.\apparam.h
# End Source File
# Begin Source File

SOURCE=.\biosrc.h
# End Source File
# Begin Source File

SOURCE=.\bspview.h
# End Source File
# Begin Source File

SOURCE=.\cdrgn.h
# End Source File
# Begin Source File

SOURCE=.\dlogutil.h
# End Source File
# Begin Source File

SOURCE=.\dotmatrx.h
# End Source File
# Begin Source File

SOURCE=.\drawseq.h
# End Source File
# Begin Source File

SOURCE=.\fea2seg.h
# End Source File
# Begin Source File

SOURCE=.\fstyle.h
# End Source File
# Begin Source File

SOURCE=.\fstylep.h
# End Source File
# Begin Source File

SOURCE=.\glbpic.h
# End Source File
# Begin Source File

SOURCE=.\gphdraw.h
# End Source File
# Begin Source File

SOURCE=.\gtrdraw.h
# End Source File
# Begin Source File

SOURCE=.\gxydraw.h
# End Source File
# Begin Source File

SOURCE=.\import.h
# End Source File
# Begin Source File

SOURCE=.\layout.h
# End Source File
# Begin Source File

SOURCE=.\legend.h
# End Source File
# Begin Source File

SOURCE=.\mapgene.h
# End Source File
# Begin Source File

SOURCE=.\medview.h
# End Source File
# Begin Source File

SOURCE=.\prtgene.h
# End Source File
# Begin Source File

SOURCE=.\pubdesc.h
# End Source File
# Begin Source File

SOURCE=.\saldist.h
# End Source File
# Begin Source File

SOURCE=.\saled.h
# End Source File
# Begin Source File

SOURCE=.\saledit.h
# End Source File
# Begin Source File

SOURCE=.\salfiles.h
# End Source File
# Begin Source File

SOURCE=.\salpanel.h
# End Source File
# Begin Source File

SOURCE=.\salparam.h
# End Source File
# Begin Source File

SOURCE=.\seqanal.h
# End Source File
# Begin Source File

SOURCE=.\seqfltr.h
# End Source File
# Begin Source File

SOURCE=.\seqgraph.h
# End Source File
# Begin Source File

SOURCE=.\seqgrphx.h
# End Source File
# Begin Source File

SOURCE=.\seqmtrx.h
# End Source File
# Begin Source File

SOURCE=.\seqpcc.h
# End Source File
# Begin Source File

SOURCE=.\seqscrl.h
# End Source File
# Begin Source File

SOURCE=.\seqsub.h
# End Source File
# Begin Source File

SOURCE=.\udviewer.h
# End Source File
# Begin Source File

SOURCE=.\vsm.h
# End Source File
# Begin Source File

SOURCE=.\vsmfile.h
# End Source File
# Begin Source File

SOURCE=.\vsmpriv.h
# End Source File
# Begin Source File

SOURCE=.\vsmutil.h
# End Source File
# End Group
# End Target
# End Project
