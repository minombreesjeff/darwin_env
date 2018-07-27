/*
 * Argument type passed to the wait() call. If you don't know what it is, then
 * leave it blank. The macro will place a pointer to the type for a cast...
 * This will either be "union wait" or "int".
 */

#define WAIT_ARG_TYPE int

/*
 * Argument type passed to the signal() call (second parameter). If you don't
 * know what it is then don't define it.... this is placed in as a cast as-is.
 */

#define SIGNAL_ARG_TYPE void(*)(int)


/* any additional missing headers */

#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

/* any extra externs */

extern int h_errno;

/* fix for missing gettimeofday() call */

#include <sys/procstats.h>
#define gettimeofday(t, tz)	get_process_stats((t), PS_SELF, NULL, NULL)
