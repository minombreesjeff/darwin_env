#!/bin/sh
# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#            National Center for Biotechnology Information (NCBI)
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government do not place any restriction on its use or reproduction.
#  We would, however, appreciate having the NCBI and the author cited in
#  any work or product based on this material
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
# ===========================================================================
#
# File Name:  setrsrc.sh
#
# Author:  Robert Smith
#
# Version Creation Date:   4/3/2003
#
# $Revision: 1.1 $
#
# File Description:
#
#   For the NCBI C and C++ toolkits, reset the Macintosh file types properly.
#
#   Recursively looks for files in the C and C++ toolkits with certain extensions
#   to reset their file and creator types.
#   For certain extensions only looks in certain folders.
#
#   BUG: Will not work with files with spaces, quotes or backslashes in their names.
#
# Modifications log:
#
#   $Log: setrsrc.sh,v $
#   Revision 1.1  2003/04/03 20:45:30  rsmith
#   Script to set macintosh file & creation types. Duplicates setrsrc.c functionality.
#
#

PATH=$PATH:/Developer/Tools
export PATH

output=
#output=-t

# set the types of the source files.
find ~/ncbi \( -name '*.h' -or -name '*.hpp' -or -name '*.c' -or -name '*.cpp' -o -name '*.r' \) | \
    xargs $output SetFile -c 'ttxt' -t 'TEXT' 
find ~/ncbi_cxx \( -name '*.h' -or -name '*.hpp' -or -name '*.c' -or -name '*.cpp' -o -name '*.r' \) | \
    xargs $output SetFile -c 'ttxt' -t 'TEXT' 

# Metrowerks project files.
find ~/ncbi  -path '*/link/macmet/*.mcp'  | \
    xargs $output SetFile -c 'CWIE' -t 'MMPr' 
find ~/ncbi_cxx  -path '*/compilers/mac_prj/*.mcp'  | \
    xargs $output SetFile -c 'CWIE' -t 'MMPr' 

# Our Applescripts that build Metrowerks projects.
find ~/ncbi  -path '*/make/*.met'  | \
    xargs $output SetFile -c 'ToyS' -t 'TEXT' 
find ~/ncbi_cxx -path '*/compilers/mac_prj/*.met'  | \
    xargs $output SetFile -c 'ToyS' -t 'TEXT' 

# Metrowerks property list compiler files.
find ~/ncbi  -path '*/link/macmet/*.plc'  | \
    xargs $output SetFile -c 'ttxt' -t 'TEXT' 
find ~/ncbi_cxx  -path '*/compilers/mac_prj/*.plc' | \
    xargs $output SetFile -c 'ttxt' -t 'TEXT' 

