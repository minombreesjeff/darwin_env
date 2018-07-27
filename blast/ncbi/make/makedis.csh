#!/bin/csh -f
#
# $Id: makedis.csh,v 1.99 2004/05/03 18:12:03 beloslyu Exp $
#
##                            PUBLIC DOMAIN NOTICE                          
#               National Center for Biotechnology Information
#                                                                          
#  This software/database is a "United States Government Work" under the   
#  terms of the United States Copyright Act.  It was written as part of    
#  the author's official duties as a United States Government employee and 
#  thus cannot be copyrighted.  This software/database is freely available 
#  to the public for use. The National Library of Medicine and the U.S.    
#  Government have not placed any restriction on its use or reproduction.  
#                                                                          
#  Although all reasonable efforts have been taken to ensure the accuracy  
#  and reliability of the software and data, the NLM and the U.S.          
#  Government do not and cannot warrant the performance or results that    
#  may be obtained by using this software or data. The NLM and the U.S.    
#  Government disclaim all warranties, express or implied, including       
#  warranties of performance, merchantability or fitness for any particular
#  purpose.                                                                
#                                                                          
#  Please cite the author in any work or product based on this material.   
#  Author: Karl Sirotkin <sirotkin@ncbi.nlm.nih.gov>
#
#  IBM AIX section: cpsosa@us.ibm.com  Jun-2001
#
# Script to untar and make the NCBI toolkit on Solaris.
#

# NOTE:  use "/bin/tcsh" above if "/bin/csh" is absent (e.g. on QNX OS)


set MFLG=""

if ("X$1" == "X-n") then
	set MFLG="-n"
	shift	
endif

set tar_file = $1
set cwd = `pwd`

# do we need to extract the tar archive?
if ( "X$tar_file" != "X" && "$tar_file" != "-") then
	if (! -r "$tar_file") then
		echo Unable to find the file "$tar_file"
		exit 1
	endif

	if (-d "ncbi") then
		echo "ncbi directory already exists, please remove or rename it"
		exit 2
	endif

	ls -l $tar_file
	tar xvf $tar_file
else
	# make sure that ncbi/build directory exists
	if ( ! -d "ncbi/build" ) then
		echo 'ncbi/build directory should exist. Did you extract ncbi.tar.Z?'
		exit 2
	endif
endif

set os=`uname -s`

#by default any Unix has Motif installed. In case of Linux do a check later.
set HAVE_MOTIF=1
#darwin will use native Mac GUI
set HAVE_MAC=0
#we will try to build OpenGL version of vibrant
set HAVE_OGL=1

switch ($os)
case SunOS:
	switch (`uname -r`)
	case "4.1*":
		set platform=sun
		breaksw
	default:
		if ( `uname -p` == i386 ) then
			set platform=solarisintel
			if ("$?CC" == 1) then
				if ("$CC" == "gcc") then
					set platform=solaris-gcc
				endif
			endif
		else
			set platform=solaris
			if ("$?CC" == 1) then
				if ("$CC" == "gcc") then
					set platform=solaris-gcc
				endif
			else if ("$?SOLARIS_MODE" == 1) then
				if ("$SOLARIS_MODE" == "64") then
					set platform=solaris64
				endif
			endif
		endif
		breaksw
	endsw
	set HAVE_OGL=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 /usr/openwin/include )
		if (-d $i/GL) then
			set HAVE_OGL=1
			echo OpenGL found at $i/GL
			break
		endif
	end
	breaksw
case IRIX*:
	switch (`uname -r`)
	case "4.*":
		set platform=sgi4
		breaksw
	case "5.*":
		set platform=sgi5
		breaksw
	case "6.5":
		#set platform=sgi-mips4
		set platform=sgi
		breaksw
	case "6.[0-4]":
		set platform=sgi
		breaksw
	default:
		set platform=sgi
		breaksw
	endsw
	breaksw
