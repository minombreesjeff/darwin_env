#!/bin/sh
set -e

[ -n "$DSTROOT" ] || exit 1
[ -n "$DEVELOPER_DIR" ] || exit 1

for dylib in "$DSTROOT""$DEVELOPER_DIR"/usr/lib/*.dylib; do
	if [ ! -L "$dylib" ]; then
		old_name=`otool -D "$dylib" | sed "1d"`
		new_name=`echo "$old_name" | sed "s|$DEVELOPER_DIR|@rpath|"`

		# fix LC_ID_DYLIB
		install_name_tool -id "$new_name" "$dylib"

		# fix LC_LOAD_DYLIB entries in other binaries
		for file in "$DSTROOT""$DEVELOPER_DIR"/usr/bin/* "$DSTROOT""$DEVELOPER_DIR"/usr/lib/*.dylib; do
			if [ ! -L "$file" ]; then
				install_name_tool -change "$old_name" "$new_name" "$file"
			fi
		done
	fi
done

for bin in "$DSTROOT""$DEVELOPER_DIR"/usr/bin/*; do
	# add LC_RPATH entry
	install_name_tool -add_rpath @executable_path/../.. "$bin"
done
