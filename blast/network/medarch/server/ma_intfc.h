/*
*
*
* RCS Modification History:
* $Log: ma_intfc.h,v $
* Revision 6.0  1997/08/25 18:36:20  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:57  epstein
* add RCS log revision history
*
*/


#include <stdio.h>
#include <dlfcn.h>
#include "getmedart.h"

#ifndef	MODULE_MA_INTFC_H
#define	MODULE_MA_INTFC_H

#ifndef	MAX_FORMAT_HANDLERS
#define	MAX_FORMAT_HANDLERS	512
#endif
#ifndef	MAX_TOKEN_SIZE
#define	MAX_TOKEN_SIZE		2048
#endif
#ifndef	MAX_FORMAT_LINE
#define	MAX_FORMAT_LINE		4096
#endif


typedef struct ma_Obj {
	void	*(*initialize)(char *, void *);	/* Initialize */
	int	 (*open_file)(void *, char *file_name );
	int	 (*setup_file)(void *, char *file_name, FILE *fd );
	int	 (*process)(void *, MedArt *);	/* Convert format */
	int	 (*close)(void *);		/* Finish up */
	char	*(*get_error)(void *);		/* Get error text */
	void	*data;				/* Other data */
} ma_Obj;

int	 ma_GetHandlers( char *file_name );
void	*ma_ValidHandler( char *cmd_flag );
int	 ma_OpenHandler( void *handler, char *file, FILE *fd );
int	 ma_WriteArticle( void *handler, MedArt *article );
int	 ma_CloseHandler( void *handler );
char	*ma_GetErrorString( void *handler );

#endif /* MODULE_MA_INTFC_H */
