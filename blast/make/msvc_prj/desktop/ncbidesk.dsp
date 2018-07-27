# Microsoft Developer Studio Project File - Name="ncbidesk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbidesk - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbidesk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbidesk.mak" CFG="ncbidesk - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbidesk - Win32 DebugMT" (based on "Win32 (x86) Static Library")
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
# ADD CPP /nologo /MTd /W3 /GR /Zi /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\vibrant" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\desktop" /I "..\..\..\cdromlib" /I "..\..\..\tools" /I "..\..\..\biostruc" /I "..\..\..\access" /I "..\..\..\connect" /I "..\..\..\cn3d" /I "..\..\..\gif" /I "..\..\..\network\blast3\client" /I "..\..\..\network\nsclilib" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "ncbidesk - Win32 DebugMT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\desktop\aacomp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\apparam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\asn2graphic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\biosrc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\bspview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\cdrgn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dlgutil1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dlgutil2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dotmatrx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dotvibrant.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\drawseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\e2docsum.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\e2trmlst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\fea2seg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\fstyle.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gbfview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\glbpic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gphdraw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gphview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gtrdraw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gxydraw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\import.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingenext.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingengraph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingenwin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\layout.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\mapgene.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\medview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\prtgene.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\pubdesc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\saldist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\saled.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salfiles.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salogif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salpanel.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salparam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salsa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqanal.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqfltr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqgraph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqgrphx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqmtrx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqpcc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqscrl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqsub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\smdlg1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\smdlg2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\smdlg3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\smtools.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\styleman.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\stylemui.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\udvgraph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\udvopen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\udvpanel.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsmfile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsmutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\desktop\aacomp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\apparam.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\asn2graphic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\biosrc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\bspview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\cdrgn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dlogutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\dotmatrx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\drawseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\fea2seg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\fstyle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\fstylep.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\glbpic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gphdraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gtrdraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\gxydraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\import.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingenext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingengraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\ingenwin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\layout.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\legend.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\mapgene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\medview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\prtgene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\pubdesc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\saldist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\saled.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\saledit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salfiles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salogif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salpanel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\salparam.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqanal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqfltr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqgraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqgrphx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqmtrx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqpcc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqscrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\seqsub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\udviewer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsmfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsmpriv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\desktop\vsmutil.h
# End Source File
# End Group
# End Target
# End Project
