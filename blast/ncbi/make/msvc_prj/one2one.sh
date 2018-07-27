#! /bin/sh
#
# $Id: one2one.sh,v 1.10 2003/08/22 15:33:10 lavr Exp $
# ===========================================================================
# 
#                            PUBLIC DOMAIN NOTICE
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
# 
# ===========================================================================
# 
# Author:  Anton Lavrentiev
# 
# MSVC project file converter. Change configuration in a
# single configuration project file.
#
# ===========================================================================

usage() {
  echo 'Automatic single config for Microsoft Visual C++ Project File V 6.0'
  echo 'Usage:'
  echo `basename $0` '[--without-dizzy] filename config'
  exit 1
}

if [ _$1 = _--without-dizzy ]; then
  without_dizzy="$1"
  shift
else
  without_dizzy=""
fi

if [ _$1 = _ -o _$2 = _ ]; then
  usage
elif [ \! -f $1 ]; then
  echo "ERROR: File \"$1\" must exist. Stop."
  exit 3
fi

dir=`dirname $0`

if [ \! -x ${dir:-.}/all2one.sh ]; then
  echo "ERROR: Script \"${dir:-.}/all2one.sh\" not found. Stop."
  exit 3
fi

if [ \! -x ${dir:-.}/one2all.sh ]; then
  echo "ERROR: Script \"${dir:-.}/one2all.sh\" not found. Stop."
  exit 3
fi

file=/tmp/$$_file
cp -f $1 $file

${dir:-.}/one2all.sh $without_dizzy $file </dev/null >/tmp/$$_list 2>&1
exit=$?
if [ `head -1 /tmp/$$_list | grep -c 'ERROR'` != 0 ]; then
  cat /tmp/$$_list | sed -e "s|$file|$1|g"
  rm -f /tmp/$$_list $file
  exit $exit
fi
rm -f $file.bak

${dir:-.}/all2one.sh $file $2 </dev/null >/tmp/$$_list 2>&1
if [ `tail -1 /tmp/$$_list | grep -c 'ERROR'` != 0 ]; then
  cat /tmp/$$_list | sed -e "s|$file|$1|g"
  rm -f /tmp/$$_list $file
  exit $exit
fi
rm -f $file.bak

echo "${1}:" Configured \
\"\
`tail -1 /tmp/$$_list | sed -e 's|^.* "||' -e 's|" .*\$||'`\
\"
mv -f $1 $1.bak

PATH=$PATH:/usr/sbin:/sbin
doser='sed -e s/$//'
if [ `echo $OSTYPE   2>&1 | grep -c -i cygwin` -ge 1 -o  \
     `echo $MACHTYPE 2>&1 | grep -c -i cygwin` -ge 1 -o  \
     `mount          2>&1 | grep -c -i cygwin` -ge 1 ]; then
  echo >$1
  test "`ls -l $1 | tr '\t' ' ' | sed 's/  */ /g' | cut -f5 -d' '`" != "1"  && \
    doser='sed -n p'
fi
$doser $file >$1
rm -f $file /tmp/$$_list
touch -r $1.bak $1

exit 0