case OSF1:
	set platform=alphaOSF1
	set HAVE_MOTIF=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 )
		if (-d $i/Xm) then
			set HAVE_MOTIF=1
			echo Motif found at $i/Xm
			break
		endif
	end
	breaksw
case Linux:
    echo "libs version is:"
	ls -l /lib/libc.so*
	echo "the gcc version is:"
	gcc -v
	switch (`uname -m`)
	case "ia64":
		if (-e `which ecc`) then
			set platform=linux_ecc
		else
			set platform=linux
		endif
		breaksw
	case "ppc":
		set platform=ppclinux
		breaksw
	case "parisc":
		set platform=hppalinux
		breaksw
	case "i?86":
		set platform=linux-x86
		breaksw
	default:
		set platform=linux
		breaksw
	endsw
	#check do we have Motif on linux installed
	set HAVE_MOTIF=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 )
		if (-d $i/Xm) then
			set HAVE_MOTIF=1
			echo Motif found at $i/Xm
			echo "Warning: Motif version 1.2 is recommended"
			break
		endif
	end
	#check do we have OpenGL installed
	set HAVE_OGL=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 /usr/openwin/include )
		if (-d $i/GL) then
			set HAVE_OGL=1
			echo OpenGL found at $i/GL
			break
		endif
	end
	breaksw
case FreeBSD:
	set platform=freebsd
	set HAVE_MOTIF=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 )
		if (-d $i/Xm) then
			set HAVE_MOTIF=1
			echo Motif found at $i/Xm
			break
		endif
	end
	breaksw
case Darwin:
	set platform=darwin
	set HAVE_MOTIF=0
	set HAVE_MAC=1
	breaksw
case NetBSD:
	set platform=netbsd
	set HAVE_MOTIF=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 )
		if (-d $i/Xm) then
			set HAVE_MOTIF=1
			echo Motif found at $i/Xm
			break
		endif
	end
	breaksw
case AIX:
	set arch=`uname -M`
	set platform=ibm_auto
	if ("$?AIX_MODE" == 1) then
		if ("$AIX_MODE" == "64") then
			echo "64-bit mode build was selected for AIX"
			set platform=ibm_auto64
		endif
	endif
	echo "AIX machine is $arch"
	set HAVE_MOTIF=0
	foreach i (/usr/X11R6/include /usr/X11R6/include/X11 /usr/include \
		/usr/include/X11 )
		if (-d $i/Xm) then
			set HAVE_MOTIF=1
			echo Motif found at $i/Xm
			break
		endif
	end
	breaksw
case HP-UX:
	switch (`uname -m`)
	case "ia64":
		set platform=hpux_ia64
		breaksw
	case "9000/800":
		set platform=hpux
		breaksw
	default:
		set platform=hpux
		breaksw
	endsw
	breaksw
case QNX:
    #uname -a: QNX qnxrulez 6.1.0 2001/06/25-15:31:48edt x86pc x86
	set platform=qnx
	breaksw
default:
	echo Platform not found : `uname -a`
	goto BADPLATFORM
	breaksw
endsw

echo platform is $platform
uname -a

set NCBI_DOT_MK = ncbi/platform/${platform}.ncbi.mk

if (! -r "$NCBI_DOT_MK") then
  goto BADPLATFORM
endif

set noglob
# take the file $NCBI_DOT_MK and convert it to be suitable for csh eval:
# (1) remove comments at the beginning of the lines
# (2) change variable referenses to be in curly brackets - $AAA -> ${AAA}
# (3) remove excessive spaces around the equal sign
# (4) change Makefile assignments to csh ones: AAA=bb cc -> setenv AAA "bb cc"
eval `sed -e 's/^ *#.*//g' -e 's/\$(\([a-zA-Z_]*\))/\${\1}/g' -e 's/ *= */=/g' -e 's/^\([^=]*\)=\(.*\)$/setenv \1 "\2";/' < $NCBI_DOT_MK`
unset noglob

