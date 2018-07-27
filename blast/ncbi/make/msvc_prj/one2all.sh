#! /bin/sh
#
# $Id: one2all.sh,v 1.40 2003/10/23 16:02:07 ucko Exp $
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
# MSVC project file converter. Expand a signle configuration project file
# to multi-configuration project file.
#
# ===========================================================================

readfirstln() {
  cat $1 | while read p; do
    test -z "$p"  &&  continue
    echo "$p"
    break
  done
}

config() {
  # Argument checking first.
  #
  if [ _$1 = _--without-dizzy ]; then
    dizzy='no'
    shift
  else
    dizzy='yes'
  fi
  if [ _$1 = _ -o \! -f $1 ]; then
    echo "ERROR: File \"$1\" inexistent. Stop."
    return 3
  fi
  if [ _$2 != _Debug -a _$2 != _Release ]; then
    echo "ERROR: Unknown release parameter \"$2\". Stop."
    return 3
  fi
  DEBUG=$2
  if [ _$3 = _ ]; then
    CFG="/ML"
  elif [ $3 = MT ]; then
    CFG="/MT"
  elif [ $3 = DLL ]; then
    CFG="/MD"
  else
    echo "ERROR: Unknown configuration \"$3\". Stop."
    return 3
  fi
  SFX="$3"

  # Our config is ${DEBUG}${SFX}. Compiler switch is ${CFG}.
  #
  if [ $DEBUG = Debug ]; then
    CFG="${CFG}d"
  fi

  # Init the sed script, which will make conversion.
  #
  cat /dev/null >/tmp/$$_cmd.sed

  # First replace config name wherever appropriate.
  #
  for p in Release Debug ; do
    for q in DLL MT ; do
      cat <<-EOF >>/tmp/$$_cmd.sed
	/^# PROP / !s|$p$q|$p|g
	/^# PROP .*"[^ ]*$p$q[^ ]*"/ s|$p$q|$p|g
	EOF
    done
    if [ $p != $DEBUG$SFX ]; then
      cat <<-EOF >>/tmp/$$_cmd.sed
	/^# PROP / !s|$p|$DEBUG$SFX|g
	/^# PROP .*"[^ ]*$p[^ ]*"/ s|$p|$DEBUG$SFX|g
	EOF
    fi
  done

  # Replace debugging macro.
  #
  if [ $DEBUG = Debug ]; then
    echo 's|NDEBUG|_DEBUG|g' >>/tmp/$$_cmd.sed
  else
    echo 's|_DEBUG|NDEBUG|g' >>/tmp/$$_cmd.sed
  fi

  # Does the project use wxWindows (or is a part of wxWindows)?
  #
  if [ `grep -c '^# ADD .*CPP .*__WXDEBUG__' $1` != 0 -o \
       `grep -c '^# ADD .*CPP .*__WXMSW__' $1`   != 0 -o \
       `grep -c '^# ADD .*CPP .*WX.\\{2,3\\}INGDLL' $1` != 0 ]; then
    GUI="wxwin"

    # Flag proper macros for DLL mode.
    #
    if [ _$SFX = _DLL ]; then
      if [ `grep -c '^# ADD .*CPP .*/D *["]\\{0,1\\}WXMAKINGDLL=*[0-9]*["]\\{0,1\\}' $1` != 0 ]; then
        WXDLL='/D "WXMAKINGDLL=1"'
      else
        WXDLL='/D "WXUSINGDLL=1"'
      fi
    else
      WXDLL=""
    fi

  # Is this an FLTK-dependent project?
  #
  elif [ `grep -c '^# ADD .*LINK32 .*fltk[a-z]*[.]lib' $1` != 0 ]; then
     GUI="fltk"
  else
     GUI=""
  fi
 
  # Either replace with hooks, or just remove the compiler switches,
  # which may be configuration-dependent or inconsistent.
  #
  cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*CPP / {
	  s|  */O[0-9A-Za-z]*| @O|
	  s|  */O[0-9A-Za-z]*||g
	  s|  */Gm||g
	  s|  */GZ||g
	  s|  */FR||g
	  s|  */Fr||g
	  s|  */c| @c|
	  s|  */c||g
	  s|  */ZI| @Z|
	  s|  */ZI||g
	  s|  */Zi| @Z|
          s|  */Zi||g
	  s|  */Z7| @Z|
	  s|  */Z7||g
	  s|  */D  *"\{0,1\}DEBUG=*[0-9]*"\{0,1\}| @D|g
	EOF

  # Remove some wxWindows macros, which are surely configuration-dependent.
  #
  if [ "$GUI" = "wxwin" ]; then
    cat <<-EOF >>/tmp/$$_cmd.sed
	  s|  */D  *["]\\{0,1\\}__WXDEBUG__=*[0-9]*["]\\{0,1\\}||g
	  s|  */D  *["]\\{0,1\\}WXUSINGDLL=*[0-9]*["]\\{0,1\\}||g
	  s|  */D  *["]\\{0,1\\}WXMAKINGDLL=*[0-9]*["]\\{0,1\\}||g
	EOF
  fi

  # Clear the linker debug option.
  #
  cat <<-EOF >>/tmp/$$_cmd.sed
	}
	/^# ADD .*LINK32 / s|  */debug||g
	EOF

  # When requested, remove include/library paths, which refer to DIZZY.
  #
  if [ "$dizzy" = "no" ]; then
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*CPP / s|  */I  *["]\\{0,1\\}\\\\\\\\[Dd][Ii][Zz][Zz][Yy]\\\\[^ ]*||g
	/^# ADD .*RSC / s|  */i  *["]\\{0,1\\}\\\\\\\\[Dd][Ii][Zz][Zz][Yy]\\\\[^ ]*||g
	/^# ADD .*LINK32 / s|  */libpath:["]\\{0,1\\}\\\\\\\\[Dd][Ii][Zz][Zz][Yy]\\\\[^ ]*||g
	EOF
  fi

  # Configuration-dependent changes: replace hooks and
  # add more compiler options where appropriate.
  #
  if [ $DEBUG = Debug ]; then
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# PROP / s|  *Use_Debug_Libraries  *0| Use_Debug_Libraries 1|
	/^# ADD .*LINK32 / {
	  s|  */pdb:[^ ]*||g
	  s|/mach|/pdb:none /debug &|
	}
	/^# ADD .*CPP / {
	  s|@O|/Od|
	  s|@Z|/Z7|
	  s| @Z||g
	  s|/W[^ ]*|& /Gm|
	  s|@c|/GZ /c|
	  s|@D|/D "DEBUG=1"|
	  s| @D||g
	EOF
  else
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# PROP / s|  *Use_Debug_Libraries  *1| Use_Debug_Libraries 0|
	/^# ADD .*LINK32 / s|  */pdbtype[^ ]*||g
	/^# ADD .*CPP / {
	  s|@O|/O2|
	  s| @Z||g
	  s|@c|/c|
	  s| @D||g
	EOF
  fi

  # Now replace the code generation switch.
  #
  for p in "/MD" "/ML" "/MT" ; do
    cat <<-EOF >>/tmp/$$_cmd.sed
	  s|  *${p}d| $p|g
	  s| $p| @C|g
	EOF
  done

  # Finish with code generation switch, and then make sure
  # that incremental linking is on except for wxWindows DLLs (slow).
  #
  cat <<-EOF >>/tmp/$$_cmd.sed
	  t cfgrst
	  :cfgrst
	  s|@C|$CFG|
	  t cfgokay
	  s|/nologo|& $CFG|
	  t cfgokay
	  s| CPP|& $CFG|
	  :cfgokay
	  s| @C||g
	}
	/^# ADD .*LINK32 / s|  */incremental[:yesno]*||g
	EOF
  if [ `grep -c '^# ADD .*LINK32 .*/dll' $1` = 0 ]; then
    INCRLNK='yes'
  elif [ -n "$GUI" -a `echo "$WXDLL" | grep -c '^# ADD .*CPP .*WXMAKINGDLL'` != 0 ]; then
    INCRLNK='no'
  else
    INCRLNK=""
  fi
  test "$INCRLNK" = "yes" -a $DEBUG != Debug  &&  INCRLNK='no'
  if [ -n "$INCRLNK" ]; then
    echo "/^# ADD .*LINK32 / s|/nologo|& /incremental:$INCRLNK|" >>/tmp/$$_cmd.sed
  fi

  # wxWindows-specific changes from now on.
  #
  if [ "$GUI" = "wxwin" ]; then
    # Define __WXDEBUG__ for debug configs.
    #
    if [ $DEBUG = Debug ]; then
      WXDEBUG='/D "__WXDEBUG__=1"'
    else
      WXDEBUG=""
    fi

    # ${WX} is to be added to compiler options.
    #
    WX=`echo $WXDEBUG $WXDLL`

    if [ -n "$WX" ]; then
      cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*CPP / {
	  t reset
	  :reset
	  s|/Y|$WX &|
	  t success
	  s|/D|$WX &|
	  t success
	  s|/c|$WX &|
	  t success
	  s| *\$| $WX|
	  :success
	}
	EOF
    fi

    # Enforce /subsystem:windows.
    #
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*LINK32 / s|/subsystem:[A-Za-z]*|@s|g
	/^# ADD LINK32 / {
	  t wxlrst
	  :wxlrst
	  s|@s|/subsystem:windows|
	  t wxlcnt
	  s|/nologo|& /subsystem:windows|
	  t wxlcnt
	  s|\$| /subsystem:windows|
	  :wxlcnt
	}
	/^# ADD .*LINK32 / s| *@s||g
	EOF

    # Take care of libraries: remove all wxWindows ones
    #
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*LINK32 .*wx[dl]\\{0,1\\}[.]lib/ {
	  s/  *jpegd\\{0,1\\}[.]lib//g
	  s/  *pngd\\{0,1\\}[.]lib//g
	  s/  *tiffd\\{0,1\\}[.]lib//g
	  s/  *xpmd\\{0,1\\}[.]lib//g
	  s/  *zlibd\\{0,1\\}[.]lib//g
	  s/  *wx[dl]\\{0,1\\}[.]lib/ @wx/g
	EOF

    # Insert them back but with correct names (which we use).
    # Note that in DLL mode only one (import) library has to be included.
    # The note above was true formely; now images libs are all static.
    #
    #if [ _$SFX != _DLL ]; then
      cat <<-EOF >>/tmp/$$_cmd.sed
	  s/@wx/jpeg.lib png.lib tiff.lib zlib.lib wx.lib/
	EOF
    #else
    #  cat <<-EOF >>/tmp/$$_cmd.sed
    #	  s/@wx/wx.lib/
    #	EOF
    #fi
    cat <<-EOF >>/tmp/$$_cmd.sed
	  s/ @wx//g
	}
	EOF
  fi

  # FLTK specific changes from now on
  #
  if [ "$GUI" = "fltk" ]; then
    # Enforce /subsystem:windows.
    #
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*LINK32 / s|/subsystem:[A-Za-z]*|@s|g
	/^# ADD LINK32 / {
	  t fllrst
	  :fllrst
	  s|@s|/subsystem:windows|
	  t fllcnt
	  s|/nologo|& /subsystem:windows|
	  t fllcnt
	  s|\$| /subsystem:windows|
	  :fllcnt
	}
	/^# ADD .*LINK32 / s| *@s||g
	EOF

    # Take care of libraries: remove all FLTK ones
    #
    cat <<-EOF >>/tmp/$$_cmd.sed
	/^# ADD .*LINK32 * fltk[a-z]*[.]lib/ {
	  s/  *fltk[a-z]*[.]lib/ @fltk/g
	EOF

    # Insert them back but with correct names (which we use).
    # Note that in DLL mode only one (import) library has to be included.
    #
    #if [ _$SFX != _DLL ]; then
    #  cat <<-EOF >>/tmp/$$_cmd.sed
    #	  s/@fltk/fltkforms.lib fltkimages.lib fltkgl.lib fltk.lib/
    #	EOF
    #else
      cat <<-EOF >>/tmp/$$_cmd.sed
	  s/@fltk/fltkdll.lib/
	EOF
    #fi
    cat <<-EOF >>/tmp/$$_cmd.sed
	  s/ @fltk//g
	}
	EOF
  fi

  # Print name of configuration, which was created, and run the script.
  #
  echo "${DEBUG}${SFX}" ${GUI:+"("$GUI")"}

  sed -f /tmp/$$_cmd.sed $1 >/tmp/$$_new
  cp -f /tmp/$$_new $1
  rm -f /tmp/$$_cmd.sed /tmp/$$_new

  return 0
}

