#! /bin/sh
# $Id: install.sh,v 1.36 2004/04/14 17:14:08 ivanov Exp $
# Authors:  Denis Vakatov    (vakatov@ncbi.nlm.nih.gov)
#           Vladimir Ivanov  (ivanov@ncbi.nlm.nih.gov)
#           Anton Lavrentiev (lavr@ncbi.nlm.nih.gov)
#
# Deploy sources, headers, libraries and executables for the further use
# by the "external" users' projects

# Cmd.-line args  -- source and destination
script="$0"
builddir="${1:-//u/coremake/ncbi}"
target="${2:-//u/coremake/public/ncbi.last}"

if test -n "$3" ; then
  echo "USAGE:  `basename $script` [build_dir] [install_dir]"
fi


error()
{
  echo "[`basename $script`] ERROR:  $1"
  exit 1
}


makedir()
{
  test -d "$1"  ||  mkdir $2 "$1"  ||  error "Cannot create \"$1\""
}


echo "[`basename $script`] NCBI C:  \"$builddir\" to \"$target\"..."
sleep 2


# Derive the destination dirs
incdir="$target"/include
srcdir="$target"/src
bindir="$target"/bin
datdir="$target"/data
errdir="$target"/errmsg

# Alternate dirs (mirrors)
srcdir_a="$target"/altsrc
dbgdir_a="$target"/dbglib
libdir_a="$target"/lib
datdir_a="$target"/../../c/data
errdir_a="$target"/../../c/errmsg


# Check
test -d "$builddir"  ||  error "Absent build dir \"$builddir\""


# Reset the public directory
test -d "$target"  &&  find "$target" -type f -exec rm -f {} \; >/dev/null 2>&1
test -d "$target"  ||  mkdir -p "$target"
test -d "$target"  ||  error "Cannot create target dir \"$target\""


# Make all dirs
makedir "$incdir" -p
makedir "$incdir"/connect
makedir "$incdir"/ctools
makedir "$srcdir" -p
makedir "$srcdir"/access
makedir "$srcdir"/api
makedir "$srcdir"/asnlib
makedir "$srcdir"/biostruc
makedir "$srcdir"/biostruc/cdd
makedir "$srcdir"/biostruc/cn3d
makedir "$srcdir"/cdromlib
makedir "$srcdir"/cn3d
makedir "$srcdir"/corelib
makedir "$srcdir"/connect
makedir "$srcdir"/connect/test
makedir "$srcdir"/ctools
makedir "$srcdir"/ddv
makedir "$srcdir"/demo
makedir "$srcdir"/desktop
makedir "$srcdir"/gif
makedir "$srcdir"/network/blast3/client  -p
makedir "$srcdir"/network/entrez/client  -p
makedir "$srcdir"/network/id1arch        -p
makedir "$srcdir"/network/medarch/client -p
makedir "$srcdir"/network/nsclilib       -p
makedir "$srcdir"/network/nsdemocl       -p
makedir "$srcdir"/network/spell/client   -p
makedir "$srcdir"/network/taxon1/common  -p
makedir "$srcdir"/network/taxon1/taxon2  -p
makedir "$srcdir"/network/vibnet         -p
makedir "$srcdir"/object
makedir "$srcdir"/sequin
makedir "$srcdir"/tools
makedir "$srcdir"/vibrant
makedir "$bindir"   -p
makedir "$datdir"   -p
makedir "$errdir"   -p
makedir "$srcdir_a" -p
makedir "$dbgdir_a" -p
makedir "$libdir_a" -p
makedir "$datdir_a" -p
makedir "$errdir_a" -p


# Copy files

