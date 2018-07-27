# Microsoft Developer Studio Project File - Name="object" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=object - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "object.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "object.mak" CFG="object - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "object - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "object - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "object - Win32 Debug"

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
# ADD LIB32 /nologo /out:"Debug\ncbiobj.lib"

!ELSEIF  "$(CFG)" == "object - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "object___Win32_Release"
# PROP BASE Intermediate_Dir "object___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "object___Win32_Release"
# PROP Intermediate_Dir ""
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
# ADD BASE LIB32 /nologo /out:"Debug\ncbiobj.lib"
# ADD LIB32 /nologo /out:"object___Win32_Release\ncbiobj.lib"

!ENDIF 

# Begin Target

# Name "object - Win32 Debug"
# Name "object - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cdromlib\accmmdbs.c
# End Source File
# Begin Source File

SOURCE=..\api\alignmgr.c
# End Source File
# Begin Source File

SOURCE=..\api\alignval.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff1.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff2.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff3.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff4.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff5.c
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff6.c
# End Source File
# Begin Source File

SOURCE=..\api\blocks.c
# End Source File
# Begin Source File

SOURCE=..\api\codon.c
# End Source File
# Begin Source File

SOURCE=..\api\ddvcolor.c
# End Source File
# Begin Source File

SOURCE=..\ddv\ddvcreate.c
# End Source File
# Begin Source File

SOURCE=..\api\edutil.c
# End Source File
# Begin Source File

SOURCE=..\access\ent2api.c
# End Source File
# Begin Source File

SOURCE=..\api\ffprint.c
# End Source File
# Begin Source File

SOURCE=..\api\findrepl.c
# End Source File
# Begin Source File

SOURCE=..\api\ftusrstr.c
# End Source File
# Begin Source File

SOURCE=..\api\gather.c
# End Source File
# Begin Source File

SOURCE=..\api\gbfeat.c
# End Source File
# Begin Source File

SOURCE=..\api\gbftglob.c
# End Source File
# Begin Source File

SOURCE=..\api\gbparint.c
# End Source File
# Begin Source File

SOURCE=..\api\jzcoll.c
# End Source File
# Begin Source File

SOURCE=..\api\jzmisc.c
# End Source File
# Begin Source File

SOURCE=..\api\lsqfetch.c
# End Source File
# Begin Source File

SOURCE=..\api\maputil.c
# End Source File
# Begin Source File

SOURCE=..\api\mconsist.c
# End Source File
# Begin Source File

SOURCE=..\access\mimapi.c
# End Source File
# Begin Source File

SOURCE=..\api\needleman.c
# End Source File
# Begin Source File

SOURCE=.\objacces.c
# End Source File
# Begin Source File

SOURCE=.\objalign.c
# End Source File
# Begin Source File

SOURCE=.\objalignloc.c
# End Source File
# Begin Source File

SOURCE=.\objall.c
# End Source File
# Begin Source File

SOURCE=.\objbibli.c
# End Source File
# Begin Source File

SOURCE=.\objblock.c
# End Source File
# Begin Source File

SOURCE=.\objcode.c
# End Source File
# Begin Source File

SOURCE=..\access\objent2.c
# End Source File
# Begin Source File

SOURCE=.\objfdef.c
# End Source File
# Begin Source File

SOURCE=.\objfeat.c
# End Source File
# Begin Source File

SOURCE=.\objgen.c
# End Source File
# Begin Source File

SOURCE=.\objloc.c
# End Source File
# Begin Source File

SOURCE=.\objmdrs.c
# End Source File
# Begin Source File

SOURCE=.\objmedli.c
# End Source File
# Begin Source File

SOURCE=..\api\objmgr.c
# End Source File
# Begin Source File

SOURCE=..\access\objmim.c
# End Source File
# Begin Source File

SOURCE=.\objmime.c
# End Source File
# Begin Source File

SOURCE=.\objproj.c
# End Source File
# Begin Source File

SOURCE=.\objprt.c
# End Source File
# Begin Source File

SOURCE=.\objpub.c
# End Source File
# Begin Source File

SOURCE=.\objpubme.c
# End Source File
# Begin Source File

SOURCE=.\objres.c
# End Source File
# Begin Source File

SOURCE=.\objseq.c
# End Source File
# Begin Source File

SOURCE=.\objsset.c
# End Source File
# Begin Source File

SOURCE=.\objsub.c
# End Source File
# Begin Source File

SOURCE=..\api\pgppop.c
# End Source File
# Begin Source File

SOURCE=..\access\pmfapi.c
# End Source File
# Begin Source File

SOURCE=..\api\prtutil.c
# End Source File
# Begin Source File

SOURCE=..\access\qblastapi.c
# End Source File
# Begin Source File

