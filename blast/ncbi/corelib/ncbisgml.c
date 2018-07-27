/*   ncbimisc.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  ncbimisc.c
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   10/23/91
*
* $Revision: 6.4 $
*
* File Description: 
*   	miscellaneous functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-15-93 Schuler     This file created (SGML functions were in ncbimisc)
* 12-22-93 Schuler     Converted ERRPOST((...)) to ErrPostEx(...)
*
*
* $Log: ncbisgml.c,v $
* Revision 6.4  2002/12/09 14:40:37  kans
* fixed NUM_SGML_ENTITY, sgml_bb_array
*
* Revision 6.3  2002/12/06 20:12:40  kans
* Nlm_LoadSgmlTableFromLocalArray populates sgml arrays if sgmlbb.ent file is unavailable
*
* Revision 6.2  2002/10/03 16:22:04  kans
* changed fgets to Nlm_FileGets
*
* Revision 6.1  1997/10/29 02:44:35  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:17:01  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.6  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
* ==========================================================================
*/
 
#define THIS_MODULE g_corelib
#define THIS_FILE _this_file

#include <ncbi.h>
#include <ncbiwin.h>

extern char * g_corelib;
static char * _this_file = __FILE__;

/* maximum character from SGML to pass to ErrPost()  */
#define SGML_ERROR_MSG_LIM 100

static char **sgml_entity;
static char **sgml_ascii;
static int num_sgml_entity = 0;


/*****************************************************************************
*
*   SgmlLoadTable()
*
*****************************************************************************/
#define NUM_SGML_ENTITY 52

typedef struct sgml_bb {
  char * entity;
  char * ascii;
} Sgml_Bb;

static Sgml_Bb sgml_bb_array [NUM_SGML_ENTITY] = {
  {"gt", ">"},
  {"lt", "<"},
  {"amp", "&"},
  {"agr", "alpha"},
  {"Agr", "Alpha"},
  {"bgr", "beta"},
  {"Bgr", "Beta"},
  {"ggr", "gamma"},
  {"Ggr", "Gamma"},
  {"dgr", "delta"},
  {"Dgr", "Delta"},
  {"egr", "epsilon"},
  {"Egr", "Epsilon"},
  {"zgr", "zeta"},
  {"Zgr", "Zeta"},
  {"eegr", "eta"},
  {"EEgr", "Eta"},
  {"thgr", "theta"},
  {"THgr", "Theta"},
  {"igr", "iota"},
  {"Igr", "Iota"},
  {"kgr", "kappa"},
  {"Kgr", "Kappa"},
  {"lgr", "lambda"},
  {"Lgr", "Lambda"},
  {"mgr", "mu"},
  {"Mgr", "Mu"},
  {"ngr", "nu"},
  {"Ngr", "Nu"},
  {"xgr", "xi"},
  {"Xgr", "Xi"},
  {"ogr", "omicron"},
  {"Ogr", "Omicron"},
  {"pgr", "pi"},
  {"Pgr", "Pi"},
  {"rgr", "rho"},
  {"Rgr", "Rho"},
  {"sgr", "sigma"},
  {"Sgr", "Sigma"},
  {"sfgr", "s"},
  {"tgr", "tau"},
  {"Tgr", "Tau"},
  {"ugr", "upsilon"},
  {"Ugr", "Upsilon"},
  {"phgr", "phi"},
  {"PHgr", "Phi"},
  {"khgr", "chi"},
  {"KHgr", "Chi"},
  {"psgr", "psi"},
  {"PSgr", "Psi"},
  {"ohgr", "omega"},
  {"OHgr", "Omega"}
};

static int Nlm_LoadSgmlTableFromLocalArray (void)

{
	int i, x;

	x = NUM_SGML_ENTITY;

	sgml_entity = (char**) MemNew(x * sizeof(char*));
	sgml_ascii  = (char**) MemNew(x * sizeof(char*));

	for (i = 0; i < x; i++)
	{
		sgml_entity[i] = StrSave(sgml_bb_array [i].entity);
		sgml_ascii[i] = StrSave(sgml_bb_array [i].ascii);
	}
	num_sgml_entity = x;
	return x;
}

NLM_EXTERN int LIBCALL  Nlm_SgmlLoadTable (void)
{
	char buf[80], *p1, *p2;
	FILE *fp;
	int i, x;

	if (num_sgml_entity > 0)
		return (int)num_sgml_entity;

    if (! FindPath("ncbi", "ncbi", "data", buf, sizeof (buf)))
        ErrPostEx(SEV_INFO,E_SGML,1,"SgmlLoadTable:  FindPath(ncbi,ncbi,data,...) failed");
    strncat(buf,"sgmlbb.ent",sizeof buf);

	if ((fp = FileOpen(buf,"r")) ==NULL)
		return Nlm_LoadSgmlTableFromLocalArray ();
	
	if (!Nlm_FileGets(buf,sizeof buf,fp))
	{
		ErrPostEx(SEV_WARNING,E_SGML,1,"Error reading file [sgmlbb.ent]");
		return FALSE;
	}
	x = atoi(buf);
	sgml_entity = (char**) MemNew(x * sizeof(char*));
	sgml_ascii  = (char**) MemNew(x * sizeof(char*));
	for (i = 0; i < x; i++)
	{
		if ((p1 = Nlm_FileGets(buf,sizeof buf,fp)) == NULL) break;
		while ((*p1 < ' ') && (*p1 != '\0')) p1++;  /* skip any leading junk */
		if (*p1 == '\0') break;
		if ( (p2 = strchr(p1,'\t')) == NULL )   break;
		*p2++ = '\0';
		sgml_entity[i] = StrSave(p1);
		p1 = p2;
		while (*p2 >= ' ') p2++;
		*p2 = '\0';
		sgml_ascii[i] = StrSave(p1);
	}
	FileClose(fp);
	num_sgml_entity = x;
	return x;
}

