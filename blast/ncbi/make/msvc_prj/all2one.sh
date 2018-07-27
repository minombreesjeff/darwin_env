#! /bin/sh
#
# $Id: all2one.sh,v 1.19 2002/12/31 16:21:19 lavr Exp $
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
# MSVC project file converter. Make single-configuration project file from a
# multi-configuration project file.
#
# ===========================================================================

usage() {
  echo 'Config extractor for Microsoft Visual C++ Project File V 6.0'
  echo 'Usage:'
  echo `basename $0` 'filename [config]'
  exit 1
}

if [ _$1 = _ ]; then
  usage
elif [ \! -f $1 ]; then
  echo "ERROR: File \"$1\" must exist. Stop."
  exit 3
fi

if [ _$2 = _SecretWord ]; then
  CVSUSE=1
  CFG=""
else
  CVSUSE=0
  CFG=${2:-""}
fi

file=/tmp/$$_file

# DOS-2-UNIX conversion (shouldn't be here, but just in case for sanity).
#
sed -e 's/*$//' $1 >$file

# Check signature.
#
cat <<-"EOF" >/tmp/$$_genusign
	# Microsoft Developer Studio Project File
	# Microsoft Developer Studio Generated Build File, Format Version 6.00
	# ** DO NOT EDIT **

	# TARGTYPE
	EOF
cat /dev/null >/tmp/$$_filesign
head -1 $file | sed -e 's/ -.*//' >>/tmp/$$_filesign
head -3 $file | tail +2 >>/tmp/$$_filesign
head -5 $file | tail +4 | sed -e 's/ *".*$//' >>/tmp/$$_filesign
diff /tmp/$$_filesign /tmp/$$_genusign >/dev/null 2>&1
exit=$?
rm -f /tmp/$$_filesign /tmp/$$_genusign
if [ $exit != 0 ]; then
  echo "ERROR: $1 doesn't look like MSVC++ Project File. Stop."
  rm -f $file
  exit 2
fi

# Extract configuration part of the project file and count configurations.
#
cat <<EOF >/tmp/$$_.sed
/^!IF /,/^!ENDIF/ {
  /^!ENDIF/ q
  p
}
d
EOF
NCFG=`sed -n -f /tmp/$$_.sed $file | egrep -c '^[!ELS]+IF'`
rm -f /tmp/$$_.sed
if [ $CVSUSE = 0 ]; then
  echo "${1}: $NCFG configuration(s) found"
elif [ `grep '^# Name "' $file | sed -e 's/^# Name //g' | grep -c DebugDLL` = 0 ]; then
  echo "ERROR: ${1}: Only DebugDLL configuration can be checked in to CVS. Stop."
  rm -f $file
  exit 2
fi