SOURCE=..\api\salpacc.c
# End Source File
# Begin Source File

SOURCE=..\api\salpedit.c
# End Source File
# Begin Source File

SOURCE=..\api\salprop.c
# End Source File
# Begin Source File

SOURCE=..\api\salpstat.c
# End Source File
# Begin Source File

SOURCE=..\api\salsap.c
# End Source File
# Begin Source File

SOURCE=..\api\salstruc.c
# End Source File
# Begin Source File

SOURCE=..\api\salutil.c
# End Source File
# Begin Source File

SOURCE=..\api\samutil.c
# End Source File
# Begin Source File

SOURCE=..\api\satutil.c
# End Source File
# Begin Source File

SOURCE=..\api\seqmgr.c
# End Source File
# Begin Source File

SOURCE=..\api\seqport.c
# End Source File
# Begin Source File

SOURCE=..\api\sequtil.c
# End Source File
# Begin Source File

SOURCE=..\api\simple.c
# End Source File
# Begin Source File

SOURCE=..\api\sqnutil1.c
# End Source File
# Begin Source File

SOURCE=..\api\sqnutil2.c
# End Source File
# Begin Source File

SOURCE=..\api\sqnutil3.c
# End Source File
# Begin Source File

SOURCE=..\access\strucapi.c
# End Source File
# Begin Source File

SOURCE=..\api\subutil.c
# End Source File
# Begin Source File

SOURCE=..\api\tofasta.c
# End Source File
# Begin Source File

SOURCE=..\api\tofile.c
# End Source File
# Begin Source File

SOURCE=..\api\tomedlin.c
# End Source File
# Begin Source File

SOURCE=..\api\udvseq.c
# End Source File
# Begin Source File

SOURCE=..\api\utilpars.c
# End Source File
# Begin Source File

SOURCE=..\api\utilpub.c
# End Source File
# Begin Source File

SOURCE=..\api\valid.c
# End Source File
# Begin Source File

SOURCE=..\access\vecscnapi.c
# End Source File
# Begin Source File

SOURCE=..\api\viewmgr.c
# End Source File
# Begin Source File

SOURCE=..\api\wprint.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\api\a2ferr.h
# End Source File
# Begin Source File

SOURCE=..\api\a2ferrdf.h
# End Source File
# Begin Source File

SOURCE=..\cdromlib\accentr.h
# End Source File
# Begin Source File

SOURCE=.\alignloc.h
# End Source File
# Begin Source File

SOURCE=..\api\alignmgr.h
# End Source File
# Begin Source File

SOURCE=..\api\alignval.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\all.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\allpub.h
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff.h
# End Source File
# Begin Source File

SOURCE=..\api\asn2ff6.h
# End Source File
# Begin Source File

SOURCE=..\api\asn2ffg.h
# End Source File
# Begin Source File

SOURCE=..\api\asn2ffp.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnacces.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnalign.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnbibli.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnblock.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asncode.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnent2.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnfdef.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnfeat.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asngen.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnid0.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnloc.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnmdrs.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnmedli.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnmime.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnmla.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnneten.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnproj.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnprt.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnpub.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnpubme.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnres.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnseq.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnsset.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asnsubmt.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\asntaxon.h
# End Source File
# Begin Source File

SOURCE=..\api\blocks.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\blstspc.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\cdrom.h
# End Source File
# Begin Source File

SOURCE=..\api\codon.h
# End Source File
# Begin Source File

SOURCE=..\api\ddvcolor.h
# End Source File
# Begin Source File

SOURCE=..\ddv\ddvcreate.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\echo_asn.h
# End Source File
# Begin Source File

SOURCE=..\api\edutil.h
# End Source File
# Begin Source File

SOURCE=..\access\ent2api.h
# End Source File
# Begin Source File

SOURCE=..\api\errdefn.h
# End Source File
# Begin Source File

SOURCE=..\api\explore.h
# End Source File
# Begin Source File

SOURCE=..\api\ffprint.h
# End Source File
# Begin Source File

SOURCE=..\api\findrepl.h
# End Source File
# Begin Source File

SOURCE=..\api\ftusrstr.h
# End Source File
# Begin Source File

SOURCE=..\api\gather.h
# End Source File
# Begin Source File

SOURCE=..\api\gbfeat.h
# End Source File
# Begin Source File

SOURCE=..\api\gbftdef.h
# End Source File
# Begin Source File

SOURCE=..\api\gbparlex.h
# End Source File
# Begin Source File

SOURCE=..\api\jzcoll.h
# End Source File
# Begin Source File

SOURCE=..\api\jzmisc.h
# End Source File
# Begin Source File

SOURCE=..\api\lsqfetch.h
# End Source File
# Begin Source File

SOURCE=.\mapmime.h
# End Source File
# Begin Source File

