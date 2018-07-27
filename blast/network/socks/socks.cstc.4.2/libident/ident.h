/*
** ident.h
**
** Author: Peter Eriksson <pen@lysator.liu.se>
** Intruder: Pär Emanuelsson <pell@lysator.liu.se>
*/

#ifndef __IDENT_H__
#define __IDENT_H__

#ifdef __P
#  undef __P
#endif

#ifdef __STDC__
#  define __P(AL)	AL
#else
#  define __P(AL)	()
#endif

/*
 * Sigh, GCC v2 complains when using undefined struct tags
 * in function prototypes...
 */
#if defined(__GNUC__) && !defined(IPPROTO_IP)
#  define __STRUCT_IN_ADDR_P	void *
#else
#  define __STRUCT_IN_ADDR_P	struct in_addr *
#endif

#if defined(__GNUC__) && !defined(DST_NONE)
#  define __STRUCT_TIMEVAL_P	void *
#else
#  define __STRUCT_TIMEVAL_P	struct timeval *
#endif


#ifndef IDBUFSIZE
#  define IDBUFSIZE 2048
#endif

#ifndef IDPORT
#  define IDPORT	113
#endif

typedef struct
{
  int fd;
  char buf[IDBUFSIZE];
} ident_t;

typedef struct {
  int lport;                    /* Local port */
  int fport;                    /* Far (remote) port */
  char *identifier;             /* Normally user name */
  char *opsys;                  /* OS */
  char *charset;                /* Charset (what did you expect?) */
} IDENT;                        /* For higher-level routines */

/* Low-level calls and macros */
#define id_fileno(ID)	((ID)->fd)

extern ident_t * id_open __P((__STRUCT_IN_ADDR_P laddr,
			   __STRUCT_IN_ADDR_P faddr,
			   __STRUCT_TIMEVAL_P timeout));
  
extern int    id_close __P((ident_t *id));
  
extern int    id_query __P((ident_t *id,
			    int lport,
			    int fport,
			    __STRUCT_TIMEVAL_P timeout));
  
extern int    id_parse __P((ident_t *id,
			    __STRUCT_TIMEVAL_P timeout,
			    int *lport,
			    int *fport,
			    char **identifier,
			    char **opsys,
			    char **charset));
  
/* High-level calls */
extern IDENT *ident_lookup __P((int fd, int timeout));

extern char  *ident_id __P((int fd, int timeout));

void          ident_free __P((IDENT *id));
#endif