# Check number of configurations, in case of 1 and CVS - check standard paths.
#
if [ $NCFG -le 1 ]; then

  # If not in CVS - just leave
  #
  if [ $CVSUSE = 0 ]; then
    rm -f $file
    exit 0
  fi

  echo "${1}: Analyzing include and library path(s)..."

  # Standard relative path (from argument 3 if in CVS).
  #
  pr='./../../..'
  if [ -n "$3" ]; then
    for k in `echo $3 | sed -e 's|/| |g'` ; do
      if [ "$k" != "." ]; then
        if [ "$pr" != "." ]; then
          if [ "$k" = ".." ]; then
            pr=`echo "$pr" | sed -e 's|/..$||'`
          else
            pr="${pr}/.."
          fi
        fi
      fi
    done
  fi

  # Start preparing message file and sed script.
  #
  echo "${1}: The following path(s) modified:" >/tmp/$$_msg
  cat /dev/null >/tmp/$$_path.sed

  # Some helper functions.
  # Make case-insensitive text pattern from given string.
  #
  expandcs() {
    temp=""
    for q in `echo $1 | sed -e 's|.|& |g'` ; do
      if [ `echo $q | grep -c '[A-Za-z]'` != 0 ]; then
        temp="${temp}[`echo $q | tr '[a-z]' '[A-Z]'``echo $q | tr '[A-Z]' '[a-z]'`]"
      else
        temp="${temp}$q"
      fi
    done
    echo "$temp"
  }

  # In the given string replace slashes with backslashes.
  #
  backslash() {
    echo $1 | sed -e 's|/|\\\\|g'
  }

  # Make slash-insensitive (back or forward) pattern from given string. 
  #
  slashify() {
    echo $1 | sed -e 's|/|[/\\\\]|g'
  }

  # The loop for all packages we have to care about.
  #
  for k in "wxwin" "ncbi" "fltk" ; do

    # Absolute path prefix (DIZZY).
    #
    pa='//DIZZY/public/'$k
 
    # Form all paths and convert them to ugly DOS format
    # (backslashed and yet case-insensitive). Dragons here, careful!
    # Don't forget to double all backslashes, as they will appear in shell
    # substitutions and sed script (and thus must be escaped).
    #
    inca="${pa}/include"
    incr="${pr}/${k}/include"
    liba="${pa}/DebugDLL"
    libr="${pr}/${k}/DebugDLL"

    incas=`expandcs "$inca"`
    tmp=`echo "$incr" | sed -e 's|^[/.]*||'`
    incrs=`expandcs "$tmp"`
    libas=`expandcs "$liba"`
    tmp=`echo "$libr" | sed -e 's|^[/.]*||'`
    librs=`expandcs "$tmp"`

    inca=`backslash "$inca"`
    incr=`backslash "$incr"`
    liba=`backslash "$liba"`
    libr=`backslash "$libr"`
    incas=`slashify "$incas"`
    incrs=`slashify "$incrs"`
    libas=`slashify "$libas"`
    librs=`slashify "$librs"`

    # Continue to prepare message file (used afterwards).
    #
    echo /tmp/$$_stampi${k} Include: "$incr" and "$inca" >>/tmp/$$_msg
    echo /tmp/$$_stampr${k} Resource: "$incr" and "$inca" >>/tmp/$$_msg
    echo /tmp/$$_stampl${k} Library: "$libr" and "$liba" >>/tmp/$$_msg
    #for q in $inca $incas $incr $incrs $liba $libas $libr $librs ; do
    #  echo $q
    #done

    # Continue sed command script, which incorporates include and lib paths
    # (if and only if resembling paths were in the project file), and,
    # upon successful substitutions, leaves non-empty stamp files in /tmp.
    #
    cat <<-EOF >>/tmp/$$_path.sed
	/^# ADD .*CPP / {
	  s|/I  *[^ ]*${incrs}[^ ]*|@I|g
	  s|/I  *[^ ]*${incas}[^ ]*|@I|g
	  s|@I|/I "${incr}" /I "${inca}"|w /tmp/$$_stampi$k
	  s| *@I||g
	}
	/^# ADD .*RSC / {
	  s|/i  *[^ ]*${incrs}[^ ]*|@i|g
	  s|/i  *[^ ]*${incas}[^ ]*|@i|g
	  s|@i|/i "${incr}" /i "${inca}"|w /tmp/$$_stampr$k
	  s| *@i||g
	}
	/^# ADD .*LINK32 / {
	  s|/libpath:[^ ]*${librs}[^ ]*|@L|g
	  s|/libpath:[^ ]*${libas}[^ ]*|@L|g
	  s|@L|/libpath:"${libr}" /libpath:"${liba}"|w /tmp/$$_stampl$k
	  s| *@L||g
	}
	EOF
  done
  #cat /tmp/$$_path.sed

  # Ok, time to run the sed script and make the dirty hacker's job...
  #
  sed -f /tmp/$$_path.sed $file >$file.new

  # If the output file was changed, see what stamps are non-empty,
  # then print the corresponding message(s) from message file.
  #
  diff $file $file.new >/dev/null 2>&1
  if [ $? != 0 ]; then
    head -1 /tmp/$$_msg
    for q in "i" "r" "l" ; do
      for k in /tmp/$$_stamp${q}* ; do
        if [ -s $k ]; then
          grep "$k" /tmp/$$_msg | sed -e "s|^$k ||"
        fi
      done
    done

    # Prepare backup copy. As the resulting file has to be checked in,
    # it has to be in the UNIX text format. No conversion necessary.
    #
    mv $1 $1.bak
    cp $file.new $1
    touch -r $1.bak $1
  fi

  # Clean up and leave.
  #
  rm -f $file $file.new /tmp/$$_path.sed /tmp/$$_msg /tmp/$$_stamp*
  exit 0
fi

# More than 1 configuration at this point.
#
if [ $CVSUSE = 1 ]; then
  echo "ERROR: $1 contains $NCFG configurations, cannot be checked in. Stop."
  rm -f $file
  exit 2
fi

# Extract configuration names to check the project consistency.
#
grep '^!MESSAGE "' $file | sed -e 's/^!MESSAGE //' -e 's/  *(.*).*$//' >/tmp/$$_mnames
grep '^# Name "' $file | sed -e 's/^# Name //g' >/tmp/$$_nnames

# Check the consistency.
#
diff /tmp/$$_mnames /tmp/$$_nnames >/dev/null 2>&1
if [ $? != 0 -o `cat /tmp/$$_nnames | wc -l` != $NCFG ]; then
  ERROR=1
else
  ERROR=0
fi
rm -f /tmp/$$_mnames /tmp/$$_nnames
if [ $ERROR = 1 ]; then
  echo "ERROR: Project file inconsistency found in $1. Stop."
  rm -f $file
  exit 2
fi

# Check for per-config issues.
#
DEP=`grep -c '^# PROP  *AllowPerConfigDependencies  *1' $file`
cat <<EOF >/tmp/$$_dep.sed
/^# Begin Target/,\$ p
d
EOF
sed -n -f /tmp/$$_dep.sed $file | grep '^!IF' >/dev/null 2>&1
if [ $DEP = 0 -o $? != 0 ]; then
  DEP=0
fi
rm -f /tmp/$$_dep.sed $file