cp -p "$builddir"/corelib/tsprintf.h            "$incdir"
cp -p "$builddir"/corelib/gifgen.h              "$incdir"
cp -p "$builddir"/corelib/ncbi*.h               "$incdir"
cp -p "$builddir"/corelib/tree*.h               "$incdir"
cp -p "$builddir"/corelib/matrix.h              "$incdir"
cp -p "$builddir"/corelib/binary.h              "$incdir"
cp -p "$builddir"/corelib/*.c                   "$srcdir"/corelib
cp -p "$builddir"/corelib/core*.h               "$srcdir"/corelib
cp -p "$builddir"/corelib/regex.h               "$incdir"
cp -p "$builddir"/asnlib/*.h                    "$srcdir"/asnlib
mv "$srcdir"/asnlib/asn.h                 "$incdir"
cp -p "$builddir"/asnlib/*.c                    "$srcdir"/asnlib
cp -p "$builddir"/connect/*.c                   "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_priv.h           "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_comm.h           "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_host_infop.h     "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_server_infop.h   "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_servicep.h       "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_dispd.h          "$srcdir"/connect
cp -p "$builddir"/connect/ncbi_lbsmd.h          "$srcdir"/connect
cp -p "$builddir"/connect/*.h                   "$incdir"
mv "$incdir"/ncbi_*.h                           "$incdir"/connect
mv "$incdir"/connect_export.h                   "$incdir"/connect
cp -p "$builddir"/connect/test/*.[ch]           "$srcdir"/connect/test
cp -p "$builddir"/ctools/*.c                    "$srcdir"/ctools
cp -p "$builddir"/ctools/*.h                    "$incdir"/ctools
cp -p "$builddir"/object/*.c                    "$srcdir"/object
cp -p "$builddir"/object/*.h                    "$incdir"
cp -p "$builddir"/access/*.c                    "$srcdir"/access
cp -p "$builddir"/access/*.h                    "$incdir"
cp -p "$builddir"/asnstat/*.h                   "$incdir"
cp -p "$builddir"/api/*.c                       "$srcdir"/api
cp -p "$builddir"/api/*.h                       "$incdir"
cp -p "$builddir"/cdromlib/*.c                  "$srcdir"/cdromlib
cp -p "$builddir"/cdromlib/*.h                  "$incdir"
cp -p "$builddir"/biostruc/*.c                  "$srcdir"/biostruc
cp -p "$builddir"/biostruc/*.h                  "$incdir"
cp -p "$builddir"/biostruc/cdd/*.c              "$srcdir"/biostruc/cdd
cp -p "$builddir"/biostruc/cdd/*.h              "$incdir"
cp -p "$builddir"/biostruc/cn3d/*.c             "$srcdir"/biostruc/cn3d
cp -p "$builddir"/biostruc/cn3d/*.h             "$incdir"
cp -p "$builddir"/tools/*.c                     "$srcdir"/tools
cp -p "$builddir"/tools/*.h                     "$incdir"
cp -p "$builddir"/link/mswin/ncbirc.h           "$srcdir"
cp -p "$builddir"/vibrant/*.c                   "$srcdir"/vibrant
cp -p "$builddir"/vibrant/*.h                   "$incdir"
cp -p "$builddir"/desktop/*.c                   "$srcdir"/desktop
cp -p "$builddir"/desktop/*.h                   "$incdir"
cp -p "$builddir"/gif/*.c                       "$srcdir"/gif
cp -p "$builddir"/gif/*.h                       "$incdir"
cp -p "$builddir"/cn3d/*.c                      "$srcdir"/cn3d
cp -p "$builddir"/cn3d/*.h                      "$incdir"
cp -p "$builddir"/ddv/*.c                       "$srcdir"/ddv
cp -p "$builddir"/ddv/*.h                       "$incdir"


# Copy network files

cp -p "$builddir"/network/entrez/client/*.c     "$srcdir"/network/entrez/client
cp -p "$builddir"/network/entrez/client/*.h     "$incdir"
cp -p "$builddir"/network/nsclilib/*.c          "$srcdir"/network/nsclilib
cp -p "$builddir"/network/nsclilib/*.h          "$incdir"
cp -p "$builddir"/network/medarch/client/*.c    "$srcdir"/network/medarch/client
cp -p "$builddir"/network/medarch/client/*.h    "$incdir"
cp -p "$builddir"/network/taxon1/common/*.c     "$srcdir"/network/taxon1/common
cp -p "$builddir"/network/taxon1/common/*.h     "$incdir"
cp -p "$builddir"/network/taxon1/taxon2/*.c     "$srcdir"/network/taxon1/taxon2
cp -p "$builddir"/network/taxon1/taxon2/*.h     "$incdir"
cp -p "$builddir"/network/blast3/client/*.c     "$srcdir"/network/blast3/client
cp -p "$builddir"/network/blast3/client/*.h     "$incdir"
cp -p "$builddir"/network/id1arch/*.c           "$srcdir"/network/id1arch
cp -p "$builddir"/network/id1arch/*.h           "$incdir"
cp -p "$builddir"/network/nsdemocl/*.[hc]       "$srcdir"/network/nsdemocl
cp -p "$builddir"/network/spell/client/*.c      "$srcdir"/network/spell/client
cp -p "$builddir"/network/spell/client/*.h      "$incdir"
cp -p "$builddir"/demo/*.c                      "$srcdir"/demo
cp -p "$builddir"/sequin/*.c                    "$srcdir"/sequin
cp -p "$builddir"/sequin/*.h                    "$incdir"
cp -p "$builddir"/network/vibnet/*.c            "$srcdir"/network/vibnet
cp -p "$builddir"/network/vibnet/*.h            "$incdir"


# Copy generated files

for i in 'Debug' 'Release' ; do
  for j in '' 'DLL' ; do
    if test -d "$builddir"/make/msvc_prj/corelib/ncbi/$i$j ; then

      # Object files
      makedir "$target/$i$j" -p
      cp -p "$builddir"/make/msvc_prj/corelib/ncbimain/$i$j/ncbimain.obj "$target/$i$j"
      cp -p "$builddir"/make/msvc_prj/corelib/ncbi/$i$j/ncbithr.obj      "$target/$i$j"

      # Libraries
      cp -p `find "$builddir" -name '*.lib' | grep "$i$j/"` "$target/$i$j"

    fi
  done
done

# Executables (one of ReleaseDLL, Release)
for i in 'DLL' '' ; do
  if test -d "$builddir"/make/msvc_prj/corelib/ncbi/Release$i ; then
    cp -p `find "$builddir" -name '*.exe' | grep "Release$i/"` "$bindir"
    break
  fi
done

# Data
cp -pr "$builddir"/data/* "$datdir"
cp -pr "$builddir"/data/* "$datdir_a"
cp -pr "$builddir"/errmsg/* "$errdir"
cp -pr "$builddir"/errmsg/* "$errdir_a"


# Fill alt source tree
cd "$srcdir"
x_dirs=`find . -type d -print`
for ddd in $x_dirs ; do
  cd "$srcdir/$ddd"
  cp -p *.[ch] "$srcdir_a"
done


# Fill mirror dirs
if test -d "$target"/DebugDLL ; then
  cp -p "$target"/DebugDLL/*.* "$dbgdir_a"
fi
if test -d "$target"/ReleaseDLL ; then
  cp -p "$target"/ReleaseDLL/*.* "$libdir_a"
fi


# CVS checkout info file
cp -p "$builddir"/cvs_info "$target"


exit 0
