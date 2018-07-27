#! /bin/sh
#    $Id: PubStruct_control.sh,v 6.24 2000/12/29 17:26:29 kimelman Exp $
#  ===========================================================================
# 
#                             PUBLIC DOMAIN NOTICE
#             National Center for Biotechnology Information (NCBI)
# 
#   This software/database is a "United States Government Work" under the
#   terms of the United States Copyright Act.  It was written as part of
#   the author's official duties as a United States Government employee and
#   thus cannot be copyrighted.  This software/database is freely available
#   to the public for use. The National Library of Medicine and the U.S.
#   Government do not place any restriction on its use or reproduction.
#   We would, however, appreciate having the NCBI and the author cited in
#   any work or product based on this material
# 
#   Although all reasonable efforts have been taken to ensure the accuracy
#   and reliability of the software and data, the NLM and the U.S.
#   Government do not and cannot warrant the performance or results that
#   may be obtained by using this software or data. The NLM and the U.S.
#   Government disclaim all warranties, express or implied, including
#   warranties of performance, merchantability or fitness for any particular
#   purpose.
# 
#  ===========================================================================
# 
#  Author:  Michael Kimelman
# 
#  PubStruct maintainance script 
#                    
#  Modifications:  
#  --------------------------------------------------------------------------
#  $Log: PubStruct_control.sh,v $
#  Revision 6.24  2000/12/29 17:26:29  kimelman
#  PDQBACH -> BACH
#
#  Revision 6.23  2000/08/08 20:03:49  kimelman
#  BACH10 -> PDQBACH
#
#  Revision 6.22  1999/05/03 14:46:10  kimelman
#  schema initialization dropped from list : too risky.
#
#  Revision 6.21  1998/11/19 23:52:42  kimelman
#  dust cleaning
#
#  Revision 6.20  1998/11/06 18:59:06  kimelman
#  PubStruct loading transaction granularity changed
#
#  Revision 6.19  1998/10/22 17:03:02  kimelman
#  user args check added
#  downpath is configurable now.
#
# Revision 6.18  1998/10/16  05:42:22  kimelman
# parallel asn loading
#
#  Revision 6.17  1998/09/30 15:58:59  kimelman
#  *** empty log message ***
#
#  Revision 6.16  1998/09/30 15:54:10  kimelman
#  *** empty log message ***
#
#  Revision 6.15  1998/09/30 15:51:18  kimelman
#  environment settings added to stub_loader log.
#  Now it's possible to run it.
#
#  Revision 6.14  1998/09/30 15:37:43  kimelman
#  processing added for stub_loader default log file
#
#  Revision 6.13  1998/08/08 04:53:45  kimelman
#  stub_loader & quotes
#
#  Revision 6.12  1998/08/05 21:12:34  kimelman
#  skip/load bugfix
#
#  Revision 6.11  1998/08/05 17:51:01  kimelman
#  --enforce mode & /tmp downpath
#
#  Revision 6.10  1998/07/16 20:07:06  kimelman
#  mode 'update' added to scan source fiels directory
#  a lot of QA for user 'lists' added using this 'scan' feature
#  option 'enforce' added to avoid too terrifying QA
#
#  Revision 6.9  1998/07/14 20:24:47  kimelman
#  FT schema & smart load
#
#  Revision 6.8  1998/06/17 15:33:02  kimelman
#  added cmp_log removing
#
#  Revision 6.7  1998/06/12 17:43:51  kimelman
#  reload_procs: no total removing
#
#  Revision 6.6  1998/06/05 17:59:54  kimelman
#  help tags changed
#
#  Revision 6.5  1998/05/14 16:11:15  kimelman
#  Compression stuff added in debug mode.
#  few bugs fixed, related to OpenServer/SQL Server switching
#
#  Revision 6.4  1998/05/08 18:12:35  kimelman
#  minibugs & style
#
#  Revision 6.3  1998/05/08 03:00:43  kimelman
#  uniform configuration addaed
#
#  Revision 6.2  1998/04/15 15:58:29  kimelman
#  grammar
#
#  Revision 6.1  1998/04/15 14:53:57  kimelman
#  1. Make retrieval uniform from open server and sql server.
#  2. mmdbsrv retrival performance tuning:
#  	- size of read-in buffers
#  	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
#  	  table
#  3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
#
# 
#  ==========================================================================
# 

