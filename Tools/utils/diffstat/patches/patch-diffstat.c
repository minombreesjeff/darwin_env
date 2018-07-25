--- diffstat.c.orig	2005-05-24 09:25:12.000000000 -0700
+++ diffstat.c	2005-05-24 09:26:26.000000000 -0700
@@ -118,7 +118,7 @@
 #ifdef HAVE_MALLOC_H
 #include <malloc.h>
 #else
-extern char *malloc();
+extern void *malloc(size_t);
 #endif
 
 #ifdef HAVE_GETOPT_H
