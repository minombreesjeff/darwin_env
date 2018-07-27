#! /bin/sh
#
# $Id: one2all_r.sh,v 1.1 2003/08/05 18:23:23 lavr Exp $
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
# MSVC project file recursive converter that converts single-config files
# into multiple config files on the entire subtree.
#
# ===========================================================================

usage() {
  echo 'Automatic single config for Microsoft Visual C++ Project File V 6.0'
  echo 'Usage:'
  echo `basename $0` '[--without-dizzy]'
  exit 1
}

if [ _$1 = _--without-dizzy ]; then
  without_dizzy="$1"
  shift
else
  without_dizzy=""
fi

if [ "$1" = "--help" -o "$1" = "-h" -o "$1" = "-?" ]; then
  usage
fi

dir=`dirname $0`

if [ \! -x ${dir:-.}/one2all.sh ]; then
  echo "ERROR: Script \"${dir:-.}/one2all.sh\" not found. Stop."
  exit 3
fi

find . -name "*.dsp" -exec ${dir:-.}/one2all.sh $without_dizzy {} \;