progname="$0"
if [ ! -f $progname ] ; then
    progname="`which $progname`"
fi
if [ ! -f $progname ] ; then
    echo "ERROR: can't determine $0 home directory"
    exit 1
fi
progdir=`dirname $progname`
options="$*"

#default PubStruct production DB server
DBserver=BACH 
DBname=PubStruct
DBuser=kimelman
DBpasswrd=kmlmNu
FILEpath=/net/vaster/mmdb/mmdb/data
tempname=/tmp/$$

load_list=empty
download_list=empty
delete_list=empty
enforce=no
verbose=no
stubfile=st_loader.run.$$

RLOADER="run_parallel guess ${progdir}/st_loader"

stub_loader() {
    echo "$RLOADER $* <<EOF"   >>$stubfile
    cat                        >>$stubfile
    echo "EOF"                 >>$stubfile
}


LOADER="${RLOADER}"

usage() {

cat <<EOF

  Usage : $progname [ options ]

options:
    --delete_list <filename> 
                    - delete Structures with mmdb_id listed in <filename>
    --path <path_to_gzipped_files>   (default $FILEpath)
                    - set path to directory contained gzipped asns
    --downpath <path_to_download>   (default $downpath)
                    - set path to downloading directory
    --load_list <filename>    
                    - load to database structures with mmdb_id listed in <filename>
                      special cases for <filename>
                    'all'    - loading every structure found in directory
                    'update' - create a list of files to load using file datestamps and 
                               Database most recent load datestamps

    --download_list <filename>
                    - download structures with mmdb_id listed in <filename>
                      special <filename>='all' leads to dumping all active structures
                      from database ( $DBserver.$DBname )
    --DBserver      - SQL server name          ( default $DBserver )
    --DBname        - Database   name          ( default $DBname   )
    --DBuser        - SQL server user name     ( default $DBuser   )
    --DBpasswd      - SQL server user password ( default $DBpasswrd )
    --SYBASE        - SYBASE environment variable ( default $SYBASE )
    --verbose       - turn on debugging mode
    -v2  <filename> - replace DBloader by stub which output all income list 
                      to <filename> ( default $stubfile )
    --enforce       - enforce loading your list of data (and ignore QA screams)

Examples:

 loading full structure data from /net/vaster/mmdb/mmdb/data:
  agate{kml}pubstruct> $progname --load_list all ;
 
 The same but loading data from saturn :
  agate{kml}pubstruct> $progname --load_list all --path /net/saturn/pubmed2/MMDB
  
 Montly update (/net/vaster/mmdb/mmdb/data has already been updated):
  agate{kml}pubstruct> $progname --delete_list ../deleted --load_list ../updated
 or just 
  agate{kml}pubstruct> $progname --load_list update

EOF
    exit $1
}

while [ $# -gt 0 ]; do
  has_param=no
  if [ $# -ge 2 ] ; then
    case "$2" in 
        "--*")
            ;;
        *)
            has_param=yes
            ;;
    esac
  fi
  case "$1" in
    "--load_list" )
        [ $has_param = yes ] || usage 1
        load_list="$2"
        shift
    ;;
    "--download_list" )
        [ $has_param = yes ] || usage 1
        download_list="$2"
        shift
    ;;
    "--delete_list" )
        [ $has_param = yes ] || usage 1
        delete_list="$2"
        shift
    ;;
    "--path" ) ## path to directory with *.val.gz files
        [ $has_param = yes ] || usage 1
        FILEpath="$2"
        shift
    ;;
    "--downpath" ) ## path to directory with *.val.gz files
        [ $has_param = yes ] || usage 1
        downpath="$2"
        shift
    ;;
    "--DBserver" )
        [ $has_param = yes ] || usage 1
        DBserver="$2"
        shift
    ;;
    "--DBname" )
        [ $has_param = yes ] || usage 1
        DBname="$2"
        shift
    ;;
    "--DBuser" )
        [ $has_param = yes ] || usage 1
        DBuser="$2"
        shift
    ;;
    "--DBpasswd" )
        [ $has_param = yes ] || usage 1
        DBpasswrd="$2"
        shift
    ;;

    "--SYBASE" )
        SYBASE="$2"
        [ $has_param = yes ] || usage 1
        export SYBASE
        shift
    ;;

    ## turn on verbose mode
    "--verbose" | "-v" )
        verbose=yes
    ;;
    "-v2" )
        LOADER=stub_loader
        if [ "x$2" != x ] ; then
            stubfile=$2
            shift
        fi
        echo "#! /bin/sh"                       >$stubfile
        echo "# $stubfile - created at `date`" >>$stubfile
        echo "### HEADERS ###"                 >>$stubfile
        echo "progdir=${progdir} "             >>$stubfile
        echo ". ${progdir}/st_configure.sh"    >>$stubfile
        echo "### EXECUTION PART ###"          >>$stubfile
    ;;
    "--enforce" | "-ef" )
        enforce=yes
    ;;
    * )
        echo "${progname}: Unrecognized argument: $1" 
        usage 1
    ;;
  esac
  shift