/*****************************************************************************
*
*   Sgml2Ascii(sgml, ascii, buflen)
*
*****************************************************************************/
NLM_EXTERN char * LIBCALL  Nlm_Sgml2Ascii (const char *sgml, char *ascii, size_t buflen)   /* length of ascii buffer */
{
	const char *from;
	char tbuf[40], *to, *tmp;
	int i, j;

	if (! num_sgml_entity)
		SgmlLoadTable();

	from = sgml;
	to = ascii;
	*to = '\0';
	buflen--;
	*(to+buflen) = '\0';

	while ((*from != '\0') && (buflen))
	{
		if (*from == '&')      /* entity start */
		{
			from++;
			tmp = tbuf;
			j = 0;
			while ((*from != ';') && (*(from + 1) != '\0') && (j < 39))
			{
				*tmp++ = *from++;
				j++;
			}
			*tmp = '\0';
			from++;
			for (i = 0; i < num_sgml_entity; i++)
			{
				if (! Nlm_StringCmp(tbuf, sgml_entity[i]))
					break;
			}
			if (i >= num_sgml_entity)
			{
				char bad[SGML_ERROR_MSG_LIM];
				bad[0] = '\0';
				strncat(bad,sgml,SGML_ERROR_MSG_LIM);
				ErrPostEx(SEV_ERROR,E_SGML,3,"Unrecognized SGML entity &%s in [%s]",tbuf,bad);
			}
			else
			{
				*to = '<'; to++;
				to = Nlm_StringMove(to, sgml_ascii[i]);
				*to = '>'; to++;
				buflen -= (Nlm_StringLen(sgml_ascii[i]) + 2);
			}
		}
		else if (*from == '<')   /* region tag */
		{
			while ((*from != '>') && (*from != '\0'))    /* ignore for now */
				from++;
			if (*from == '\0')
			{
				char bad[SGML_ERROR_MSG_LIM];
				bad[0] = '\0';
				strncat(bad,sgml,SGML_ERROR_MSG_LIM);
				ErrPostEx(SEV_ERROR,E_SGML,2, "Unbalanced <> in SGML [%s]",bad);
			}
			else
				from++;
		}
		else
		{
			*to = *from;
			to++; from++; buflen--;
		}
	}

	*to = '\0';
	return to;
}

/*****************************************************************************
*
*   Sgml2AsciiLen(sgml)
*
*****************************************************************************/
NLM_EXTERN size_t LIBCALL  Nlm_Sgml2AsciiLen (const char *sgml)
{
	const char *from;
	char tbuf[40], *tmp;
	int len = 0, i, j;

	if (! num_sgml_entity)
		SgmlLoadTable();

	from = sgml;

	while (*from != '\0')
	{
		if (*from == '&')      /* entity start */
		{
			from++;
			tmp = tbuf;
			j = 0;
			while ((*from != ';') && (*(from + 1) != '\0') && (j < 39))
			{
				*tmp++ = *from++;
				j++;
			}
			*tmp = '\0';
			from++;
			for (i = 0; i < num_sgml_entity; i++)
			{
				if (! Nlm_StringCmp(tbuf, sgml_entity[i]))
					break;
			}
			if (i >= num_sgml_entity)
			{
				char bad[SGML_ERROR_MSG_LIM];
				bad[0] = '\0';
				strncat(bad,sgml,SGML_ERROR_MSG_LIM);
				ErrPostEx(SEV_ERROR,E_SGML,3,"Unrecognized SGML entity &%s in [%s]",tbuf,bad);
			}
			else
				len += ((Nlm_Int2)Nlm_StringLen(sgml_ascii[i]) + 2);
		}
		else if (*from == '<')   /* region tag */
		{
			while ((*from != '>') && (*from != '\0'))    /* ignore for now */
				from++;
			if (*from == '\0')
			{
				char bad[SGML_ERROR_MSG_LIM];
				bad[0] = '\0';
				strncat(bad,sgml,SGML_ERROR_MSG_LIM);
				ErrPostEx(SEV_ERROR,E_SGML,2,"Unbalanced <> in SGML [%s]",bad);
			}
			else
				from++;
		}
		else
		{
			from++; len++;
		}
	}

	return len;
}