cd ncbi/build
ln -s ../make/*.unx .
ln -s ../make/ln-if-absent .
mv makeall.unx makefile

if ( -r ../demo/.BLAST_VERSION ) then
	echo BLAST version is `cat ../demo/.BLAST_VERSION`
endif


#  Inherited to this system is the requirement to use:
#    TO USE VIBRANT
# to have for makeall, this line
#  LIB30=libncbicn3d.a LIB28=libvibgif.a LIB4=libvibrant.a LIB20=libncbidesk.a 
#  LIB45=libddvlib.a  and for the makenet, this symbol
#  BLIB31=libvibnet.a 
#

#uncomment two following lines to don't build X11 apps
#set HAVE_OGL=0
#set HAVE_MOTIF=0

# if $OPENGL_TARGETS (in <platform>.ncbi.mk) is defined, 
# then add the appropriate flags, libraries, and binaries for OpenGL apps
if ("$?OPENGL_TARGETS" == "1" && "$HAVE_OGL" == "1" ) then
    set OGL_NCBI_LIBS="$OPENGL_NCBI_LIBS"
    set OGL_INCLUDE="$OPENGL_INCLUDE"
    set OGL_LIBS="$OPENGL_LIBS"
    set OGL_TARGETS="$OPENGL_TARGETS"
else
    set OGL_NCBI_LIBS=""
    set OGL_INCLUDE=""
    set OGL_LIBS=""
    set OGL_TARGETS=""
endif

# if LIBPNG_DIR and ZLIB_DIR (in <platform>.ncbi.mk) are defined, 
# then add the appropriate flags and libraries for PNG support
if ("$?LIBPNG_DIR" == "1" && "$?ZLIB_DIR" == "1") then
    set PNG_INCLUDE="-D_PNG -I$LIBPNG_DIR -I$ZLIB_DIR"
    set PNG_LIBS="$LIBPNG_DIR/libpng.a $ZLIB_DIR/libz.a"
else
    set PNG_INCLUDE=""
    set PNG_LIBS=""
endif

set VIBWWWBLAST=(psiblast.REAL psiblast_cs.REAL blast.REAL blast_cs.REAL)
set NONVIBWWWBLAST=(nph-viewgif.cgi wblast2.REAL wblast2_cs.REAL bl2bag.cgi)
set WWWBLAST=($VIBWWWBLAST $NONVIBWWWBLAST)

if ( "$HAVE_MOTIF" == 1 ) then
	set ALL_VIB=(LIB30=libncbicn3d.a \
		LIB28=libvibgif.a \
		LIB4=libvibrant.a \
		LIB20=libncbidesk.a \
		LIB45=libddvlib.a \
		$OGL_NCBI_LIBS \
		VIBFLAG=\"$NCBI_VIBFLAG\" \
		VIBLIBS=\"$NCBI_DISTVIBLIBS\")
	#By default we don't need to build demo programs with vibrant lib
	#set DEMO_VIB=(LIB4=-lvibrant \
	#	VIBLIBS=\"$NCBI_DISTVIBLIBS\" \
	#	VIBFLAG=\"$NCBI_VIBFLAG\")
	set DEMO_VIB=()

	set NET_VIB=(BLIB31=libvibnet.a \
		VIBLIBS=\"$NCBI_DISTVIBLIBS\" \
		OGLLIBS=\"$OGL_LIBS $PNG_LIBS\" \
		VIBFLAG=\"$NCBI_VIBFLAG\" \
		VIB=\"Psequin Nentrez udv ddv blastcl3 \
		idfetch bl2seq asn2gb entrez2 gbseqget \
		$WWWBLAST $OGL_TARGETS\") 
else if ( "$HAVE_MAC" == 1 ) then
	set ALL_VIB=(LIB30=libncbicn3d.a \
		LIB28=libvibgif.a \
		LIB4=libvibrant.a \
		LIB20=libncbidesk.a \
		LIB45=libddvlib.a \
		$OGL_NCBI_LIBS \
		VIBFLAG=\"$NCBI_VIBFLAG\" \
		VIBLIBS=\"$NCBI_DISTVIBLIBS\")
	#By default we don't need to build demo programs with vibrant lib
	#set DEMO_VIB=(LIB4=-lvibrant \
	#	VIBLIBS=\"$NCBI_DISTVIBLIBS\" \
	#	VIBFLAG=\"$NCBI_VIBFLAG\")
	set DEMO_VIB=()

	set NET_VIB=(BLIB31=libvibnet.a \
		VIBLIBS=\"$NCBI_DISTVIBLIBS\" \
		OGLLIBS=\"$OGL_LIBS $PNG_LIBS\" \
		VIBFLAG=\"$NCBI_VIBFLAG\" \
		VIB_POST_LINK=\"/Developer/Tools/Rez -t APPL ../link/macmet/Carbon.r -o\" \
		VIB=\"Psequin udv ddv blastcl3 \
		idfetch bl2seq asn2gb entrez2 gbseqget $WWWBLAST \") 
else # no Motif, build only ascii-based applications
    set OGL_NCBI_LIBS=""
    set OGL_INCLUDE=""
    set OGL_LIBS=""
    set OGL_TARGETS=""

	set ALL_VIB=()
	set DEMO_VIB=()
	set NET_VIB=(VIB=\"blastcl3 idfetch bl2seq asn2gb $NONVIBWWWBLAST \") 
endif

set CMD='make $MFLG \
   CFLAGS1=\"$NCBI_OPTFLAG $NCBI_CFLAGS1 $OGL_INCLUDE $PNG_INCLUDE\" \
   LDFLAGS1=\"$NCBI_LDFLAGS1\" OTHERLIBS=\"$NCBI_OTHERLIBS\" \
   SHELL=\"$NCBI_MAKE_SHELL\" LCL=\"$NCBI_DEFAULT_LCL\" \
   RAN=\"$NCBI_RANLIB\" AR=\"$NCBI_AR\" CC=\"$NCBI_CC\" $ALL_VIB all'
eval echo $CMD
eval echo $CMD | sh 

set make_stat = $status

if ( $make_stat != 0 ) then
	cat <<EoF

Fatal error building NCBI core libraries.
Please be sure that you have X11 and Motif libraries installed.
The NCBI toolkit FAQ at ftp://ftp.ncbi.nih.gov/toolbox/FAQ.html may be helpful.

EoF
	exit 1
endif

set CMD='make $MFLG -f makedemo.unx CFLAGS1=\"$NCBI_OPTFLAG $NCBI_CFLAGS1\" \
   LDFLAGS1=\"$NCBI_LDFLAGS1\" SHELL=\"$NCBI_MAKE_SHELL\" \
   LCL=\"$NCBI_DEFAULT_LCL\" RAN=\"$NCBI_RANLIB\" AR=\"$NCBI_AR\" CC=\"$NCBI_CC\" $DEMO_VIB'
eval echo $CMD
eval echo $CMD | sh 

set demo_stat = $status

#
# In case platform supports multi-threading, remake the apps which
# should be multithreaded, if at all possible.
#  Might repeat what is done above on some platforms.
#

set mtapps = "blastall blastpgp seedtop megablast rpsblast blastclust"

rm -f $mtapps


set CMD='make $MFLG -f makedemo.unx CFLAGS1=\"$NCBI_OPTFLAG $NCBI_CFLAGS1\" \
   LDFLAGS1=\"$NCBI_LDFLAGS1\" SHELL=\"$NCBI_MAKE_SHELL\" \
   LCL=\"$NCBI_DEFAULT_LCL\" RAN=\"$NCBI_RANLIB\" AR=\"$NCBI_AR\" CC=\"$NCBI_CC\"  \
   THREAD_OBJ=$NCBI_THREAD_OBJ THREAD_OTHERLIBS=\"$NCBI_MT_OTHERLIBS\" \
   $DEMO_VIB $mtapps'
eval echo $CMD
eval echo $CMD | sh 

set threaded_demo_stat = $status

if ("$?THREAD_OTHERLIBS" == "1") then
	set CMD='make $MFLG -f makenet.unx \
		CFLAGS1=\"$NCBI_OPTFLAG $NCBI_CFLAGS1 $OGL_INCLUDE\" \
		LDFLAGS1=\"$NCBI_LDFLAGS1\" SHELL=\"$NCBI_MAKE_SHELL\" \
		AR=\"$NCBI_AR\" CC=\"$NCBI_CC\" RAN=\"$NCBI_RANLIB\" OTHERLIBS=\"$NCBI_OTHERLIBS\" \
		THREAD_OTHERLIBS=\"$NCBI_THREAD_OTHERLIBS\" \
		NETENTREZVERSION=\"$NETENTREZVERSION\" $NET_VIB'
else
	set CMD='make $MFLG -f makenet.unx \
		CFLAGS1=\"$NCBI_OPTFLAG $NCBI_CFLAGS1 $OGL_INCLUDE\" \
		LDFLAGS1=\"$NCBI_LDFLAGS1\" SHELL=\"$NCBI_MAKE_SHELL\" \
		AR=\"$NCBI_AR\" CC=\"$NCBI_CC\" RAN=\"$NCBI_RANLIB\" OTHERLIBS=\"$NCBI_OTHERLIBS\" \
		NETENTREZVERSION=\"$NETENTREZVERSION\" $NET_VIB'
endif
eval echo $CMD
eval echo $CMD | sh 

set net_stat = $status

if ($make_stat != 0 || $demo_stat != 0 || $threaded_demo_stat != 0 || $net_stat != 0) then
   echo FAILURE primary make status = $make_stat, demo = $demo_stat, threaded_demo = $threaded_demo_stat, net = $net_stat
	cat <<EOF
#######
#        #####   #####    ####   #####
#        #    #  #    #  #    #  #    #
#####    #    #  #    #  #    #  #    #
#        #####   #####   #    #  #####
#        #   #   #   #   #    #  #   #
#######  #    #  #    #   ####   #    #
EOF
   exit 1
else
   # we are in ncbi/build directory now. Let us make the VERSION file
   echo "Put the date stamp to the file ../VERSION"
   echo "The executable files were built on `date`" > ../VERSION
   echo "The version number of each individual application" >> ../VERSION
   echo "may be found in the appropriate documentation files in ./ncbi/doc/" >> ../VERSION
   echo "uname -a ouput is: `uname -a`" >> ../VERSION
   foreach i ( Nentrez Psequin asn2ff asn2xml asn2gb asndhuff asntool bl2seq \
	blastall blastcl3 blastclust blastpgp cdscan checksub \
	copymat ddv demo_regexp demo_regexp_grep dosimple entrcmd entrez \
	errhdr fa2htgs fastacmd findspl fmerge formatdb getfeat getmesh \
	getpub getseq gil2bin idfetch impala indexpub makemat makeset \
	megablast ncbisort netentcf rpsblast seedtop seqtest sequin entrez2 \
	tbl2asn test_regexp testcore testobj testval udv vecscreen Cn3D \
	blast debruijn $WWWBLAST )
	if ( -x ./$i ) then
		rm -f ../bin/$i
		if ( $os == "Darwin" ) then
			/Developer/Tools/Rez Carbon.r -o $i
		endif
		ln ./$i ../bin/
	endif
   end
   echo '*********************************************************'
   echo '*The new binaries are located in ./ncbi/build/ directory*'
   echo '*********************************************************'
   exit 0
endif

BADPLATFORM:
cat << EoF
Your platform is not supported.
To port ncbi toolkit to your platform consult
the files ./ncbi/platform/*.ncbi.mk
The NCBI toolkit FAQ at ftp://ncbi.nlm.nih.gov/toolbox/FAQ.html may be useful.
EoF
exit 0
