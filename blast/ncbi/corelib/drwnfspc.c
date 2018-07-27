#include <ncbilcl.h>
#ifdef OS_UNIX_DARWIN
#  include <AvailabilityMacros.h>
#  ifndef MAC_OS_X_VERSION_10_2
#    define OS_UNIX_DARWIN_OLD
#  endif
#endif
#if defined(OS_MAC)  ||  defined(OS_UNIX_DARWIN_OLD)
#  include "FSpCompat.c"
#endif