done

# QA : check options for correctness
#------------------------------------
if [ $load_list = empty -a $download_list = empty -a  $delete_list = empty ] ; then
  echo "WARNING: No action specified!!!"
  echo "${progname}: ${options}" 
  usage 0
fi

# QA passed
#------------------------------------
if [ $verbose = yes ] ; then
  set -x
fi

. ${progdir}/st_configure.sh

ISQL="$SYBASE_isql -S$DBserver -U$DBuser -P$DBpasswrd"

atexit() {
    for fn in clean clean1 clean2 $tempname
    do
        [ ! -f $fn ] || rm -f $fn
    done
    rm $tempname.* 2>/dev/null
}

get_alive() {
  if [ ! -r $tempname.DB_alive ] ; then
    DB_alive_query="select '__mmdb__', mmdb_id from Struct where state=0 and suppressed=0 order by mmdb_id"
    printf "use $DBname\ngo\n${DB_alive_query}\ngo\n" | $ISQL | grep '__mmdb__' | awk '  { print $2 }' >$tempname.DB_alive
  fi
}

cook_deletelist() { #  $remove_list
  ls $FILEpath | grep gz | sed 's/[.]val[.]gz$//g' | sort -n >$tempname.a

  get_alive
  diff -c $tempname.DB_alive $tempname.a | grep "^- " | awk ' { print $2 }' >$1
  rm $tempname.a
}

difflist() { 
    user_list=$1
    our_list=$2
   
    cat $user_list | sort -n >$tempname.1
    if diff $tempname.1 $our_list >$tempname.2
    then
        cat /dev/null >$3
        cat /dev/null >$4
    else
        cat $tempname.2 | grep "^> " | awk ' { print "mmdb", $2 }' >$3
        cat $tempname.2 | grep "^< " | awk ' { print "mmdb", $2 }' >$4
    fi
    rm $tempname.2 $tempname.1
}

#==================================================================
# Start Actions
#

trap atexit 0 1

if [ $load_list = all ] ; then
  ## prepare list of files to load
  load_list=$tempname
  ls $FILEpath | grep gz | sed 's/[.]val[.]gz$//g' | sort -n >$load_list
elif [ $load_list != empty ] ; then
  ## prepare list of files to load

  # calculate number of days since last load to PubStruct
  DB_last_load_query="select 'data',  min(datediff(day,date,getdate())) from Struct where state=0 and suppressed=0"
  DAYS=`printf "use $DBname\ngo\n${DB_last_load_query}\ngo\n" | $ISQL | grep 'data' | awk '  { print $2 }'`
  
  # create list of all alive entries in pubstruct
  get_alive
  
  ( # create list of entries which exist in file repository but not in DB :
      ( cd $FILEpath ; # create list of all entries on file repository
        find .               -type f -name "*.val.gz" | sed 's/^[.]\///g;s/[.]val[.]gz$//g' | sort -n 
      ) >$tempname.fr_all
      diff $tempname.DB_alive $tempname.fr_all 2>/dev/null | grep "^> " | awk ' { print $2 }' >$tempname.new
      rm $tempname.fr_all
  ) &
  ( # create list files which were changed since last DB update
      ( cd $FILEpath 
        find . -mtime -$DAYS -type f -name "*.val.gz" | sed 's/^[.]\///g;s/[.]val[.]gz$//g' | sort -n 
      ) >$tempname.fresh
  ) &
  wait
  # here we add 'new' entries to 'fresh' list even if they have older update date
  diff  $tempname.fresh $tempname.new 2>/dev/null | grep "^> " | awk ' { print $2 }' >>$tempname.fresh
  rm $tempname.new
  mv $tempname.fresh $tempname
  
  if [ $load_list = update ]; then
    load_list=$tempname
    [ $delete_list != empty ] || delete_list=update
  else
    # let's check user-provided list for correctness
    if [ ! -f $load_list ] ; then
        echo "can not open $load_list"
        exit 1
    fi
    difflist $load_list $tempname $tempname.plus $tempname.minus
    if [ `cat $tempname.plus  $tempname.minus | wc -l` -gt 0 ] ; then
        cat <<EOF
