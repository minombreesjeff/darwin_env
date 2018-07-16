#!/bin/sh
set -e

[ -n "$DSTROOT" ] || exit 1
[ -n "$DEVELOPER_DIR" ] || exit 1
[ -n "$RPATHVERS" ] || exit 1

for dylib in "$DSTROOT""$RPATHVERS"/*.dylib; do
	if [ ! -L "$dylib" ]; then
		old_name=`otool -D "$dylib" | sed "1d"`
		new_name=`echo "$old_name" | sed "s|$DEVELOPER_DIR|@rpath|"`

		# fix LC_ID_DYLIB
		install_name_tool -id "$new_name" "$dylib"

		# fix LC_LOAD_DYLIB entries in other binaries
		for file in "$DSTROOT""$RPATHVERS"/../auto/SVN/*/*.bundle; do
			install_name_tool -change "$old_name" "$new_name" "$file"
		done
	fi
done

for bundle in "$DSTROOT""$RPATHVERS"/../auto/SVN/*/*.bundle; do
	# add LC_RPATH entry
	# eight is right out!
	install_name_tool -add_rpath @loader_path/../../../../../../.. "$bundle"
done