configcb()
{
  cat /dev/null >/tmp/$$_cmd.sed

  # First replace config name wherever appropriate.
  #
  for p in Release Debug ; do
    for q in DLL MT ; do
      cat <<-EOF >>/tmp/$$_cmd.sed
	s|$p *$q|$p|g
	EOF
    done
    if [ $p != $2$3 ]; then
      cat <<-EOF >>/tmp/$$_cmd.sed
	s|$p|$2$3|g
	EOF
    fi
  done
  sed -f /tmp/$$_cmd.sed $1
  rm -f /tmp/$$_cmd.sed
}

usage() {
  echo 'Automatic multi config for Microsoft Visual C++ Project File V 6.0'
  echo 'Usage:'
  echo `basename $0` '[--without-dizzy] filename'
  exit 1
}

if [ _$1 = _--without-dizzy ]; then
  without_dizzy="$1"
  shift
else
  without_dizzy=""
fi

if [ _$1 = _ ]; then
  usage
elif [ \! -f $1 ]; then
  echo "ERROR: File \"$1\" must exist. Stop."
  exit 3
fi

if [ _$2 != _ ]; then
  if [ $2 != 4 -a $2 != 3 -a $2 != 2 ]; then
    echo "ERROR: Unknown special parameter \"$2\". Stop."
    exit 3
  fi
  if [ $2 = 2 ]; then
    CFGSET='DLL'
  elif [ $2 = 3 ]; then
    CFGSET='MT'
  else
    CFGSET='DLL ""'
  fi