$FILEpath contains the `cat $tempname.plus | grep "mmdb" | wc -l` new mmdbs in addition to your list: 
`cat $tempname.plus | grep "mmdb" | awk ' { print $2 }' | sort -n `

the list of new entries from $FILEpath does NOT contain the 
following mmdb from your list:
`cat $tempname.minus | grep "mmdb" | awk ' { print $2 }' | sort -n `

EOF
        [ $enforce = yes ] || exit 1
    fi
    rm $tempname $tempname.plus  $tempname.minus
  fi
fi
if [ $download_list = all ] ; then
  get_alive
  cat $tempname.DB_alive >$download_list
fi

OPTS="--dbpath=$DBserver:$DBname=$DBuser,$DBpasswrd --path=$FILEpath"
[ $verbose  = no  ] || OPTS="$OPTS --verbose"
[ $enforce  = no  ] || OPTS="$OPTS --enforce"

if [ $load_list != empty ] ; then
  if [ -r $load_list ] ; then
    [ ! -f cmp_log ] || rm -f cmp_log
    ## prepare remove list
    remove_list=$tempname.rm
    cook_deletelist  $remove_list
    ${LOADER} --load              --state=0 $OPTS <$load_list
    if [ $? -eq 0 ] ; then
        ${LOADER} --download --checks --state=0 $OPTS --downpath=/tmp <$load_list
    fi
  else
    echo "input file <$load_list> doesnt exist - loading failed"
  fi
fi
if [ $delete_list != empty ] ; then
  cook_deletelist $tempname.dl
  if [ $delete_list = update ] ; then
    delete_list=$tempname.dl
  else
    # let's check user-provided list for correctness
    if [ ! -f $delete_list ] ; then
        echo "can not open $delete_list"
        exit 1
    fi
    difflist $delete_list $tempname.dl $tempname.plus $tempname.minus
    # list of mmdb which has already deleted from DB.
    if [ x$remove_list != x ]; then
        diff $remove_list $tempname.dl 2>/dev/null | grep "^< " | awk ' { print $2 }' >$tempname.5
    else
        echo "" >$tempname.5
    fi
    diff $tempname.5 $tempname.minus  2>/dev/null | grep "^> " | awk ' { print "attempt to delete already delted entry ", $2 }'
    rm $tempname.5 $tempname.minus
    if [ `cat $tempname.plus | wc -l` -gt 0 ] ; then
        cat <<EOF
The following entries are forgeted to be deleted from PubStruct:
`cat $tempname.plus`
EOF
        [ $enforce = yes ] || exit 1
    fi
    rm $tempname.plus $tempname.dl
  fi
  [ x$remove_list = x ] || rm $remove_list
  ${LOADER} --remove  --state=0 $OPTS <$delete_list
fi

if [ $download_list != empty ] ; then
  if [ -r $download_list ] ; then
    ${LOADER} --download $OPTS --downpath=$downpath <$download_list
  else
    echo "input file <$download_list> doesnt exist - downloading failed"
  fi
fi


if [ -f cmp_log ] ; then
  if [ `cat cmp_log | wc -l ` -ne 0 ] ; then
    echo "FAILED!!!!!!!!"
    mail -s"pdb loading failed" kimelman <<EOF

$0 : $*

`cat cmp_log`
EOF
    rm cmp_log
    exit
  else
    rm cmp_log
  fi
fi
[ ! -d mmdb1 ] || rm -rf mmdb1
[ ! -d mmdb  ] || [ -r $download_list ] || rm -rf mmdb

exit 0

#
# The end
#