# Query the user to choose a configuration.
# Keep /tmp clean as the user may choose to cancel (Ctrl-C).
#
while true; do
  if [ $DEP != 0 ]; then
    echo "WARNING: $1 contains per-configuration dependencies,"
    echo 'which may or may not be handled correctly by this script.'
  fi
  sed -e 's/*$//' $1 | grep '^# Name "' | sed -e 's/^# Name //g' >/tmp/$$_cfg
  cat /dev/null >/tmp/$$_cfglist
  i=1
  for j in `sed -e 'y/ "/~%/' /tmp/$$_cfg` ; do
    echo ${i}: `echo $j | sed -e 'y/~/ /' -e 's/%//g'` >>/tmp/$$_cfglist
    i=`expr "$i" + 1`
  done
  rm -f /tmp/$$_cfg
  cat /tmp/$$_cfglist
  if [ _$CFG = _ ]; then
    rm -f /tmp/$$_cfglist
    echo 'Please enter required configuration number'
    read i
    if [ ${i:=0} -ge 1 -a $i -le $NCFG ]; then
      break
    fi
    echo "Invalid configuration number \"$i\""
    echo "${1}: $NCFG configuration(s) found"
  else
    i=`grep "\\\\<$CFG\\\\>\\\$" /tmp/$$_cfglist | sed -e 's/:.*//'`
    rm -f /tmp/$$_cfglist
    if [ _$i = _ ]; then
      echo "ERROR: Configuration \"$CFG\" not found in $1. Stop."
      exit 2
    fi
    break
  fi
done

# DOS-2-UNIX conversion once more and final.
#
sed -e 's/*$//' $1 >$file

# Find the chosen configuration.
#
CFG=`grep '^# Name "' $file | sed -e 's/^# Name //' | head -$i | tail -1`
echo "Configuration ${i}: $CFG will be extracted"

# Start extracting beginning of the file, changing configuration name.
#
cat <<EOF >/tmp/$$_head.sed
/^!MESSAGE "/q
/CFG/ s/=.*\$/=$CFG/
/^CFG=/ s/"//g
p
d
EOF
sed -n -f /tmp/$$_head.sed $file >/tmp/$$_head
rm -f /tmp/$$_head.sed
grep "$CFG" $file | grep -v 'CFG' | head -1 >>/tmp/$$_head
echo '!MESSAGE ' >>/tmp/$$_head
echo >>/tmp/$$_head

# Now extract middle part (just before configurations begin).
#
cat <<EOF >/tmp/$$_mid.sed
/^# Begin Project/,/^!IF/ {
  /^# PROP  *AllowPerConfigDependencies / s/  *1/ 0/
  /^!IF / q
  p
}
d
EOF
sed -n -f /tmp/$$_mid.sed $file >>/tmp/$$_head
rm -f /tmp/$$_mid.sed

# Extract all configurations, and select the required among them.
#
cat <<EOF >/tmp/$$_cfg.sed
/^!IF /,/^!ENDIF/ {
  p
  /^!ENDIF/ q
}
d
EOF
sed -n -f /tmp/$$_cfg.sed $file >/tmp/$$_cfg
cat <<EOF >/tmp/$$_cfg.sed
/^!.*CFG.*== *$CFG\$/,/^!E/ p
d
EOF
sed -n -f /tmp/$$_cfg.sed /tmp/$$_cfg >/tmp/$$_cfg1
rm -f /tmp/$$_cfg.sed

# Enclose extracted config in proper IF...ENDIF.
#
head -1 /tmp/$$_cfg1 | sed -e 's/^!ELSEIF /!IF /' >/tmp/$$_cfg
tail +2 /tmp/$$_cfg1 | sed -e 's/^!ELSEIF .*/!ENDIF /' >>/tmp/$$_cfg
rm -f /tmp/$$_cfg1

# Concatenate extracted config with what has been extracted earlier.
#
cat /tmp/$$_head /tmp/$$_cfg >/tmp/$$_out
rm -f /tmp/$$_head /tmp/$$_cfg
echo >>/tmp/$$_out

# Finalize the project file, adding Target and Source sections.
#
cat <<EOF >/tmp/$$_tail.sed
/^# Begin Target/,/^# End Project/ {
  /^!IF /,/^!ENDIF/ {
    /^!.*CFG.*== *$CFG\$/,/^!E/ {
      s/^!ELSEIF \(.*CFG.*== *$CFG\)\$/!IF \1/
      s/^!ELSEIF .*/!ENDIF/
      p
    }
    d
  }
  /^# Name / {
    /^# Name $CFG/ !d
  }
  p
}
d
EOF
sed -n -f /tmp/$$_tail.sed $file >>/tmp/$$_out
rm -f /tmp/$$_tail.sed

# Remove consequent empty lines (could occur earlier in substitutions)
# and replace original project file (backup kept in .bak).
# Output is always in native (UNIX) text file format.
#
mv $1 $1.bak
cat <<EOF >/tmp/$$_pretty.sed
/./ {
  p
  d
}
/^\$/ p
:Empty
/^\$/ {
  N
  s/.//
  b Empty
}
p
EOF
sed -n -f /tmp/$$_pretty.sed /tmp/$$_out >$1
rm -f /tmp/$$_pretty.sed /tmp/$$_out $file
touch -r $1.bak $1

exit 0