else
  CFGSET='DLL MT ""'
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
cat <<-EOF >/tmp/$$_.sed
	/^!IF /,/^!ENDIF/ {
	  /^!ENDIF/ q
	  p
	}
	d
	EOF
NCFG=`sed -n -f /tmp/$$_.sed $file | egrep -c '^[!ELS]+IF '`
rm -f /tmp/$$_.sed
if [ $NCFG -gt 1 ]; then
  echo "ERROR: $1 contains more than one configuration ($NCFG). Stop."
  rm -f $file
  exit 2
fi

# Check for per-config dependencies, and warn if any.
#
DEP=`grep -c '^# PROP  *AllowPerConfigDependencies  *1' $file`
cat <<-EOF >/tmp/$$_prop.sed
	/^# Begin Target/,\$ p
	d
	EOF
sed -n -f /tmp/$$_prop.sed $file | grep '^!IF ' >/dev/null 2>&1
if [ $DEP != 0 -a $? = 0 ]; then
  echo "WARNING: $1 contains per-configuration dependencies,"
  echo 'which may or may not be handled correctly by this script.'
fi
rm -f /tmp/$$_prop.sed

# Extract beginning of the file.
#
cat <<-EOF >/tmp/$$_head.sed
	/^!MESSAGE "/ {
	  w /tmp/$$_cmp
	  q
	}
	p
	d
	EOF
