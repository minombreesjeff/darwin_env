#! /bin/sh
# $Id: install.sh,v 1.2 2001/12/17 21:16:10 ivanov Exp $
# Author:  Denis Vakatov (vakatov@ncbi.nlm.nih.gov)
#          Vladimir Ivanov (ivanov@ncbi.nlm.nih.gov)
#
# Deploy sources, headers, libraries and executables for the further use
# by the "external" users' projects

# Cmd.-line args  -- source and destination

builddir="$1"
target="$2"

if test -z "$target" ; then
  echo "USAGE:  `basename $0` [build_dir] [install_dir]"
fi

builddir="${builddir:-//u/coremake/ncbi_c}"
target="${target:-//u/coremake/public_c_test}"

echo "[`basename $0`]  NCBI C:   \"$builddir\" to \"$target\"..."


# Derive the destination dirs
incdir="$target/include"
srcdir="$target/altsrc"
dbgdir="$target/dbglib"
libdir="$target/lib"
bindir="$target/bin"
datdir="$target/data"

# Alternate dirs (mirrors)
srcdir_a="$target/src"
dbgdir_a="$target/DebugMT"
libdir_a="$target/ReleaseMT"


# Check
if test ! -d "$builddir" ; then
  echo "[$0] ERROR:  Absent build dir \"$builddir\""
  exit 1
fi

# Reset the public directory
if test -d "$target" ; then
  rm -rf "$target"
fi
mkdir -p "$target"

if test $? -ne 0 ; then
  echo "[$0] ERROR:  Cannot create target dir \"$target\""
  exit 2
fi


# Make dirs without mirrors

mkdir -p "$incdir"
mkdir -p "$incdir/connect"
mkdir -p "$incdir/ctools"
mkdir -p "$srcdir"
mkdir -p "$dbgdir"
mkdir -p "$libdir"
mkdir -p "$bindir"
mkdir -p "$datdir"


# Copy files

bd="$builddir"

