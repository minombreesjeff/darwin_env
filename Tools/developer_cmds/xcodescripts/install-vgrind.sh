set -ex

BINDIR="$DSTROOT"/"$DT_TOOLCHAIN_DIR"/usr/bin
SHAREDIR="$DSTROOT"/"$DT_TOOLCHAIN_DIR"/usr/share

install -c -o root -g wheel -m 0755 \
	"$PROJECT_DIR"/vgrind/vgrind.sh \
	"$BINDIR"/vgrind

install -d -o root -g wheel -m 0755 "$SHAREDIR"/groff/site-tmac
install -c -o root -g wheel -m 0644 \
	"$PROJECT_DIR"/vgrind/tmac.vgrind \
	"$SHAREDIR"/groff/site-tmac/tmac.vgrind

install -d -o root -g wheel -m 0755 "$SHAREDIR"/misc
install -c -o root -g wheel -m 0644 \
	"$PROJECT_DIR"/vgrind/vgrindefs.src \
	"$SHAREDIR"/misc/vgrindefs
cap_mkdb -f "$SHAREDIR"/misc/vgrindefs "$PROJECT_DIR"/vgrind/vgrindefs.src

ln -f "$SHAREDIR"/man/man1/vgrind.1 "$SHAREDIR"/man/man1/vfontedpr.1