sed -n -f /tmp/$$_head.sed $file >/tmp/$$_head
rm -f /tmp/$$_head.sed
config /tmp/$$_head Release `echo $CFGSET | sed -e 's| .*||'` >/dev/null 2>&1
sed -e 's/ "/ %/' -e 's/" /% /' /tmp/$$_cmp >/tmp/$$_comp
mv /tmp/$$_comp /tmp/$$_cmp

# Extract the middle part.
#
cat <<-EOF >/tmp/$$_mid.sed
	/^# Begin Project/,/^!IF / {
	  /^# PROP  *AllowPerConfigDependencies / s/  *1/ 0/
	  /^!IF / q
	  /^# PROP BASE / {
	    i\\

	    q
	  }
	  p
	}
	d
	EOF
sed -n -f /tmp/$$_mid.sed $file >/tmp/$$_mid
rm -f /tmp/$$_mid.sed

# Now extract configuration-dependent part, and make required configs.
#
cat <<-EOF >/tmp/$$_cut.sed
	/^!IF /,/^!E/ {
	  s/^!ELSEIF .*\$/!ENDIF /
	  H
	}
	/^!ENDIF/ {
	  x
	  q
	}
	d
	EOF
sed -n -f /tmp/$$_cut.sed $file >/tmp/$$_cfgtmpl
if [ \! -s /tmp/$$_cfgtmpl ]; then
  cat <<-EOF >/tmp/$$_cut.sed
	/^# PROP BASE /,/^# Begin Target/ {
	  /# Begin Target/ q
	  p
	}
	d
	EOF
  echo "!IF  \"\$(CFG)\" == \"`sed -e 's/^.* %//' -e 's/% .*\$//' /tmp/$$_cmp`\"" >/tmp/$$_cfgtmpl
  echo >>/tmp/$$_cfgtmpl
  sed -n -f /tmp/$$_cut.sed $file >>/tmp/$$_cfgtmpl
  echo >>/tmp/$$_cfgtmpl
  echo '!ENDIF ' >>/tmp/$$_cfgtmpl
