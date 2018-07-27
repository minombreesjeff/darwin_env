#include <ncbi.h>

#define TEST_VERSION

#define OPT_TUPLES  1
#define OPT_SHORT   2

void ErrMsgRoot_PrintHeader (ErrMsgRootPtr idx, unsigned int opts, FILE *fd);



int main (int argc, char **argv)
{
	ErrMsgRootPtr idx;
	FILE *fout = stdout;
	char mod[32];
	int i;
	unsigned int opts =0;
	
	mod[0] = '\0';

	ErrSetOptFlags(EO_LOGTO_STDERR);
	ErrClearOptFlags(EO_SHOW_CODES | EO_BEEP);
	ErrSetMessageLevel(SEV_MAX);
	ErrSetLogLevel(SEV_WARNING);
	
	for (i=1; i<argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case '2' :
				opts |= OPT_TUPLES;
				break;
			case 's' :
				opts |= OPT_SHORT;
				break;
			}
		}
		else if (mod[0] == '\0')
		{
			char *p;
			strncat(mod,argv[i],sizeof mod);
			if ((p = strstr(argv[i],".msg")) != NULL)
				*p = '\0';
		}
		else if (fout == stdout)
		{
			ErrSetFatalLevel(SEV_INFO);
			fout = fopen(argv[i],"w");
			ErrSetFatalLevel(SEV_FATAL);
		}
	}
	
	if (mod[0] == '\0')
	{
		fprintf(stderr,"\nUSAGE:  errhdr [options] msgfile [hdrfile] \n\n");
		fprintf(stderr,"  options:  -2 for code,subcode tuples\n");
		fprintf(stderr,"            -s for short subcode defines\n");
		return 1;
	}


	if ((idx  = ErrGetMsgRoot(mod)) ==NULL)
	{
		return 1;
	}

	if (idx->list == NULL  /*not_avail*/ )
	{
		fprintf(stderr,"Unable to find error message file for module \"%s\"\n",mod);
		return 1;
	}
		
	ErrMsgRoot_PrintHeader(idx,opts,fout);	
	return 0;
}

void ErrMsgRoot_PrintHeader (ErrMsgRootPtr idx, unsigned int opts, FILE *fd)
{
	ErrMsgNode *lev1;
	ErrMsgNode *lev2;
	
	fprintf(fd,"#ifndef __MODULE_%s__\n",idx->name);
	fprintf(fd,"#define __MODULE_%s__\n\n",idx->name);

	for (lev1=idx->list; lev1; lev1=lev1->next)
	{
	
		if (opts & OPT_TUPLES)
		{
			fprintf(fd,"#define ERR_%s  %d,0\n",lev1->name,(int)lev1->code);
			for (lev2=lev1->list; lev2; lev2=lev2->next)
				fprintf(fd,"#define ERR_%s_%s  %d,%d\n",lev1->name,lev2->name,
						(int)lev1->code,(int)lev2->code);
		}
		else 
		{
			fprintf(fd,"#define ERR_%s  %d\n",lev1->name,(int)lev1->code);
			for (lev2=lev1->list; lev2; lev2=lev2->next)
			{
				if (opts & OPT_SHORT)
					fprintf(fd,"#define    SUB_%s  %d\n",
							lev2->name,(int)lev2->code);
				else
					fprintf(fd,"#define    SUB_%s_%s  %d\n",
							lev1->name,lev2->name,(int)lev2->code);
			}
		}
	}

	fprintf(fd,"\n#endif\n");
}


Int2
Nlm_Main(void)
{
    fprintf(stderr,"Kludge main executed in file %s", __FILE__);
    exit (1);
}
