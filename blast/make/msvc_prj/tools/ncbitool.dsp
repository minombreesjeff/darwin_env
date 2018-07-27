# Microsoft Developer Studio Project File - Name="ncbitool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbitool - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbitool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbitool.mak" CFG="ncbitool - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbitool - Win32 DebugMT" (based on "Win32 (x86) Static Library")
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
# ADD CPP /nologo /MTd /W3 /GR /Zi /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\tools" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\ddv" /I "..\..\..\cdromlib" /I "..\..\..\biostruc" /I "..\..\..\vibrant" /I "..\..\..\desktop" /I "..\..\..\network\taxon1\common" /I "..\..\..\network\taxon1\taxon2" /I "..\..\..\network\blast3\client" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "ncbitool - Win32 DebugMT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\tools\actutils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg0.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalg5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalgn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastkar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastool.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastutl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bxmlobj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\csim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\db_slist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\dotseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\dust.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\falign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\g_any.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\gapxdrop.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\impatool.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\kappa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\lookup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\mbalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\mblast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\motif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisam.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisort.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\newkar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\pattern1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\pobutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\posit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\posit2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\pseed3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\readdb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\rpsutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\salign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\salptool.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\sec.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\seg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\sim2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\sim3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\sim4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\simutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\slist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\spidey.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\splutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\stsutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\suggadj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\suggest.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\suggslp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\taxblast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\tfuns.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\toasn3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\toporg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkbias.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkcnsrt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkdust.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkepi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkfltr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkgraph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkpcc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkptpf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urksigu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urktree.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\vecscrn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\wwwbutl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\xmlblast.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\tools\actutils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bandalgn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastkar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blastpri.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\blstxml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\bxmlobj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\db_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\dotseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\dust.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\gapxdrop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\lnfac.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\lookup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\mbalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\mblast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\motif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisam.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisami.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisort.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\ncbisrti.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\pobutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\posit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\profiles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\readdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\rpsutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\salign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\salmedia.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\salptool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\sec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\seed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\seg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\simutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\spidey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\splutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\stsutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\suggest.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\suggslp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\taxblast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\terr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\tfuns.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\toasn3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\toporg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkbias.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkcnsrt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkdust.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkepi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkfltr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkgraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkpcc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkptpf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urksigu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urktree.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\urkutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\vecscrn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\wwwblast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\tools\xmlblast.h
# End Source File
# End Group
# End Target
# End Project