fi
rm -f /tmp/$$_cut.sed
cat /dev/null >/tmp/$$_title
cat /dev/null >/tmp/$$_cfg
cat /dev/null >/tmp/$$_message
for i in Release Debug ; do
  for j in $CFGSET ; do
    if [ $j = '""' ]; then
      j=""
    fi
    cp /tmp/$$_cfgtmpl /tmp/$$_cfg_$i$j
    config $without_dizzy /tmp/$$_cfg_$i$j $i $j >>/tmp/$$_message
    if [ $? != 0 ]; then
      tail -1 /tmp/$$_message
      rm -f /tmp/$$_*
      exit 3
    fi
    grep '^!IF ' /tmp/$$_cfg_$i$j >>/tmp/$$_title
    cat /tmp/$$_cfg_$i$j >>/tmp/$$_cfg
    rm -f /tmp/$$_cfg_$i$j
  done
done
echo "${1}:" `cat /tmp/$$_message`
rm -f /tmp/$$_cfgtmpl /tmp/$$_message

# Summarize configurations in the header and tail parts of the project.
#
sed -e 's/^!IF.*CFG)" == //' -e 'y/ "/~%/' /tmp/$$_title >/tmp/$$_tlt
mv /tmp/$$_tlt /tmp/$$_title
cat /dev/null >/tmp/$$_msg
cat /dev/null >/tmp/$$_name
for i in `cat /tmp/$$_title` ; do
  sed -e "s/ %.*% / $i /" -e 'y/~%/ "/' /tmp/$$_cmp >>/tmp/$$_msg
  echo "# Name $i" | tr '~%' ' "' >>/tmp/$$_name
done
sed -e 's/ .*$/ /' /tmp/$$_cmp >>/tmp/$$_msg
echo >>/tmp/$$_msg
rm -f /tmp/$$_title

# Extract the tail part and modify name list on-the-fly.
#
cat <<-EOF >/tmp/$$_tail.sed
	/^# Begin Target/,/^# End Project/ {
	  /^# Name /  {
	    r /tmp/$$_name
	    d
	  }
	  /^!IF /,/^!ENDIF/ d
	  p
	}
	d
	EOF
sed -n -f /tmp/$$_tail.sed $file >/tmp/$$_tail
rm -f /tmp/$$_tail.sed /tmp/$$_name $file

# Glue all parts but the tail.
#
cat /tmp/$$_head /tmp/$$_msg /tmp/$$_mid /tmp/$$_cfg >/tmp/$$_newhead
mv /tmp/$$_newhead /tmp/$$_head
rm -f /tmp/$$_msg /tmp/$$_mid /tmp/$$_cfg

# Replace ENDIF..IF with ELSEIF.
#
cat <<-EOF >/tmp/$$_elseif.sed
	/^!ENDIF/,/^!IF / {
	  s/^!IF /!ELSEIF /
	  /^!ELSEIF / !d
	}
	EOF
sed -f /tmp/$$_elseif.sed /tmp/$$_head >/tmp/$$_newhead
rm -f /tmp/$$_elseif.sed
mv /tmp/$$_newhead /tmp/$$_head
cat <<-EOF >>/tmp/$$_head
	!ENDIF 

	EOF