SOURCE=.\mapproj.h
# End Source File
# Begin Source File

SOURCE=.\mappubme.h
# End Source File
# Begin Source File

SOURCE=..\api\maputil.h
# End Source File
# Begin Source File

SOURCE=..\api\mconsist.h
# End Source File
# Begin Source File

SOURCE=..\access\mimapi.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\mmdb1.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\mmdb2.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\mmdb3.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\ni_asn.h
# End Source File
# Begin Source File

SOURCE=.\objacces.h
# End Source File
# Begin Source File

SOURCE=.\objalign.h
# End Source File
# Begin Source File

SOURCE=.\objalignloc.h
# End Source File
# Begin Source File

SOURCE=.\objall.h
# End Source File
# Begin Source File

SOURCE=.\objbibli.h
# End Source File
# Begin Source File

SOURCE=.\objblock.h
# End Source File
# Begin Source File

SOURCE=.\objcode.h
# End Source File
# Begin Source File

SOURCE=..\access\objent2.h
# End Source File
# Begin Source File

SOURCE=.\objfdef.h
# End Source File
# Begin Source File

SOURCE=.\objfeat.h
# End Source File
# Begin Source File

SOURCE=.\objgen.h
# End Source File
# Begin Source File

SOURCE=.\objloc.h
# End Source File
# Begin Source File

SOURCE=.\objmdrs.h
# End Source File
# Begin Source File

SOURCE=.\objmedli.h
# End Source File
# Begin Source File

SOURCE=..\api\objmgr.h
# End Source File
# Begin Source File

SOURCE=..\access\objmim.h
# End Source File
# Begin Source File

SOURCE=.\objmime.h
# End Source File
# Begin Source File

SOURCE=.\objproj.h
# End Source File
# Begin Source File

SOURCE=.\objprt.h
# End Source File
# Begin Source File

SOURCE=.\objpub.h
# End Source File
# Begin Source File

SOURCE=.\objpubd.h
# End Source File
# Begin Source File

SOURCE=.\objpubme.h
# End Source File
# Begin Source File

SOURCE=.\objres.h
# End Source File
# Begin Source File

SOURCE=.\objseq.h
# End Source File
# Begin Source File

SOURCE=.\objsset.h
# End Source File
# Begin Source File

SOURCE=.\objsub.h
# End Source File
# Begin Source File

SOURCE=..\api\parsegb.h
# End Source File
# Begin Source File

SOURCE=..\api\pgppop.h
# End Source File
# Begin Source File

SOURCE=..\access\pmfapi.h
# End Source File
# Begin Source File

SOURCE=..\api\prtutil.h
# End Source File
# Begin Source File

SOURCE=..\api\puberr.h
# End Source File
# Begin Source File

SOURCE=..\access\qblastapi.h
# End Source File
# Begin Source File

SOURCE=..\api\salpacc.h
# End Source File
# Begin Source File

SOURCE=..\api\salpedit.h
# End Source File
# Begin Source File

SOURCE=..\api\salprop.h
# End Source File
# Begin Source File

SOURCE=..\api\salpstat.h
# End Source File
# Begin Source File

SOURCE=..\api\samutil.h
# End Source File
# Begin Source File

SOURCE=..\api\satutil.h
# End Source File
# Begin Source File

SOURCE=..\api\seqmgr.h
# End Source File
# Begin Source File

SOURCE=..\api\seqport.h
# End Source File
# Begin Source File

SOURCE=..\api\sequtil.h
# End Source File
# Begin Source File

SOURCE=..\api\simple.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\spell.h
# End Source File
# Begin Source File

SOURCE=..\api\sqnutils.h
# End Source File
# Begin Source File

SOURCE=..\access\strucapi.h
# End Source File
# Begin Source File

SOURCE=..\api\subutil.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\sugmap.h
# End Source File
# Begin Source File

SOURCE=..\asnstat\taxon1.h
# End Source File
# Begin Source File

SOURCE=..\api\tofasta.h
# End Source File
# Begin Source File

SOURCE=..\api\tofile.h
# End Source File
# Begin Source File

SOURCE=..\api\tomedlin.h
# End Source File
# Begin Source File

SOURCE=..\api\udvseq.h
# End Source File
# Begin Source File

SOURCE=..\api\utilpars.h
# End Source File
# Begin Source File

SOURCE=..\api\utilpub.h
# End Source File
# Begin Source File

SOURCE=..\api\valid.h
# End Source File
# Begin Source File

SOURCE=..\api\validatr.h
# End Source File
# Begin Source File

SOURCE=..\api\validerr.h
# End Source File
# Begin Source File

SOURCE=..\access\vecscnapi.h
# End Source File
# Begin Source File

SOURCE=..\api\viewmgr.h
# End Source File
# End Group
# End Target
# End Project
