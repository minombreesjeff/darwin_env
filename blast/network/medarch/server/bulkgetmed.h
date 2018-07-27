/*
*
*
* RCS Modification History:
* $Log: bulkgetmed.h,v $
* Revision 6.0  1997/08/25 18:36:00  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:23  epstein
* add RCS log revision history
*
*/


#ifndef	DEFS_MODULE_BULKGETMED_H
#define	DEFS_MODULE_BULKGETMED_H

#include	"database.h"
#include	"getmedart.h"

/* This is the maximum number of bulk access channels */
/* The current version only uses 10, but we may want to add
   more later.  They don't cost much. */

#define	MAX_BULK_CHANNELS	15

typedef	char	BaseHandle[20];
typedef	char	BulkHandle[32];

typedef enum
    { inactive, idle, query_sent, query_ok, have_results, have_data, at_end }
DbState;

typedef	enum {
	SyncHandler,		ArticleHandler,		AuthorHandler,
	AbstractHandler,	MeshHandler,		QualifierHandler,
	SubstanceHandler,	XrefHandler,		SupportHandler,
	GeneSymbolHandler,	IsoHandler,		DoneHandler
} Handler;


typedef struct Channel {
    BaseHandle	base_handle;
    long	flag_mask;
    long	flag_value;
    char	*query_format;
    Handler	handler;
    RETCODE	(*load)();
    int		(*store)();
    DBINT	(*get_ui)();
} Channel;

typedef struct BChan {
    BulkHandle	 name;
    DBPROCESS	*db;
    RETCODE	 rc;
    DbState	 state;
    Channel	*channel;
    void	*data;
    int		 errors;
} BChan;

typedef	struct BulkDb {
    int		flags;
    int		bulk_id;
    BChan	*iso_channel;
    int		n_channels;
    DBINT	last_ui;
    int		articles;
    BChan	channel[MAX_BULK_CHANNELS];
} BulkDb;

extern BulkDb	*BulkOpenMedline( char *, const char *, const int );
extern void	 BulkCloseMedline( BulkDb * );
extern int	 BulkGetMedline( BulkDb *, MedArt * );

#endif /* DEFS_MODULE_BULKGETMED_H */