grep '^SOURCE=' $1 | sed 's/^SOURCE=//;s/[\\]/\\\\/g;s/[.]/[.]/g' >/tmp/$$_flist

for i in `cat /tmp/$$_flist | sed 's/[[]//g;s/]//g'`; do
  file="/tmp/$$_`echo $i | sed 's/[./\\]/_/g'`"
  cat <<-EOF >/tmp/$$_cbext.sed
	/^SOURCE=$i\$/,/^SOURCE=/ {
	  /^SOURCE=$i\$/ d
	  /^[#] End Source File/ !H
	  /^!ENDIF/ {
	    x
	    q
	  }
	  /^[#] End Custom Build/ {
	    x
	    q
	  }
	  d
	}
	d
	EOF
  sed -f /tmp/$$_cbext.sed $1 >$file
done
rm -f /tmp/$$_cbext.sed

cat /dev/null >/tmp/$$_cb.sed
for k in `cat /tmp/$$_flist | sed 's/[[]//g;s/]//g'`; do
  file="/tmp/$$_`echo $k | sed 's/[./\\]/_/g'`"
  test -s $file  ||  continue
  if [ `readfirstln $file | grep -c '^[!]'` = 0 ]; then
    echo "!IF  \"\$(CFG)\" == \"`sed -e 's/^.* %//' -e 's/% .*\$//' /tmp/$$_cmp`\"" >$file.1
    echo           >>$file.1
    cat $file      >>$file.1
    echo           >>$file.1
    echo '!ENDIF ' >>$file.1
    mv $file.1 $file
  fi
  cat /dev/null >$file.new
  for i in Release Debug ; do
    for j in $CFGSET ; do
      if [ $j = '""' ]; then
        j=""
      fi
      configcb $file $i $j >>$file.new
    done
  done
  echo >$file

  # Replace ENDIF..IF with ELSEIF.
  #
  cat <<-EOF >/tmp/$$_elseif.sed
	/^!ENDIF/,/^!IF / {
	  s/^!IF /!ELSEIF /
	  /^!ELSEIF / !d
	}
	EOF
  sed -f /tmp/$$_elseif.sed $file.new >>$file
  rm -f /tmp/$$_elseif.sed $file.new
  cat <<-EOF >>$file
	!ENDIF

	EOF
  cat <<-EOF >>/tmp/$$_cb.sed
	/^SOURCE=$k\$/,/^[#] End Source File/ {
	  /^SOURCE=$k\$/ {
	    p
	    r $file
	    d
	  }
	  /^[#] End Source File/ p
	  d
	}
	EOF
done

sed -f /tmp/$$_cb.sed /tmp/$$_tail >/tmp/$$_tail.1

# Final catenation of results to a single project file.
#
cat /tmp/$$_head /tmp/$$_tail.1 >/tmp/$$_proj
rm -f /tmp/$$_cmp /tmp/$$_head /tmp/$$_tail /tmp/$$_tail.1 /tmp/$$_cb.sed
for i in `cat /tmp/$$_flist | sed 's/[[]//g;s/]//g'`; do
  file="/tmp/$$_`echo $i | sed 's/[./\\]/_/g'`"
  rm -f $file
done
rm -f /tmp/$$_flist

# Remove consequent empty lines (could occur earlier in substitutions)
# and replace original project file (backup kept in .bak).
# Make the output file to be a DOS (CR/LF) text file.
#
cat <<-EOF >/tmp/$$_pretty.sed
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
mv $1 $1.bak

PATH=$PATH:/usr/sbin:/sbin
doser='sed -e s/$//'
if [ `echo $OSTYPE   2>&1 | grep -c -i cygwin` -ge 1 -o  \
     `echo $MACHTYPE 2>&1 | grep -c -i cygwin` -ge 1 -o  \
     `mount          2>&1 | grep -c -i cygwin` -ge 1 ]; then
  echo >$1
  test "`ls -l $1 | tr '\t' ' ' | sed 's/  */ /g' | cut -f5 -d' '`" != "1"  && \
    doser='sed -n p'
fi
sed -n -f /tmp/$$_pretty.sed /tmp/$$_proj | $doser >$1
rm -f /tmp/$$_pretty.sed /tmp/$$_proj
touch -r $1.bak $1

exit 0
