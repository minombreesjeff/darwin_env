# Microsoft Developer Studio Project File - Name="blast" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=blast - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blast.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blast.mak" CFG="blast - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blast - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDLL"
# PROP BASE Intermediate_Dir "DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\tools" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\ddv" /I "..\..\..\cdromlib" /I "..\..\..\biostruc" /I "..\..\..\vibrant" /I "..\..\..\desktop" /I "..\..\..\network\taxon1\common" /I "..\..\..\network\taxon1\taxon2" /I "..\..\..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\.." /I "..\..\..\..\..\corelib" /I "..\..\..\..\..\asnstat" /I "..\..\..\..\..\asnlib" /I "..\..\..\..\..\tools" /I "..\..\..\..\..\object" /I "..\..\..\..\..\api" /I "..\..\..\..\..\ddv" /I "..\..\..\..\..\cdromlib" /I "..\..\..\..\..\biostruc" /I "..\..\..\..\..\vibrant" /I "..\..\..\..\..\desktop" /I "..\..\..\..\..\network\taxon1\common" /I "..\..\..\..\..\network\taxon1\taxon2" /I "..\..\..\..\..\network\blast3\client" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "blast - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\aa_ungapped.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_diagnostics.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_dust.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_encoding.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_engine.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_extend.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_filter.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_gapalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_hits.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_hspstream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_inline.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_itree.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_itree.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_kappa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_lookup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_message.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_options.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_psi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_psi_priv.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_psi_priv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_seg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_seqsrc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_setup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_stat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_traceback.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_util.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\gapinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\greedy_align.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\hspstream_collector.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\link_hsps.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\lookup_util.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\lookup_wrap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\matrix_freq_ratios.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\mb_lookup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\ncbi_math.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\ncbi_std.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\pattern.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\pattern_priv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_extend.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_gapalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_lookup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_parameters.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_posit.c
# End Source File
# End Group

# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\aa_ungapped.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_diagnostics.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_dust.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_encoding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_engine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_extend.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_filter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_gapalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_hits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_hspstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_lookup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_message.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_options.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_psi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_rps.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_seg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_seqsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_setup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_stat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_toolkit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_traceback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\gapinfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\greedy_align.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\hspstream_collector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\link_hsps.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\lookup_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\lookup_wrap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\matrix_freq_ratios.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\mb_lookup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\ncbi_math.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\ncbi_std.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\pattern.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_extend.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_gapalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\phi_lookup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_parameters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\algo\blast\core\blast_posit.h
# End Source File
# End Group
# End Target
# End Project
