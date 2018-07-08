#!/bin/sh
if test "z$*" = zversion ||
   test "z$*" = z--version
then
	echo 'git-gui version 0.12.2'
else

	if [ "x${0:0:1}" = "x/" ] ; then
		myloc=$(dirname "$0")
	else
		myloc=$(dirname "$(pwd)/$0")
	fi
	prefix=${myloc%/usr/*}/usr

	exec "${prefix}/share/git-gui/lib/Git Gui.app/Contents/MacOS/Wish" "$0" "$@"
fi
