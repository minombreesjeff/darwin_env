# Microsoft Developer Studio Project File - Name="ncbiobj" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ncbiobj - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncbiobj.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncbiobj.mak" CFG="ncbiobj - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncbiobj - Win32 DebugDLL" (based on "Win32 (x86) Static Library")
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
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /GR /Z7 /Od /I "..\..\.." /I "..\..\..\corelib" /I "..\..\..\asnstat" /I "..\..\..\asnlib" /I "..\..\..\object" /I "..\..\..\api" /I "..\..\..\cdromlib" /I "..\..\..\biostruc" /I "..\..\..\ddv" /I "..\..\..\access" /I "..\..\..\connect" /I "..\..\..\tools" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "ncbiobj - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\cdromlib\acccn3ds.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignmgr2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignval.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\aliparse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\aliread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\util\creaders\alnread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff6.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnb6.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\blocks.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\cddapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\codon.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ddvcolor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\ddv\ddvcreate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\edutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\ent2api.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\fdlobj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ffprint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\findrepl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ftusrstr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gather.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbfeat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbftglob.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbparint.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jsavlt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jzcoll.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jzmisc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\lsqfetch.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\maputil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\mconsist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\mimapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\needleman.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objacces.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objalignloc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objall.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objbibli.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objblock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objcode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\objent2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objfdef.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objfeat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objgbseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objgen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objinsdseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objloc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmdrs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmedli.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\objmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\objmim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objproj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objprt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objpub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objpubme.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objres.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objsset.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objsub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objtseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\pgppop.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\pmfapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\prtutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\qblastapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpacc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpedit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salprop.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpstat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salsap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salstruc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\samutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\satutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\seqmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\seqport.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sequtil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\simple.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sqnutil1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sqnutil2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sqnutil3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\strucapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\subutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tofasta.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tofile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tomedlin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\txalign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\udvseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\utilpars.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\utilpub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\valid.c
# End Source File
# Begin Source File

SOURCE=..\..\..\access\vecscnapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\viewmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\api\wprint.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\api\a2ferr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\a2ferrdf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\cdromlib\accentr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\alignloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignmgr2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\alignval.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\aliparse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\aliread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\allpub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\util\creaders\alnread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ff6.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ffg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2ffp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\asn2gnbk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnacces.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnbibli.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnblock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asncode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnent2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnfdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnfeat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asngen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnid0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnmdrs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnmedli.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnmime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnmla.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnneten.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnproj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnprt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnpub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnpubme.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnres.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnsset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asnsubmt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\asntaxon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\blocks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\blstspc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\cddapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\cdrom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\codon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\util\creaders\creaders_export.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ddvcolor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ddv\ddvcreate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\echo_asn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\edutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\ent2api.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\errdefn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\explore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\fdlobj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ffprint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\findrepl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\ftusrstr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gather.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbfeat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbftdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\gbparlex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jsavlt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jzcoll.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\jzmisc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\lsqfetch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\mapmime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\mapproj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\mappubme.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\maputil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\mconsist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\mimapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\mmdb1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\mmdb2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\mmdb3.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\ni_asn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objacces.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objalignloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objall.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objbibli.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objblock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objcode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\objent2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objfdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objfeat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objgbseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objgen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objinsdseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmdrs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objmedli.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\objmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\objmim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objproj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objprt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objpub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objpubd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objpubme.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objres.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objsset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objsub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\object\objtseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\parsegb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\pgppop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\pmfapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\prtutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\puberr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\qblastapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpacc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpedit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salprop.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\salpstat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\samutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\satutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\seqmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\seqport.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sequtil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\simple.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\spell.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\sqnutils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\strucapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\subutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\sugmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\asnstat\taxon1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tofasta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tofile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\tomedlin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\txalign.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\udvseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\utilpars.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\utilpub.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\valid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\validatr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\validerr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\access\vecscnapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\api\viewmgr.h
# End Source File
# End Group
# End Target
# End Project