cp -p "$bd"/corelib/tsprintf.h            "$incdir"
cp -p "$bd"/corelib/gifgen.h              "$incdir"
cp -p "$bd"/corelib/ncbi*.h               "$incdir"
cp -p "$bd"/corelib/tree*.h               "$incdir"
cp -p "$bd"/corelib/matrix.h              "$incdir"
cp -p "$bd"/corelib/binary.h              "$incdir"
cp -p "$bd"/corelib/*.c                   "$srcdir"
cp -p "$bd"/corelib/core*.h               "$srcdir"
cp -p "$bd"/corelib/regex.h               "$incdir"
cp -p "$bd"/asnlib/*.h                    "$srcdir"
mv "$srcdir"/asn.h                        "$incdir"
cp -p "$bd"/asnlib/*.c                    "$srcdir"
cp -p "$bd"/connect/*.c                   "$srcdir"
cp -p "$bd"/connect/ncbi_priv.h           "$srcdir"
cp -p "$bd"/connect/ncbi_comm.h           "$srcdir"
cp -p "$bd"/connect/ncbi_server_infop.h   "$srcdir"
cp -p "$bd"/connect/ncbi_servicep.h       "$srcdir"
cp -p "$bd"/connect/ncbi_servicep_dispd.h "$srcdir"
cp -p "$bd"/connect/ncbi_servicep_lbsmd.h "$srcdir"
cp -p "$bd"/connect/*.h                   "$incdir"
mv "$incdir"/ncbi_*.h                     "$incdir/connect"
cp -p "$bd"/ctools/*.c                    "$srcdir"
cp -p "$bd"/ctools/*.h                    "$incdir/ctools"
cp -p "$bd"/object/*.c                    "$srcdir"
cp -p "$bd"/object/*.h                    "$incdir"
cp -p "$bd"/access/*.c                    "$srcdir"
cp -p "$bd"/access/*.h                    "$incdir"
cp -p "$bd"/asnstat/*.h                   "$incdir"
cp -p "$bd"/api/*.h                       "$incdir"
cp -p "$bd"/api/*.c                       "$srcdir"
cp -p "$bd"/cdromlib/*.h                  "$incdir"
cp -p "$bd"/cdromlib/*.c                  "$srcdir"
cp -p "$bd"/biostruc/*.h                  "$incdir"
cp -p "$bd"/biostruc/*.c                  "$srcdir"
cp -p "$bd"/biostruc/cdd/*.h              "$incdir"
cp -p "$bd"/biostruc/cdd/*.c              "$srcdir"
cp -p "$bd"/biostruc/cn3d/*.h             "$incdir"
cp -p "$bd"/biostruc/cn3d/*.c             "$srcdir"
cp -p "$bd"/tools/*.h                     "$incdir"
cp -p "$bd"/tools/*.c                     "$srcdir"
cp -p "$bd"/link/mswin/ncbirc.h           "$srcdir"
cp -p "$bd"/vibrant/*.h                   "$incdir"
cp -p "$bd"/vibrant/*.c                   "$srcdir"
cp -p "$bd"/desktop/*.h                   "$incdir"
cp -p "$bd"/desktop/*.c                   "$srcdir"
cp -p "$bd"/gif/*.h                       "$incdir"
cp -p "$bd"/gif/*.c                       "$srcdir"
cp -p "$bd"/cn3d/*.h                      "$incdir"
cp -p "$bd"/cn3d/*.c                      "$srcdir"
cp -p "$bd"/ddv/*.h                       "$incdir"
cp -p "$bd"/ddv/*.c                       "$srcdir"

# Copy network files

cp -p "$bd"/network/entrez/client/*.h     "$incdir"
cp -p "$bd"/network/entrez/client/*.c     "$srcdir"
cp -p "$bd"/network/nsclilib/*.h          "$incdir"
cp -p "$bd"/network/nsclilib/*.c          "$srcdir"
cp -p "$bd"/network/medarch/client/*.h    "$incdir"
cp -p "$bd"/network/medarch/client/*.c    "$srcdir"
cp -p "$bd"/network/taxon1/common/*.h     "$incdir"
cp -p "$bd"/network/taxon1/common/*.c     "$srcdir"
cp -p "$bd"/network/taxon1/taxon2/*.h     "$incdir"
cp -p "$bd"/network/taxon1/taxon2/*.c     "$srcdir"
cp -p "$bd"/network/blast3/client/*.h     "$incdir"
cp -p "$bd"/network/blast3/client/*.c     "$srcdir"
cp -p "$bd"/network/id1arch/*.h           "$incdir"
cp -p "$bd"/network/id1arch/*.c           "$srcdir"
cp -p "$bd"/network/nsdemocl/*.[hc]       "$srcdir"
cp -p "$bd"/demo/entrez.c                 "$srcdir"
cp -p "$bd"/demo/entrezcf.c               "$srcdir"
cp -p "$bd"/demo/netentcf.c               "$srcdir"
cp -p "$bd"/demo/ccpv.c                   "$srcdir"
cp -p "$bd"/demo/ccp.c                    "$srcdir"
cp -p "$bd"/demo/dustv.c                  "$srcdir"
cp -p "$bd"/demo/dst.c                    "$srcdir"
cp -p "$bd"/demo/epiv.c                   "$srcdir"
cp -p "$bd"/demo/epi.c                    "$srcdir"
cp -p "$bd"/demo/sigmev.c                 "$srcdir"
cp -p "$bd"/demo/sigme.c                  "$srcdir"
cp -p "$bd"/demo/searchv.c                "$srcdir"
cp -p "$bd"/demo/srchaa.c                 "$srcdir"
cp -p "$bd"/demo/srchnt.c                 "$srcdir"
cp -p "$bd"/demo/twopv.c                  "$srcdir"
cp -p "$bd"/demo/twop.c                   "$srcdir"
cp -p "$bd"/demo/cnsrtv.c                 "$srcdir"
cp -p "$bd"/demo/cnsrt.c                  "$srcdir"
cp -p "$bd"/demo/cnsgnv.c                 "$srcdir"
cp -p "$bd"/demo/cnsgn.c                  "$srcdir"
cp -p "$bd"/demo/udvmain.c                "$srcdir"
cp -p "$bd"/sequin/*.c                    "$srcdir"
cp -p "$bd"/sequin/*.h                    "$incdir"
cp -p "$bd"/network/vibnet/*.c            "$srcdir"
cp -p "$bd"/network/vibnet/*.h            "$incdir"

# Object files
cp -p "$bd"/make/msvc_prj/corelib/ncbimain/DebugMT/ncbimain.obj "$dbgdir"
cp -p "$bd"/make/msvc_prj/corelib/ncbimain/ReleaseMT/ncbimain.obj "$libdir"
cp -p "$bd"/make/msvc_prj/corelib/ncbi/DebugMT/ncbithr.obj "$dbgdir"
cp -p "$bd"/make/msvc_prj/corelib/ncbi/ReleaseMT/ncbithr.obj "$libdir"

# Debug libs
cp -p `find $buiddir -name '*.lib' | grep DebugMT` "$dbgdir"

# Release libs
cp -p `find $buiddir -name '*.lib' | grep ReleaseMT` "$libdir"

# Executables
cp -p `find $buiddir -name '*.exe' | grep ReleaseMT` "$bindir"

# Data
cp -pr "$builddir/data/" "$target"

# Make mirrors dirs
cp -pr "$srcdir" "$srcdir_a"
cp -pr "$dbgdir" "$dbgdir_a"
cp -pr "$libdir" "$libdir_a"
