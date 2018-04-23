--- lib/autoconf/status.m4.orig	2006-09-12 21:48:23.000000000 -0700
+++ lib/autoconf/status.m4	2010-06-22 11:39:19.000000000 -0700
@@ -696,7 +696,7 @@
 [  cat >>$CONFIG_STATUS <<_ACEOF
     # First, check the format of the line:
     cat >"\$tmp/defines.sed" <<\\CEOF
-/^[	 ]*#[	 ]*undef[	 ][	 ]*$ac_word_re[	 ]*\$/b def
+/^[	 ]*#[	 ]*undef[	 ][	 ]*$ac_word_re[	 ]*/b def
 /^[	 ]*#[	 ]*define[	 ][	 ]*$ac_word_re[(	 ]/b def
 b
 :def
